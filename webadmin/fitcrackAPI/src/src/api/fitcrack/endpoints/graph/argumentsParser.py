'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

job_graph_arguments = reqparse.RequestParser()
job_graph_arguments.add_argument('from_date', type=str,
                                     help='Show graph since this date and time. Format: %Y-%m-%d %H:%M:%S',
                                     default=None)
job_graph_arguments.add_argument('to_date', type=str,
                                     help='Show graph till this date and time. Format: %Y-%m-%d %H:%M:%S',
                                     default=None)
