'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import pagination

batch_status = api.model('Batch status object', {
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
    'status': fields.Nested(batch_status)
})

page_of_batches_model = api.inherit('Page of batches', pagination, {
    'items': fields.List(fields.Nested(batch_model))
})
