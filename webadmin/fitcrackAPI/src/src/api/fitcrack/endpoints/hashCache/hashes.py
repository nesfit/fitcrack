'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import base64
import logging
from operator import or_

from flask import request
from flask_restplus import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.hashCache.argumentsParser import hashes_parser
from src.api.fitcrack.endpoints.hashCache.responseModels import page_of_hashes_model
from src.api.fitcrack.endpoints.host.responseModels import page_of_hosts_model

from src.database.models import FcHashcache, FcHash

log = logging.getLogger(__name__)
ns = api.namespace('hashes', description='Operácie s databazou hashov')




@ns.route('')
class hashCache(Resource):

    @api.expect(hashes_parser)
    @api.marshal_with(page_of_hashes_model)
    def get(self):
        """
        Vracia list hostov
        """

        args = hashes_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        hashes = FcHash.query


        if args.search:
            print('%' + base64.b64encode(args.search.encode()).decode('ascii').rstrip("=") + '%')
            hashes = hashes.filter(or_(FcHash.result.like('%' + base64.b64encode(args.search.encode()).decode('ascii').rstrip("=") + '%'),
                                       FcHash.hash.like('%' + args.search + '%')))

        if args.order_by:
            orderBy = getattr(FcHash, args.order_by)
            if args.descending:
                orderBy = orderBy.desc()
                hashes = hashes.order_by(orderBy)

        else:
            hashes = hashes.order_by(FcHash.id.desc())



        return hashes.paginate(page, per_page, error_out=True)

