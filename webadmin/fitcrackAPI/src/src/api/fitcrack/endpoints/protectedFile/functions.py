'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
import typing

from flask_restx import abort
from sqlalchemy import exc

from settings import XTOHASHCAT_PATH, XTOHASHCAT_EXECUTABLE, PROTECTEDFILES_DIR
from src.api.fitcrack.functions import shellExec, fileUpload
from src.api.fitcrack.endpoints.hashlists.functions import validate_hash_list
from src.database import db
from src.database.models import FcEncryptedFile


ALLOWED_EXTENSIONS = set(["doc", "docx", "xls", "xlsx", "ppt", "pptx", "pdf", "rar", "zip", "7z"])


def getHashFromFile(filename, path, extract_easy_hash:bool=False):
    res = shellExec('python3 ' + XTOHASHCAT_EXECUTABLE + (' -e ' if extract_easy_hash else ' ') + os.path.join(PROTECTEDFILES_DIR, path), cwd=XTOHASHCAT_PATH, getReturnCode=True)
    if res['returnCode'] == 2:
        abort(500, 'Hashcat doesn\'t support PKZIP.')
    if res['returnCode'] != 0:
        abort(500, 'Could not extract hash from file.')
    res = res['msg'].split('\n')
    return {
        'hash': res[0],
        'hash_type': res[1]
    }


def addProtectedFile(file:typing.IO):
    """
    Extracts and verifies a hash from a protected file.
    
    On success, returns a dict for outputting to the API user.
    The protected file and its hash is added to the FcEncryptedFile table.

    If at first the verification of the extracted hash fails, the function
    tries extracting the hash again with the "easy hash" option, which
    produces a potentially shorter hash (with an increased chance of false
    positives). Due to its shorter length, this hash may pass verification;
    this is because hashcat has some limits on hash length. If the hash still
    cannot be verified, this function aborts.
    """
    
    uploadedFile = fileUpload(file, PROTECTEDFILES_DIR, ALLOWED_EXTENSIONS, withTimestamp=True)
    if uploadedFile:
        easyHash = False
        loadedHash = getHashFromFile(filename=uploadedFile['filename'], path=uploadedFile['path'], extract_easy_hash=False)
        verifyResult = validate_hash_list([loadedHash['hash'].encode('ascii')],loadedHash['hash_type'],False)
        if verifyResult['error']:
            easyHash = True
            loadedHash = getHashFromFile(filename=uploadedFile['filename'], path=uploadedFile['path'], extract_easy_hash=True)
            verifyResult = validate_hash_list([loadedHash['hash'].encode('ascii')],loadedHash['hash_type'],False)
            if verifyResult['error']:
                abort(500, 'Could not extract hash from file.')
        
        encFile = FcEncryptedFile(name=uploadedFile['filename'], path=uploadedFile['path'], hash=loadedHash['hash'].encode(),
                                    hash_type=loadedHash['hash_type'])
        try:
            db.session.add(encFile)
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'File with name ' + uploadedFile['filename'] + ' already exists.')
        return {
            'message': 'Successfully extracted hash from uploaded file.',
            'status': True,
            'hash': loadedHash['hash'],
            'hash_type': loadedHash['hash_type'],
            'hash_type_name': encFile.hash_type_name,
            'file_id': encFile.id,
            'easy_hash': easyHash
        }
    else:
        abort(500, 'We only support ' + ', '.join(str(x) for x in ALLOWED_EXTENSIONS) + '.')