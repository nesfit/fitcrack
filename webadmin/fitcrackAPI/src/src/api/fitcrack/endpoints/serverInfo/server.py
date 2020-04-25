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
from src.api.fitcrack.endpoints.serverInfo.transfer import pack, unpack
from src.api.fitcrack.endpoints.serverInfo.argumentsParser import operation, serverUsage_argument, export_options
from src.api.fitcrack.endpoints.graph.argumentsParser import job_graph_arguments
from src.database import db
from src.database.models import FcServerUsage
from settings import PROJECT_DIR, PROJECT_USER, PROJECT_NAME, BOINC_SERVER_URI
import platform
from flask import request, Response, stream_with_context
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

        return FcServerUsage.query.order_by(FcServerUsage.time.desc()).limit(1).one_or_none()


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


@ns.route('/transfer')
class systemTransfer(Resource):

    @api.expect(export_options)
    def get(self):
        """
        Configurable system data export to a package file
        """
        args = export_options.parse_args(request)
        jobs = args.get('jobs')

        return Response(stream_with_context(pack(jobs=jobs)), headers={"Content-disposition": "attachment; filename=fitcrack-export.fcp"})

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        System data import from a package file
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

        unpack(file)

        return {
            'message': 'Package data imported',
            'status': True
        }
