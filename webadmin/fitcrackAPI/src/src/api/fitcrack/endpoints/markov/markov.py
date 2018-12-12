'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
import os

from flask import request, redirect, send_from_directory
from flask_restplus import Resource, abort
from sqlalchemy import exc
from werkzeug.utils import secure_filename

from settings import HASHCAT_UTILS_PATH, EXE_OR_BIN, HCSTATS_DIR, DICTIONARY_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.markov.argumentsParser import makeMarkovFromDictionary_parser
from src.api.fitcrack.endpoints.markov.responseModels import hcStatsCollection_model
from src.api.fitcrack.functions import fileUpload, shellExec
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcHcstat, FcDictionary

log = logging.getLogger(__name__)
ns = api.namespace('markovChains', description='Endpointy ktoré slúžia na pracu s HcStats subormi.')

ALLOWED_EXTENSIONS = set(['hcstat2'])


@ns.route('')
class markovCollection(Resource):
    @api.marshal_with(hcStatsCollection_model)
    def get(self):
        """
        Vracia kolekciu HcStats suborov
        """
        return {'items': FcHcstat.query.filter(FcHcstat.deleted == False).all()}



@ns.route('/add')
class markovAdd(Resource):
    is_public = True

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Nahrava hcstats na server
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

        uploadedFile = fileUpload(file, HCSTATS_DIR, ALLOWED_EXTENSIONS, suffix='.hcstat2', withTimestamp=True)
        if uploadedFile:
            hcstats = FcHcstat(name=uploadedFile['filename'], path=uploadedFile['path'])
            try:
                db.session.add(hcstats)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'HcStats with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'File ' + uploadedFile['filename'] + ' successfuly uploaded.',
                'status': True

            }
        else:
            abort(500, 'Wrong file format')


@ns.route('/<id>')
class markov(Resource):

    def get(self, id):
        """
        Ponukne na stiahnutie binarny hcstats subor
        """

        HcStat = FcHcstat.query.filter(FcHcstat.id == id).first()
        return send_from_directory(HCSTATS_DIR, HcStat.path)

    @api.marshal_with(simpleResponse)
    def delete(self, id):
        markov = FcHcstat.query.filter(FcHcstat.id == id).one()
        if (markov.deleted):
            markov.deleted = False
        else:
            markov.deleted = True
        db.session.commit()
        return {
            'status': True,
            'message': 'Markov file sucesfully deleted.'
        }, 200



@ns.route('/makeFromDictionary')
class markovMakeFromDictionary(Resource):

    @api.marshal_with(simpleResponse)
    @api.expect(makeMarkovFromDictionary_parser)
    def post(self):
        """
        HcStat from dictionary
        """
        args = makeMarkovFromDictionary_parser.parse_args(request)
        dict = FcDictionary.query.filter(FcDictionary.id == args['dictionary_id']).first()
        if not dict:
            abort(500, 'Can not find selected dictionary.')
        filename = secure_filename(args['name'])
        path = os.path.join(HCSTATS_DIR, filename) + '.hcstat2'

        # make hcstat2 file
        shellExec(
            HASHCAT_UTILS_PATH + '/hcstat2gen.' + EXE_OR_BIN + ' ' + path + '_tmp < ' + os.path.join(DICTIONARY_DIR,
                                                                                                     dict.name))
        # comprime hcstat2 file
        shellExec('xz --compress --format=raw --stdout -9e ' + path + '_tmp > ' + path)
        # delete non-comprimed file
        os.remove(path + '_tmp')

        hcstats = FcHcstat(name=filename + '.hcstat2', path=path)
        try:
            db.session.add(hcstats)
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'HcStats with name ' + filename + ' already exists.')

        return {
            'status': True,
            'message': 'HcStat file with name ' + filename + ' created.'
        }
