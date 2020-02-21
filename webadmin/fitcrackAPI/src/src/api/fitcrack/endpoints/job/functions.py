'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''


import base64
import datetime
import json
import math
import tempfile
import time
import sys
import subprocess
import os

from os.path import basename
from uuid import uuid1
from flask_restplus import abort
from sqlalchemy import exc
from settings import DICTIONARY_DIR, HASHVALIDATOR_PATH, RULE_DIR, PCFG_DIR, PCFG_MANAGER_DIR, ROOT_DIR, PCFG_MANAGER
from src.api.fitcrack.attacks import processJob as attacks
from src.api.fitcrack.attacks.functions import compute_keyspace_from_mask, compute_prince_keyspace, count_file_lines
from src.api.fitcrack.functions import shellExec, lenStr
from src.database import db
from src.database.models import FcJob, FcHashcache, FcHostActivity, FcBenchmark, Host, FcDictionary, FcRule, FcHash
from src.api.fitcrack.endpoints.pcfg.functions import extractNameFromZipfile


def create_job(data):
    if data['name'] == '':
        abort(500, 'Name can not be empty.')

    for idx, hashObj in enumerate(data['hash_settings']['hash_list']):

        if hashObj['hash'].startswith('BASE64:'):
            decoded = base64.decodebytes(hashObj['hash'][7:].encode())
            with tempfile.NamedTemporaryFile() as fp:
                fp.write(decoded)
                fp.seek(0)
                verifyHashFormat(fp.name, data['hash_settings']['hash_type'], abortOnFail=data['hash_settings']['valid_only'])
            data['hash_settings']['hash_list'][idx]['hash']= decoded

        else:
            verifyHashFormat(hashObj['hash'], data['hash_settings']['hash_type'], abortOnFail=data['hash_settings']['valid_only'])
            data['hash_settings']['hash_list'][idx]['hash']= hashObj['hash'].encode()

    hybrid_mask_dict = False
    #Hybrid attack mask-wordlist
    if int(data['attack_settings']['attack_mode']) == 7:    hybrid_mask_dict = True

    process_func = getattr(attacks, 'process_job_' + str(data['attack_settings']['attack_mode']))

    if not process_func:
        abort(400, "Unsupported attack type")

    data['config'] = ''
    job = process_func(data)

    if job['attack_settings']['attack_mode'] == 3:
        job['attack_settings']['attack_submode'] = data['attack_settings']['attack_submode']

    # nastavenie attack modu pre hybrid
    if job['attack_settings']['attack_mode'] == 6 or job['attack_settings']['attack_mode'] == 7:
        attack_settings_control = 1

    else:
        attack_settings_control = job['attack_settings']['attack_mode']

    job['config'] += (
                         '|||attack_mode|UInt|' + lenStr(str(attack_settings_control)) + '|' +
                         str(attack_settings_control) + '|||\n' +
                         '|||attack_submode|UInt|' + lenStr(str(job['attack_settings']['attack_submode'])) + '|' +
                         str(job['attack_settings']['attack_submode']) + '|||\n' +
                         '|||name|String|' + lenStr(job['name']) + '|' + job['name'] + '|||\n' +
                         '|||hash_type|UInt|' + lenStr(job['hash_settings']['hash_type']) + '|' +
                         job['hash_settings']['hash_type'] + '|||\n'
                         )

    if job['attack_settings'].get('rule_left') and job['attack_settings']['rule_left'] != '':
        job['config'] += '|||rule_left|String|' + lenStr(str(job['attack_settings']['rule_left'])) + '|' + \
                             job['attack_settings']['rule_left'] + '|||\n'

    if job['attack_settings'].get('rule_right') and job['attack_settings']['rule_right'] != '':
        job['config'] += '|||rule_right|String|' + lenStr(str(job['attack_settings']['rule_right'])) + '|' + \
                             job['attack_settings']['rule_right'] + '|||\n'

    token = uuid1()

    if job['time_start'] == '':
        job['time_start'] = None

    if job['time_end'] == '':
        job['time_end'] = None

# db_package => db_job
    db_job = FcJob(
        token=token.hex,
        attack=job['attack_name'],
        attack_mode='1' if job['attack_settings']['attack_mode'] == 6 or job['attack_settings']['attack_mode'] == 7 else job['attack_settings']['attack_mode'],
        attack_submode=job['attack_settings']['attack_submode'],
        hash_type=job['hash_settings']['hash_type'],
        hash='check hashlist',
        status='0',
        result=None,
        keyspace=job['keyspace'],
        hc_keyspace=job['hc_keyspace'],
        indexes_verified='0',
        current_index='0',
        current_index_2='0',
        name=job['name'],
        comment=job['comment'],
        time_start=None if not job['time_start'] else  datetime.datetime.strptime(job['time_start'], '%d/%m/%Y %H:%M'),
        time_end=None if not job['time_end'] else datetime.datetime.strptime(job['time_end'], '%d/%m/%Y %H:%M'),
        cracking_time='0',
        seconds_per_workunit=job['seconds_per_job'] if job['seconds_per_job'] > 60 else 60,
        config=job['config'],
        dict1=job['dict1'] if job.get('dict1') else '',
        dict2=job['dict2'] if job.get('dict2') else '',
        charset1=job['charset1'] if job.get('charset1') else '',
        charset2=job['charset2'] if job.get('charset2') else '',
        charset3=job['charset3'] if job.get('charset3') else '',
        charset4=job['charset4'] if job.get('charset4') else '',
        rules=(job['attack_settings']['rules']['name'] if job.get('rules') else None),
        rule_left=(job['attack_settings']['rule_left'] if job['attack_settings'].get('rule_left') else ''),
        rule_right=(job['attack_settings']['rule_right'] if job['attack_settings'].get('rule_right') else ''),
        markov_hcstat=job['markov_hcstat'] if job.get('markov_hcstat') else '',
        markov_threshold=job['markov_threshold'] if job.get('markov_threshold') else 0,
        replicate_factor=1,
        deleted=False,
        grammar_id=job['attack_settings']['pcfg_grammar']['id'] if job['attack_settings'].get('pcfg_grammar') else 0
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

    for hashObj in data['hash_settings']['hash_list']:
        hash = FcHash(job_id=db_job.id, hash_type=job['hash_settings']['hash_type'], hash=hashObj['hash'])
        db.session.add(hash)

    db.session.commit()
    return db_job

def delete_job(id):
    job = FcJob.query.filter(FcJob.id == id).one()
    if (job.deleted):
        job.deleted = False
    else:
        job.deleted = True
    db.session.commit()


def verifyHashFormat(hash, hash_type, abortOnFail=False, binaryHash=False):
    result = shellExec(
        HASHVALIDATOR_PATH + ' -m ' + hash_type + ' ' + "'" + hash + "'", getReturnCode=True
    )

    if result['returnCode'] != 0:
        abort(500, 'Error in hashValidator.')

    hashes = result['msg'].rstrip().split('\n')
    hashesArr = []
    hasError = False
    for hash in hashes:
        hashArr = hash.split(' ')

        isInCache = False
        dbHash = FcHash.query.filter(FcHash.hash == bytes(hashArr[0], 'utf8'), FcHash.result != None).first()
        if dbHash:
            isInCache = True


        if abortOnFail and hashArr[1] != 'OK':
            abort(500, 'Hash ' + hashArr[0] + ' has wrong format (' + hashArr[1] + ' exception).')
        if binaryHash:
            hashArr[0] = binaryHash

        if hashArr[0] == '':
            hashesArr.append({
                'hash': hashArr[0],
                'result': 'Empty hash',
                'isInCache': False
            })
            hasError = True
        else:
            hashesArr.append({
                'hash': hashArr[0],
                'result': hashArr[1],
                'isInCache': isInCache
            })
            if hashArr[1] != 'OK':
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
    data['boinc_host_ids'] = [x.strip() for x in data['boinc_host_ids'].split(',')]

    if data.get('boinc_host_ids') and len(data['boinc_host_ids']) > 0:
        hosts = FcBenchmark.query.filter(FcBenchmark.hash_type == data['hash_type_code']). \
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
            customCharsetDict = {}
            for charset in attackSettings['charset']:
                customCharsetDict[charset['id']] = charset['keyspace']
            for mask in attackSettings['masks']:
                keyspace += compute_keyspace_from_mask(mask, customCharsetDict)

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
        rulesKeyspace = 1
        if attackSettings['rules']:
            rules = FcRule.query.filter(FcRule.id == attackSettings['rules']['id']).first()
            rulesKeyspace = rules.count

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
            display_time = str(datetime.timedelta(seconds=math.floor(display_time)))
        except OverflowError:
            display_time = 'really long'

    result = {
        "hash_code": data['hash_type_code'],
        "keyspace": keyspace,
        "hosts": hosts_dict,
        "total_power": total_power,
        "display_time": display_time
    }

    return result

def calculate_port_number(job_id):

    portNumber = 50050 + job_id
    return str(portNumber)


def start_pcfg_manager(job_id, grammar_name, keyspace):

    manager = PCFG_DIR + "/" + extractNameFromZipfile(grammar_name)
    test = PCFG_MANAGER_DIR + " server " + "-p " + str(calculate_port_number(job_id)) + " -m " + str(keyspace) + " --hashlist " + PCFG_MANAGER + "/README.md" + " -r " + PCFG_DIR + "/" + grammar_name
    process = subprocess.Popen([PCFG_MANAGER_DIR, "server", "-p", str(calculate_port_number(job_id)), "-m", str(keyspace), "--hashlist", PCFG_MANAGER + "/README.md", "-r", PCFG_DIR + "/" + grammar_name])
