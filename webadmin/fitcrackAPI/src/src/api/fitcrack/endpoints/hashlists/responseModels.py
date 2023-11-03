'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import pagination, job_nano_model
from src.api.fitcrack.endpoints.job.responseModels import hash_model

hash_list_short_model = api.model('short hash list', {
    'id': fields.Integer(readOnly=True, required=False),
    'name' : fields.String(readOnly=True,required=True),
    'hash_type_name' : fields.String('hash_type',readOnly=True),
    'hash_count' : fields.Integer(readOnly=True, required=True)
})

hash_list_long_model = api.model('long hash list', {
    'id': fields.Integer(readOnly=True, required=False),
    'name' : fields.String(readOnly=True,required=True),
    'hash_type_name' : fields.String('hash_type',readOnly=True),
    'hash_count' : fields.Integer(readOnly=True, required=True),
    'jobs' : fields.List(fields.Nested(job_nano_model)),
    'hashes' : fields.List(fields.Nested(hash_model))
})

empty_hash_list_created_model = api.model('new hashlist', {
    'id': fields.Integer(readOnly=True, required=False),
    'result' : fields.String(),
    'name' : fields.String()
})

page_of_hash_lists_model = api.inherit('Page of jobs', pagination, {
    'items': fields.List(fields.Nested(hash_list_short_model))
})

hash_addition_result_model = api.model('added hashes', {
    'result' : fields.String(),
    'id' : fields.Integer(),
    'name' : fields.String(),
    'hashCount' : fields.Integer(),
    'addedCount' : fields.Integer(),
    'erroredCount' : fields.Integer()
    #Here shall be also the list of hash errors.
})
