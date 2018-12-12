'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api




folderOrFile_model = api.model('folder or file', {
    'path': fields.String(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False)
})




directory_model = api.model('directory', {
    'path': fields.String(readOnly=True, required=False),
    'parent': fields.String(readOnly=True, required=False),
    'folders': fields.List(fields.Nested(folderOrFile_model)),
    'files': fields.List(fields.Nested(folderOrFile_model))
})
