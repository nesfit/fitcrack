'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
from time import sleep

from flask_restplus import Resource
from src.api.apiConfig import api
from src.api.fitcrack.responseModels import simpleResponse
from src.api.fitcrack.endpoints.serverInfo.responseModels import serverinfo
from src.api.fitcrack.endpoints.serverInfo.functions import isComponentRunning
from src.api.fitcrack.endpoints.serverInfo.argumentsParser import operation
from src.api.fitcrack.functions import shellExec
from settings import PROJECT_DIR, PROJECT_USER, PROJECT_NAME, BOINC_SERVER_URI
import platform
from flask import request
from flask_restplus import abort
import xml.etree.ElementTree as ET
import urllib.request


log = logging.getLogger(__name__)
ns = api.namespace('serverInfo', description='Operácie so serverom')


@ns.route('/info')
class serverInfo(Resource):

    @api.marshal_with(serverinfo)
    def get(self):
        """
        Informacie o serveri
        """

        url = BOINC_SERVER_URI + '/' + PROJECT_NAME + '/server_status.php?xml=1'
        request = urllib.request.Request(url)

        try:
            response = urllib.request.urlopen(request)
        except:
            abort(500, 'Can not load ' + url)



        xml = response.read().decode("utf8")
        tree = ET.ElementTree(ET.fromstring(xml)).getroot().find('daemon_status')

        result = []
        for deamon in tree:
            result.append({
                'name': deamon.find('command').text,
                'status': deamon.find('status').text
            })

        return {'subsystems': result}

@ns.route('/control')
class serverOperation(Resource):
    @api.expect(operation)
    @api.marshal_with(simpleResponse)
    def get(self):
        """
        Operácie so serverom (reštart, štart,stop)
        """
        args = operation.parse_args(request)
        action = args.get('operation')


        if action == "start":
            out = shellExec('/usr/bin/python ' + PROJECT_DIR + '/bin/start')
        elif action == "stop":
            out = shellExec('/usr/bin/python ' + PROJECT_DIR + '/bin/stop')
        elif action == "restart":
            out = shellExec('/usr/bin/python ' + PROJECT_DIR + '/bin/stop')
            out = shellExec('/usr/bin/python ' + PROJECT_DIR + '/bin/start')
        else:
            abort(400, "Bad operation " + action)



        return {
            "status": True,
            "message": "Operation " + action + " finished sucesfull."
        }



