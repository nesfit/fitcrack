'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
import json

from flask import request
from flask_login import current_user

from flask_restplus import Resource, abort

from src.api.apiConfig import api
from src.api.fitcrack.lang import status_to_code
from src.api.fitcrack.endpoints.batches.argumentsParser import batch_definition, batch_list
from src.api.fitcrack.endpoints.batches.responseModels import batch_with_jobs_model, page_of_batches_model
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
            query = FcBatch.query
            if not current_user.role.OPERATE_ALL_JOBS or not current_user.role.VIEW_ALL_JOBS:
                query = query.filter_by(creator_id=current_user.id)
            batch = query.filter_by(id=id).one_or_none()
        if not id or not batch: # new batch
            batch = FcBatch()
            is_new = True

        batch.name = name
        batch.jobs = FcJob.query.filter(FcJob.id.in_(jobs)).all()
        if is_new:
            batch.creator_id = current_user.id

        # queue order
        for index, job_id in enumerate(jobs):
            job = FcJob.query.filter_by(id=job_id).one_or_none()
            if job:
                job.queue_position = index

        # save batch
        try:
            if is_new:
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

        query = FcBatch.query
        if not current_user.role.VIEW_ALL_JOBS:
            query = query.filter_by(creator_id=current_user.id)
        query = query.order_by(FcBatch.id.desc())
        batches_page = query.paginate(page, per_page, error_out=True)

        return batches_page


@ns.route('/<int:id>')
class concrete_batch(Resource):

    @api.marshal_with(batch_with_jobs_model)
    @api.response(404, 'Batch doesn\'t exist')
    def get(self, id):
        """
        Returns job batch.
        """
        batch = FcBatch.query.filter_by(id=id).one_or_none()
        if batch and not current_user.role.VIEW_ALL_JOBS and batch.creator_id != current_user.id:
            abort(401, 'Unauthorized to access this batch.')
        batch.jobs.sort(key=lambda x: x.queue_position)
        return batch

    @api.response(200, 'Deleted')
    @api.response(500, 'Failed')
    def delete(self, id):
        """
        Deletes (unlinks) a batch.
        """
        batch = FcBatch.query.filter_by(id=id).one_or_none()
        if batch and not current_user.role.EDIT_ALL_JOBS and batch.creator_id != current_user.id:
            abort(401, 'Unauthorized to unlink this batch.')

        try:
            FcBatch.query.filter_by(id=id).delete()
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t delete batch.')
        return ('Deleted', 200)


@ns.route('/<int:id>/run')
class concrete_batch(Resource):

    @api.response(200, 'Sarted running')
    @api.response(200, 'Nothing to do')
    @api.response(500, 'Failed')
    def post(self, id):
        """
        Runs a batch by starting the first job that is ready.
        """
        batch = FcBatch.query.filter_by(id=id).one_or_none()
        if batch and not current_user.role.OPERATE_ALL_JOBS and batch.creator_id != current_user.id:
            abort(401, 'Unauthorized to operate this batch.')

        starter = FcJob.query.filter_by(batch_id=id).filter_by(status=0).order_by(FcJob.queue_position).first()
        if not starter:
            return ('Nothing to do', 200)
        
        starter.status = status_to_code['running']

        try:
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t run batch, try manually starting the first job.')
        return ('Sarted running', 200)


@ns.route('/<int:id>/interrupt')
class concrete_batch(Resource):

    @api.response(200, 'Interrupted')
    @api.response(200, 'Nothing to do')
    @api.response(500, 'Failed')
    def post(self, id):
        """
        Interrupts a batch by stopping the job that is running.
        """
        batch = FcBatch.query.filter_by(id=id).one_or_none()
        if batch and not current_user.role.OPERATE_ALL_JOBS and batch.creator_id != current_user.id:
            abort(401, 'Unauthorized to operate this batch.')

        stopper = FcJob.query.filter_by(batch_id=id).filter(FcJob.status >= 10).one_or_none()
        if not stopper:
            return ('Nothing to do', 200)

        stopper.status = status_to_code['finishing']

        try:
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t interrupt batch, try manually purging the running job.')
        return ('Interrupted', 200)