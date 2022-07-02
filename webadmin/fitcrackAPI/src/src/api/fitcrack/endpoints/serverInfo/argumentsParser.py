'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse

job_graph_arguments = reqparse.RequestParser()
job_graph_arguments.add_argument('from_date', type=str,
                                    help='Show graph since this date and time. Format: %Y-%m-%d %H:%M:%S',
                                    default=None)
job_graph_arguments.add_argument('to_date', type=str,
                                    help='Show graph till this date and time. Format: %Y-%m-%d %H:%M:%S',
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

delete_usage_data = reqparse.RequestParser()
delete_usage_data.add_argument('days', type=int, required=True)

export_options = reqparse.RequestParser()
export_options.add_argument('jobs', type=int, action='split', help='List of job ids to export')

dependency_list = reqparse.RequestParser()
dependency_list.add_argument('deps', type=list, location='json', help='List of dependencies to check')
