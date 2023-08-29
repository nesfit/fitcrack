'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.job.responseModels import workunit_model
from src.api.fitcrack.responseModels import pagination, host_short_model, boincHost_model, \
    user_model

hashes_model = api.model('Hashes model', {
    'id': fields.Integer(readOnly=True, required=False),
    'password': fields.String(),
    'hash_type_name': fields.String(),
    'hash': fields.String(attribute='hashText'),
    'added': fields.DateTime(),
})

hashlist_model = api.model('Hashlist model', {
    'id': fields.Integer(readOnly=True, required=False),
    'hash_type_name': fields.String(),
    'name': fields.String(),
    'added': fields.DateTime(),
    'deleted': fields.Boolean(),
    'items': fields.List(fields.Nested(hashes_model))
})

page_of_hashes_model = api.inherit('Page of hashes', pagination, {
    'items': fields.List(fields.Nested(hashes_model))
})
