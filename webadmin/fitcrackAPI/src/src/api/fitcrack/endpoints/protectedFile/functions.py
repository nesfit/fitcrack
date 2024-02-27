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
from src.database import db
from src.database.models import FcEncryptedFile


ALLOWED_EXTENSIONS = set(["doc", "docx", "xls", "xlsx", "ppt", "pptx", "pdf", "rar", "zip", "7z"])


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


def addProtectedFile(file:typing.IO):
    uploadedFile = fileUpload(file, PROTECTEDFILES_DIR, ALLOWED_EXTENSIONS, withTimestamp=True)
    if uploadedFile:
        loadedHash = getHashFromFile(filename=uploadedFile['filename'], path=uploadedFile['path'])
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
            'file_id': encFile.id
        }
    else:
        abort(500, 'We only support ' + ', '.join(str(x) for x in ALLOWED_EXTENSIONS) + '.')