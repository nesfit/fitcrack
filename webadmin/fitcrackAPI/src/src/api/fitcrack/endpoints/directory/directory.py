'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask_restplus import Resource, abort
from flask import request

from settings import SERVER_BROWSER_ROOT
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.directory.argumentsParser import directory_parser
from src.api.fitcrack.endpoints.directory.responseModels import directory_model
from src.database.models import FcMasksSet

log = logging.getLogger(__name__)
ns = api.namespace('directory', description='Endpointy ktoré slúžia na prechadzanie diskove priestoru servera.')

ALLOWED_EXTENSIONS = set(['txt'])


@ns.route('')
class maskCollection(Resource):

    @api.marshal_with(directory_model)
    @api.expect(directory_parser)
    def get(self):
        """
        Vracia zoznam suborov a zložiek v adresari
        """
        args = directory_parser.parse_args(request)

        result = {
            'path': args['path'],
            'folders': [],
            'files': [],
            'parent': os.path.abspath(os.path.join(args['path'], os.pardir))
        }
        try:
            os.listdir()
            for entry in os.listdir(os.path.join(SERVER_BROWSER_ROOT, args['path'])):
                entryPath = os.path.join(args['path'], entry)
                if os.path.isfile(entryPath):
                    result['files'].append({
                        'path':entryPath,
                        'name':entry
                    })
                else:
                    result['folders'].append({
                        'path':entryPath,
                        'name':entry
                    })
        except (FileNotFoundError):
            abort(500, 'Folder ' + args['path'] + ' not found.')

        result['folders'].sort(key=lambda x: str.lower(x['name']))
        result['files'].sort(key=lambda x: str.lower(x['name']))
        return result