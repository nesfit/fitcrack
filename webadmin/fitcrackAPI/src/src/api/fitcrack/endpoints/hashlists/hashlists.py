'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import base64
import logging
import io

from flask import request, redirect, send_file
from flask_restx import Resource, abort

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.hashlists.argumentsParser import make_empty_hash_list_parser, hash_list_parser, hash_list_add_hash_list_parser, hash_list_add_hash_file_parser, hash_list_hashes_parser_paginated, hash_list_hashes_parser, hash_list_add_protected_file_parser
from src.api.fitcrack.endpoints.hashlists.responseModels import empty_hash_list_created_model, page_of_hash_lists_model, hash_addition_result_model, page_of_hashes_model, hash_list_model_long
from src.api.fitcrack.endpoints.hashlists.functions import upload_hash_list, build_hash_query, acquire_hash_list
from src.api.fitcrack.endpoints.protectedFile.functions import addProtectedFile
from src.database import db
from src.database.models import FcHashList, FcHash

log = logging.getLogger(__name__)
ns = api.namespace('hashlist', description='Endpoints for work with hash lists.')


@ns.route('')
class hashListCollection(Resource):
    @api.expect(hash_list_parser)
    @api.marshal_with(page_of_hash_lists_model)
    def get(self):
        """
        Returns a paginated list of hash lists.
        """
        args = hash_list_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        hash_list_query = FcHashList.query

        hash_list_query = hash_list_query.filter_by(deleted=args.showDeleted)

        if args.name:
            hash_list_query = hash_list_query.filter(FcHashList.name.like('%' + args.name + '%'))

        if args.hash_type:
            hash_list_query = hash_list_query.filter(FcHashList.hash_type.like('%' + args.hash_type + '%'))

        if args.order_by:
            orderBy = getattr(FcHashList, args.order_by)
            if args.descending:
                orderBy = orderBy.desc()
            hash_list_query = hash_list_query.order_by(orderBy)
        else:
            if args.descending:
                hash_list_query = hash_list_query.order_by(FcHashList.id.desc())
            else:
                hash_list_query = hash_list_query.order_by(FcHashList.id.asc())

        hash_list_page = hash_list_query.paginate(page,per_page,error_out=True)

        return hash_list_page

    @api.marshal_with(empty_hash_list_created_model)
    @api.expect(make_empty_hash_list_parser)
    def post(self):
        """
        Creates a new empty hash list on the server with the given name.
        """
        args = make_empty_hash_list_parser.parse_args(request)

        hash_list = FcHashList(name=args['name'],hash_type=None)
        db.session.add(hash_list)
        db.session.commit()

        return {
            'result' : 'OK',
            'name' : hash_list.name,
            'id' : hash_list.id,
        }


@ns.route('/<int:id>')
class HashListHashes(Resource):
    @api.marshal_with(hash_list_model_long)
    def get(self,id:int):
        """
        Returns details about a hash list.
        """
        hash_list : FcHashList = FcHashList.query.filter(FcHashList.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')
        
        return hash_list

    @api.response(204, 'Hash list moved.')
    def delete(self, id):
        """
        Moves the hash list to the trash bin or restores it.
        """
        hash_list : FcHashList = FcHashList.query.filter(FcHashList.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')
        hash_list.deleted = not hash_list.deleted
        db.session.commit()
        return None, 204

@ns.route('/<int:id>/details')
class HashListHashes(Resource):
    @api.expect(hash_list_hashes_parser_paginated)
    @api.marshal_with(page_of_hashes_model)
    def get(self,id:int):
        """
        Returns a paginated list of hashes of the given hash list.
        """
        hash_list : FcHashList = FcHashList.query.filter(FcHashList.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')

        args = hash_list_hashes_parser_paginated.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        hash_query = build_hash_query(args,id)

        hash_page = hash_query.paginate(page,per_page,error_out=True)

        return hash_page

@ns.route('/<int:id>/download')
class exportHashes(Resource):
    @api.expect(hash_list_hashes_parser)
    def get(self, id:int):
        """
        Downloads a file containing hashes and cracked passwords in the given hash list.
        The user can set if they want all hashes or only hashes with cracked passwords.
        """
        hash_list_file = io.BytesIO()
        
        hash_list : FcHashList = FcHashList.query.filter(FcHashList.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')
        
        args = hash_list_hashes_parser.parse_args(request)
        
        hash_query = build_hash_query(args,id)

        hashes : list[FcHash] = hash_query.all()

        for hash in hashes:
            hash_list_file.write(hash.hashText.encode('utf-8'))
            hash_list_file.write(b':')
            hash_list_file.write(hash.password.encode('utf-8') if hash.password is not None else b'')
            hash_list_file.write(b'\n')

        hash_list_file.seek(0)
        filename = hash_list.name + ".txt"
        return send_file(hash_list_file, attachment_filename=filename, as_attachment=True, mimetype="text/plain")

@ns.route('/<id>/hashes')
class hashListUploadList(Resource):
    
    @api.expect(hash_list_add_hash_list_parser)
    @api.marshal_with(hash_addition_result_model)
    def post(self,id:str):
        """
        Takes a list of string hashes and ads them to hash list with the given id.

        'id' can be the string "new". In that case, instead of adding hashes to an existing hash list, a new one is created.
        """
        data = request.json
        if 'validation_mode' not in data or data['validation_mode'] not in ['fail_invalid', 'skip_invalid', 'no_validate']:
            abort(400,'validation_mode not set or set to invalid value')
        
        hash_list = acquire_hash_list(id,data['name'])

        lifted_hashes : list[str] = [hashObj['hash'] for hashObj in data['hash_list']]

        return upload_hash_list(lifted_hashes,hash_list,data['hash_type'],data['validation_mode'],False)

        
@ns.route('/<id>/file')
class hashListUploadHashFile(Resource):
    @api.expect(hash_list_add_hash_file_parser)
    @api.marshal_with(hash_addition_result_model)
    def post(self,id:str):
        """
        Takes a hash file (a text or binary file containing hashes),
        and adds the hashes contained within into the hash list with the given id.

        'id' can be the string "new". In that case, instead of adding hashes to an existing hash list, a new one is created.
        """
        args = hash_list_add_hash_file_parser.parse_args(request)

        hash_list = acquire_hash_list(id,args.name)

        if args['file'].filename == '': #I don't know if we still need this... since we are using this new and fancy way...? I suppose we do?
            abort(500, 'No selected file')

        if (args['binary'] != True):
            #We get universal newline support by wrapping the binary IO into a string IO wrapper
            text_io_wrapper = io.TextIOWrapper(args['file'].stream._file,encoding='ascii', errors='surrogateescape')
            new_hashes = text_io_wrapper.read().splitlines()
        else:
            new_hashes = [args['file'].read()]

        return upload_hash_list(new_hashes,hash_list,args['hash_type'],args['validation_mode'],args['binary'])


@ns.route('/<id>/extract')
class hashListUploadHashFile(Resource):
    @api.expect(hash_list_add_protected_file_parser)
    @api.marshal_with(hash_addition_result_model)
    def post(self,id:str):
        """
        Extract the hash from the given protected file and adds it to the hash list with the given id.
        This also creates a ProtectedFile row in the database, just like the old ProtectedFile endpoint.

        'id' can be the string "new". In that case, instead of adding hashes to an existing hash list, a new one is created.
        """
        args = hash_list_add_protected_file_parser.parse_args(request)

        hash_list = acquire_hash_list(id,args.name)
        
        if args['file'].filename == '': #I don't know if we still need this... since we are using this new and fancy way...? I suppose we do?
            abort(500, 'No selected file')

        result = addProtectedFile(args.file)

        return upload_hash_list([result['hash']],hash_list,int(result['hash_type']),'fail_invalid',False)