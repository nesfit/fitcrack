'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''


import base64
import datetime
import json
import math
import time
import sys
import subprocess
import os

from os.path import basename
from typing import TypedDict
from uuid import uuid1
from flask_restx import abort
from flask_login import current_user
from sqlalchemy import exc
from settings import DICTIONARY_DIR, HASHCAT_PATH, RULE_DIR, PCFG_DIR, ROOT_DIR
from src.api.fitcrack.attacks import processJob as attacks
from src.api.fitcrack.attacks.functions import compute_keyspace_from_mask, compute_prince_keyspace
from src.api.fitcrack.lang import status_to_code, attack_modes
from src.api.fitcrack.functions import shellExec, lenStr
from src.database import db
from src.database.models import FcJob, FcHostActivity, FcBenchmark, Host, FcDictionary, FcJobDictionary, \
    FcJobGraph, FcRule, FcHash, FcHashList, FcMask, FcUserPermission, FcSetting, FcWorkunit, FcDeviceInfo
from src.api.fitcrack.endpoints.pcfg.functions import extractNameFromZipfile


def stop_job(job):
    job.status = status_to_code['finishing']

def kill_job(job:FcJob, db):
    id = job.id
    # Job is stopped in Generator after sending BOINC commands
    if (int(job.status) != status_to_code['running']) and (int(job.status) != status_to_code['finishing']):
        job.status = status_to_code['ready']
        workunits = FcWorkunit.query.filter(FcWorkunit.job_id == id).all()
        for wu in workunits:
            device_info_for_wu = FcDeviceInfo.query.filter(FcDeviceInfo.workunit_id == wu.id).all()
            for device_info in device_info_for_wu:
                db.session.delete(device_info)

            db.session.delete(wu)
    else:
        job.kill = True

    job.indexes_verified = 0
    job.current_index = 0
    job.current_index_2 = 0
    job.workunit_sum_time = 0
    job.time_start = job.time_end = None
    if job.attack_mode == attack_modes['mask'] or job.attack_mode == attack_modes['hybrid (wordlist + mask)']:
        masks = FcMask.query.filter(FcMask.job_id == id).all()
        for mask in masks:
            mask.current_index = 0
    elif job.attack_mode in [attack_modes[modeStr] for modeStr in ['dictionary', 'combinator', 'hybrid (mask + wordlist)']]:
        dictionaries = FcJobDictionary.query.filter(FcJobDictionary.job_id == id).all()
        for dictionary in dictionaries:
            dictionary.current_index = 0

    hosts = FcHostActivity.query.filter(FcHostActivity.job_id == id).all()
    for host in hosts:
        host.status = 0

    graphData = FcJobGraph.query.filter(FcJobGraph.job_id == id).all()
    for item in graphData:
        db.session.delete(item)


def start_job(job, db):
    hosts = [ a[0] for a in db.session.query(Host.id).all() ]
    if job.host_count == 0:
        for hostId in hosts:
            host = FcHostActivity(boinc_host_id=hostId, job_id=job.id)
            db.session.add(host)
    job.status = status_to_code['running']

def create_job(data):
    if data['name'] == '':
        abort(500, 'Name can not be empty.')   

    hash_list : FcHashList = FcHashList.query.filter(FcHashList.id==data['hash_list_id']).first()
    if not hash_list:
        abort(400, 'Hash list with given id does not exist')
    if hash_list.hash_type is None:
        abort(400, 'Hash list does not have any hash type set.')
    if hash_list.hash_count == 0:
        abort(400, 'Hash list does not contain any hashes.')

    #The hash type was previously given through the API request alongside the list of hashes to be cracked.
    #Since we now store hashes externally in a hash list, we extract the hash type from the hash list and
    #inject it into the API request data--old code then finds the hash type where it expects.
    data['hash_settings'] = {'hash_type': str(hash_list.hash_type)}

    hybrid_mask_dict = False
    #Hybrid attack mask-wordlist
    if int(data['attack_settings']['attack_mode']) == 7:    hybrid_mask_dict = True

    process_func = getattr(attacks, 'process_job_' + str(data['attack_settings']['attack_mode']))

    if not process_func:
        abort(400, "Unsupported attack type")

    job = process_func(data)

    if job['attack_settings']['attack_mode'] == 3:
        job['attack_settings']['attack_submode'] = data['attack_settings']['attack_submode']

    if job['time_start'] == '':
        job['time_start'] = None

    if job['time_end'] == '':
        job['time_end'] = None


    db_job = FcJob(
        attack=job['attack_name'],
        attack_mode=job['attack_settings']['attack_mode'],
        attack_submode=job['attack_settings']['attack_submode'],
        distribution_mode=job['attack_settings']['distribution_mode'],
        hash_type=hash_list.hash_type,
        status='0',
        keyspace=job['keyspace'],
        hc_keyspace=job['hc_keyspace'],
        indexes_verified='0',
        current_index='0',
        current_index_2='0',
        name=job['name'],
        comment=job['comment'],
        time_start=None if not job['time_start'] else  datetime.datetime.strptime(job['time_start'], '%Y-%m-%dT%H:%M'),
        time_end=None if not job['time_end'] else datetime.datetime.strptime(job['time_end'], '%Y-%m-%dT%H:%M'),
        workunit_sum_time='0',
        seconds_per_workunit=job['seconds_per_job'] if job['seconds_per_job'] > 60 else 60,
        charset1=job['charset1'] if job.get('charset1') else '',
        charset2=job['charset2'] if job.get('charset2') else '',
        charset3=job['charset3'] if job.get('charset3') else '',
        charset4=job['charset4'] if job.get('charset4') else '',
        rules=(job['attack_settings']['rules']['name'] if job.get('rules') else None),
        rule_left=(job['attack_settings']['rule_left'] if job['attack_settings'].get('rule_left') else ''),
        rule_right=(job['attack_settings']['rule_right'] if job['attack_settings'].get('rule_right') else ''),
        markov_hcstat=job['markov_hcstat'] if job.get('markov_hcstat') else '',
        markov_threshold=job['markov_threshold'] if job.get('markov_threshold') else 0,
        grammar_id=job['attack_settings']['pcfg_grammar']['id'] if job['attack_settings'].get('pcfg_grammar') else 0,
        case_permute=job['attack_settings'].get('case_permute', 0),
        check_duplicates=job['attack_settings'].get('check_duplicates', 1),
        min_password_len=job['attack_settings'].get('min_password_len', 0),
        max_password_len=job['attack_settings'].get('max_password_len', 0),
        min_elem_in_chain=job['attack_settings'].get('min_elem_in_chain', 0),
        max_elem_in_chain=job['attack_settings'].get('max_elem_in_chain', 0),
        generate_random_rules=job['attack_settings'].get('generate_random_rules', 0),
        optimized=job['attack_settings'].get('optimized', 1),
        deleted=False,
        hash_list_id=job['hash_list_id']
        )

    try:
        db.session.add(db_job)
        db.session.commit()
    except exc.SQLAlchemyError as err:
        print(err,  file=sys.stderr)
        abort(400, 'Unable to add job due to internal error!')

    #Hybrid attack mask-wordlist
    if hybrid_mask_dict:    post_process_func = getattr(attacks, 'post_process_job_' + str(7))

    else:   post_process_func = getattr(attacks, 'post_process_job_' + str(job['attack_settings']['attack_mode']))

    if not post_process_func:
        abort(400, "Unsupported attack type")

    post_process_func(job, db_job)

    for host in data['hosts_ids']:
        db_host = Host.query.filter(Host.id == host).first()
        if not db_host:
            abort(400, 'Host with id ' + str(host) + ' does not exist.')
        db_host_activity = FcHostActivity(job_id=db_job.id, boinc_host_id=db_host.id)
        db.session.add(db_host_activity)

    perms = FcUserPermission(user_id=current_user.id, job_id=db_job.id, view=1, modify=1, operate=1, owner=1)
    db.session.add(perms)

    db.session.commit()
    return db_job

verifyHashFormatResultItem = TypedDict('verifyHashFormatResultItem',{'hash' : str, 'result' : str, 'isInCache' : bool})
verifyHashFormatResult = TypedDict('verifyHashFormatResult', {'items' : list[verifyHashFormatResultItem], 'error' : bool})
def verifyHashFormat(hash, hash_type, abortOnFail=False, binaryHash=False) -> verifyHashFormatResult:
    #This function is properly cursed. You will get a headache trying to understand it.
    #Should be rewritten... eventually.
       
    hashes : list[tuple[str,str]] = []

    settings = FcSetting.query.first()
    if not settings.verify_hash_format:
        if binaryHash:
            hashes = [('HASH','OK')]
        else:
            with open(hash, "r") as hashFile:
                hashes = [(h.strip(), 'OK') for h in hashFile.readlines()]
    else:
        result = shellExec(
            "{} -m {} {} --show --machine-readable".format(HASHCAT_PATH, hash_type, hash), getReturnCode=True
        )

        if binaryHash:
            hashes = [('HASH','OK') if result['returnCode'] == 0 else ('HASH', 'Token length exception')]
        else:
            hash_validity = {}
            with open(hash, "r") as hashFile:
                for h in hashFile.readlines():
                    hash_validity[h.strip()] = "OK"

            bad_hash_lines = result['msg'].rstrip().split('\n')
            for line in bad_hash_lines:
                line_parts = line.split(":")
                if len(line_parts) == 4:
                    # hashfile : line : hash : status
                    bad_hash = line_parts[2]
                    error = line_parts[3]
                    if bad_hash.find("...") > 0:
                        # if the hash is longer than a certain length, it's abbreviated with ... 
                        bad_hash_parts = bad_hash.split("...")
                        for h in hash_validity.keys():
                            if h.startswith(bad_hash_parts[0]) and h.endswith(bad_hash_parts[1]):
                                hash_validity[h] = error
                    else:
                        hash_validity[bad_hash] = error

            hashes = list(hash_validity.items())

    hashesArr = []
    hasError = False
    for hash in hashes:

        isInCache = False
        dbHash = FcHash.query.filter(FcHash.hash == bytes(hash[0], 'utf8'), FcHash.result != None).first()
        if dbHash:
            isInCache = True


        if abortOnFail and hash[1] != 'OK':
            abort(500, 'Hash ' + hash[0] + ' has wrong format (' + hash[1] + ' exception).')
        if binaryHash:
            hash = (binaryHash,hash[1])

        if hash[0] == '':
            hashesArr.append({
                'hash': hash[0],
                'result': 'Empty hash',
                'isInCache': False
            })
            hasError = True
        else:
            hashesArr.append({
                'hash': hash[0],
                'result': hash[1],
                'isInCache': isInCache
            })
            if hash[1] != 'OK':
                hasError = True
    return {
                'items': hashesArr,
                'error': hasError
            }

def computeCrackingTime(data):
    total_power = 0
    keyspace = 0
    hosts_dict = []

    attackSettings = json.loads(data['attack_settings'])

    hash_type_code = -1
    hashlist = FcHashList.query.filter(FcHashList.id == data['hash_list_id']).first()
    if hashlist:
        hash_type_code = hashlist.hash_type

    data['boinc_host_ids'] = [x.strip() for x in data['boinc_host_ids'].split(',')]

    # Check if we have valid hash type code and if we have any host
    # -1 is indicator that no hash type was selected in webadmin
    if hash_type_code != -1 and len(data['boinc_host_ids']) > 0:
        hosts = FcBenchmark.query.filter(FcBenchmark.hash_type == hash_type_code). \
            filter(FcBenchmark.attack_mode == attackSettings['attack_mode']). \
            filter(FcBenchmark.boinc_host_id.in_(data['boinc_host_ids'])).all()

        if not hosts:
            # We were unable to find benchmark data for (host, job_hash_type, job_attack_mode).
            # Instead of no data, try to use less precise data for (host, job_hash_type, X)
            # where X is any attack mode
            hosts = FcBenchmark.query.filter(FcBenchmark.hash_type == hash_type_code). \
                filter(FcBenchmark.boinc_host_id.in_(data['boinc_host_ids'])).all()

        for host in hosts:
            total_power += host.power
            hosts_dict.append(host.as_dict())

    else:
        abort(500, 'No hosts selected.')

    if attackSettings['attack_mode'] == 0:
        dictsKeyspace = 0
        for dict in attackSettings['left_dictionaries']:
            dictsKeyspace += dict['keyspace']
        rulesKeyspace = 1
        if attackSettings['rules']:
            rules = FcRule.query.filter(FcRule.id == attackSettings['rules']['id']).first()
            rulesKeyspace = rules.count

        keyspace = dictsKeyspace * rulesKeyspace

    elif attackSettings['attack_mode'] == 1:
        leftDictsKeyspace = 0
        for dict in attackSettings['left_dictionaries']:
            leftDictsKeyspace += dict['keyspace']

        rightDictsKeyspace = 0
        for dict in attackSettings['right_dictionaries']:
            rightDictsKeyspace += dict['keyspace']

        keyspace = leftDictsKeyspace * rightDictsKeyspace

    elif attackSettings['attack_mode'] == 3:
        if attackSettings.get('masks') and len(attackSettings['masks']) > 0:
            thresh = attackSettings.get('markov_treshold', None)
            customCharsetDict = {}
            for i, charset in enumerate(attackSettings['charset']):
                customCharsetDict[i + 1] = charset['keyspace']
            for mask in attackSettings['masks']:
                keyspace += compute_keyspace_from_mask(mask, customCharsetDict, thresh)

    elif attackSettings['attack_mode'] == 6:
        dictsKeyspace = 0
        for dict in attackSettings['left_dictionaries']:
            dictsKeyspace += dict['keyspace']
        keyspace = compute_keyspace_from_mask(attackSettings['mask']) * dictsKeyspace

    elif attackSettings['attack_mode'] == 7:
        dictsKeyspace = 0
        for dict in attackSettings['right_dictionaries']:
            dictsKeyspace += dict['keyspace']
        keyspace = compute_keyspace_from_mask(attackSettings['mask']) * dictsKeyspace

    elif attackSettings['attack_mode'] == 8:
        dictsKeyspace = compute_prince_keyspace(attackSettings)
        if dictsKeyspace == -1:
             abort(400, 'Unable to compute job keyspace.')
        random_rules = 0
        if attackSettings['generate_random_rules']:
            random_rules = int(attackSettings['generate_random_rules'])
        rulesKeyspace = random_rules
        if attackSettings['rules']:
            rules = FcRule.query.filter(FcRule.id == attackSettings['rules']['id']).first()
            rulesKeyspace = rules.count

        if rulesKeyspace == 0:
            keyspace = dictsKeyspace
        else:
            keyspace = dictsKeyspace * rulesKeyspace

    elif attackSettings['attack_mode'] == 9:
        if(attackSettings['keyspace_limit']):
            if(int(attackSettings['keyspace_limit']) > 0):
                keyspace = int(attackSettings['keyspace_limit'])

            else:
                keyspace = int(attackSettings['pcfg_grammar']['keyspace'])
        else:
            keyspace = int(attackSettings['pcfg_grammar']['keyspace'])

        rulesKeyspace = 1
        if attackSettings['rules']:
            rules = FcRule.query.filter(FcRule.id == attackSettings['rules']['id']).first()
            rulesKeyspace = rules.count
        keyspace = keyspace * rulesKeyspace

        # Keyspace control
        INT_MAX = sys.maxsize - 1

        if int(keyspace) >= INT_MAX:
            keyspace = INT_MAX

    display_time = None
    if (total_power > 0):
        display_time = float(keyspace / total_power)
        try:
            time_delta = datetime.timedelta(seconds=math.floor(display_time))
            if time_delta.total_seconds() < 60:
                display_time = 'About a minute'
            else:
                display_time = str(time_delta)
        except OverflowError:
            display_time = 'really long'

    result = {
        "hash_code": hash_type_code,
        "keyspace": keyspace,
        "hosts": hosts_dict,
        "total_power": total_power,
        "display_time": display_time
    }

    return result

# permission utils
def perm_base ():
    return db.session.query(FcUserPermission.job_id).filter_by(user_id=current_user.id)

def visible_jobs_ids ():
    ids = perm_base().filter_by(can_view=True).all()
    return [x[0] for x in ids]

def editable_jobs_ids ():
    ids = perm_base().filter_by(can_modify=True).all()
    return [x[0] for x in ids]

def actionable_jobs_ids ():
    ids = perm_base().filter_by(can_operate=True).all()
    return [x[0] for x in ids]

def can_view_job (id):
    return True if perm_base().filter_by(can_view=True).filter_by(job_id=id).one_or_none() else False

def can_edit_job (id):
    return True if perm_base().filter_by(can_modify=True).filter_by(job_id=id).one_or_none() else False

def can_operate_job (id):
    return True if perm_base().filter_by(can_operate=True).filter_by(job_id=id).one_or_none() else False

