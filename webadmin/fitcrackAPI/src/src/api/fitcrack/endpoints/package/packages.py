'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import base64
import logging

import datetime
import tempfile

from flask import request
from flask_restplus import Resource
from flask_restplus import abort
from sqlalchemy import func

from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination
from src.api.fitcrack.endpoints.host.argumentsParser import packageHost_parser
from src.api.fitcrack.endpoints.host.responseModels import page_of_hosts_model
from src.api.fitcrack.endpoints.package.argumentsParser import packageList_parser, packageJob_parser, \
    packageOperation, verifyHash_argument, crackingTime_argument, addPackage_model, editHostMapping_argument, \
    editPackage_argument
from src.api.fitcrack.endpoints.package.functions import delete_package, verifyHashFormat, create_package, \
    computeCrackingTime
from src.api.fitcrack.endpoints.package.responseModels import page_of_packages_model, page_of_jobs_model, \
    verifyHash_model, crackingTime_model, newPackage_model, package_model, verifyHashes_model
from src.api.fitcrack.functions import shellExec
from src.api.fitcrack.lang import statuses, package_status_text_to_code_dict
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcJob, FcHost, FcWorkunit, FcHostActivity, FcMask, FcJobGraph, FcJobDictionary

log = logging.getLogger(__name__)

ns = api.namespace('jobs', description='Operácie s package')


@ns.route('')
class packagesCollection(Resource):

    @api.expect(packageList_parser)
    @api.marshal_with(page_of_packages_model)
    def get(self):
        """
        Vracia list balíčkov
        """
        args = packageList_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        packagess_query = FcJob.query

        if args.showDeleted:
            packagess_query = packagess_query.filter_by(deleted=True)
        else:
            packagess_query = packagess_query.filter_by(deleted=False)

        if args.name:
            packagess_query = packagess_query.filter(FcJob.name.like('%' + args.name + '%'))

        if args.status:
            statusCode = statuses[args.status]
            packagess_query = packagess_query.filter_by(status=statusCode)

        if args.order_by:
            orderBy = getattr(FcJob, args.order_by)
            if args.descending:
                orderBy = orderBy.desc()
            packagess_query = packagess_query.order_by(orderBy)

        else:
            packagess_query = packagess_query.order_by(FcJob.id.desc())

        packages_page = packagess_query.paginate(page, per_page, error_out=True)
        return packages_page

    @api.expect(addPackage_model)
    @api.marshal_with(newPackage_model)
    def post(self):
        """
        Vytvorí nový package
        """

        data = request.json
        package = create_package(data)
        return {
            'message': 'Package ' + package.name + ' succesful created.',
            'status': True,
            'job_id': package.id
        }


@ns.route('/<int:id>')
@api.response(404, 'Package not found.')
class PackageByID(Resource):

    @api.marshal_with(package_model)
    def get(self, id):
        """
        Vráti konkrétny package.
        """
        package = FcJob.query.filter(FcJob.id == id).one()
        return package

    @api.expect(editPackage_argument)
    @api.marshal_with(simpleResponse)
    def put(self, id):
        args = editPackage_argument.parse_args(request)
        package = FcJob.query.filter(FcJob.id == id).one()

        if args['time_start'] == '':
            args['time_start'] = None

        if args['time_end'] == '':
            args['time_end'] = None

        package.name = args['name']
        package.comment = args['comment']
        package.seconds_per_workunit = args['seconds_per_job']
        package.time_start = datetime.datetime.now() if not args['time_start'] else datetime.datetime.strptime(args['time_start'], '%d/%m/%Y %H:%M'),
        package.time_end = None if not args['time_end'] else datetime.datetime.strptime(args['time_end'], '%d/%m/%Y %H:%M')

        db.session.commit()
        return {
            'status': True,
            'message': 'Package with name ' + package.name + ' edited.'
        }

    @api.response(204, 'Package successfully deleted.')
    def delete(self, id):
        """
        Vzmaže package
        """
        delete_package(id)
        return None, 204


@ns.route('/<int:id>/action')
@api.response(404, 'Package not found.')
class OperationWithPackage(Resource):
    @api.expect(packageOperation)
    @api.marshal_with(simpleResponse)
    def get(self, id):
        """
        Operácie so package (reštart, štart,stop)
        """
        args = packageOperation.parse_args(request)
        action = args.get('operation')

        package = FcJob.query.filter(FcJob.id == id).one()
        if action == 'start':
            package.status = 10
        elif action == 'stop':
            package.status = 12
        elif action == 'restart':
            package.status = 10
            package.result = None
            package.indexes_verified = 0
            package.current_index = 0
            package.cracking_time = 0
            package.time_start = None
            package.time_end = None
            if package.attack_mode == 3:
                masks = FcMask.query.filter(FcMask.job_id == id).all()
                for mask in masks:
                    mask.current_index = 0
            elif package.attack_mode == 0 or package.attack_mode == 1:
                dictionaries = FcJobDictionary.query.filter(FcJobDictionary.job_id == id).all()
                for dictionary in dictionaries:
                    dictionary.current_index = 0
            graphData = FcJobGraph.query.filter(FcJobGraph.job_id == id).all()
            for item in graphData:
                db.session.delete(item)
            db.session.add(FcJobGraph(progress=0, job_id=package.id))
        else:
            abort(401, 'Bad operation with job!')

        db.session.commit()

        return {
            'status': True,
            'message': 'Job with name ' + package.name + ' ' + action
        }



@ns.route('/<int:id>/host')
@api.response(404, 'Package not found.')
class packagesHost(Resource):

    @api.expect(packageHost_parser)
    @api.marshal_with(page_of_hosts_model)
    def get(self, id):
        """
        vráti hostov ktorí sa podielaju na crackovaní balíčka
        """

        args = packageHost_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        hosts_query = FcHost.query.filter_by(job_id=id)


        hosts_page = hosts_query.paginate(page, per_page, error_out=True)

        return hosts_page

    @api.expect(editHostMapping_argument)
    @api.marshal_with(simpleResponse)
    def post(self, id):
        """
        mapping hostov k jobu
        """

        args = editHostMapping_argument.parse_args(request)
        beforeHosts = FcHostActivity.query.filter_by(job_id=id).all()
        for host in beforeHosts:
            db.session.delete(host)

        for hostId in args['newHost_ids']:
            host = FcHostActivity(boinc_host_id=hostId, job_id=id)
            db.session.add(host)

        try:
            db.session.commit()
        except:
            return {
                'status': False,
                'message': 'Semething went wrong.'
            }

        return {
            'status': True,
            'message': 'Hosts edited.'
        }



@ns.route('/<int:id>/job')
@api.response(404, 'Package not found.')
class packagesJob(Resource):

    @api.expect(packageJob_parser)
    @api.marshal_with(page_of_jobs_model)
    def get(self, id):
        """
        vráti ulohy na ktore je package rozdelený
        """

        args = packageJob_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        jobs_query = FcWorkunit.query.filter_by(job_id=id)


        jobs_page = jobs_query.paginate(page, per_page, error_out=True)

        return jobs_page




@ns.route('/verifyHash')
class verifyHash(Resource):

    @api.expect(verifyHash_argument)
    @api.marshal_with(verifyHashes_model)
    def post(self):
        """
        overí formát zadaného hashu
        """

        args = verifyHash_argument.parse_args(request)
        hashes = args.get('hashes')
        hashtype = args.get('hashtype')

        if hashes.startswith('BASE64:'):
            decoded = base64.decodebytes(hashes[7:].encode())
            with tempfile.NamedTemporaryFile() as fp:
                fp.write(decoded)
                fp.seek(0)
                result = verifyHashFormat(fp.name, hashtype, binaryHash=hashes)
        else:
            with tempfile.NamedTemporaryFile() as fp:
                fp.write(hashes.encode())
                fp.seek(0)
                result = verifyHashFormat(fp.name, hashtype)
        return result


@ns.route('/crackingTime')
class getCrackingTime(Resource):
    @api.expect(crackingTime_argument)
    @api.marshal_with(crackingTime_model)
    def get(self):
        """
        Vypočíta dobu crackovania
        """
        args = crackingTime_argument.parse_args(request)
        return computeCrackingTime(args)




@ns.route('/info')
class jobsInfo(Resource):

    #@api.marshal_with(crackingTime_model)
    def get(self):
        """
        Vráti info o joboch
        """

        statusesCount = db.session.query(FcJob.status, func.count(FcJob.id)).group_by(FcJob.status).all()
        result = []
        for status in statusesCount:
            result.append(
                {
                    'status': package_status_text_to_code_dict[status[0]],
                    'count': status[1]
                }
            )
        return result