'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
import json

from flask import request

from flask_restx import Resource, abort

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.bins.responseModels import bin_model, bin_with_jobs_model, bins_model
from src.api.fitcrack.endpoints.bins.argumentsParser import name, position, job_assignment
from src.api.fitcrack.responseModels import simpleResponse

from src.database import db
from src.database.models import FcBin, FcJob

log = logging.getLogger(__name__)
ns = api.namespace('bins', description='Endpoints for job bins.')

@ns.route('')
class bins(Resource):

    @api.expect(name)
    @api.response(201, 'Created')
    @api.response(500, 'Failed')
    def post(self):
        """
        Add a named job bin.
        """
        data = request.json
        bin = FcBin(name=data['name'])
        try:
            db.session.add(bin)
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t add bin.')
        return ('Created', 201)

    @api.marshal_with(bins_model)
    def get(self):
        """
        Returns collection of job bins.
        """
        return {'items': FcBin.query.order_by(FcBin.position).all()}


@ns.route('/<int:id>')
class concrete_bin(Resource):

    # GET bin_with_jobs deprecated, use GET job_collection with bin arg

    @api.expect(name)
    @api.response(200, 'Modified')
    @api.response(404, 'Bin not found')
    def patch(self, id):
        """
        Modifies a bin. Currently only rename possible.
        """
        data = request.json
        bin = FcBin.query.get(id)
        if bin is None:
            abort(404, 'No such bin')
        else:
            bin.name = data['name']
            db.session.commit()
            return ('Modified', 200)

    @api.response(200, 'Deleted')
    @api.response(500, 'Failed')
    def delete(self, id):
        """
        Deletes a bin and adjusts positions of other bins.
        """
        # using a stored procedure, reordering happens on db level
        # see procedures in 20_create_triggers.sql inside /server/sql
        connection = db.engine.raw_connection()
        try:
            cursor = connection.cursor()
            cursor.callproc("delete_bin", [id])
            cursor.close()
            connection.commit()
            return ('Deleted', 200)
        finally:
            connection.close() 


@ns.route('/<int:id>/move')
class move_bin(Resource):

    @api.expect(position)
    @api.response(200, 'Moved')
    @api.response(404, 'Bin not found')
    def patch(self, id):
        """
        Moves a bin in stored ordering and adjusts other bins.
        """
        # using a stored procedure, reordering happens on db level
        # see procedures in 20_create_triggers.sql inside /server/sql
        data = request.json
        target = data['position']

        connection = db.engine.raw_connection()
        try:
            cursor = connection.cursor()
            cursor.callproc("move_bin", [id, target])
            cursor.close()
            connection.commit()
            return ('Moved', 200)
        finally:
            connection.close() 


@ns.route('/<int:id>/assign')
class assign_bin(Resource):

    @api.expect(job_assignment)
    @api.response(200, 'Assigned jobs')
    @api.response(404, 'Bin not found')
    @api.response(500, 'Error assigning')
    def put(self, id):
        """
        Assigns jobs to a bin. Add jobs via include array, remove via exclude.
        """
        data = request.json
        include = data.get('include', [])
        exclude = data.get('exclude', [])

        bin = FcBin.query.get(id)
        bin.jobs = bin.jobs.all()

        # exclude if any
        bin.jobs = [job for job in bin.jobs if job.id not in exclude]

        # include if any
        jobs = FcJob.query.filter(FcJob.id.in_(include)).all()
        # remove first to prevent duplicates
        bin.jobs = [job for job in bin.jobs if job.id not in include]
        bin.jobs.extend(jobs)

        if bin is None:
            abort(404, 'No such bin')
        else:
            try:
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Couldn\'t assign to bin.')
            return ('included {}, excluded {}'.format(include, exclude), 200)