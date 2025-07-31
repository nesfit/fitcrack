'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import pagination
from src.api.fitcrack.endpoints.job.responseModels import hash_model, job_nano_model


hash_list_model_short = api.model('short hash list', {
    'id': fields.Integer(readOnly=True, required=False),
    'name' : fields.String(readOnly=True,required=True),
    'hash_type_name' : fields.String('hash_type',readOnly=True),
    'hash_count' : fields.Integer(readOnly=True, required=True),
    'cracked_hash_count' : fields.Integer(readOnly=True, required=True),
    'added': fields.DateTime(readOnly=True, required=True),
    'job_count' : fields.Integer(readOnly=True, required=True),
    'is_locked' : fields.Boolean(readOnly=True, required=True),
    'deleted' : fields.Boolean(readOnly=True, required=True)
})


hash_list_model_long = api.inherit('long hash list', hash_list_model_short, {
    'jobs' : fields.List(fields.Nested(job_nano_model)),
})


empty_hash_list_created_model = api.model('new hash list', {
    'id': fields.Integer(readOnly=True, required=False),
    'result' : fields.String(),
    'name' : fields.String()
})


page_of_hash_lists_model = api.inherit('Page of jobs', pagination, {
    'items': fields.List(fields.Nested(hash_list_model_short))
})


page_of_hashes_model = api.inherit('Page of hashes of a given hash list', pagination, {
    'items' : fields.List(fields.Nested(hash_model))
})


error_model = api.model('a hash and a validation error message', {
    'order_in_input' : fields.Integer(),
    'error' : fields.String()
})


hash_addition_result_model = api.model('added hashes', {
    'result' : fields.String(),
    'id' : fields.Integer(),
    'name' : fields.String(),
    'hashCount' : fields.Integer(),
    'addedCount' : fields.Integer(),
    'erroredCount' : fields.Integer(),
    'errors': fields.List(fields.Nested(error_model))
})


hash_extraction_result_model = api.inherit('extracted hashes', hash_addition_result_model, {
    'extracted_easy_hash' : fields.Boolean()
})