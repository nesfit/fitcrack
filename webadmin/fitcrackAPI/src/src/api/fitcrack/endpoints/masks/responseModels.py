'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

maskSet_model = api.model('maskSet', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'time': fields.DateTime(readOnly=True, required=False),
    'data': fields.String()
})
