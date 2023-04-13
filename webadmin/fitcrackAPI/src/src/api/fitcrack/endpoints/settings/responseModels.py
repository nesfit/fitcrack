'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api

settings_model = api.model('Settings', {
    'default_seconds_per_workunit': fields.Integer(),
    'workunit_timeout_factor': fields.Integer(),
    'hwmon_temp_abort': fields.Integer(),
    'bench_all': fields.Boolean(),
    'distribution_coefficient_alpha': fields.Float(),
    't_pmin': fields.Integer(),
    'ramp_up_workunits': fields.Boolean(),
    'ramp_down_coefficient': fields.Float(),
    'verify_hash_format': fields.Boolean(),
    'auto_add_hosts_to_running_jobs': fields.Boolean(),
    'max_mangled_passwords': fields.Integer()
})