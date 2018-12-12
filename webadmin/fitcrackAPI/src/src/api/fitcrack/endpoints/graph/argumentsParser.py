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