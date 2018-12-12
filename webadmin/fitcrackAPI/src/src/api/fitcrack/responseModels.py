'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

"""response models which are used in multiple endpoints"""

from flask_restplus import fields

from src.api.apiConfig import api

pagination = api.model('A page of results', {
    'page': fields.Integer(description='Number of this page of results'),
    'pages': fields.Integer(description='Total number of pages of results'),
    'per_page': fields.Integer(description='Number of items per page of results'),
    'total': fields.Integer(description='Total number of results'),
})

simpleResponse = api.model('simpleResponse', {
    'status': fields.Boolean(description="True if success, False if failed."),
    'message': fields.String(description="Some details...")
})

host_short_model = api.model('Host short', {
    'id': fields.Integer(readOnly=True, required=False),
    'boinc_host_id': fields.Integer(readOnly=True, required=False),
    'power': fields.Integer(readOnly=True, required=False),
    'job_id': fields.Integer(readOnly=True, required=False),
    'status': fields.Integer(readOnly=True, required=False),
    'status_text': fields.String(),
    'time': fields.DateTime(readOnly=True, required=False),
})

user_model = api.model('Fc User', {
    'name': fields.String(),
})


boincHostActivity_model = api.model('Host activity', {
    'last_seen': fields.String(),
    'seconds_delta': fields.String(),
    'online': fields.Boolean()
})

boincHost_model = api.model('Host boinc', {
    'id': fields.Integer(readOnly=True, required=False),
    'domain_name': fields.String(),
    'ip_address': fields.String(attribute='last_ip_addr'),
    'p_model': fields.String(),
    'os_name': fields.String(),
    'user': fields.Nested(user_model),
    'last_active': fields.Nested(boincHostActivity_model),
    'deleted': fields.Boolean()
})

package_short_model = api.model('Package short', {
    'id': fields.Integer(readOnly=True, required=False, description='id package'),
    'name': fields.String(required=True, description='meno package'),
    'comment': fields.String(required=False),
    'priority': fields.Integer(),
    'attack_mode': fields.String(required=True),
    'attack': fields.String(required=True),
    'status': fields.String(required=False),
    'status_text': fields.String(required=False),
    'status_tooltip': fields.String(required=False),
    'status_type': fields.String(),
    'result': fields.String(required=False),
    'progress': fields.Float(required=False),
    'time': fields.DateTime(required=False),
    'cracking_time': fields.Float(),
    'hash_type': fields.String(required=True),
    'hash': fields.String(required=True),
    'time_start': fields.String(required=True),
    'time_end': fields.String(required=True),
    'password': fields.String(),
    'deleted': fields.Boolean()
})