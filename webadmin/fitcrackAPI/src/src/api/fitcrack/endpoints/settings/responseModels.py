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
    'default_bench_all': fields.Boolean()
})