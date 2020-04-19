'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
import json

from flask import request

from flask_restplus import Resource, abort

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.batches.argumentsParser import batch_definition, batch_list
from src.api.fitcrack.endpoints.batches.responseModels import batch_model, page_of_batches_model
from src.api.fitcrack.responseModels import simpleResponse

from src.database import db
from src.database.models import FcBatch, FcJob

log = logging.getLogger(__name__)
ns = api.namespace('batches', description='Endpoints for job batches.')

@ns.route('')
class batches(Resource):

    @api.expect(batch_definition)
    @api.response(201, 'Created')
    @api.response(200, 'Updated')
    @api.response(500, 'Failed')
    def put(self):
        """
        Add or change a batch with job queue. Provide ID to update existing.
        """
        data = request.json
        # sort out args
        id = data.get('id', None)
        name = data['name']
        jobs = data['jobs']

        if id:
            batch = FcBatch.query.filter_by(id=id).one_or_none()
        if not id or not batch: # new batch
            batch = FcBatch()

        batch.name = name
        batch.jobs = FcJob.query.filter(FcJob.id.in_(jobs)).all()

        # queue order
        for index, job_id in enumerate(jobs):
            job = FcJob.query.filter_by(id=job_id).one_or_none()
            if job:
                job.queue_position = index

        # save batch
        try:
            if not id:
                db.session.add(batch)
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t create batch.')
        return (batch.id, 201) if not id else ('Updated', 200)

    @api.expect(batch_list)
    @api.marshal_with(page_of_batches_model)
    def get(self):
        """
        Returns paginated collection of job batches.
        """
        args = batch_list.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)

        batches_query = FcBatch.query.order_by(FcBatch.id.desc())
        batches_page = batches_query.paginate(page, per_page, error_out=True)

        return batches_page


@ns.route('/<int:id>')
class concrete_batch(Resource):

    @api.response(200, 'Deleted')
    @api.response(500, 'Failed')
    def delete(self, id):
        """
        Deletes a batch.
        """
        try:
            FcBatch.query.filter_by(id=id).delete()
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t delete batch.')
        return ('Deleted', 200)