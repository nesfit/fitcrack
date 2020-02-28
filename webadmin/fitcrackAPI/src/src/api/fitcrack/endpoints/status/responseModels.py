'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

status_model = api.model('status', {
	'time': fields.DateTime(readOnly=True, required=False),
	'status': fields.String(required=False),
	'status_text': fields.String(required=False),
	'status_tooltip': fields.String(required=False),
	'status_type': fields.String(),
})

JobStatusCollection_model = api.inherit('Job status collection', {
	'items': fields.List(fields.Nested(status_model))
})
