'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import simpleResponse

protectedFile_model = api.model('Protected file', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'hash': fields.String(readOnly=True, required=False),
    'time': fields.DateTime(readOnly=True, required=False),
})
protectedFilesCollection_model = api.inherit('Protected files collection', {
    'items': fields.List(fields.Nested(protectedFile_model))
})

excryptedFileUploaded_model = api.inherit('Encryped file uploaded response', simpleResponse, {
    'hash': fields.String(),
    'hash_type': fields.String(),
    'hash_type_name': fields.String(),
    'file_id': fields.Integer()
})
