'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

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

previewPasswords_model = api.model('Changed passwords preview', {
    'passwordsPreview': fields.List(fields.Nested(api.model("Changed password preview", {
        'finalPassword': fields.String(required=True, description='Password after rule mangling'),
        'retCode': fields.Integer(required=True, 
            description='Return code of password mangling function (Length of password if OK, -3 if comment, -2 if empty rule or password, -1 if syntax error)')
        })), required=True)
})

randomRule_model = api.model('Random rule', {
    'randomRule' : fields.String(required=True, description='Generated random rule')
})