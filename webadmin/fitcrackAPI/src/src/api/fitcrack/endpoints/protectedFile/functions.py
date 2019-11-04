'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
from flask_restplus import abort

from settings import XTOHASHCAT_PATH, XTOHASHCAT_EXECUTABLE, PROTECTEDFILES_DIR
from src.api.fitcrack.functions import shellExec


def getHashFromFile(filename, path):
    res = shellExec('python3 ' + XTOHASHCAT_EXECUTABLE + ' ' + os.path.join(PROTECTEDFILES_DIR, path), cwd=XTOHASHCAT_PATH, getReturnCode=True)
    if res['returnCode'] == 2:
        abort(500, 'Hashcat doesn\'t support PKZIP.')
    if res['returnCode'] != 0:
        abort(500, 'Could not extract hash from file.')
    res = res['msg'].split('\n')
    return {
        'hash': res[0],
        'hash_type': res[1]
    }
