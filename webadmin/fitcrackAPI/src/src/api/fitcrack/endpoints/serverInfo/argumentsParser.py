'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

package_graph_arguments = reqparse.RequestParser()
package_graph_arguments.add_argument('from_date', type=str,
                                     help='datum od kedy sa ma graf zobrazovat. fromat - %Y-%m-%d %H:%M:%S',
                                     default=None)
package_graph_arguments.add_argument('to_date', type=str,
                                     help='datum do kedy sa ma graf zobrazovat. fromat - %Y-%m-%d %H:%M:%S',
                                     default=None)

operation = reqparse.RequestParser()
operation.add_argument('operation', type=str, required=True,  help='operácia čo sa má vykonať na serveri',
                       choices=["start", "stop", "restart"])


serverUsage_argument = reqparse.RequestParser()
serverUsage_argument.add_argument('cpu', type=float, required=True)
serverUsage_argument.add_argument('ram', type=float, required=True)
serverUsage_argument.add_argument('net_recv', type=float, required=True)
serverUsage_argument.add_argument('net_sent', type=float, required=True)
serverUsage_argument.add_argument('hdd_read', type=float, required=True)
serverUsage_argument.add_argument('hdd_write', type=float, required=True)