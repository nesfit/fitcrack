'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

rule_model = api.model('rule', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'count': fields.Integer(readOnly=True),
    'time': fields.DateTime(readOnly=True, required=False)
})

rules_model = api.model('rules collection', {
    'items': fields.List(fields.Nested(rule_model))
})

ruleData_model = api.inherit('Rules with data', {
    'data': fields.String(),
    'status': fields.Boolean()
})
