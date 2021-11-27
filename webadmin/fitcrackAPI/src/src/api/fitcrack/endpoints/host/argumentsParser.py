'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import inputs

from src.api.fitcrack.argumentsParser import pagination

jobHost_parser = pagination.copy()
jobHost_parser.add_argument('name', type=str, required=False, help='Filter according to name')

jobHost_parser.add_argument('order_by', type=str, required=False, help='Ordering of the results', choices=['domain_name', 'os_name', 'p_model', 'time', 'status'])
jobHost_parser.add_argument('descending', type=inputs.boolean, required=False)
jobHost_parser.add_argument('showDeleted', type=inputs.boolean, required=False, default=False)
jobHost_parser.add_argument('all', type=inputs.boolean, required=False, default=False)
