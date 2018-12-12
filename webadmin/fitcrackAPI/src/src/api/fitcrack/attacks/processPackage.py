'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
from pathlib import Path

from flask_restplus import abort

from settings import HASHCAT_DIR, \
    HASHCAT_PATH, EXE_OR_BIN, DICTIONARY_DIR, RULE_DIR, HCSTATS_DIR, CHARSET_DIR
from src.api.fitcrack.attacks.functions import make_dict_from_mask, check_mask_syntax, compute_keyspace_from_mask, \
    coun_file_lines
from src.api.fitcrack.functions import shellExec
from src.database import db
# process brute force package
from src.database.models import FcMask, FcDictionary, FcRule, FcHcstat, FcCharset, FcJobDictionary


# dictionary attack
def process_package_0(package):
    package['attack_settings']['attack_submode'] = 0
    package['hc_keyspace'] = 0

    for dictObj in package['attack_settings']['left_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        package['hc_keyspace'] += dict.keyspace

    ruleFileMultiplier = 1
    if package['attack_settings']['rules']:
        rules = FcRule.query.filter(FcRule.id == package['attack_settings']['rules']['id']).first()
        ruleFileMultiplier = coun_file_lines(os.path.join(RULE_DIR, rules.path))
        if ruleFileMultiplier == 0:
            ruleFileMultiplier = 1
        if not rules:
            abort(500, 'Wrong rules file selected.')
        if not os.path.exists(os.path.join(RULE_DIR, rules.path)):
            abort(500, 'Rules file does not exist.')
        package['attack_settings']['attack_submode'] = 1
        package['rules'] = rules.name

    package['attack_name'] = 'dictionary'
    package['keyspace'] = package['hc_keyspace'] * ruleFileMultiplier

    return package


def post_process_package_0(data, db_package):
    for dict in data['attack_settings']['left_dictionaries']:
        packageDict = FcJobDictionary(job_id=db_package.id, dictionary_id=dict['id'])
        db.session.add(packageDict)


# combination attack
def process_package_1(package):
    package['attack_settings']['attack_submode'] = 0
    if package['attack_settings']['rule_left'] and package['attack_settings']['rule_right']:
        package['attack_settings']['attack_submode'] = 3
    elif package['attack_settings']['rule_left']:
        package['attack_settings']['attack_submode'] = 1
    elif package['attack_settings']['rule_right']:
        package['attack_settings']['attack_submode'] = 2

    dictsLeftKeyspace = 0
    dictsRightKeyspace = 0
    for dictObj in package['attack_settings']['left_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        dictsLeftKeyspace += dict.keyspace

    for dictObj in package['attack_settings']['right_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        dictsRightKeyspace += dict.keyspace



    keyspace = dictsLeftKeyspace * dictsRightKeyspace

    package['attack_name'] = 'combinator'
    package['hc_keyspace'] = dictsLeftKeyspace
    package['keyspace'] = keyspace
    return package


def post_process_package_1(data, db_package):
    for dict in data['attack_settings']['left_dictionaries']:
        packageDict = FcJobDictionary(job_id=db_package.id, dictionary_id=dict['id'])
        db.session.add(packageDict)

    for dict in data['attack_settings']['right_dictionaries']:
        packageDict = FcJobDictionary(job_id=db_package.id, dictionary_id=dict['id'], is_left=False)
        db.session.add(packageDict)



# mask attack
def process_package_3(package, hashcatKeyspace=True):
    if not package['attack_settings'].get('attack_submode'):
        package['attack_settings']['attack_submode'] = 0
    # check masks syntax
    for mask in package['attack_settings']['masks']:
        check_mask_syntax(mask)

    # charsets
    hashcatArgs = ''
    charsetsSize = []
    if package['attack_settings'].get('charset'):
        if len(package['attack_settings']['charset']) > 4:
            abort(500, 'Maximum of charsets files is 4.')
        for i, charset in enumerate(package['attack_settings']['charset'], 1):
            charsetPath = os.path.join(CHARSET_DIR, charset['name'])
            charsetsSize = dict()
            with open(charsetPath, 'rb') as f:
                content = f.read()
                charsetsSize[i] = len(content)

            hashcatArgs += ' -' + str(i) + ' ' + charsetPath
            hexCharset = content.hex()
            package['config'] += '|||charset' + str(i) + '|String|' + lenStr(
                hexCharset) + '|' + hexCharset + '|||\n'
            package['charset' + str(i) + '_id'] = charset['id']

    # compute keyspace
    package['mask_table'] = []
    package['keyspace'] = 0
    package['hc_keyspace'] = 0
    for (i, mask) in enumerate(package['attack_settings']['masks']):
        keyspace_for_mask = compute_keyspace_from_mask(mask, charsetsSize)
        hc_keyspace_for_mask = 0
        if hashcatKeyspace:
            hc_keyspace_for_mask = shellExec(
                HASHCAT_PATH + ' -m ' + package['hash_settings']['hash_type'] + ' --keyspace -a 3 ' + mask + ' ' + hashcatArgs,
                cwd=HASHCAT_DIR, abortOnError=True)
            if hc_keyspace_for_mask == '':
                abort(500, 'Server can not compute keyspace for mask ' + mask)
            try:
                package['hc_keyspace'] += int(hc_keyspace_for_mask)
            except ValueError:
                abort(500, 'Hashcat says: "' + hc_keyspace_for_mask + '".')

        package['keyspace'] += keyspace_for_mask
        package['mask_table'].append(
            {
                'mask': mask,
                'keyspace': keyspace_for_mask,
                'hc_keyspace': hc_keyspace_for_mask
            }
        )

    if package['attack_settings'].get('markov'):
        markov = FcHcstat.query.filter(FcHcstat.id == package['attack_settings']['markov']['id']).first()
        if not markov:
            abort(500, 'Wrong markov file selected.')
        if not os.path.exists(os.path.join(HCSTATS_DIR, markov.path)):
            abort(500, 'Markov file does not exist.')
        package['markov_hcstat'] = markov.name
        package['attack_settings']['attack_submode'] = 1

    markovTreshold = package['attack_settings']['markov_treshold'] if package['attack_settings'].get(
        'markov_treshold') and package['attack_settings']['markov_treshold'] > 1 else None

    package['attack_name'] = 'mask'
    package['markov_threshold'] = markovTreshold
    return package


def post_process_package_3(data, db_package):
    for mask in data['mask_table']:
        db_mask = FcMask(job_id=db_package.id,
                         mask=mask['mask'],
                         current_index='0',
                         keyspace=mask['keyspace'],
                         hc_keyspace=mask['hc_keyspace'])
        db.session.add(db_mask)


# hybrid attack
def process_package_6(package):
    package['attack_settings']['attack_submode'] = 0
    if package['attack_settings']['rule_left'] and package['attack_settings']['rule_right']:
        package['attack_settings']['attack_submode'] = 3
    elif package['attack_settings']['rule_left']:
        package['attack_settings']['attack_submode'] = 1
    elif package['attack_settings']['rule_right']:
        package['attack_settings']['attack_submode'] = 2


    check_mask_syntax(package['attack_settings']['mask'])
    rightDict = make_dict_from_mask(package['attack_settings']['mask'])
    package['attack_settings']['right_dictionaries'] = [ rightDict ]

    dictsLeftKeyspace = 0
    dictsRightKeyspace = rightDict.keyspace
    for dictObj in package['attack_settings']['left_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        dictsLeftKeyspace += dict.keyspace

    keyspace = dictsLeftKeyspace * dictsRightKeyspace

    package['attack_name'] = 'hybrid (Wordlist + Mask)'
    package['hc_keyspace'] = dictsLeftKeyspace
    package['keyspace'] = keyspace
    return package


def post_process_package_6(data, db_package):
    for dict in data['attack_settings']['left_dictionaries']:
        packageDict = FcJobDictionary(job_id=db_package.id, dictionary_id=dict['id'])
        db.session.add(packageDict)

    for dict in data['attack_settings']['right_dictionaries']:
        packageDict = FcJobDictionary(job_id=db_package.id, dictionary_id=dict.id, is_left=False)
        db.session.add(packageDict)

# hybrid attack
def process_package_7(package):
    package['attack_settings']['attack_submode'] = 0
    if package['attack_settings']['rule_left'] and package['attack_settings']['rule_right']:
        package['attack_settings']['attack_submode'] = 3
    elif package['attack_settings']['rule_left']:
        package['attack_settings']['attack_submode'] = 1
    elif package['attack_settings']['rule_right']:
        package['attack_settings']['attack_submode'] = 2

    check_mask_syntax(package['attack_settings']['mask'])
    leftDict = make_dict_from_mask(package['attack_settings']['mask'])
    package['attack_settings']['left_dictionaries'] = [ leftDict ]

    dictsLeftKeyspace = leftDict.keyspace
    dictsRightKeyspace = 0
    for dictObj in package['attack_settings']['right_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        dictsRightKeyspace += dict.keyspace

    keyspace = dictsLeftKeyspace * dictsRightKeyspace

    package['attack_name'] = 'hybrid (Mask + Wordlist)'
    package['hc_keyspace'] = dictsLeftKeyspace
    package['keyspace'] = keyspace
    return package


def post_process_package_7(data, db_package):
    for dict in data['attack_settings']['left_dictionaries']:
        packageDict = FcJobDictionary(job_id=db_package.id, dictionary_id=dict.id)
        db.session.add(packageDict)

    for dict in data['attack_settings']['right_dictionaries']:
        packageDict = FcJobDictionary(job_id=db_package.id, dictionary_id=dict['id'], is_left=False)
        db.session.add(packageDict)



