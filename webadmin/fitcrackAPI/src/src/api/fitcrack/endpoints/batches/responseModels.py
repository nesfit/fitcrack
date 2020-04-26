'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import pagination, job_micro_model

batch_status = api.model('Batch status object', {
    'code': fields.Integer(),
    'text': fields.String(),
    'description': fields.String(),
    'icon': fields.String(),
    'color': fields.String()
})

batch_model = api.model('Job batch', {
    'id': fields.Integer(readOnly=True),
    'name': fields.String(),
    'total_jobs': fields.Integer(readOnly=True),
    'waiting_jobs': fields.Integer(readOnly=True),
    'status': fields.Nested(batch_status),
    'current_user_can_edit': fields.Boolean(),
    'current_user_can_operate': fields.Boolean()
})

batch_with_jobs_model = api.inherit('Job batch with list of jobs', batch_model, {
    'jobs': fields.List(fields.Nested(job_micro_model))
})

page_of_batches_model = api.inherit('Page of batches', pagination, {
    'items': fields.List(fields.Nested(batch_model))
})

