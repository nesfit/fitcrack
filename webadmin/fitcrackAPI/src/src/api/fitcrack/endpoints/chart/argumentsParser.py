'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse

dateformat = '%Y-%m-%dT%H:%M:%S'

time_series_bounds = reqparse.RequestParser()
time_series_bounds.add_argument('from', type=str,
                                     help='Begin time series from this date and time. Format: {}'.format(dateformat),
                                     default=None)
time_series_bounds.add_argument('to', type=str,
                                     help='End time series on this date and time. Format: {}'.format(dateformat),
                                     default=None)
