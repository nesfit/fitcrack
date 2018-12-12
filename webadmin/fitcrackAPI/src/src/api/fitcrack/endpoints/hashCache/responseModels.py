'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.package.responseModels import job_model
from src.api.fitcrack.responseModels import pagination, host_short_model, package_short_model, boincHost_model, \
    user_model






hashes_model = api.model('Hashes model', {
    'id': fields.Integer(readOnly=True, required=False),
    'password': fields.String(),
    'hash_type_name': fields.String(),
    'hash': fields.String(attribute='hashText'),
    'added': fields.DateTime(),
    'package': fields.Nested(package_short_model)
})


page_of_hashes_model = api.inherit('Page of hashes', pagination, {
    'items': fields.List(fields.Nested(hashes_model))
})


