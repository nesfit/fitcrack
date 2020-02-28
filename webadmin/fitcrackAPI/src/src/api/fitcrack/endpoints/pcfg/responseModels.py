'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields
from src.api.apiConfig import api

pcfg_model = api.model('PCFG', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'keyspace': fields.Integer(readOnly=True, required=False),
    'time_added': fields.DateTime(readOnly=True, required=False)
})

pcfgs_model = api.inherit('PCFGs collection', {
    'items': fields.List(fields.Nested(pcfg_model))
})

pcfgData_model = api.inherit('PCFG with data', {
    'data': fields.String(),
    'status': fields.Boolean()
})
