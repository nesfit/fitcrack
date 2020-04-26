
'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
import re
import time

from flask_restplus import abort
from werkzeug.utils import secure_filename

from settings import DICTIONARY_DIR, MASK_PROCESSOR_PATH, HASHCAT_EXECUTABLE, PRINCE_PROCESSOR_PATH
from src.api.fitcrack.functions import shellExec
from src.database import db
from src.database.models import FcDictionary


def make_dict_from_mask(mask, filename=None):
    if not filename:

        filename = 'hybrid_attack' + str(int(time.time())) + '.txt'

    filename = secure_filename(filename)
    path = os.path.join(DICTIONARY_DIR, filename)


    print(MASK_PROCESSOR_PATH + ' ' + mask + ' -o ' + path)
    print(shellExec(MASK_PROCESSOR_PATH + ' ' + mask + ' -o ' + path))

    dictionary = FcDictionary(name=filename, path=filename, keyspace=compute_keyspace_from_mask(mask), deleted=True)
    db.session.add(dictionary)
    db.session.commit()
    return dictionary


def check_mask_syntax(mask):
    if not re.fullmatch("^(\?[ludhHsab]|[ -~])+$", mask):
        abort(400, 'Wrong mask ' + mask)


def compute_keyspace_from_mask(mask, charsetsSize=dict(), thresh=None):
    keyspace = 1
    nextCharSymbol = False
    for char in mask:
        if nextCharSymbol:
            multiplier = keyspace_dict.get(char, None)
            try:
                if int(char) >= 1 and int(char) <= 4:
                    multiplier = charsetsSize.get(int(char), None)
            except ValueError:
                pass

            if not multiplier:
                continue

            if thresh and multiplier > thresh:
                multiplier = thresh 
            keyspace *= multiplier
            nextCharSymbol = False
            continue
        if char == '?':
            nextCharSymbol = True
        else:
            nextCharSymbol = False


    return keyspace

def compute_prince_keyspace(attackSettings):
    run_cmd = ["cat"]
    for dict in attackSettings['left_dictionaries']:
        run_cmd.append(os.path.join(DICTIONARY_DIR, dict['name']))
    run_cmd.append("|")
    run_cmd.append(PRINCE_PROCESSOR_PATH)
    if attackSettings['case_permute']:
        run_cmd.append("--case-permute")
    if not attackSettings['check_duplicates']:
        run_cmd.append("--dupe-check-disable")
    if attackSettings['min_password_len']:
        run_cmd.append("--pw-min=" + str(attackSettings['min_password_len']))
    if attackSettings['max_password_len']:
        run_cmd.append("--pw-max=" + str(attackSettings['max_password_len']))
    if attackSettings['min_elem_in_chain']:
        run_cmd.append("--elem-cnt-min=" + str(attackSettings['min_elem_in_chain']))
    if attackSettings['max_elem_in_chain']:
        run_cmd.append("--elem-cnt-max=" + str(attackSettings['max_elem_in_chain']))
    run_cmd.append("--keyspace")
    compute_keyspace_command = ' '.join(run_cmd)
    print(compute_keyspace_command)
    try:
        return int(shellExec(compute_keyspace_command))
    except Exception:
        return -1

keyspace_dict = {
    'l': 26,
    'u': 26,
    'd': 10,
    'h': 16,
    'H': 16,
    's': 33,
    'a': 95,
    'b': 256
}
