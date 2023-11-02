'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse, inputs
from werkzeug.datastructures import FileStorage

from src.api.apiConfig import api
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

#Taken from the former endpoint for adding jobs.
hash_list_add_hash_list_parser = api.schema_model('addHashList_hashes', {
   'type': 'object',
   'required': ['hash_type'],
   'properties': {
         'hash_type': {
            'default': '0',
            'type': 'string',
            'description': 'Hash code from /hashcat/hashTypes'
         },
         'hash_list': {
            'type': 'array',
            'items': {
               'type': 'object',
               'properties': {
                     'hash': {
                        'type': 'string',
                        'description': 'hash'
                     }
               }
            }
         },
         'valid_only': {
            'default': False,
            'type': 'boolean',
            'description': 'Specifies whether hash validation is enforced'
         }
   }
})


hash_list_add_hash_file_parser = reqparse.RequestParser()
hash_list_add_hash_file_parser.add_argument('hash_type',type=int,required=True)
hash_list_add_hash_file_parser.add_argument('valid_only',type=inputs.boolean, required=True)
hash_list_add_hash_file_parser.add_argument('file', location='files', type=FileStorage, required=True)
