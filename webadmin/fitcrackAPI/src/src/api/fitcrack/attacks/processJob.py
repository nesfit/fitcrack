'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import sys
import os

from pathlib import Path
from flask_restplus import abort
from settings import HASHCAT_DIR, HASHCAT_PATH, EXE_OR_BIN, DICTIONARY_DIR, RULE_DIR, HCSTATS_DIR, CHARSET_DIR
from src.api.fitcrack.attacks.functions import check_mask_syntax, compute_keyspace_from_mask, compute_prince_keyspace
from src.api.fitcrack.functions import shellExec, lenStr
from src.database import db
from src.database.models import FcMask, FcDictionary, FcRule, FcHcstat, FcCharset, FcJobDictionary


# dictionary attack
def process_job_0(job):
    job['attack_settings']['attack_submode'] = 0
    job['hc_keyspace'] = 0

    for dictObj in job['attack_settings']['left_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        job['hc_keyspace'] += dict.keyspace

    ruleFileMultiplier = 1

    if job['attack_settings']['rules']:
        rules = FcRule.query.filter(FcRule.id == job['attack_settings']['rules']['id']).first()
        ruleFileMultiplier = rules.count

        if ruleFileMultiplier == 0:
            ruleFileMultiplier = 1

        if not rules:
            abort(500, 'Wrong rules file selected.')

        if not os.path.exists(os.path.join(RULE_DIR, rules.path)):
            abort(500, 'Rules file does not exist.')

        job['attack_settings']['attack_submode'] = 1
        job['rules'] = rules.name

    job['attack_name'] = 'dictionary'
    job['keyspace'] = job['hc_keyspace'] * ruleFileMultiplier

    return job

def post_process_job_0(data, db_job):
    for dict in data['attack_settings']['left_dictionaries']:
        jobDict = FcJobDictionary(job_id=db_job.id, dictionary_id=dict['id'])
        db.session.add(jobDict)


def process_job_1(job):
    job['attack_settings']['attack_submode'] = 0

    if job['attack_settings']['rule_left'] and job['attack_settings']['rule_right']:
        job['attack_settings']['attack_submode'] = 3

    elif job['attack_settings']['rule_left']:
        job['attack_settings']['attack_submode'] = 1

    elif job['attack_settings']['rule_right']:
        job['attack_settings']['attack_submode'] = 2

    dictsLeftKeyspace = 0
    dictsRightKeyspace = 0

    for dictObj in job['attack_settings']['left_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        dictsLeftKeyspace += dict.keyspace

    for dictObj in job['attack_settings']['right_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        dictsRightKeyspace += dict.keyspace


    keyspace = dictsLeftKeyspace * dictsRightKeyspace

    job['attack_name'] = 'combinator'
    job['hc_keyspace'] = dictsLeftKeyspace
    job['keyspace'] = keyspace
    return job


def post_process_job_1(data, db_job):
    for dict in data['attack_settings']['left_dictionaries']:
        jobDict = FcJobDictionary(job_id=db_job.id, dictionary_id=dict['id'])
        db.session.add(jobDict)

    for dict in data['attack_settings']['right_dictionaries']:
        jobDict = FcJobDictionary(job_id=db_job.id, dictionary_id=dict['id'], is_left=False)
        db.session.add(jobDict)



# mask attack
def process_job_3(job, hashcatKeyspace=True):
    if not job['attack_settings'].get('attack_submode'):
        job['attack_settings']['attack_submode'] = 0
    # check masks syntax
    for mask in job['attack_settings']['masks']:
        check_mask_syntax(mask)

    # charsets
    hashcatArgs = ''
    charsetsSize = {}
    if job['attack_settings'].get('charset'):
        if len(job['attack_settings']['charset']) > 4:
            abort(500, 'Maximum of charsets files is 4.')
        for i, charset in enumerate(job['attack_settings']['charset'], 1):
            charsetPath = os.path.join(CHARSET_DIR, charset['name'] + '.hcchr')
            charsetsSize = dict()
            with open(charsetPath, 'rb') as f:
                content = f.read()
                charsetsSize[i] = len(content)

            hashcatArgs += ' -' + str(i) + ' ' + charsetPath
            hexCharset = content.hex()
            job['charset' + str(i)] = hexCharset

    # compute keyspace
    if job['attack_settings'].get(
        'markov_treshold') and job['attack_settings']['markov_treshold'] > 1:
        markovTresh = job['attack_settings']['markov_treshold']
        job['markov_threshold'] = job['attack_settings']['markov_treshold']

        job['mask_table'] = []
        job['keyspace'] = 0
        job['hc_keyspace'] = 0
        for (i, mask) in enumerate(job['attack_settings']['masks']):
            keyspace_for_mask = compute_keyspace_from_mask(mask, charsetsSize, markovTresh)
            hc_keyspace_for_mask = 0
            if hashcatKeyspace:

                tmp = HASHCAT_PATH + ' -m ' + job['hash_settings']['hash_type'] + ' -a 3 ' + mask + ' --markov-threshold ' +  str(job['attack_settings']['markov_treshold'])
                hc_keyspace_for_mask = shellExec(
                    HASHCAT_PATH + ' -m ' + job['hash_settings']['hash_type'] + ' --keyspace -a 3 ' + mask + ' --markov-threshold ' +  str(job['attack_settings']['markov_treshold']),
                    cwd=HASHCAT_DIR, abortOnError=True)
                if hc_keyspace_for_mask == '':
                    abort(500, 'Server can not compute keyspace for mask ' + mask)
                try:
                    job['hc_keyspace'] += int(hc_keyspace_for_mask)
                except ValueError:
                    abort(500, 'Hashcat says: "' + hc_keyspace_for_mask + '".')

            job['keyspace'] += keyspace_for_mask
            job['mask_table'].append(
                {
                    'mask': mask,
                    'keyspace': keyspace_for_mask,
                    'hc_keyspace': hc_keyspace_for_mask
                }
            )

    else:
        job['mask_table'] = []
        job['keyspace'] = 0
        job['hc_keyspace'] = 0
        for (i, mask) in enumerate(job['attack_settings']['masks']):
            keyspace_for_mask = compute_keyspace_from_mask(mask, charsetsSize)
            hc_keyspace_for_mask = 0
            if hashcatKeyspace:
                hc_keyspace_for_mask = shellExec(
                    HASHCAT_PATH + ' -m ' + job['hash_settings']['hash_type'] + ' --keyspace -a 3 ' + mask + ' ' + hashcatArgs,
                    cwd=HASHCAT_DIR, abortOnError=True)
                if hc_keyspace_for_mask == '':
                    abort(500, 'Server can not compute keyspace for mask ' + mask)
                try:
                    job['hc_keyspace'] += int(hc_keyspace_for_mask)
                except ValueError:
                    abort(500, 'Hashcat says: "' + hc_keyspace_for_mask + '".')

            job['keyspace'] += keyspace_for_mask
            job['mask_table'].append(
                {
                    'mask': mask,
                    'keyspace': keyspace_for_mask,
                    'hc_keyspace': hc_keyspace_for_mask
                }
            )



    if job['attack_settings'].get('markov'):
        markov = FcHcstat.query.filter(FcHcstat.id == job['attack_settings']['markov']['id']).first()
        if not markov:
            abort(500, 'Wrong markov file selected.')
        if not os.path.exists(os.path.join(HCSTATS_DIR, markov.path)):
            abort(500, 'Markov file does not exist.')
        job['markov_hcstat'] = markov.name

    job['attack_name'] = 'mask'

    return job


def post_process_job_3(data, db_job):
    for mask in data['mask_table']:
        db_mask = FcMask(job_id=db_job.id,
                         mask=mask['mask'],
                         current_index='0',
                         keyspace=mask['keyspace'],
                         hc_keyspace=mask['hc_keyspace'])
        db.session.add(db_mask)


# hybrid attack
def process_job_6(job, actually7=False):
    job['attack_settings']['attack_submode'] = 0
    if job['attack_settings']['rule_left']:
        job['attack_settings']['attack_submode'] = 1
    elif job['attack_settings']['rule_right']:
        job['attack_settings']['attack_submode'] = 2

    mask = job['attack_settings']['mask']

    check_mask_syntax(mask)
    maskKeyspace = compute_keyspace_from_mask(mask)

    job['mask_data'] = {
        'mask': mask,
        'keyspace': maskKeyspace,
        'hc_keyspace': maskKeyspace
    }

    dictsKeyspace = 0
    for dictObj in job['attack_settings']['left_dictionaries' if not actually7 else 'right_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

        dictsKeyspace += dict.keyspace

    keyspace = dictsKeyspace * maskKeyspace

    job['attack_name'] = 'hybrid (Wordlist + Mask)' if not actually7 else 'hybrid (Mask + Wordlist)'
    job['hc_keyspace'] = dictsKeyspace if not actually7 else maskKeyspace
    job['keyspace'] = keyspace
    return job


def post_process_job_6(data, db_job):
    for dict in data['attack_settings']['left_dictionaries']:
        jobDict = FcJobDictionary(job_id=db_job.id, dictionary_id=dict['id'])
        db.session.add(jobDict)

    mask = data['mask_data']
    db_mask = FcMask(job_id=db_job.id,
                     mask=mask['mask'],
                     current_index='0',
                     keyspace=mask['keyspace'],
                     hc_keyspace=mask['hc_keyspace'])
    db.session.add(db_mask)

# hybrid attack
def process_job_7(job):
    return process_job_6(job, True)


def post_process_job_7(data, db_job):
    mask = data['mask_data']
    db_mask = FcMask(job_id=db_job.id,
                     mask=mask['mask'],
                     current_index='0',
                     keyspace=mask['keyspace'],
                     hc_keyspace=mask['hc_keyspace'])
    db.session.add(db_mask)

    for dict in data['attack_settings']['right_dictionaries']:
        jobDict = FcJobDictionary(job_id=db_job.id, dictionary_id=dict['id'], is_left=False)
        db.session.add(jobDict)

# PRINCE attack
def process_job_8(job):
    job['attack_settings']['attack_submode'] = 0

    for dictObj in job['attack_settings']['left_dictionaries']:
        dict = FcDictionary.query.filter(FcDictionary.id == dictObj['id']).first()
        if not dict:
            abort(500, 'Wrong dictionary selected.')

        if not os.path.exists(os.path.join(DICTIONARY_DIR, dict.path)):
            abort(500, 'Dictionary does not exist.')

    job['hc_keyspace'] = compute_prince_keyspace(job['attack_settings'])
    if job['hc_keyspace'] == -1:
        abort(400, 'Unable to compute job keyspace.')
    random_rules_count = 0
    if job['attack_settings']['generate_random_rules']:
        random_rules_count = int(job['attack_settings']['generate_random_rules'])
    ruleFileMultiplier = random_rules_count

    if job['attack_settings']['rules']:
        rules = FcRule.query.filter(FcRule.id == job['attack_settings']['rules']['id']).first()
        ruleFileMultiplier = rules.count

        if not rules:
            abort(500, 'Wrong rules file selected.')

        if not os.path.exists(os.path.join(RULE_DIR, rules.path)):
            abort(500, 'Rules file does not exist.')

        job['attack_settings']['attack_submode'] = 1
        job['rules'] = rules.name

    prince_settings = ["case_permute", "check_duplicates", "min_password_len", "max_password_len", "min_elem_in_chain", "max_elem_in_chain", "shuffle_dict", 'generate_random_rules']
    for setting in prince_settings:
        job[setting] = job['attack_settings'][setting]

    job['attack_name'] = 'prince'

    # Keyspace limit control
    if job['attack_settings']['keyspace_limit'] < job['hc_keyspace']:
        job['hc_keyspace'] = job['attack_settings']['keyspace_limit']

    if ruleFileMultiplier == 0:
        job['keyspace'] = job['hc_keyspace']
    else:
        job['keyspace'] = job['hc_keyspace'] * ruleFileMultiplier

    return job

def post_process_job_8(data, db_job):
    for dict in data['attack_settings']['left_dictionaries']:
        jobDict = FcJobDictionary(job_id=db_job.id, dictionary_id=dict['id'])
        db.session.add(jobDict)

# pcfg attack
def process_job_9(job):
    job['attack_settings']['attack_submode'] = 0
    job['attack_name'] = 'pcfg'

    # Keyspace control
    INT_MAX = sys.maxsize - 1

    ruleFileMultiplier = 1

    if job['attack_settings']['rules']:
        rules = FcRule.query.filter(FcRule.id == job['attack_settings']['rules']['id']).first()
        ruleFileMultiplier = rules.count

        if ruleFileMultiplier == 0:
            ruleFileMultiplier = 1

        if not rules:
            abort(500, 'Wrong rules file selected.')

        if not os.path.exists(os.path.join(RULE_DIR, rules.path)):
            abort(500, 'Rules file does not exist.')

        job['attack_settings']['attack_submode'] = 1
        job['rules'] = rules.name
        print("\nRules:")
        print(str(ruleFileMultiplier) + "\n")

    if (int(job['attack_settings']['pcfg_grammar']['keyspace']) * int(ruleFileMultiplier)) >= INT_MAX:
        job['keyspace'] = INT_MAX
        job['attack_settings']['pcfg_grammar']['keyspace'] = INT_MAX

        if int(job['attack_settings']['pcfg_grammar']['keyspace']) >= INT_MAX:
            job['hc_keyspace'] = INT_MAX

        else:   job['hc_keyspace'] = job['attack_settings']['pcfg_grammar']['keyspace']

    else:
        job['keyspace'] = int(job['attack_settings']['pcfg_grammar']['keyspace']) * int(ruleFileMultiplier)
        job['hc_keyspace'] = job['attack_settings']['pcfg_grammar']['keyspace']

    # Keyspace limit control
    if (int(job['attack_settings']['pcfg_grammar']['keyspace'])) >= int(job['attack_settings']['keyspace_limit']):
        job['hc_keyspace'] = job['attack_settings']['keyspace_limit']
        job['keyspace'] = job['attack_settings']['keyspace_limit'] * int(ruleFileMultiplier)

    print("\nKeyspace: ")
    print(str(job['keyspace']) + "\n")
    print("\nHC_Keyspace: ")
    print(str(job['hc_keyspace']) + "\n")

    return job


def post_process_job_9(data, db_job):
    print("\n PCFG attack post_process\n")

