'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse, fields

from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination

rule_parser = pagination.copy()
rule_parser.add_argument('search', type=str, required=False)

preview_request = api.model('Preview request model', {
   'passwords': fields.List(fields.String(), required=True, description='Password to be mangled'),
   'rules': fields.List(fields.String(), required=True, description='Rules for password mangling')
})