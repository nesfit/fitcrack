'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
from flask_restplus import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.markov.responseModels import hcStatsCollection_model
from src.api.fitcrack.endpoints.rule.responseModels import rule_model
from src.api.fitcrack.responseModels import simpleResponse

log = logging.getLogger(__name__)
ns = api.namespace('template', description='Endpointy ktoré slúžia na pracu s šablonami utokov.')



@ns.route('')
class ruleCollection(Resource):

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Nahrava template json na server
        """
        pass

    @api.marshal_with(hcStatsCollection_model)
    def get(self):
        """
        Vracia kolekciu HcStats suborov
        """
        pass
        #return {'items': FcRule.query.filter(FcRule.deleted == False).all()}


@ns.route('/<id>')
class rule(Resource):

    def get(self, id):
        """
        Vrati šablonu
        """


        return {
            'name': 'test',
            'data': '{"hosts_ids":[{"id":1,"domain_name":"DESKTOP-B22HVA6","ip_address":"10.0.75.1","p_model":"Intel(R) Core(TM) i3-8350K CPU @ 4.00GHz [Family 6 Model 158 Stepping 11]","os_name":"Microsoft Windows 10","user":{"name":"Matus"},"last_active":{"last_seen":"2018-08-07 14:14:25","seconds_delta":"6","online":true},"deleted":false}],"seconds_per_job":3600,"attack_settings":{"attack_mode":0,"attack_name":"dict","rules":{"id":2,"name":"fitcrack.txt","time":"2018-08-07T13:16:33"},"left_dictionaries":[{"id":1,"name":"hashcatForumPassword.txt","keyspace":100,"time":"2018-08-07T11:52:50"},{"id":2,"name":"reverseRules.txt","keyspace":100,"time":"2018-08-07T13:26:42"}]},"hash_settings":{"hash_type":"0"}}'
        }


