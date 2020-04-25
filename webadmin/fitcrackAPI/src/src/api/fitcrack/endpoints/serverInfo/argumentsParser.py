'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

job_graph_arguments = reqparse.RequestParser()
job_graph_arguments.add_argument('from_date', type=str,
                                     help='Date from which graph should appear. fromat - %Y-%m-%d %H:%M:%S',
                                     default=None)
job_graph_arguments.add_argument('to_date', type=str,
                                     help='Date from which graph should appear. fromat - %Y-%m-%d %H:%M:%S',
                                     default=None)

operation = reqparse.RequestParser()
operation.add_argument('operation', type=str, required=True,  help='Operationfor steps on server',
                       choices=["start", "stop", "restart"])


serverUsage_argument = reqparse.RequestParser()
serverUsage_argument.add_argument('cpu', type=float, required=True)
serverUsage_argument.add_argument('ram', type=float, required=True)
serverUsage_argument.add_argument('net_recv', type=float, required=True)
serverUsage_argument.add_argument('net_sent', type=float, required=True)
serverUsage_argument.add_argument('hdd_read', type=float, required=True)
serverUsage_argument.add_argument('hdd_write', type=float, required=True)

export_options = reqparse.RequestParser()
export_options.add_argument('jobs', type=int, action='split', help='List of job ids to export')
