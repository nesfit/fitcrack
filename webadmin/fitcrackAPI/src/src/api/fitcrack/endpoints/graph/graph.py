'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask import request
from flask_restplus import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.graph.argumentsParser import job_graph_arguments
from src.api.fitcrack.endpoints.graph.functions import computeJobsGraph, computeHostGraph, \
    computeHostPercentageGraph
from src.api.fitcrack.endpoints.graph.responseModels import job_graph_model, pie_graph_model

log = logging.getLogger(__name__)
ns = api.namespace('graph', description='Endpoints for graph presentation')


@ns.route('/jobsProgress')
class runningPackages(Resource):

    @api.expect(job_graph_arguments)
    @api.marshal_with(job_graph_model)
    def get(self):
        """
        Returns 2D graph representing progress of started jobs.
        """

        args = job_graph_arguments.parse_args(request)
        fromDate = args['from_date']
        toDate = args['to_date']

        graphData = computeJobsGraph(fromDate, toDate)
        return graphData

@ns.route('/jobsProgress/<int:id>')
class runningPackage(Resource):

    @api.expect(job_graph_arguments)
    @api.marshal_with(job_graph_model)
    def get(self, id):
        """
        Returns 2D graph representing progress of started job.
        """

        args = job_graph_arguments.parse_args(request)
        fromDate = args['from_date']
        toDate = args['to_date']

        graphData = computeJobsGraph(fromDate, toDate, jobId=id)
        return graphData


@ns.route('/hostsComputing')
class hostsComputing(Resource):

    @api.expect(job_graph_arguments)
    @api.marshal_with(job_graph_model)
    def get(self):
        """
        Returns 2D graph representing computing power of active hosts.
        """

        args = job_graph_arguments.parse_args(request)
        fromDate = args['from_date']
        toDate = args['to_date']

        graphData = computeHostGraph(fromDate, toDate)

        return graphData


@ns.route('/hostsComputing/<int:id>')
class hostsComputingSingle(Resource):

    @api.expect(job_graph_arguments)
    @api.marshal_with(job_graph_model)
    def get(self, id):
        """
        Returns 2D graph representing computing power of active host.
        """

        args = job_graph_arguments.parse_args(request)
        fromDate = args['from_date']

        graphData = computeHostGraph(fromDate, jobId=id)

        return graphData


@ns.route('/hostPercentage/<int:id>')
class hostPercentage(Resource):

    @api.expect(job_graph_arguments)
    @api.marshal_with(pie_graph_model)
    def get(self, id):
        """
        Returns 2D graph representing ratio of host's computing power.
        """

        args = job_graph_arguments.parse_args(request)
        fromDate = args['from_date']

        graphData = computeHostPercentageGraph(fromDate, id)

        return graphData
