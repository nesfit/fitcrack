'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import inputs

from src.api.fitcrack.argumentsParser import pagination

packageHost_parser = pagination.copy()
packageHost_parser.add_argument('name', type=str, required=False, help='filtrovanie uzlov podľa názvu')

packageHost_parser.add_argument('order_by', type=str, required=False, help='zoradenie výsledkov',
                                choices=['domain_name', 'os_name', 'p_model', 'time', 'status'])
packageHost_parser.add_argument('descending', type=inputs.boolean, required=False)
packageHost_parser.add_argument('showDeleted', type=inputs.boolean, required=False, default=False)
packageHost_parser.add_argument('all', type=inputs.boolean, required=False, default=False)




