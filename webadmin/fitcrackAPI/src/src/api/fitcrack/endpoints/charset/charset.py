'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask import request, redirect, send_file
from flask_restplus import Resource, abort
from sqlalchemy import exc

from settings import CHARSET_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.charset.argumentsParser import updateCharset_parser
from src.api.fitcrack.endpoints.charset.responseModels import charset_model
from src.api.fitcrack.endpoints.markov.responseModels import hcStatsCollection_model
from src.api.fitcrack.functions import fileUpload
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcCharset

from base64 import b64encode

log = logging.getLogger(__name__)
ns = api.namespace('charset', description='Endpoints for work with charset files.')

ALLOWED_EXTENSIONS = set(['txt', 'hcchr', 'charset'])


@ns.route('')
class charsetCollection(Resource):

    @api.marshal_with(hcStatsCollection_model)
    def get(self):
        """
        Ruturns collection of HcStats files.
        """
        return {'items': FcCharset.query.filter(FcCharset.deleted == False).all()}


@ns.route('/add')
class charsetAdd(Resource):
    is_public = True

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Uploads charset file on server.
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

        uploadedFile = fileUpload(file, CHARSET_DIR, ALLOWED_EXTENSIONS, suffix='.hcchr')
        if uploadedFile:
            size = os.path.getsize(os.path.join(CHARSET_DIR, uploadedFile['path']))
            charset = FcCharset(name=uploadedFile['stem'], path=uploadedFile['path'], keyspace=size)
            try:
                db.session.add(charset)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Charset with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'File ' + uploadedFile['filename'] + ' successfully uploaded.',
                'status': True
            }
        else:
            abort(500, 'Wrong file format')


@ns.route('/<id>')
class charset(Resource):

    @api.marshal_with(charset_model)
    def get(self, id):
        """
        Returns information about charset with data.
        """

        charset = FcCharset.query.filter(FcCharset.id == id).first()

        with open(os.path.join(CHARSET_DIR, charset.path), 'rb') as file:
            content = file.read()

        return {
            'id': charset.id,
            'name': charset.name,
            'keyspace': charset.keyspace,
            'time': charset.time,
            'data': b64encode(content).decode('utf-8')
        }

    @api.marshal_with(simpleResponse)
    def delete(self, id):
        """
        Deletes charset.
        """
        charset = FcCharset.query.filter(FcCharset.id == id).one()
        if (charset.deleted):
            charset.deleted = False
        else:
            charset.deleted = True
        db.session.commit()
        path = os.path.join(CHARSET_DIR, charset.path)
        if os.path.exists(path):
            os.remove(path)
        return {
            'status': True,
            'message': 'Charset sucesfully deleted.'
        }, 200


@ns.route('/<id>/download')
class downloadCharset(Resource):

    def get(self, id):
        """
        Downloads charset.
        """

        charset = FcCharset.query.filter(FcCharset.id == id).first()
        return send_file(os.path.join(CHARSET_DIR, charset.path), as_attachment=True, attachment_filename=charset.path)
