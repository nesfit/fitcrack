'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask import request, redirect, send_file
from flask_restx import Resource, abort

from settings import PROTECTEDFILES_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.protectedFile.responseModels import protectedFilesCollection_model, \
    excryptedFileUploaded_model
from src.api.fitcrack.endpoints.protectedFile.functions import addProtectedFile
from src.database.models import FcEncryptedFile

log = logging.getLogger(__name__)
ns = api.namespace('protectedFiles', description='Endpoints for operations with files with passwords.')


@ns.route('/')
class filesCollection(Resource):
    @api.marshal_with(protectedFilesCollection_model)
    def get(self):
        """
        Returns collection of hashed files.
        """
        return {'items': FcEncryptedFile.query.all()}


@ns.route('/add')
class filesAdd(Resource):

    @api.marshal_with(excryptedFileUploaded_model)
    def post(self):
        """
        Uploads hashed files on server.
        """
        # check if the post request has the file part
        if 'file' not in request.files:
            abort(500, 'No file part')
            return redirect(request.url)
        file = request.files['file']
        # if user does not select file, browser also
        # submit a empty part without filename
        if file.filename == '':
            abort(500, 'No selected file')

        return addProtectedFile(file)



@ns.route('/<id>')
class protectedFile(Resource):

    def get(self, id):
        """
        Downloads hashed file.
        """
        encryptedFile = FcEncryptedFile.query.filter(FcEncryptedFile.id == id).first()
        path = os.path.join(PROTECTEDFILES_DIR, encryptedFile.path)
        return send_file(path, attachment_filename=encryptedFile.path, as_attachment=True)

