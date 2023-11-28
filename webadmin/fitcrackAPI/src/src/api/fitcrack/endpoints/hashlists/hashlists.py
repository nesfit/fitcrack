'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import base64
import logging
import io

from flask import request, redirect
from flask_restx import Resource, abort

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.hashlists.argumentsParser import make_empty_hash_list_parser, hash_list_parser, hash_list_add_hash_list_parser, hash_list_add_hash_file_parser, hash_list_hashes_parser
from src.api.fitcrack.endpoints.hashlists.responseModels import empty_hash_list_created_model, page_of_hash_lists_model, hash_addition_result_model, hash_list_model, page_of_hashes_model
from src.api.fitcrack.endpoints.hashlists.functions import upload_hash_list
from src.api.fitcrack.endpoints.protectedFile.functions import addProtectedFile
from src.database import db
from src.database.models import FcHashlist, FcHash

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

        hash_list_query = FcHashlist.query

        hash_list_query = hash_list_query.filter_by(deleted=args.showDeleted)

        if args.name:
            hash_list_query = hash_list_query.filter(FcHashlist.name.like('%' + args.name + '%'))

        if args.hash_type:
            hash_list_query = hash_list_query.filter(FcHashlist.hash_type.like('%' + args.hash_type + '%'))

        if args.order_by:
            orderBy = getattr(FcHashlist, args.order_by)
            if args.descending:
                orderBy = orderBy.desc()
            hash_list_query = hash_list_query.order_by(orderBy)
        else:
            if args.descending:
                hash_list_query = hash_list_query.order_by(FcHashlist.id.desc())
            else:
                hash_list_query = hash_list_query.order_by(FcHashlist.id.acs())

        hash_list_page = hash_list_query.paginate(page,per_page,error_out=True)

        return hash_list_page

    @api.marshal_with(empty_hash_list_created_model)
    @api.expect(make_empty_hash_list_parser)
    def post(self):
        """
        Creates a new empty hash list on the server with the given name.
        """
        args = make_empty_hash_list_parser.parse_args(request)

        hashlist = FcHashlist(name=args['name'],hash_type=None)
        db.session.add(hashlist)
        db.session.commit()

        return {
            'result' : 'OK',
            'name' : hashlist.name,
            'id' : hashlist.id,
        }


@ns.route('/<id>')
class HashListHashes(Resource):
    @api.marshal_with(hash_list_model)
    def get(self,id:str):
        """
        Returns details about a hash list.
        """
        hash_list : FcHashlist = FcHashlist.query.filter(FcHashlist.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')
        
        return hash_list
    

@ns.route('/<id>/details')
class HashListHashes(Resource):
    @api.expect(hash_list_hashes_parser)
    @api.marshal_with(page_of_hashes_model)
    def get(self,id:str):
        """
        Returns a paginated list of hashes of the given hash list.
        """
        hash_list : FcHashlist = FcHashlist.query.filter(FcHashlist.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')

        args = hash_list_hashes_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        hash_query = FcHash.query

        hash_query = hash_query.filter_by(hashlist_id=id)

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

        hash_page = hash_query.paginate(page,per_page,error_out=True)

        return hash_page

@ns.route('/<id>/hashes')
class hashListUploadList(Resource):
    
    @api.expect(hash_list_add_hash_list_parser)
    @api.marshal_with(hash_addition_result_model)
    def post(self,id:str):
        """
        Takes a list of string hashes and ads them to hash list with the given id.
        """
        data = request.json
        
        hash_list : FcHashlist = FcHashlist.query.filter(FcHashlist.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')

        lifted_hashes : list[str] = [hashObj['hash'] for hashObj in data['hash_list']]

        return upload_hash_list(lifted_hashes,hash_list,data['hash_type'],data['valid_only'])

        
@ns.route('/<id>/file')
class hashListUploadHashFile(Resource):
    @api.expect(hash_list_add_hash_file_parser)
    @api.marshal_with(hash_addition_result_model)
    def post(self,id:str):
        """
        Takes a hash file (a text or binary file containing hashes),
        and adds the hashes contained within into the hashlist with the given id.
        """
        args = hash_list_add_hash_file_parser.parse_args(request)

        hash_list : FcHashlist = FcHashlist.query.filter(FcHashlist.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')

        if args['file'].filename == '': #I don't know, if we still need this... since we are using this new and fancy way...? I suppose we do?
            abort(500, 'No selected file')

        if (args['binary'] != True):
            #We get universal newline support by wrapping the binary IO into a string IO wrapper
            text_io_wrapper = io.TextIOWrapper(args['file'].stream._file,encoding='ascii', errors='surrogateescape')
            new_hashes = text_io_wrapper.read().splitlines()
        else:
            new_hashes = [args['file'].read()]

        return upload_hash_list(new_hashes,hash_list,args['hash_type'],args['valid_only'])


@ns.route('/<id>/extract')
class hashListUploadHashFile(Resource):
    @api.marshal_with(hash_addition_result_model)
    def post(self,id:str):
        """
        Extract the hash from the given protected file and adds it to the hash list with the given id.
        This also creates a ProtectedFile row in the database, just like the old ProtectedFile endpoint.
        """
        # check if the post request has the file part
        if 'file' not in request.files:
            abort(500, 'No file part')
            return redirect(request.url)
        file = request.files['file']
        # if user does not select file, browser also
        # submit a empty part without filename
        if file.filename == '':
            abort(500, 'No selected file')

        hash_list : FcHashlist = FcHashlist.query.filter(FcHashlist.id==id).first()
        if not hash_list:
            abort(404, 'Hash list not found')
        
        result = addProtectedFile(file)

        return upload_hash_list([result['hash']],hash_list,int(result['hash_type']),True)
