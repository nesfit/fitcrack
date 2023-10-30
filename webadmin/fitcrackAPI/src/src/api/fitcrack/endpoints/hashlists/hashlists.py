'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import base64
import logging

from flask import request
from flask_restx import Resource, abort

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.hashlists.argumentsParser import make_empty_hash_list_parser, hash_list_parser, hash_list_add_hash_list_parser
from src.api.fitcrack.endpoints.hashlists.responseModels import empty_hash_list_created_model, page_of_hash_lists_model, hash_addition_result_model
from src.api.fitcrack.endpoints.hashlists.functions import validate_hash_list
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

        #Some authenthication and filtration here perhaps?

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
            hash_list_query = hash_list_query.order_by(FcHashlist.id.desc())

        hash_list_page = hash_list_query.paginate(page,per_page,error_out=True)

        return hash_list_page

    @api.marshal_with(empty_hash_list_created_model)
    @api.expect(make_empty_hash_list_parser)
    def post(self):
        """
        Creates a new empty hash list on the server.
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

@ns.route('/<id>/hashes')
class hashListUploadList(Resource):
    
    @api.expect(hash_list_add_hash_list_parser)
    @api.marshal_with(hash_addition_result_model)
    def post(self,id:str):
        data = request.json
        
        hashlist : FcHashlist = FcHashlist.query.filter(FcHashlist.id==id).first()
        if not hashlist:
            abort(404, 'Hash list not found')

        validate_hash_list(data['hash_list'],data['hash_type'],data['valid_only'])

        #Converts hashes from a string to a byte sequence.
        for hashObj in data['hash_list']:
            if hashObj['hash'].startswith('BASE64:'):
                decoded = base64.decodebytes(hashObj['hash'][7:].encode())
                hashObj['hash'] = decoded
            else:
                hashObj['hash'] = hashObj['hash'].encode()

        #TODO: What kind of behaviour do we want from the endpoint?
        #TODO: As of now, it just stupidly appends.

        #TODO: We definitely just want to append the good hashes, or not?
        #TODO: I believe as of right now... we cannot create invalid hashes with the add job endpoint...


        #TODO: I think we want nice duplicate check like in a set?
        for hashObj in data['hash_list']:
            hash = FcHash(hashlist_id=hashlist.id, hash_type=data['hash_type'], hash=hashObj['hash'])
            db.session.add(hash)

        db.session.commit()
        #Holy fuck, we need rollbacks; the return may fail. Or just make all the failable actions first and then just do a return of vars.
        
        return {
            'result' : 'OK',
            'id' : hashlist.id,
            'name' : hashlist.name,
            'hashCount' : hashlist.hash_count,
            'addedCount' : len(data['hash_list']),
            'erroredCount' : 0
        }
