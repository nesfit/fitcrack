'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse, inputs
from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination

batch_list = pagination.copy()

batch_definition = api.schema_model('batch_definition', {
  'properties': {
    'id': {'type': 'integer'},
    'name': {'type': 'string', 'required': True},
    'jobs': {'type': 'array', 'items': {'type': 'integer'}, 'required': True},
  }
})