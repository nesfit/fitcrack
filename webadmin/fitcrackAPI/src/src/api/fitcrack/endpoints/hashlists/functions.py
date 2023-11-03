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

def upload_hash_list(new_hashes:list[str],hash_list:FcHashlist,hash_type:int,valid_only:bool):
    """
    Takes a list of string hashes and adds them to the given hash list.

    If hash_list does not have a hash type, then this sets the hash list to the given type.
    If hash_list does have a type and the input hash_type does not match, then this function fails and aborts.
    """

    def convert_hash_list_to_binary(hashObj:str):
        if hashObj.startswith('BASE64:'):
            return base64.decodebytes(hashObj[7:].encode())
        else:
            return hashObj.encode()
    
    validate_hash_list(new_hashes,hash_type,valid_only)

    hash_list_bin = map(convert_hash_list_to_binary, new_hashes) #Warning: map instead of list; beware if you need to read twice.

    #TODO: What kind of behaviour do we want from the endpoint?
    #TODO: As of now, it just stupidly appends.

    #TODO: We definitely just want to append the good hashes, or not?
    #TODO: We can create invalid hashes, so we need a toggle to set whether we want to accept; which we already do I suppose.

    if hash_list.hash_type is None:
        hash_list.hash_type = hash_type
    elif hash_list.hash_type != hash_type:
        abort(400,f'Hash type mismatch. Trying to add hashes of type {hash_type} to hash list of type {hash_list.hash_type}.')

    #TODO: I think we want a nice duplicate check like in a set?
    for hashObj in hash_list_bin:
        hash = FcHash(hashlist_id=hash_list.id, hash_type=hash_type, hash=hashObj)
        db.session.add(hash)

    db.session.commit()
    #Holy fuck, we need rollbacks; the return may fail. Or just make all the failable actions first and then just do a return of vars.
    
    return {
        'result' : 'OK',
        'id' : hash_list.id,
        'name' : hash_list.name,
        'hashCount' : hash_list.hash_count,
        'addedCount' : len(new_hashes),
        'erroredCount' : 0
    }

def validate_hash_list(hash_list:list[str],hash_type:str,valid_only:bool):
    '''
    This was taken from the former add job endpoint. I don't exactly get what this
    is doing as of writing this comment, but it's verifying hashes, I suppose.
    The parameters hash_list, hash_type, and valid_only are the same as the ones
    in hash_list_add_hash_list_parser in argumentsParser.py in this module.

    TODO: I don't think this does anything if valid_only is set to false?
    But this behaviour was present in the old endpoint as well?
    '''
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