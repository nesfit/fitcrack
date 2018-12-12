'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask import request, redirect, send_from_directory
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

log = logging.getLogger(__name__)
ns = api.namespace('charset', description='Endpointy ktoré slúžia na pracu s charset subormi.')

ALLOWED_EXTENSIONS = set(['txt', 'hcchr', 'charset'])


@ns.route('')
class charsetCollection(Resource):

    @api.marshal_with(hcStatsCollection_model)
    def get(self):
        """
        Vracia kolekciu HcStats suborov
        """
        return {'items': FcCharset.query.filter(FcCharset.deleted == False).all()}


@ns.route('/add')
class charsetAdd(Resource):
    is_public = True

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Nahrava charset subor na server
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
            charset = FcCharset(name=uploadedFile['filename'], path=uploadedFile['path'])
            try:
                db.session.add(charset)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Charset with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'File ' + uploadedFile['filename'] + ' successfuly uploaded.',
                'status': True
            }
        else:
            abort(500, 'Wrong file format')


@ns.route('/<id>')
class charset(Resource):

    @api.marshal_with(charset_model)
    def get(self, id):
        """
        Vrati info o charsete spolu s datami
        """

        charset = FcCharset.query.filter(FcCharset.id == id).first()

        with open(os.path.join(CHARSET_DIR, charset.path), 'rb') as file:
            content = file.read()

        try:
            content = content.decode(encoding='Windows-1252')
        except UnicodeDecodeError:
            return {
                'id': charset.id,
                'name': charset.name,
                'time': charset.time,
                'data': str(content),
                'canDecode': False
            }


        return {
            'id': charset.id,
            'name': charset.name,
            'time': charset.time,
            'data': content,
            'canDecode': True
        }

    @api.marshal_with(simpleResponse)
    def delete(self, id):
        charset = FcCharset.query.filter(FcCharset.id == id).one()
        if (charset.deleted):
            charset.deleted = False
        else:
            charset.deleted = True
        db.session.commit()
        return {
            'status': True,
            'message': 'Charset sucesfully deleted.'
        }, 200


@ns.route('/<id>/download')
class downloadCharset(Resource):

    def get(self, id):
        """
        Stiahne charset
        """

        charset = FcCharset.query.filter(FcCharset.id == id).first()
        return send_from_directory(CHARSET_DIR, charset.path)


@ns.route('/<id>/update')
class updateCharset(Resource):

    @api.expect(updateCharset_parser)
    @api.marshal_with(simpleResponse)
    def post(self, id):
        """
        Nahradí charset novým stringom
        """

        args = updateCharset_parser.parse_args(request)
        newData = args.get('newCharset', None)

        charset = FcCharset.query.filter(FcCharset.id == id).first()
        file = open(os.path.join(CHARSET_DIR, charset.path), 'r+')

        file.seek(0)
        file.write(newData)
        file.truncate()
        file.close()

        return {
            'message': 'File ' + charset.name + ' successfuly changed.',
            'status': True
        }
