'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from pathlib import Path
from flask_restplus import Resource, abort
from flask import request

from settings import SERVER_BROWSER_ROOT as prefix
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.directory.argumentsParser import directory_parser
from src.api.fitcrack.endpoints.directory.responseModels import directory_model
from src.database.models import FcMasksSet

log = logging.getLogger(__name__)
ns = api.namespace('directory', description='Endpoinsts for browsing filesystem of server.')

ALLOWED_EXTENSIONS = set(['txt'])

prefixlen = len(prefix)


@ns.route('')
class maskCollection(Resource):

    @api.marshal_with(directory_model)
    @api.expect(directory_parser)
    def get(self):
        """
        Returns list of files in directory.
        """
        args = directory_parser.parse_args(request)
        ipath = args['path']
        realpath = ipath if ipath.startswith(prefix) else prefix + ipath
        is_root_dir = os.path.abspath(realpath) == os.path.abspath(prefix)

        if not os.path.realpath(realpath).startswith(prefix):
            abort(500, 'Folder not accessible')

        result = {
            'path': realpath,
            'files': [],
            'folders': [],
            'parent': '' if is_root_dir else Path(realpath).parent
        }

        try:
            os.listdir()
            for entry in os.listdir(realpath):
                entryPath = os.path.join(realpath, entry)
                if os.path.isfile(entryPath):
                    result['files'].append({
                        'path': entryPath,
                        'name': entry
                    })
                else:
                    result['folders'].append({
                        'path': entryPath,
                        'name': entry
                    })
        except (FileNotFoundError):
            abort(500, 'Folder ' + realpath + ' not found.')

        result['folders'].sort(key=lambda x: str.lower(x['name']))
        result['files'].sort(key=lambda x: str.lower(x['name']))
        return result
