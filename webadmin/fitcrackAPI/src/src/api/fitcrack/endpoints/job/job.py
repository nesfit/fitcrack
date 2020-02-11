'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''
'''
packagesCollection => jobsCollection
packageList_parser => jobList_parser
page_of_packages_model => page_of_jobs_model
packages_page => jobs_page
packagess_query => jobs_query
packagesJob => workunitsJob
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
from src.api.fitcrack.endpoints.host.argumentsParser import jobHost_parser
from src.api.fitcrack.endpoints.host.responseModels import page_of_hosts_model
from src.api.fitcrack.endpoints.job.argumentsParser import jobList_parser, jobWorkunit_parser, \
    jobOperation, verifyHash_argument, crackingTime_argument, addJob_model, editHostMapping_argument, \
    editJob_argument
from src.api.fitcrack.endpoints.job.functions import delete_job, verifyHashFormat, create_job, \
    computeCrackingTime, start_pcfg_manager
from src.api.fitcrack.endpoints.job.responseModels import page_of_jobs_model, page_of_jobs_model, \
    verifyHash_model, crackingTime_model, newJob_model, job_big_model, verifyHashes_model, job_nano_list_model
from src.api.fitcrack.functions import shellExec
from src.api.fitcrack.lang import statuses, job_status_text_to_code_dict, attack_modes
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcJob, FcHost, FcWorkunit, FcHostActivity, FcMask, FcJobGraph, FcJobDictionary, FcPcfg, FcJobStatus

log = logging.getLogger(__name__)

ns = api.namespace('job', description='Operations with jobs.')

# packagesCollection => jobsCollection
@ns.route('')
class jobsCollection(Resource):
# packageList_parser => jobList_parser
    @api.expect(jobList_parser)
    @api.marshal_with(page_of_jobs_model)
    #page_of_packages_model => page_of_jobs_model
    def get(self):
        """
        Returns list of jobs.
        """
        args = jobList_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)
        # packagess_query => jobs_query
        jobs_query = FcJob.query

        if args.showDeleted:
            jobs_query = jobs_query.filter_by(deleted=True)
        else:
            jobs_query = jobs_query.filter_by(deleted=False)

        if args.name:
            jobs_query = jobs_query.filter(FcJob.name.like('%' + args.name + '%'))

        if args.status:
            statusCode = statuses[args.status]
            jobs_query = jobs_query.filter_by(status=statusCode)

        if args.attack_mode:
            attack_code = attack_modes[args.attack_mode]
            jobs_query = jobs_query.filter_by(attack_mode=attack_code)

        if args.order_by:
            orderBy = getattr(FcJob, args.order_by)
            if args.descending:
                orderBy = orderBy.desc()
            jobs_query = jobs_query.order_by(orderBy)

        else:
            jobs_query = jobs_query.order_by(FcJob.id.desc())


        # packages_page => jobs_page
        jobs_page = jobs_query.paginate(page, per_page, error_out=True)

        return jobs_page

    @api.expect(addJob_model)
    @api.marshal_with(newJob_model)
    def post(self):
        """
        Creates new job.
        """
        #create_package => create_job
        #package => job
        data = request.json
        job = create_job(data)

        return {
            'message': 'Job ' + job.name + ' succesful created.',
            'status': True,
            'job_id': job.id
        }

# PackageByID => JobByID
@ns.route('/<int:id>')
@api.response(404, 'Job not found.')
class JobByID(Resource):

    @api.marshal_with(job_big_model)
    def get(self, id):
        """
        Returns job.
        """

        job = FcJob.query.filter(FcJob.id == id).one()
        return job

    @api.expect(editJob_argument)
    @api.marshal_with(simpleResponse)
    def put(self, id):
        """
        Changes created job.
        """

        args = editJob_argument.parse_args(request)
        job = FcJob.query.filter(FcJob.id == id).one()

        if args['time_start'] == '':
            args['time_start'] = None

        if args['time_end'] == '':
            args['time_end'] = None

        job.name = args['name']
        job.comment = args['comment']
        job.seconds_per_workunit = args['seconds_per_job']
        job.time_start = datetime.datetime.now() if not args['time_start'] else datetime.datetime.strptime(args['time_start'], '%d/%m/%Y %H:%M'),
        job.time_end = None if not args['time_end'] else datetime.datetime.strptime(args['time_end'], '%d/%m/%Y %H:%M')

        db.session.commit()
        return {
            'status': True,
            'message': 'Job with name ' + job.name + ' edited.'
        }

    @api.response(204, 'Job successfully deleted.')
    def delete(self, id):
        """
        Deletes job.
        """
        delete_job(id)
        return None, 204


@ns.route('/<int:id>/action')
@api.response(404, 'Job not found.')
class OperationWithJob(Resource):
    @api.expect(jobOperation)
    @api.marshal_with(simpleResponse)
    def get(self, id):
        """
        Operations with job(restart, start, stop).
        """
        args = jobOperation.parse_args(request)
        action = args.get('operation')
        job = FcJob.query.filter(FcJob.id == id).one()

        if action == 'start':
            if job.attack_mode == 9:
                pcfg = FcPcfg.query.filter(FcPcfg.id == job.grammar_id).one()
                start_pcfg_manager(job.id, pcfg.name, pcfg.keyspace)
            job.status = 10
        elif action == 'stop':
            job.status = 12
        elif action == 'restart':
            job.status = 10
            job.result = None
            job.indexes_verified = 0
            job.current_index = 0
            job.cracking_time = 0
            job.time_start = None
            job.time_end = None
            if job.attack_mode == 3:
                masks = FcMask.query.filter(FcMask.job_id == id).all()
                for mask in masks:
                    mask.current_index = 0
            elif job.attack_mode == 0 or job.attack_mode == 1:
                dictionaries = FcJobDictionary.query.filter(FcJobDictionary.job_id == id).all()
                for dictionary in dictionaries:
                    dictionary.current_index = 0
            graphData = FcJobGraph.query.filter(FcJobGraph.job_id == id).all()
            for item in graphData:
                db.session.delete(item)
            db.session.add(FcJobGraph(progress=0, job_id=job.id))
        elif action == 'kill':
            # Job is stopped in Generator after sending BOINC commands
            # job.status = 0
            job.indexes_verified = 0
            job.current_index = 0
            job.cracking_time = 0
            job.time_end = None
            if job.attack_mode == 3:
                masks = FcMask.query.filter(FcMask.job_id == id).all()
                for mask in masks:
                    mask.current_index = 0
            elif job.attack_mode == 0 or job.attack_mode == 1:
                dictionaries = FcJobDictionary.query.filter(FcJobDictionary.job_id == id).all()
                for dictionary in dictionaries:
                    dictionary.current_index = 0
            hosts = FcHostActivity.query.filter(FcHostActivity.job_id == id).all()
            for host in hosts:
                host.status = 0
            graphData = FcJobGraph.query.filter(FcJobGraph.job_id == id).all()
            for item in graphData:
                db.session.delete(item)

            workunits = FcWorkunit.query.filter(FcWorkunit.job_id == id).all()
            for item in workunits:
                db.session.delete(item)
            job.kill = True

        else:
            abort(401, 'Bad operation with job!')

        db.session.commit()

        return {
            'status': True,
            'message': 'Job with name ' + job.name + ' ' + action
        }



@ns.route('/<int:id>/host')
@api.response(404, 'job not found.')
class jobsHost(Resource):

    @api.expect(jobHost_parser)
    @api.marshal_with(page_of_hosts_model)
    def get(self, id):
        """
        Returns list of hosts that are working on job.
        """

        args = jobHost_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        hosts_query = FcHost.query.filter_by(job_id=id)


        hosts_page = hosts_query.paginate(page, per_page, error_out=True)

        return hosts_page

    @api.expect(editHostMapping_argument)
    @api.marshal_with(simpleResponse)
    def post(self, id):
        """
        Mapping of hosts to job.
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


# packagesJob => workunitsJob
@ns.route('/<int:id>/job')
@api.response(404, 'Job not found.')
class workunitsJob(Resource):

    @api.expect(jobWorkunit_parser)
    @api.marshal_with(page_of_jobs_model)
    def get(self, id):
        """
        Returns workunits to which job was devides.
        """
        # packageJob_parser => job_parser
        args = jobWorkunit_parser.parse_args(request)
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
        Verifies format of uploaded hash.
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
        Calculates cracking time.
        """
        args = crackingTime_argument.parse_args(request)
        return computeCrackingTime(args)




@ns.route('/info')
class jobsInfo(Resource):

    #@api.marshal_with(crackingTime_model)
    def get(self):
        """
        Returns information about jobs.
        """

        statusesCount = db.session.query(FcJob.status, func.count(FcJob.id)).group_by(FcJob.status).all()
        result = []
        for status in statusesCount:
            result.append(
                {
                    'status': job_status_text_to_code_dict[status[0]],
                    'count': status[1]
                }
            )
        return result


@ns.route('/lastJobs')
class lastJobs(Resource):

    @api.marshal_with(job_nano_list_model)
    def get(self):
        """
        Returns the last jobs at which there were changes of the state.
        """

        states = FcJobStatus.query.order_by(FcJobStatus.time.desc()).all()

        ids = []
        jobs_to_return = []

        for jobStatus in states:
            if not ids.__contains__(jobStatus.job_id) and len(ids) != 6:
                ids.append(jobStatus.job_id)

        for jobId in ids:
            job = FcJob.query.filter(FcJob.id == jobId).one()
            if job.deleted == 0:
                jobs_to_return.append(job)

        return {'items': jobs_to_return}
