'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

dictionary_model = api.model('Dictionary', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'keyspace': fields.Integer(readOnly=True, required=False),
    'time': fields.DateTime(readOnly=True, required=False),
})

dictionaries_model = api.inherit('Dictionaries collection', {
    'items': fields.List(fields.Nested(dictionary_model))
})

dictData_model = api.inherit('Dictionary with data', {
    'data': fields.String(),
    'status': fields.Boolean()
})
