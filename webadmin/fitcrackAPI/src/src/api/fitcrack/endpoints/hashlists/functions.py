'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import base64
import tempfile

from flask_restx import abort

from src.api.fitcrack.endpoints.job.functions import verifyHashFormat
from src.database import db
from src.database.models import FcHash, FcHashlist


def upload_hash_list(new_hashes:list[str|bytes],hash_list:FcHashlist,hash_type:int,valid_only:bool):
    """
    Takes a list of strings or bytes objects representing hashes and adds them to the given hash list.

    If an entry in the list is a bytes object, it will be added to the hash list as is.
    
    If an entry in the list is a string object, it will be converted to a bytes object like so:
    If it starts with "BASE64:", the object will be base64 decoded, otherwise it will be converted
    to its utf-8 representation.

    If hash_list does not have a hash type, then this sets the hash list to the given type.
    If hash_list does have a type and the input hash_type does not match, then this function fails and aborts.
    """

    def hash_to_bytes(hashObj:str|bytes) -> bytes:
        if type(hashObj) == bytes:
            return hashObj
        if hashObj.startswith('BASE64:'):
            return base64.decodebytes(hashObj[7:].encode())
        else:
            return hashObj.encode()
    
    validate_hash_list(new_hashes,hash_type,valid_only,False)

    new_hashes_bin = set(map(hash_to_bytes, new_hashes))

    if hash_list.hash_type is None:
        hash_list.hash_type = hash_type
    elif hash_list.hash_type != hash_type:
        abort(400,f'Hash-type mismatch. Trying to add hashes of type {hash_type} to hash list of type {hash_list.hash_type}.')
    
    pre_existing_hashes : set[bytes] = {x[0] for x in db.session.query(FcHash.hash).filter((FcHash.hashlist == hash_list) & (FcHash.hash.in_(new_hashes_bin))).all()}

    hashes_to_add = new_hashes_bin - pre_existing_hashes

    for hash_binary_string in hashes_to_add:
        hash = FcHash(hashlist_id=hash_list.id, hash_type=hash_type, hash=hash_binary_string)
        db.session.add(hash)

    db.session.commit()
    
    return {
        'result' : 'OK',
        'id' : hash_list.id,
        'name' : hash_list.name,
        'hashCount' : hash_list.hash_count,
        'addedCount' : len(hashes_to_add),
        'erroredCount' : 0
    }

def validate_hash_list(hash_list:list[str],hash_type:str,valid_only:bool,binary_hash = False): ...
def validate_hash_list(hash_list:list[bytes],hash_type:str,valid_only:bool,binary_hash = True): ...
def validate_hash_list(hash_list:list[str|bytes],hash_type:str,valid_only:bool,binary_hash:bool):
    '''
    This was taken from the former add job endpoint. I don't exactly get what this
    is doing as of writing this comment, but it's verifying hashes, I suppose.
    The parameters hash_list, hash_type, and valid_only are the same as the ones
    in hash_list_add_hash_list_parser in argumentsParser.py in this module.

    TODO: I don't think this does anything if valid_only is set to false?
    But this behaviour was present in the old endpoint as well?
    '''
    if binary_hash:
        if len(hash_list) != 1:
            abort(400,'Binary hash list may contain only exactly one binary hash.')
        with tempfile.NamedTemporaryFile() as fp:
            fp.write(hash_list[0])
            fp.seek(0)
            verifyHashFormat(fp.name, hash_type, abortOnFail=valid_only, binaryHash=hashes)
    else:
        hashes = '\n'.join(hash_list)
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