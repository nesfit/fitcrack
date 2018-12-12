'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

log_model = api.model('log', {
    'type': fields.String(readOnly=True, required=False),
    'time': fields.DateTime(readOnly=True, required=False),
    'host': fields.Integer(readOnly=True, required=False),
    'package': fields.Integer(readOnly=True, required=False),
    'log': fields.String(readOnly=True, required=False),
    'raw': fields.String(readOnly=True, required=False)
})
logsCollection_model = api.inherit('logs collection', {
    'items': fields.List(fields.Nested(log_model))
})
