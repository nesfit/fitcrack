'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import pagination

hash_list_short_model = api.model('short hashlist', {
    'id': fields.Integer(readOnly=True, required=False),
    'name' : fields.String(readOnly=True,required=True),
    'hash_type_name' : fields.String('hash_type',readOnly=True),
    'hash_count' : fields.Integer(readOnly=True, required=True)
})

empty_hash_list_created_model = api.model('new hashlist', {
    'id': fields.Integer(readOnly=True, required=False),
    'result' : fields.String(),
    'name' : fields.String(),
})

page_of_hash_lists_model = api.inherit('Page of jobs', pagination, {
    'items': fields.List(fields.Nested(hash_list_short_model))
})

""" charset_model = api.model('charset', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'keyspace': fields.Integer(),
    'time': fields.DateTime(readOnly=True, required=False),
    'data': fields.String()
})

charsetCollection_model = api.inherit('Charset collection', {
    'items': fields.List(fields.Nested(charset_model))
}) """