'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask import request
from flask_restx import Resource, abort

from sqlalchemy import func, Integer, desc

from src.database import db
from src.database.models import FcJobGraph, FcWorkunit, Host

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.chart.helpers import set_bounds_filter, build_job_datasets, \
                                                     build_wu_datasets, build_doughnut, \
                                                     DateFormatError, DatePrecedenceError
from src.api.fitcrack.endpoints.chart.argumentsParser import time_series_bounds, dateformat
from src.api.fitcrack.endpoints.chart.responseModels import time_series_model, doughnut_model
from src.api.fitcrack.endpoints.batches.functions import getIdsFromBatch

log = logging.getLogger(__name__)
ns = api.namespace('chart', description='Endpoints for charting data')

def try_bounds (query, model, bounds):
    try:
        query = set_bounds_filter(query, model, bounds)
    except DateFormatError:
        abort(400, 'Couldn\'t parse dates. Format should be {}.'.format(dateformat))
    except DatePrecedenceError:
        abort(400, 'Provided end date is before the provided begin date.')
    return query

@ns.route('/jobProgress')
class jobProgress(Resource):

    @api.expect(time_series_bounds)
    @api.marshal_with(time_series_model)
    def get(self):
        """
        Returns datasets for time series representing progress of started jobs.
        """
        query = FcJobGraph.query
        bounds = time_series_bounds.parse_args(request)
        query = try_bounds(query, FcJobGraph, bounds)
            
        return {
            'datasets': build_job_datasets(query)
        }

@ns.route('/jobProgress/<int:id>')
class jobProgressSingle(Resource):

    @api.expect(time_series_bounds)
    @api.marshal_with(time_series_model)
    def get(self, id):
        """
        Returns data for time series representing progress of a job.
        """
        query = FcJobGraph.query
        bounds = time_series_bounds.parse_args(request)
        query = try_bounds(query, FcJobGraph, bounds)
        query = query.filter_by(job_id=id)
            
        return {
            'datasets': build_job_datasets(query)
        }


@ns.route('/jobWorkunits')
class jobWorkunits(Resource):

    @api.expect(time_series_bounds)
    @api.marshal_with(time_series_model)
    def get(self):
        """
        Returns datasets for time series representing workunits distribution to hosts.
        """
        query = FcWorkunit.query
        bounds = time_series_bounds.parse_args(request)
        query = try_bounds(query, FcWorkunit, bounds)
            
        return {
            'datasets': build_wu_datasets(query)
        }


@ns.route('/jobWorkunits/<int:id>')
class jobWorkunitsSingle(Resource):

    @api.expect(time_series_bounds)
    @api.marshal_with(time_series_model)
    def get(self, id):
        """
        Returns datasets for time series representing workunits distribution to hosts for single job.
        """
        query = FcWorkunit.query
        bounds = time_series_bounds.parse_args(request)
        query = try_bounds(query, FcWorkunit, bounds)
        query = query.filter_by(job_id=id)
            
        return {
            'datasets': build_wu_datasets(query)
        }


@ns.route('/batchWorkunits/<int:id>')
class batchWorkunits(Resource):

    @api.marshal_with(time_series_model)
    def get(self, id):
        """
        Returns datasets for time series representing workunits distribution to hosts in batch.
        """
        job_ids = getIdsFromBatch(id)
        query = FcWorkunit.query
        bounds = time_series_bounds.parse_args(request)
        query = try_bounds(query, FcWorkunit, bounds)
        query = query.filter(FcWorkunit.job_id.in_(job_ids))
            
        return {
            'datasets': build_wu_datasets(query)
        }

def get_dough (ids):
    return db.session.query(Host.domain_name.label('label'),
        func.cast(func.sum(FcWorkunit.hc_keyspace), Integer()).label('value')).filter(
        FcWorkunit.job_id.in_(ids)).group_by(FcWorkunit.boinc_host_id).join(FcWorkunit.host).all()

@ns.route('/jobContribution/<int:id>')
class jobContribution(Resource):

    @api.expect(time_series_bounds)
    @api.marshal_with(doughnut_model)
    def get(self, id):
        """
        Returns data for doughnut chart representing contribution of hosts in job.
        """
        query = get_dough([id])

        labels, data = build_doughnut(query)
        return {
            'labels': labels,
            'data': data,
            'meta': {
                'id': id
            }
        }


@ns.route('/batchContribution/<int:id>')
class batchContribution(Resource):

    @api.marshal_with(doughnut_model)
    def get(self, id):
        """
        Returns data for doughnut chart representing contribution of hosts in batch.
        """
        job_ids = getIdsFromBatch(id)
        query = get_dough(job_ids)

        labels, data = build_doughnut(query)
        return {
            'labels': labels,
            'data': data,
            'meta': {
                'id': id
            }
        }
