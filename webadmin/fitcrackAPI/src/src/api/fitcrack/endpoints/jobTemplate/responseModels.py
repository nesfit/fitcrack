'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api

template_model = api.model('Job template', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'created': fields.DateTime(readOnly=True, required=False),
})

templates_model = api.inherit('Job template collection', {
    'items': fields.List(fields.Nested(template_model))
})

template_data = api.inherit('Template with job settings', {
    'template': fields.String(readOnly=True, required=False)
})
