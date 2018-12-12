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
from os.path import basename
from uuid import uuid1

import sys

import os
from flask_restplus import abort
from sqlalchemy import exc

from settings import DICTIONARY_DIR, HASHVALIDATOR_PATH, RULE_DIR
from src.api.fitcrack.attacks import processPackage as attacks
from src.api.fitcrack.attacks.functions import compute_keyspace_from_mask, coun_file_lines
from src.api.fitcrack.functions import shellExec, lenStr
from src.database import db
from src.database.models import FcJob, FcHashcache, FcHostActivity, FcBenchmark, Host, FcDictionary, FcRule, FcHash


def create_package(data):
    if data['name'] == '':
        abort(500, 'Name can not be empty.')


    for idx, hashObj in enumerate(data['hash_settings']['hash_list']):
        if hashObj['hash'].startswith('BASE64:'):
            decoded = base64.decodebytes(hashObj['hash'][7:].encode())
            with tempfile.NamedTemporaryFile() as fp:
                fp.write(decoded)
                fp.seek(0)
                verifyHashFormat(fp.name, data['hash_settings']['hash_type'], abortOnFail=True)
            data['hash_settings']['hash_list'][idx]['hash']= decoded
        else:
            verifyHashFormat(hashObj['hash'], data['hash_settings']['hash_type'], abortOnFail=True)
            data['hash_settings']['hash_list'][idx]['hash']= hashObj['hash'].encode()


    process_func = getattr(attacks, 'process_package_' + str(data['attack_settings']['attack_mode']))
    if not process_func:
        abort(400, "Unsupported attack type")

    package = process_func(data)
    package['config'] = (
                         '|||attack_mode|UInt|' + lenStr(str(package['attack_settings']['attack_mode'])) + '|' +
                         str(package['attack_settings']['attack_mode']) + '|||\n' +
                         '|||attack_submode|UInt|' + lenStr(str(package['attack_settings']['attack_submode'])) + '|' +
                         str(package['attack_settings']['attack_submode']) + '|||\n' +
                         '|||name|String|' + lenStr(package['name']) + '|' + package['name'] + '|||\n' +
                         '|||hash_type|UInt|' + lenStr(package['hash_settings']['hash_type']) + '|' +
                         package['hash_settings']['hash_type'] + '|||\n'
                         )

    if package['attack_settings'].get('rule_left') and package['attack_settings']['rule_left'] != '':
        package['config'] += '|||rule_left|String|' + lenStr(str(package['attack_settings']['rule_left'])) + '|' + \
                             package['attack_settings']['rule_left'] + '|||\n'

    if package['attack_settings'].get('rule_right') and package['attack_settings']['rule_right'] != '':
        package['config'] += '|||rule_right|String|' + lenStr(str(package['attack_settings']['rule_right'])) + '|' + \
                             package['attack_settings']['rule_right'] + '|||\n'

    token = uuid1()

    if package['time_start'] == '':
        package['time_start'] = None

    if package['time_end'] == '':
        package['time_end'] = None


    db_package = FcJob(
        token=token.hex,
        attack=package['attack_name'],
        attack_mode=package['attack_settings']['attack_mode'],
        attack_submode=package['attack_settings']['attack_submode'],
        hash_type=package['hash_settings']['hash_type'],
        hash='check hashlist',
        status='0',
        result=None,
        keyspace=package['keyspace'],
        hc_keyspace=package['hc_keyspace'],
        indexes_verified='0',
        current_index='0',
        current_index_2='0',
        name=package['name'],
        comment=package['comment'],
        time_start=None if not package['time_start'] else  datetime.datetime.strptime(package['time_start'], '%d/%m/%Y %H:%M'),
        time_end=None if not package['time_end'] else datetime.datetime.strptime(package['time_end'], '%d/%m/%Y %H:%M'),
        cracking_time='0',
        seconds_per_workunit=package['seconds_per_job'] if package['seconds_per_job'] > 60 else 60,
        config=package['config'],
        dict1=package['dict1'] if package.get('dict1') else '',
        dict2=package['dict2'] if package.get('dict2') else '',
        charset1=package['charset1'] if package.get('charset1') else '',
        charset2=package['charset2'] if package.get('charset2') else '',
        charset3=package['charset3'] if package.get('charset3') else '',
        charset4=package['charset4'] if package.get('charset4') else '',
        rules=(package['rules'] if package.get('rules') else None),
        rule_left=(package['attack_settings']['rule_left'] if package['attack_settings'].get('rule_left') else ''),
        rule_right=(package['attack_settings']['rule_right'] if package['attack_settings'].get('rule_right') else ''),
        markov_hcstat=package['markov_hcstat'] if package.get('markov_hcstat') else '',
        markov_threshold=package['markov_threshold'] if package.get('markov_threshold') else 0,
        replicate_factor=1,
        deleted=False
    )


    try:
        db.session.add(db_package)
        db.session.commit()
    except exc.SQLAlchemyError as err:
        print(err,  file=sys.stderr)
        abort(400, 'Unable to add package due to internal error!')

    post_process_func = getattr(attacks, 'post_process_package_' + str(package['attack_settings']['attack_mode']))
    if not post_process_func:
        abort(400, "Unsupported attack type")

    post_process_func(package, db_package)

    for host in data['hosts_ids']:
        db_host = Host.query.filter(Host.id == host).first()
        if not db_host:
            abort(400, 'Host with id ' + str(host) + ' does not exist.')
        db_host_activity = FcHostActivity(job_id=db_package.id, boinc_host_id=db_host.id)
        db.session.add(db_host_activity)

    for hashObj in data['hash_settings']['hash_list']:
        hash = FcHash(job_id=db_package.id, hash_type=package['hash_settings']['hash_type'], hash=hashObj['hash'])
        db.session.add(hash)

    db.session.commit()
    return db_package






def delete_package(id):
    package = FcJob.query.filter(FcJob.id == id).one()
    if (package.deleted):
        package.deleted = False
    else:
        package.deleted = True
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
            rulesKeyspace = coun_file_lines(os.path.join(RULE_DIR,rules.path))

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
            for mask in attackSettings['masks']:
                keyspace += compute_keyspace_from_mask(mask)

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
