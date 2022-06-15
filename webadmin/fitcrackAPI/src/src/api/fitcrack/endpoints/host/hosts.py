'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask import request
from flask_restx import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.host.argumentsParser import jobHost_parser
from src.api.fitcrack.endpoints.host.responseModels import page_of_hosts_model, boincHostDetail_model, boincHostBenchmarks_model
from src.api.fitcrack.responseModels import simpleResponse
from src.api.fitcrack.endpoints.job.functions import stop_job
from src.database import db
from src.database.models import Host, FcHost, FcHostActivity, FcHostStatus, FcBenchmark, FcJob

from sqlalchemy import exc

log = logging.getLogger(__name__)
ns = api.namespace('hosts', description='Operations with hosts.')




@ns.route('')
class hostsCollection(Resource):

    @api.expect(jobHost_parser)
    @api.marshal_with(page_of_hosts_model)
    def get(self):
        """
        Returns list of hosts.
        """

        args = jobHost_parser.parse_args(request)
        page = args.get('page', None)
        per_page = args.get('per_page', None)

        hosts_page = Host.query

        if args.showDeleted:
            hosts_page = hosts_page.filter(Host.deleted == 1)
        else:
            hosts_page = hosts_page.filter(Host.deleted == None)


        if args.name:
            hosts_page = hosts_page.filter(Host.domain_name.like('%' + args.name + '%'))

        if args.order_by:
            orderBy = getattr(Host, args.order_by)
            if args.descending:
                orderBy = orderBy.desc()
                hosts_page = hosts_page.order_by(orderBy)

        else:
            hosts_page = hosts_page.order_by(Host.id.desc())

        if args.all:
            return {
                'items': hosts_page.all()
            }
        else:
            return hosts_page.paginate(page, per_page, error_out=True)


@ns.route('/<int:id>')
@api.response(404, 'Host not found.')
class HostByID(Resource):

    @api.marshal_with(boincHostDetail_model)
    def get(self, id):
        """
        Returns exact host.
        """

        host = Host.query.filter(Host.id == id).one()
        return host


    def delete(self, id):
        """
        Hides or shows a host from the list
        """
        hostStatus = FcHostStatus.query.filter(FcHostStatus.boinc_host_id == id).one()
        hostStatus.deleted = not hostStatus.deleted
        db.session.commit()
        return 'Host visibility toggled', 200


@ns.route('/<int:id>/benchmarks')
@api.response(404, 'Host not found.')
class BenchmarksByID(Resource):

    @api.marshal_with(boincHostBenchmarks_model)
    def get(self, id):
        """
        Returns benchmarks for this host.
        """

        benchmarks = FcBenchmark.query.filter(FcBenchmark.boinc_host_id == id)
        return {
            'items': benchmarks.all()
        }

    @api.expect(boincHostBenchmarks_model)
    def post(self, id):
        """
        Uploads benchmarks for boinc host.
        """

        benchmarks = request.get_json().get('items')
        for benchmark in benchmarks:
            existing_bench = FcBenchmark.query.filter(FcBenchmark.boinc_host_id == id,
                                                      FcBenchmark.hash_type == benchmark['hash_type'],
                                                      FcBenchmark.attack_mode == benchmark['attack_mode']).first()
            if existing_bench:
                existing_bench.power = benchmark['power']
                db.session.commit()
            else:
                new_bench = FcBenchmark(boinc_host_id=id, hash_type=benchmark['hash_type'], attack_mode=benchmark['attack_mode'], power=benchmark['power'])
                try:
                    db.session.add(new_bench)
                    db.session.commit()
                except exc.SQLAlchemyError:
                    pass


@ns.route('/<id>/unassignAllJobs')
class dictionaryDownload(Resource):
    @api.marshal_with(simpleResponse)
    def put(self, id):
        """
        Unassign all jobs.
        """

        hostActivities = FcHostActivity.query.filter(FcHostActivity.boinc_host_id == id).all()
        for hostAct in hostActivities:
            db.session.delete(hostAct)

        jobsWithHost = FcHost.query.filter(FcHost.boinc_host_id == id).all()
        for jobHost in jobsWithHost:
            job = FcJob.query.filter(FcJob.id == jobHost.job_id).one()
            if len(job.hosts) == 0:
                stop_job(job)
            db.session.delete(jobHost)

        db.session.commit()

        return {
            'message': 'All jobs unassigned.',
            'status': True
        }

@ns.route('/info')
class hostsInfo(Resource):

    #@api.marshal_with(host_info_model)
    def get(self):
        """
        Returns information about hosts.
        """

        totalHosts = Host.query.count()
        activeHosts = 0
        for hostStatus in FcHostStatus.query.all():
            if hostStatus.online:
                activeHosts +=1
        inactiveHosts = totalHosts - activeHosts
        return {
            'totalHosts': totalHosts,
            'activeHosts': activeHosts,
            'inactiveHosts': inactiveHosts
        }
