'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import base64
import tempfile

from typing import TypedDict

from src.api.fitcrack.endpoints.job.functions import verifyHashFormat

HashListItem = TypedDict('HashListItem',{'hash' : str})

def validate_hash_list(hash_list:list[HashListItem],hash_type:str,valid_only:bool):
    '''
    This was taken from the former add job endpoint. I don't exactly get what this
    is doing as of writing this comment, but it's verifying hashes, I suppose.
    The parameters hash_list, hash_type, and valid_only are the same as the ones
    in hash_list_add_hash_list_parser in argumentsParser.py in this module.

    TODO: I don't think this does anything if valid_only is set to false?
    But this behaviour was present in the old endpoint as well?
    '''
    hashes = '\n'.join([hashObj['hash'] for hashObj in hash_list])
    if hashes.startswith('BASE64:'):
        decoded = base64.decodebytes(hashes[7:].encode())
        with tempfile.NamedTemporaryFile() as fp:
            fp.write(decoded)
            fp.seek(0)
            verifyHashFormat(fp.name, hash_type, abortOnFail=valid_only, binaryHash=hashes)
    else:
        with tempfile.NamedTemporaryFile() as fp:
            fp.write(hashes.encode())
            fp.seek(0)
            verifyHashFormat(fp.name, hash_type, abortOnFail=valid_only)