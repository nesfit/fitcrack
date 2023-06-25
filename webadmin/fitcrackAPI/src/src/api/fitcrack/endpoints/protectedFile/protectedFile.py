'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask import request, redirect, send_file
from flask_restx import Resource, abort
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
ns = api.namespace('protectedFiles', description='Endpoints for operations with files with passwords.')

ALLOWED_OFFICE_EXTENSIONS = set(["doc", "docx", "xls", "xlsx", "ppt", "pptx", "pdf", "rar", "zip", "7z"])
ALLOWED_WALLET_EXTENSIONS = set(["dat", "json", "seco"])

@ns.route('/')
class filesCollection(Resource):
    @api.marshal_with(protectedFilesCollection_model)
    def get(self):
        """
        Returns collection of hashed files.
        """
        return {'items': FcEncryptedFile.query.all()}


@ns.route('/addFile')
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

        uploadedFile = fileUpload(file, PROTECTEDFILES_DIR, ALLOWED_OFFICE_EXTENSIONS, withTimestamp=True)
        if uploadedFile:
            loadedHash = getHashFromFile(filename=uploadedFile['filename'], path=uploadedFile['path'])
            encFile = FcEncryptedFile(name=uploadedFile['filename'], path=uploadedFile['path'], hash=loadedHash['hash'].encode(),
                                      hash_type=loadedHash['hash_type'])
            try:
                db.session.add(encFile)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'File with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'Successfully extracted hash from uploaded file.',
                'status': True,
                'hash': loadedHash['hash'],
                'hash_type': loadedHash['hash_type'],
                'hash_type_name': encFile.hash_type_name,
                'file_id': encFile.id
            }
        else:
            abort(500, 'We only support ' + ', '.join(str(x) for x in ALLOWED_OFFICE_EXTENSIONS) + '.')


@ns.route('/addWallet')
class walletsAdd(Resource):

    @api.marshal_with(excryptedFileUploaded_model)
    def post(self):
        """
        Uploads crypto wallets on server.
        """
        # check if the post request has the file part
        if 'file' not in request.files:
            abort(500, 'No file part')
            return redirect(request.url)
        file = request.files['file']
        # if user does not select file, browser also
        # submit a empty part without filename
        if file.filename == '':
            abort(500, 'No selected wallet file')
        uploadedFile = fileUpload(file, PROTECTEDFILES_DIR, ALLOWED_WALLET_EXTENSIONS, withTimestamp=True)
        if uploadedFile:
            loadedHash = getHashFromFile(filename=uploadedFile['filename'], path=uploadedFile['path'])
            encFile = FcEncryptedFile(name=uploadedFile['filename'], path=uploadedFile['path'], hash=loadedHash['hash'].encode(),
                                      hash_type=loadedHash['hash_type'])
            try:
                db.session.add(encFile)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Wallet with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'Successfully extracted hash from uploaded wallet.',
                'status': True,
                'hash': loadedHash['hash'],
                'hash_type': loadedHash['hash_type'],
                'hash_type_name': encFile.hash_type_name,
                'file_id': encFile.id
            }
        else:
            abort(500, 'We only support ' + ', '.join(str(x) for x in ALLOWED_WALLET_EXTENSIONS) + '.')


@ns.route('/<id>')
class protectedFile(Resource):

    def get(self, id):
        """
        Downloads hashed file.
        """
        encryptedFile = FcEncryptedFile.query.filter(FcEncryptedFile.id == id).first()
        path = os.path.join(PROTECTEDFILES_DIR, encryptedFile.path)
        return send_file(path, attachment_filename=encryptedFile.path, as_attachment=True)

