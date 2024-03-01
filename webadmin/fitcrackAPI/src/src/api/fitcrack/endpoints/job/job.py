'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os
import base64
import logging

import datetime
import tempfile
import io

from flask import request, send_file
from flask_restx import Resource
from flask_restx import abort
from flask_login import current_user
from sqlalchemy import func, exc

from settings import DICTIONARY_DIR

from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination
from src.api.fitcrack.endpoints.host.argumentsParser import jobHost_parser
from src.api.fitcrack.endpoints.host.responseModels import page_of_hosts_model
from src.api.fitcrack.endpoints.job.argumentsParser import jobList_parser, jobWorkunit_parser, \
    jobOperation, verifyHash_argument, crackingTime_argument, addJob_model, editHostMapping_argument, \
    editJob_argument, multiEditHosts_argument, jobList_argument, multiJobOperation_argument, job_permissions_arguments
from src.api.fitcrack.endpoints.job.functions import verifyHashFormat, create_job, \
    stop_job, start_job, kill_job, computeCrackingTime, visible_jobs_ids, editable_jobs_ids, actionable_jobs_ids, \
    can_view_job, can_edit_job, can_operate_job
from src.api.fitcrack.endpoints.job.responseModels import page_of_jobs_model, page_of_jobs_model, \
    verifyHash_model, crackingTime_model, newJob_model, job_big_model, verifyHashes_model, \
    job_nano_list_model, job_user_permissions_model
from src.api.fitcrack.functions import shellExec
from src.api.fitcrack.attacks.functions import compute_prince_keyspace
from src.api.fitcrack.lang import status_to_code, job_status_text_to_code_dict, attack_modes
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcJob, Host, FcHost, FcWorkunit, FcHostActivity, FcMask, FcJobGraph, \
    FcJobDictionary, FcPcfg, FcJobStatus, FcRule, FcUserPermission, FcUser, FcSetting

log = logging.getLogger(__name__)

ns = api.namespace('job', description='Operations with jobs.')

@ns.route('')
class jobsCollection(Resource):
    @api.expect(jobList_parser)
    @api.marshal_with(page_of_jobs_model)
    def get(self):
        """
        Returns list of jobs.
        """
        args = jobList_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)
        jobs_query = FcJob.query

        if not current_user.role.VIEW_ALL_JOBS:
            ids = visible_jobs_ids()
            jobs_query = jobs_query.filter(FcJob.id.in_(ids))

        if args.bin is not None:
            jobs_query = jobs_query.filter(FcJob.bins.any(id=args.bin))

        if args.batch is not None:
            jobs_query = jobs_query.filter_by(batch_id=args.batch)

        if args.showDeleted:
            jobs_query = jobs_query.filter_by(deleted=True)
        elif not args.batch: # always show both in batches
            jobs_query = jobs_query.filter_by(deleted=False)

        if args.name:
            jobs_query = jobs_query.filter(FcJob.name.like('%' + args.name + '%'))

        if args.status:
            statusCode = status_to_code[args.status]
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

        jobs_page = jobs_query.paginate(page, per_page, error_out=True)

        return jobs_page

    @api.expect(addJob_model)
    @api.marshal_with(newJob_model)
    def post(self):
        """
        Creates new job.
        """
        if not current_user.role.ADD_NEW_JOB:
            abort(401, 'Unauthorized to add a job.')

        data = request.json
        job = create_job(data)

        return {
            'message': 'Job \"' + job.name + '\" successfully created.',
            'status': True,
            'job_id': job.id
        }

    @api.expect(jobList_argument)
    @api.response(200, 'Jobs moved to/from trash')
    @api.response(500, 'Failed moving to/from trash')
    def patch(self):
        """
        Moves jobs between trash and visible listing.
        """

        args = jobList_argument.parse_args(request)
        ids = args['job_ids']
        query = FcJob.query.filter(FcJob.id.in_(ids))
        if not current_user.role.EDIT_ALL_JOBS:
            editable = editable_jobs_ids()
            query = query.filter(FcJob.id.in_(editable))
        jobs = query.all()
        if (len(jobs) == 0):
            abort(400, 'No jobs selected or you don\'t have permissions on them.')

        for job in jobs:
            if not job.deleted and job.status == status_to_code['running']:
                # before moving to trash, stop job if running
                stop_job(job)

            job.deleted = not job.deleted
            
        try:
            db.session.commit()
        except:
            return 'Oops', 500

        return 'Moved', 200


@ns.route('/multiJobOperation')
class multiJobOperation(Resource):
    @api.expect(multiJobOperation_argument)
    @api.marshal_with(simpleResponse)
    def post(self):
        args = multiJobOperation_argument.parse_args(request)
        ids = args['job_ids']
        operation = args['operation']
        query = FcJob.query.filter(FcJob.id.in_(ids))
        if not current_user.role.EDIT_ALL_JOBS:
            editable = editable_jobs_ids()
            query = query.filter(FcJob.id.in_(editable))
        jobs = query.all()
        if (len(jobs) == 0):
            abort(400, 'No jobs selected or you don\'t have permissions on them.')

        for job in jobs:
            if operation == 'stop':
                stop_job(job)
            elif operation == 'start':
                start_job(job, db)
            elif operation == 'kill':
                kill_job(job, db)

        try:
            db.session.commit()
        except:
            return {
                'status': False,
                'message': 'Something went wrong.'
            }

        return {
            'status': False,
            'message': 'Jobs ' + operation + "ed."
        }



@ns.route('/host')
@api.response(404, 'job not found.')
class multiJobsHost(Resource):

    @api.expect(multiEditHosts_argument)
    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Mapping of hosts to multiple jobs.
        """

        args = multiEditHosts_argument.parse_args(request)
        ids = args['job_ids']
        requested_count = len(ids)
        skipped = 0
        if not current_user.role.EDIT_ALL_JOBS:
            editable = editable_jobs_ids()
            ids = [id for id in ids if id in editable]
            skipped = requested_count - len(ids)
            
        beforeHosts = FcHostActivity.query.filter(FcHostActivity.job_id.in_(ids)).all()
        for host in beforeHosts:
            db.session.delete(host)

        newIds = args['newHost_ids']
        if not newIds:
            try:
                db.session.commit()
                return {
                    'status': True,
                    'message': 'Hosts unassigned.'
                }
            except:
                return {
                    'status': False,
                    'message': 'Something went wrong.'
                }

        for jobId in ids:
            for hostId in newIds:
                host = FcHostActivity(boinc_host_id=hostId, job_id=jobId)
                db.session.add(host)

        try:
            db.session.commit()
        except:
            return {
                'status': False,
                'message': 'Something went wrong.'
            }

        return {
            'status': True,
            'message': 'Hosts assigned to {} job(s).{}'.format(str(len(ids)), " " + str(skipped) + " skipped due to permissions." if skipped else "")
        }

@ns.route('/<int:id>')
@api.response(404, 'Job not found.')
class JobByID(Resource):

    @api.marshal_with(job_big_model)
    def get(self, id):
        """
        Returns job.
        """
        if not current_user.role.VIEW_ALL_JOBS and not can_view_job(id):
            abort(401, 'Unauthorized access to job.')

        job = FcJob.query.filter(FcJob.id == id).one_or_none()
        if not job:
            abort(404, 'Job not found.')

        if job.grammar_id:
            pcfg = FcPcfg.query.filter(FcPcfg.id == job.grammar_id).one()
            job.grammar_name = pcfg.name
            job.grammar_keyspace = pcfg.keyspace

        return job

    @api.expect(editJob_argument)
    @api.marshal_with(simpleResponse)
    def put(self, id):
        """
        Changes created job.
        """
        if not current_user.role.EDIT_ALL_JOBS and not can_edit_job(id):
            abort(401, 'Unauthorized access to job.')

        args = editJob_argument.parse_args(request)
        job = FcJob.query.filter(FcJob.id == id).one()

        if args['time_start'] == '':
            args['time_start'] = None

        if args['time_end'] == '':
            args['time_end'] = None
        
        dicts = job.left_dictionaries
        if dicts:
            args['left_dictionaries'] = []
            for dct in dicts:
                d = {}
                d['name'] = dct.dictionary.name
                args['left_dictionaries'].append(d)

        rule_file = job.rulesFile
        job.name = args['name']
        job.comment = args['comment']
        job.seconds_per_workunit = args['seconds_per_job']
        settings = FcSetting.query.first()
        if job.seconds_per_workunit < settings.t_pmin:
            abort(400, 'Value for \"Time for workunit\" is smaller than absolute minimum seconds per workunit ({}).'.format(settings.t_pmin))
        job.time_start = None if not args['time_start'] else datetime.datetime.strptime(args['time_start'], '%Y-%m-%dT%H:%M'),
        job.time_end = None if not args['time_end'] else datetime.datetime.strptime(args['time_end'], '%Y-%m-%dT%H:%M')

        if job.attack_mode == attack_modes['prince']:
            opts_changing_keyspace = ["case_permute", "check_duplicates", "min_password_len", "max_password_len", "min_elem_in_chain", "max_elem_in_chain"]
            if any(getattr(job, option) != args[option] for option in opts_changing_keyspace):
                # Recompute new keyspace/hc_keyspace
                new_hc_keyspace = compute_prince_keyspace(args)
                if new_hc_keyspace == -1:
                    abort(400, 'Unable to compute new job keyspace.\nJob \"' + job.name + '\" was not edited.')
            else:
                new_hc_keyspace = job.hc_keyspace

            random_rules_count = 0
            if args['generate_random_rules']:
                random_rules_count = int(args['generate_random_rules'])
            ruleFileMultiplier = random_rules_count
            if rule_file:
                ruleFileMultiplier = rule_file.count

            new_keyspace = new_hc_keyspace
            if ruleFileMultiplier != 0:
                new_keyspace = new_hc_keyspace * ruleFileMultiplier

            if new_keyspace > (2 ** 63) - 1: # due db's bigint range
                abort(400, 'Job keyspace is higher than maximal allowed value 2^64.')

            job.hc_keyspace = new_hc_keyspace
            job.keyspace = new_keyspace
            # Prince settings
            job.case_permute = args['case_permute']
            job.check_duplicates = args['check_duplicates']
            job.min_password_len = args['min_password_len']
            job.max_password_len = args['max_password_len']
            job.min_elem_in_chain = args['min_elem_in_chain']
            job.max_elem_in_chain = args['max_elem_in_chain']
            job.generate_random_rules = args['generate_random_rules']

        try:
            db.session.commit()
            return {
                'status': True,
                'message': 'Job \"' + job.name + '\" edited.'
            }
        except:
            db.session().rollback()
            abort(400, 'Unable to edit this job. Please check if new settings are correct.')

    @api.response(204, 'Job moved.')
    def delete(self, id):
        """
        Moves the job to the trash bin or restores it.
        """
        job = FcJob.query.filter(FcJob.id == id).one()
        job.deleted = not job.deleted
        db.session.commit()
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
        if not current_user.role.OPERATE_ALL_JOBS and not can_operate_job(id):
            abort(401, 'Unauthorized access to job.')

        args = jobOperation.parse_args(request)
        action = args.get('operation')
        job = FcJob.query.filter(FcJob.id == id).one()

        if action == 'start':
            start_job(job, db)
        elif action == 'stop':
            stop_job(job)
        elif action == 'restart':
            job.status = status_to_code['running']
            job.indexes_verified = 0
            job.current_index = 0
            job.current_index_2 = 0
            job.workunit_sum_time = 0
            job.time_start = None
            job.time_end = None
            if job.attack_mode == attack_modes['mask'] or job.attack_mode == attack_modes['hybrid (wordlist + mask)']:
                masks = FcMask.query.filter(FcMask.job_id == id).all()
                for mask in masks:
                    mask.current_index = 0
            elif job.attack_mode in [attack_modes[modeStr] for modeStr in ['dictionary', 'combinator', 'hybrid (mask + wordlist)']]:
                dictionaries = FcJobDictionary.query.filter(FcJobDictionary.job_id == id).all()
                for dictionary in dictionaries:
                    dictionary.current_index = 0
                    dictionary.current_pos = 0
            graphData = FcJobGraph.query.filter(FcJobGraph.job_id == id).all()
            for item in graphData:
                db.session.delete(item)
            db.session.add(FcJobGraph(progress=0, job_id=job.id))
        elif action == 'kill':
            kill_job(job, db)
        else:
            abort(400, 'Bad operation with job!')

        db.session.commit()

        return {
            'status': True,
            'message': 'Job \"' + job.name + '\" ' + action + "ed."
        }


@ns.route('/<int:id>/permissions')
class userPermissions(Resource):
    
    @api.marshal_with(job_user_permissions_model)
    @api.response(500, 'permission aggregation failed')
    def get(self, id):
        """
        Returns list of users with their permissions on this job
        """
        data = FcUserPermission.query.filter_by(job_id=id).all()
        return {
            'items': [{
                'id': d.user.id,
                'username': d.user.username,
                'mail': d.user.mail,
                'view': d.view,
                'modify': d.modify,
                'operate': d.operate,
                'owner': d.owner
            } for d in data]
        }

    @api.expect(job_permissions_arguments)
    @api.response(200, 'permissions updated')
    @api.response(500, 'permission management failed')
    def put(self, id):
        """
        Assigns specified permissions on specified job to the user
        """
        args = job_permissions_arguments.parse_args(request)
        user_id = args.get('user_id')

        record = FcUserPermission.query.filter_by(job_id=id).filter_by(user_id=user_id).one_or_none()
        if not record:
            record = FcUserPermission(job_id=id, user_id=user_id, view=0, modify=0, operate=0, owner=0)
            db.session.add(record)
        
        for perm in 'view', 'modify', 'operate':
            old = getattr(record, perm)
            new = args.get(perm)
            setattr(record, perm, old if not new else new)

        try:
            db.session.commit()
        except:
            db.session().rollback()
            abort(500, 'Permission management failed.')

        return 'Permissions updated.', 200



@ns.route('/<int:id>/host')
@api.response(404, 'job not found.')
class jobsHost(Resource):

    @api.expect(jobHost_parser)
    @api.marshal_with(page_of_hosts_model)
    def get(self, id):
        """
        Returns list of hosts that are working on job.
        """
        if not current_user.role.VIEW_ALL_JOBS and not can_view_job(id):
            abort(401, 'Unauthorized access to job.')

        args = jobHost_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        hosts_query = FcHost.query.filter_by(job_id=id)


        hosts_page = hosts_query.paginate(page, per_page, error_out=True)

        return hosts_page

    @api.deprecated
    @api.expect(editHostMapping_argument)
    @api.marshal_with(simpleResponse)
    def post(self, id):
        """
        Deprecated in favor of /job/host - Mapping of hosts to job.
        """
        if not current_user.role.EDIT_ALL_JOBS and not can_edit_job(id):
            abort(401, 'Unauthorized access to job.')

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
@api.response(404, 'Job not found.')
class workunitsJob(Resource):

    @api.expect(jobWorkunit_parser)
    @api.marshal_with(page_of_jobs_model)
    def get(self, id):
        """
        Returns workunits making up this job.
        """
        if not current_user.role.EDIT_VIEW_JOBS and not can_view_job(id):
            abort(401, 'Unauthorized access to job.')

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
    def post(self):
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
