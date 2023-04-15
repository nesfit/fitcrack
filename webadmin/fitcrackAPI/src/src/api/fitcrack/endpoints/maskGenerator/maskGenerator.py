'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask import request, redirect, send_file
from flask_restx import Resource, abort
from sqlalchemy import exc

from settings import CHARSET_DIR, DICTIONARY_DIR, MASKS_DIR, ROOT_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.maskGenerator.src.MaskGenerator import MaskGenerator
from src.api.fitcrack.endpoints.markov.responseModels import hcStatsCollection_model
from src.api.fitcrack.endpoints.masks.argumentsParser import updateMask_parser
from src.api.fitcrack.endpoints.masks.responseModels import maskSet_model
from src.api.fitcrack.functions import fileUpload
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcMasksSet
from src.database.models import FcCharset

log = logging.getLogger(__name__)
ns = api.namespace('maskGenerator', description='Endpoints for work with mask generator.')

@ns.route('')
class generateMasks(Resource):

    @api.marshal_with(simpleResponse)
    def post(self):

        maskFilename = request.json.get('filename')

        if maskFilename == '':
            abort(500, 'Filename cannot be empty.')

        if os.path.isfile(MASKS_DIR + "/" + maskFilename + ".hcmask"):
            abort(500, 'File with this name already exists.')

        maskGenerator = MaskGenerator()
        message = maskGenerator.generateMaskFile(request.json, MASKS_DIR, DICTIONARY_DIR, CHARSET_DIR)

        if message == "Success":

            maskFilename = request.json.get('filename') + '.hcmask'
            maskSet = FcMasksSet(name=maskFilename, path=maskFilename)
            try:
                db.session.add(maskSet)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Masks set with name ' + maskFilename + ' already exists.')

            charsets = maskGenerator.getCustomCharsetList()

            for key in charsets:
                charsetFilename = request.json.get('filename') + "_" + key + ".hcchr"
                size = os.path.getsize(os.path.join(CHARSET_DIR, charsetFilename))
                charset = FcCharset(name=request.json.get('filename') + "_" + key, path=charsetFilename, keyspace=size)
                try:
                    db.session.add(charset)
                    db.session.commit()
                except exc.IntegrityError as e:
                    db.session().rollback()
                    abort(500, 'Charset with name ' + charsetFilename + ' already exists.')

            return {
                'message': "Successfully created mask file " + maskFilename,
                'status': True
            }
        else:
            abort(500, 'Error generating masks: ' + message)
