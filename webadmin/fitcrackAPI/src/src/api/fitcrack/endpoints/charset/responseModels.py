'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

charset_model = api.model('charset', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'keyspace': fields.Integer(),
    'time': fields.DateTime(readOnly=True, required=False),
    'data': fields.String(),
    'canDecode': fields.Boolean()
})
