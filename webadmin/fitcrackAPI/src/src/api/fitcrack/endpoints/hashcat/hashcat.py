'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask_restx import Resource
from src.api.apiConfig import api
from src.api.fitcrack.attacks.hashtypes import hashtypes
from src.api.fitcrack.endpoints.hashcat.functions import getTable
from src.api.fitcrack.endpoints.hashcat.responseModels import hashType_list, attackMode_list

log = logging.getLogger(__name__)
ns = api.namespace('hashcat', description='Endpoints for hashcat usage.')



@ns.route('/hashTypes')
class hashTypesCollection(Resource):

    @api.marshal_with(hashType_list)
    def get(self):
        """
        Returns list of supported hashes.
        """
        # hashtype = getTable("Hash modes")

        return {'hashtypes': hashtypes}


@ns.route('/attackModes')
class attackModes(Resource):

    @api.marshal_with(attackMode_list)
    def get(self):
        """
        Returns list of supported attacks.
        """
        attackModes = getTable("Attack Modes")

        return {"attackmodes": attackModes}
