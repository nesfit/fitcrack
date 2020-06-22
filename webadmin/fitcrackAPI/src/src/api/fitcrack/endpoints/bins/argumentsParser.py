'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse, inputs
from src.api.apiConfig import api
# from src.api.fitcrack.argumentsParser import pagination

name = api.schema_model('bin_name', {'properties': {'name': {'type': 'string'}}})
position = api.schema_model('bin_position', {'properties': {'position': {'type': 'integer'}}})

job_assignment = api.schema_model('job_assignment', {
  'properties': {
    'include': {'type': 'array', 'items': {'type': 'integer'}},
    'exclude': {'type': 'array', 'items': {'type': 'integer'}},
  }
})