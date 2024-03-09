'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse

settings_arguments = reqparse.RequestParser()
settings_arguments.add_argument('default_seconds_per_workunit', type=int, help='', required=False, location='json')
settings_arguments.add_argument('workunit_timeout_factor', type=int, help='', required=False, location='json')
settings_arguments.add_argument('hwmon_temp_abort', type=int, help='', required=False, location='json')
settings_arguments.add_argument('bench_all', type=bool, help='', required=False, location='json')
settings_arguments.add_argument('distribution_coefficient_alpha', type=float, help='', required=False, location='json')
settings_arguments.add_argument('t_pmin', type=int, help='', required=False, location='json')
settings_arguments.add_argument('ramp_up_workunits', type=bool, help='', required=False, location='json')
settings_arguments.add_argument('ramp_down_coefficient', type=float, help='', required=False, location='json')
settings_arguments.add_argument('verify_hash_format', type=bool, help='', required=False, location='json')
settings_arguments.add_argument('auto_add_hosts_to_running_jobs', type=bool, help='', required=False, location='json')
settings_arguments.add_argument('merge_masks', type=bool, help='', required=False, location='json')