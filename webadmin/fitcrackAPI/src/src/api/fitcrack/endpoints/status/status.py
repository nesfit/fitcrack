'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask_restx import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.status.responseModels import JobStatusCollection_model
from src.database.models import FcJobStatus

log = logging.getLogger(__name__)
ns = api.namespace('status', description='Endpoints for reading changes in job status.')


@ns.route('')
class statusCollection(Resource):

    @api.marshal_with(JobStatusCollection_model)
    def get(self):
        """
        Returns collection of all job statuses.
        """
        return {'items': FcJobStatus.query.all()}


@ns.route('/<id>')
class statusCollection(Resource):

    @api.marshal_with(JobStatusCollection_model)
    def get(self, id):
        """
        Returns collection of all job statuses.
        """
        return {'items': FcJobStatus.query.filter(FcJobStatus.job_id == id).all()}
