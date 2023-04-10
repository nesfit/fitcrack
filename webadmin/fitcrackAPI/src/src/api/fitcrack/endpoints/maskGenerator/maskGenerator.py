'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask import request, redirect, send_file
from flask_restx import Resource, abort
from sqlalchemy import exc

from settings import DICTIONARY_DIR, MASKS_DIR, ROOT_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.maskGenerator.src.MaskGenerator import MaskGenerator
from src.api.fitcrack.endpoints.markov.responseModels import hcStatsCollection_model
from src.api.fitcrack.endpoints.masks.argumentsParser import updateMask_parser
from src.api.fitcrack.endpoints.masks.responseModels import maskSet_model
from src.api.fitcrack.functions import fileUpload
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcMasksSet

log = logging.getLogger(__name__)
ns = api.namespace('maskGenerator', description='Endpoints for work with mask generator.')

@ns.route('')
class generateMasks(Resource):

    @api.marshal_with(simpleResponse)
    def get(self):
        return {
            'message': 'Hello from GET',
            'status': True
        }

    @api.marshal_with(simpleResponse)
    def post(self):

        filename = request.json.get('filename')

        if filename == '':
            abort(500, 'Filename cannot be empty.')

        if os.path.isfile(MASKS_DIR + "/" + filename + ".hcmask"):
            abort(500, 'File with this name already exists.')

        maskGenerator = MaskGenerator()
        message = maskGenerator.generateMaskFile(request.json, MASKS_DIR, DICTIONARY_DIR)

        if message == "Successfully created mask file.":
            filename = 'test.hcmask' if request.json.get('filename') == '' else request.json.get('filename') + '.hcmask'
            maskSet = FcMasksSet(name=filename, path=filename)
            try:
                db.session.add(maskSet)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Masks set with name ' + filename + ' already exists.')

            return {
                'message': message,
                'status': True
            }
        else:
            abort(500, 'Error generating masks: ' + message)
