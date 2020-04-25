'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

settings_arguments = reqparse.RequestParser()
settings_arguments.add_argument('default_seconds_per_workunit', type=int, help='', required=False, location='json')
settings_arguments.add_argument('default_workunit_timeout_factor', type=int, help='', required=False, location='json')
settings_arguments.add_argument('default_hwmon_temp_abort', type=int, help='', required=False, location='json')
settings_arguments.add_argument('default_bench_all', type=bool, help='', required=False, location='json')
