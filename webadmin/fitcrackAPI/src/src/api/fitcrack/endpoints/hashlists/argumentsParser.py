'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse, inputs

from src.api.fitcrack.argumentsParser import pagination

hash_list_parser = pagination.copy()
hash_list_parser.add_argument('name', type=str, required=False, help='filter by name')
hash_list_parser.add_argument('hash_type', type=str, required=False, help='filter by hash type')
hash_list_parser.add_argument('order_by', type=str, required=False, help='result ordering',
                                choices=['name', 'hash_type']) #'hash_count', 'job_count' maybe later
hash_list_parser.add_argument('descending', type=inputs.boolean, required=False)
hash_list_parser.add_argument('showDeleted', type=inputs.boolean, required=False, default=False)


make_empty_hash_list_parser = reqparse.RequestParser()
make_empty_hash_list_parser.add_argument('name', type=str, required=True)
