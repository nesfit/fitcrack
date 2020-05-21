'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

settings_model = api.model('Settings', {
    'default_seconds_per_workunit': fields.Integer(),
    'default_workunit_timeout_factor': fields.Integer(),
    'default_hwmon_temp_abort': fields.Integer(),
    'default_bench_all': fields.Boolean(),
    'default_distribution_coefficient_alpha': fields.Float(),
    'default_tp_min': fields.Integer(),
    'default_ramp_up_workunits': fields.Integer(),
    'default_ramp_down_coefficient': fields.Float()
})