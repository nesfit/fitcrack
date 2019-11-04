'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import inputs

from src.api.fitcrack.argumentsParser import pagination

hashes_parser = pagination.copy()
hashes_parser.add_argument('search', type=str, required=False, help='Filter hashes according to password or hash')

hashes_parser.add_argument('order_by', type=str, required=False, help='Ordering of results',
                                choices=['result', 'hash_type', 'hash', 'added'])
hashes_parser.add_argument('descending', type=inputs.boolean, required=False)
