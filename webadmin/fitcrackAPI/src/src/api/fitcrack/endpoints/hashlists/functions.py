'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''
from __future__ import annotations

import base64
import tempfile
from typing import TYPE_CHECKING, Optional, Literal

from flask_restx import abort

from src.api.fitcrack.endpoints.job.functions import verifyHashFormat
from src.database import db
from src.database.models import FcHash, FcHashList

if TYPE_CHECKING:
    from flask_restx import reqparse

def upload_hash_list(new_hashes:list[str|bytes],hash_list:FcHashList,hash_type:int,validation_mode:Literal['fail_invalid']|Literal['skip_invalid']|Literal['no_validate'], binary_hash:bool):
    """
    Takes a list of strings or bytes objects representing hashes and adds them to the given hash list.

    If an entry in the list is a bytes object, it will be added to the hash list as is.
    
    If an entry in the list is a string object, it will be converted to a bytes object like so:
    If it starts with "BASE64:", the object will be base64 decoded, otherwise it will be converted
    to its utf-8 representation.

    If hash_list does not have a hash type, then this sets the hash list to the given type.
    If hash_list does have a type and the input hash_type does not match, then this function fails and aborts.
    If hash_list is locked, this function fails and does not add anything.
    """

    def hash_to_bytes(hashObj:str|bytes) -> bytes:
        if type(hashObj) == bytes:
            return hashObj
        if hashObj.startswith('BASE64:'):
            return base64.decodebytes(hashObj[7:].encode())
        else:
            return hashObj.encode()


    if hash_list.is_locked:
        abort(400,'Hash list is locked for editing.')
    
    new_hashes_bin_list = list(map(hash_to_bytes, new_hashes))
    new_hashes_bin = set(new_hashes_bin_list)

    if not binary_hash:
        validation_result = validate_hash_list(new_hashes_bin,hash_type,False)
    else:
        if len(new_hashes_bin) != 1:
            abort(500,'If input is binary, then hash list may only contain one hash.')
        base_64 = base64.encodebytes(list(new_hashes_bin)[0]).decode("ascii")
        validation_result = validate_hash_list(new_hashes_bin,hash_type,False, f'BASE64<{base_64[0:64]}{"..." if len(base_64) > 64 else ""}>') #Placeholder is the first 64 characters of BASE 64 representation

    errored_validation_results = [x for x in validation_result['items'] if x['result'] != 'OK']
    error_output = {bytes(x['hash'],encoding='ascii'):x['result'] for x in errored_validation_results}

    if validation_result['error'] == True:
        if validation_mode == 'fail_invalid':
            return {
            'result' : 'FUCKED',
            'id' : hash_list.id,
            'name' : hash_list.name,
            'hashCount' : hash_list.hash_count,
            'addedCount' : 0,
            'erroredCount' : len(errored_validation_results),
            'errors' : [{
                'order_in_input': new_hashes_bin_list.index(hash),
                'error' : error}
                for (hash,error) in error_output.items()]
            },400
        if validation_mode == 'skip_invalid':
            new_hashes_bin = new_hashes_bin - error_output.keys()

    if hash_list.hash_type is None:
        hash_list.hash_type = hash_type
    elif hash_list.hash_type != hash_type:
        abort(400,f'Hash-type mismatch. Trying to add hashes of type {hash_type} to hash list of type {hash_list.hash_type}.')
    
    pre_existing_hashes : set[bytes] = {x[0] for x in db.session.query(FcHash.hash).filter((FcHash.hash_list == hash_list) & (FcHash.hash.in_(new_hashes_bin))).all()}

    hashes_to_add = new_hashes_bin - pre_existing_hashes

    for hash_binary_string in hashes_to_add:
        hash = FcHash(hash_list_id=hash_list.id, hash_type=hash_type, hash=hash_binary_string)
        db.session.add(hash)

    #WARNING: This function is called at the end of endpoints, so the db session might contain
    #more objects than just the hashhes added in this function. For example, the adding endpoints
    #have a "new" variant that also adds a new hash list.
    db.session.commit()
    
    return {
        'result' : 'OK',
        'id' : hash_list.id,
        'name' : hash_list.name,
        'hashCount' : hash_list.hash_count,
        'addedCount' : len(hashes_to_add),
        'erroredCount' : 0 if validation_mode == 'no_validate' else len(errored_validation_results),
        'errors' : [] if validation_mode == 'no_validate' else [
            {'order_in_input': new_hashes_bin_list.index(hash),
             'error' : error}
            for (hash,error) in error_output.items()
            ]
    },200


def validate_hash_list(hash_list:list[bytes],hash_type:str,valid_only:bool,binary_placeholder:Optional[str]=None):
    '''
    This was taken from the former add job endpoint. I don't exactly get what this
    is doing as of writing this comment, but it's verifying hashes, I suppose.
    The parameters hash_list, hash_type, and valid_only are the same as the ones
    in hash_list_add_hash_list_parser in argumentsParser.py in this module.

    TODO: I don't think this does anything if valid_only is set to false?
    But this behaviour was present in the old endpoint as well?
    '''
    if binary_placeholder and len(hash_list) != 1:
        abort(500,'Input list of hashes may contain only one binary hash.')
    hashes = b'\n'.join(hash_list)
    with tempfile.NamedTemporaryFile() as fp:
        fp.write(hashes)
        fp.seek(0)
        return verifyHashFormat(fp.name, hash_type, abortOnFail=valid_only,binaryHash=binary_placeholder)


def build_hash_query(args:reqparse.ParseResult,hash_list_id:int):
    """
    Builds an FcHash query for requests using
    `src.api.fitcrack.endpoints.hashlists.argumentsParser.hash_list_hashes_parser`
    as input. Does not handle the paginated version differently from the non-paginated version.
    So you need to add the pagination bits yourself. The purpose of this function is to de-deduplicate
    the building the query (the same query is used in two places as of writing this).
    """
    hash_query = FcHash.query

    hash_query = hash_query.filter_by(hash_list_id=hash_list_id)

    if args.cracked is not None:
        if args.cracked:
            hash_query = hash_query.filter(FcHash.result != None)
        else:
            hash_query = hash_query.filter(FcHash.result == None)

    if args.order_by:
        orderBy = getattr(FcHash, args.order_by)
        if args.descending:
            orderBy = orderBy.desc()
        hash_query = hash_query.order_by(orderBy)
    else:
        if args.descending:
            hash_query = hash_query.order_by(FcHash.id.desc())
        else:
            hash_query = hash_query.order_by(FcHash.id.asc())

    return hash_query


def acquire_hash_list(id:str,name:Optional[str]=None) -> FcHashList:
    """
    Common function for all "add hashes to hash list" endpoints. Given an id, returns an FcHashList object.
    
    If id is a number, the function will pull the hash list from the database.
    If id is the string "new", it will create a new hash list object and add it to the db.session
    (but will not commit the session). The hash list will be named after the 'name' argument from the supplied args
    or a placeholder name will be used if name is not supplied.
    """
    if id == 'new':
        hash_list = FcHashList(name=name if name is not None else f'Unnamed automatically created hash list')
        db.session.add(hash_list)
    else:
        try:
            id = int(id)
        except ValueError:
            abort(400,'id is neither an integer nor "new"')
        hash_list : FcHashList = FcHashList.query.filter(FcHashList.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')
    return hash_list
