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

file_content = api.model('File info and content data', {
    'name': fields.String(),
    'path': fields.String(),
    'data': fields.String()
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

job_permissions = api.model('Job permissions object', {
    'view': fields.Boolean(),
    'edit': fields.Boolean(),
    'operate': fields.Boolean(),
    'owner': fields.Boolean()
})

job_nano_model = api.model('Job nano', {
    'id': fields.Integer(readOnly=True, required=False, description='id of the job'),
    'name': fields.String(required=True, description='job name'),
    'status': fields.Integer(readOnly=True, required=False, description='state of the job'),
})

boincHost_model = api.model('Host boinc', {
    'id': fields.Integer(readOnly=True, required=False),
    'domain_name': fields.String(),
    'ip_address': fields.String(attribute='last_ip_addr'),
    'p_model': fields.String(),
    'os_name': fields.String(),
    'user': fields.Nested(user_model),
    'last_active': fields.Nested(boincHostActivity_model),
    'deleted': fields.Boolean(),
    'jobs': fields.Nested(job_nano_model)
})

job_short_model = api.model('Job short', {
    'id': fields.Integer(readOnly=True, required=False, description='id job'),
    'name': fields.String(required=True, description='name of the job'),
    'comment': fields.String(required=False),
    'permissions': fields.Nested(job_permissions),
    'batch_id': fields.Integer(), # to know if job is enqueued and where
    'queue_position': fields.Integer(),
    'attack_mode': fields.String(required=True),
    'attack': fields.String(required=True),
    'host_count': fields.Integer(),
    'keyspace': fields.String(required=True),
    'hc_keyspace': fields.String(required=True),
    'status': fields.String(required=False),
    'status_text': fields.String(required=False),
    'status_tooltip': fields.String(required=False),
    'status_type': fields.String(),
    'progress': fields.Float(required=False),
    'time': fields.DateTime(required=False),
    'cracking_time': fields.Float(),
    'hash_type': fields.String(required=True),
    'time_start': fields.DateTime(required=True),
    'time_end': fields.DateTime(required=True),
    'deleted': fields.Boolean()
})

job_micro_model = api.model('Job micro', {
    'id': fields.Integer(readOnly=True, required=False, description='job id'),
    'name': fields.String(required=True, description='job name'),
    'permissions': fields.Nested(job_permissions),
    'queue_position': fields.Integer(),
    'attack': fields.String(),
    'status': fields.String(required=False),
    'status_text': fields.String(required=False),
    'status_tooltip': fields.String(required=False),
    'status_type': fields.String(),
    'progress': fields.Float(required=False),
})