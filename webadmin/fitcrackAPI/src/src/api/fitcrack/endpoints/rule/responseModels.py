'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

rule_model = api.model('rule', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'time': fields.DateTime(readOnly=True, required=False)
})


ruleData_model = api.inherit('Rules with data', {
    'data': fields.String(),
    'status': fields.Boolean()
})
