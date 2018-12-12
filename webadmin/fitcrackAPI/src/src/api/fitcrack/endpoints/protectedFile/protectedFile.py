'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask import request, redirect, send_from_directory
from flask_restplus import Resource, abort
from sqlalchemy import exc

from settings import PROTECTEDFILES_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.protectedFile.functions import getHashFromFile
from src.api.fitcrack.endpoints.protectedFile.responseModels import protectedFilesCollection_model, \
    excryptedFileUploaded_model
from src.api.fitcrack.functions import fileUpload
from src.database import db
from src.database.models import FcEncryptedFile

log = logging.getLogger(__name__)
ns = api.namespace('protectedFiles', description='Endpointy ktoré slúžia na pracu so zaheslovanými súbormi.')

ALLOWED_EXTENSIONS = set(["doc", "docx", "xls", "xlsx", "ppt", "pptx", "pdf", "rar", "zip", "7z"])


@ns.route('/')
class filesCollection(Resource):
    @api.marshal_with(protectedFilesCollection_model)
    def get(self):
        """
        Vracia kolekciu zaheslovaných suborov
        """
        return {'items': FcEncryptedFile.query.all()}


@ns.route('/add')
class filesAdd(Resource):
    is_public = True

    @api.marshal_with(excryptedFileUploaded_model)
    def post(self):
        """
        Nahrava zaheslovaný subor na server
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

        uploadedFile = fileUpload(file, PROTECTEDFILES_DIR, ALLOWED_EXTENSIONS, withTimestamp=True)
        if uploadedFile:
            hash = getHashFromFile(filename=uploadedFile['filename'], path=uploadedFile['path'])
            encFile = FcEncryptedFile(name=uploadedFile['filename'], path=uploadedFile['path'], hash=hash['hash'],
                                      hash_type=hash['hash_type'])
            try:
                db.session.add(encFile)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'File with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'Successfully extracted hash form file.',
                'status': True,
                'hash': hash['hash'],
                'hash_type': hash['hash_type'],
                'hash_type_name': encFile.hash_type_name,
                'file_id': encFile.id
            }
        else:
            abort(500, 'We only support ' + ', '.join(str(x) for x in ALLOWED_EXTENSIONS) + '.')




@ns.route('/<id>')
class protectedFile(Resource):

    def get(self, id):
        """
        Ponukne na stiahnutie zaheslovaný subor subor
        """

        file = FcEncryptedFile.query.filter(FcEncryptedFile.id == id).first()
        return send_from_directory(PROTECTEDFILES_DIR, file.name)
