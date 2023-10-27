'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask import request
from flask_restx import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.hashlists.argumentsParser import make_empty_hash_list_parser, hash_list_parser
from src.api.fitcrack.endpoints.charset.responseModels import charset_model, charsetCollection_model
from src.api.fitcrack.endpoints.hashlists.responseModels import empty_hash_list_created_model, page_of_hash_lists_model
from src.database import db
from src.database.models import FcHashlist

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
