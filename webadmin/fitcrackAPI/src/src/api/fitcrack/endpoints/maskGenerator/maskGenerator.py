'''
About: Endpoint handling request for the mask generation process.
Author: Samuel Hribik
'''

import logging

import os
from flask import request, redirect, send_file
from flask_restx import Resource, abort
from sqlalchemy import exc

from settings import CHARSET_DIR, DICTIONARY_DIR, MASKS_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.maskGenerator.src.MaskGenerator import MaskGenerator
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

        mask_filename = request.json.get('filename')

        if mask_filename == '':
            abort(500, 'Filename cannot be empty.')

        if os.path.isfile(MASKS_DIR + "/" + mask_filename + ".hcmask"):
            abort(500, 'File with this name already exists.')

        mask_generator = MaskGenerator()
        message = mask_generator.generateMaskFile(request.json, MASKS_DIR, DICTIONARY_DIR, CHARSET_DIR)

        if message == "Success":

            mask_filename = request.json.get('filename') + '.hcmask'
            mask_set = FcMasksSet(name=mask_filename, path=mask_filename)
            try:
                db.session.add(mask_set)
                db.session.commit()
            except exc.IntegrityError:
                db.session().rollback()
                abort(500, 'Masks set with name ' + mask_filename + ' already exists.')

            charsets = mask_generator.getCustomCharsetList()

            for charset_number in charsets:
                charset_filename = request.json.get('filename') + "_" + charset_number + ".hcchr"
                size = os.path.getsize(os.path.join(CHARSET_DIR, charset_filename))
                charset = FcCharset(name=request.json.get('filename') + "_" + charset_number, path=charset_filename, keyspace=size)
                try:
                    db.session.add(charset)
                    db.session.commit()
                except exc.IntegrityError:
                    db.session().rollback()
                    abort(500, 'Charset with name ' + charset_filename + ' already exists.')

            return {
                'message': "Successfully created mask file " + mask_filename,
                'status': True
            }
        else:
            abort(500, 'Error generating masks: ' + message)
