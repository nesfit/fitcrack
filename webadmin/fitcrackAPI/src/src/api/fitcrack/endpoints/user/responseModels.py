'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

role_model = api.model('User role', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(),
    'MANAGE_USERS': fields.Boolean(default=False),
    'ADD_NEW_JOB': fields.Boolean(default=False),
    'UPLOAD_DICTIONARIES': fields.Boolean(default=False),
    'VIEW_ALL_JOBS': fields.Boolean(default=False),
    'EDIT_ALL_JOBS': fields.Boolean(default=False),
    'OPERATE_ALL_JOBS': fields.Boolean(default=False),
    'ADD_USER_PERMISSIONS_TO_JOB': fields.Boolean(default=False)
})

role_list_model = api.model('User role list', {
    'items': fields.List(fields.Nested(role_model))
})

fc_user_model = api.model('User', {
    'id': fields.Integer(readOnly=True, required=False),
    'username': fields.String(readOnly=True, required=False),
    'mail': fields.String(),
    'role': fields.Nested(role_model)
})

user_list_model = api.model('User list', {
    'items': fields.List(fields.Nested(fc_user_model))
})

isLoggedIn_model = api.model('isLoggedIn', {
    'user': fields.Nested(fc_user_model),
    'loggedIn': fields.Boolean()
})
