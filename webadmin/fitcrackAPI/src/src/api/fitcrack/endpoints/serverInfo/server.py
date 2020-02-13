'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
from time import sleep

from flask_restplus import Resource
from src.api.apiConfig import api
from src.api.fitcrack.responseModels import simpleResponse
from src.api.fitcrack.endpoints.serverInfo.responseModels import serverinfo, usageinfoList, usageinfo
from src.api.fitcrack.endpoints.serverInfo.functions import getCpuMemData
from src.api.fitcrack.endpoints.serverInfo.argumentsParser import operation, serverUsage_argument
from src.api.fitcrack.endpoints.graph.argumentsParser import job_graph_arguments
from src.api.fitcrack.functions import shellExec
from src.database import db
from src.database.models import FcServerUsage
from settings import PROJECT_DIR, PROJECT_USER, PROJECT_NAME, BOINC_SERVER_URI
import platform
from flask import request
from flask_restplus import abort
import xml.etree.ElementTree as ET
import urllib.request


log = logging.getLogger(__name__)
ns = api.namespace('serverInfo', description='Operation with server.')


@ns.route('/info')
class serverInfo(Resource):

    @api.marshal_with(serverinfo)
    def get(self):
        """
        Information about server.
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
        Operations with server(restart, start, stop).
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



@ns.route('/getUsageData')
class serverUtil(Resource):

    @api.expect(job_graph_arguments)
    @api.marshal_with(usageinfoList)
    def get(self):
        """
        Returns data from table fc_server_usage according to a given time
        """

        args = job_graph_arguments.parse_args(request)
        fromDate = args['from_date']
        toDate = args['to_date']

        return getCpuMemData(fromDate, toDate)


@ns.route('/actualUsageData')
class actualUsageData(Resource):

    @api.marshal_with(usageinfo)
    def get(self):
        """
        Returns last record from table fc_server_usage
        """

        return FcServerUsage.query.order_by(FcServerUsage.time.desc()).limit(1).one()


@ns.route('/saveData')
class saveData(Resource):
    is_public = True

    @api.expect(serverUsage_argument)
    @api.marshal_with(simpleResponse)
    def get(self):
        """
        Function for saving of new data into the table fc_server_usage
        """
        args = serverUsage_argument.parse_args(request)

        serverUsage = FcServerUsage(cpu=args['cpu'], ram=args['ram'], net_recv=args['net_recv'],
                                    net_sent=args['net_sent'], hdd_read=args['hdd_read'], hdd_write=args['hdd_write'])
        try:
            db.session.add(serverUsage)
            db.session.commit()
        except:
            db.session().rollback()
            abort(500, 'An error occurred while saving new statistics data')

        return {
            'message': 'Usage data saved',
            'status': True
        }
