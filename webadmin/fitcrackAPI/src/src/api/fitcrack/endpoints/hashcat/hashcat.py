'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask_restplus import Resource
from src.api.apiConfig import api
from src.api.fitcrack.attacks.hashtypes import hashtypes
from src.api.fitcrack.endpoints.hashcat.functions import getTable
from src.api.fitcrack.endpoints.hashcat.responseModels import hashType_list, attackMode_list

log = logging.getLogger(__name__)
ns = api.namespace('hashcat', description='Endpointy ktoré priamo využívajú nástroj hashcat')



@ns.route('/hashTypes')
class hashTypesCollection(Resource):

    @api.marshal_with(hashType_list)
    def get(self):
        """
        Vracia zoznam podporovaných hashov
        """
        # hashtype = getTable("Hash modes")

        return {'hashtypes': hashtypes}


@ns.route('/attackModes')
class attackModes(Resource):

    @api.marshal_with(attackMode_list)
    def get(self):
        """
        Vracia zoznam podporovaných utokov
        """
        attackModes = getTable("Attack Modes")

        return {"attackmodes": attackModes}



