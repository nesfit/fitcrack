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
hash_list_parser.add_argument('hash_type', type=int, required=False, help='filter by hash type code')
hash_list_parser.add_argument('order_by', type=str, required=False, help='result ordering',
                                choices=['name', 'hash_type']) #'hash_count', 'job_count' maybe later
hash_list_parser.add_argument('descending', type=inputs.boolean, required=False)
hash_list_parser.add_argument('showDeleted', type=inputs.boolean, required=False, default=False)


#REMEMBER TO CHANGE THIS AND THE NON-PAGINATED VERSION IN LOCKSTEP!
hash_list_hashes_parser_paginated = pagination.copy()
hash_list_hashes_parser_paginated.add_argument('cracked',type=inputs.boolean,required=False,help='show only cracked/not cracked hashes')
hash_list_hashes_parser_paginated.add_argument('order_by',type=str,required=False,help='result ordering',
                              choices=['hash', 'result'])
hash_list_hashes_parser_paginated.add_argument('descending', type=inputs.boolean, required=False)


#REMEMBER TO CHANGE THIS AND THE PAGINATED VERSION IN LOCKSTEP!
hash_list_hashes_parser = reqparse.RequestParser()
hash_list_hashes_parser.add_argument('cracked',type=inputs.boolean,required=False,help='show only cracked/not cracked hashes')
hash_list_hashes_parser.add_argument('order_by',type=str,required=False,help='result ordering',
                              choices=['hash', 'result'])
hash_list_hashes_parser.add_argument('descending', type=inputs.boolean, required=False)


make_empty_hash_list_parser = reqparse.RequestParser()
make_empty_hash_list_parser.add_argument('name', type=str, required=True)


#Taken from the former endpoint for adding jobs.
hash_list_add_hash_list_parser = api.schema_model('addHashList_hashes', {
   'type': 'object',
   'required': ['hash_type', 'validation_mode'],
   'properties': {
         'hash_type': {
            'default': '0',
            'type': 'integer',
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
         'validation_mode': {
            'type': 'string',
            'description': 'Can be either "fail_invalid", "skip_invalid", or "no_validate"'
         },
         'name' : {
            'default' : None,
            'type' : 'string',
            'description': 'Can be supplied only if `id` is `new`. Will set the name of the newly created hash list to that name.'
         }
   }
})


hash_list_add_hash_file_parser = reqparse.RequestParser()
hash_list_add_hash_file_parser.add_argument('hash_type',type=int,required=True)
hash_list_add_hash_file_parser.add_argument('validation_mode',type=str, required=True,
                                            choices=['fail_invalid','skip_invalid','no_validate'])
hash_list_add_hash_file_parser.add_argument('file', location='files', type=FileStorage, required=True)
hash_list_add_hash_file_parser.add_argument('binary', type=inputs.boolean, required=True)
hash_list_add_hash_file_parser.add_argument('name', type=str, required=False,help='Can be supplied only if `id` is `new`. Will set the name of the newly created hash list to that name.')

hash_list_add_protected_file_parser = reqparse.RequestParser()
hash_list_add_protected_file_parser.add_argument('file', location='files', type=FileStorage, required=True)
hash_list_add_protected_file_parser.add_argument('name', type=str, required=False,help='Can be supplied only if `id` is `new`. Will set the name of the newly created hash list to that name.')