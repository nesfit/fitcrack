'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api

charset_model = api.model('charset', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'keyspace': fields.Integer(),
    'time': fields.DateTime(readOnly=True, required=False),
    'data': fields.String()
})

charsetCollection_model = api.inherit('Charset collection', {
    'items': fields.List(fields.Nested(charset_model))
})