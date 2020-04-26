'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import pagination, job_short_model

bin_model = api.model('Job bin', {
    'id': fields.Integer(readOnly=True),
    'position': fields.Integer(readOnly=True),
    'name': fields.String(),
    'job_count': fields.Integer()
})

bin_with_jobs_model = api.inherit('Job bin with jobs list', bin_model, {
    'jobs': fields.List(fields.Nested(job_short_model))
})

bins_model = api.inherit('Job bin collection', {
    'items': fields.List(fields.Nested(bin_model))
})
