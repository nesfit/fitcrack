'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask_restplus import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.status.responseModels import JobStatusCollection_model
from src.database.models import FcJobStatus
from src.api.fitcrack.lang import statuses, package_status_text_to_code_dict

log = logging.getLogger(__name__)
ns = api.namespace('status', description='Endpointy pro čtení změn job statusů.')


@ns.route('')
class statusCollection(Resource):

    @api.marshal_with(JobStatusCollection_model)
    def get(self):
        """
        Vraci kolekci vsech Job Statusu
        """
        return {'items': FcJobStatus.query.all()}


@ns.route('/<id>')
class statusCollection(Resource):

    @api.marshal_with(JobStatusCollection_model)
    def get(self, id):
        """
        Vraci kolekci vsech Job Statusu
        """
        return {'items': FcJobStatus.query.filter(FcJobStatus.job_id == id).all()}
