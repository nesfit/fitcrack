'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.job.responseModels import workunit_model
from src.api.fitcrack.responseModels import pagination, host_short_model, job_short_model, boincHost_model, \
    user_model

page_of_hosts_model = api.inherit('Page of hosts', pagination, {
    'items': fields.List(fields.Nested(boincHost_model))
})

workunitWithJob_model = api.inherit('Workunit with job', workunit_model, {
    'job': fields.Nested(job_short_model, attribute='job')
})

boincHostDetail_model = api.model('Host detail boinc', {
    'id': fields.Integer(readOnly=True, required=False),
    'domain_name': fields.String(),
    'p_model': fields.String(),
    'user': fields.Nested(user_model),
    'os_name': fields.String(),
    'fc_host': fields.Nested(host_short_model),
    'active': fields.Boolean(),
    'workunits': fields.List(fields.Nested(workunitWithJob_model)),
    'jobs': fields.List(fields.Nested(job_short_model))
})

boincHostBenchmarks_model = api.model('Boinc host benchmarks', {
    'hash_type': fields.Integer(),
    'attack_mode': fields.Integer(),
    'power': fields.Integer()
})
