'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''
import logging
from itertools import islice

import os

import time
from pathlib import Path

from flask import request, redirect
from flask_restplus import Resource, abort
from sqlalchemy import exc

from settings import PCFG_DIR, HASHCAT_PATH, HASHCAT_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.pcfg.argumentsParser import pcfg_parser, pcfgFromFile_parser
from src.api.fitcrack.endpoints.pcfg.functions import readingFromFolderPostProcces
from src.api.fitcrack.endpoints.pcfg.functions import unzipGrammarToPcfgFolder
from src.api.fitcrack.endpoints.pcfg.functions import deleteUnzipedFolderDirectory
from src.api.fitcrack.endpoints.pcfg.responseModels import pcfgs_model, pcfgData_model, \
    pcfg_model
from src.api.fitcrack.functions import shellExec, fileUpload, allowed_file, getFilesFromFolder
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcPcfg

log = logging.getLogger(__name__)
ns = api.namespace(
    'pcfg', description='Endpointy ktoré slúžia na pracu s pcfg')


ALLOWED_EXTENSIONS = set(['zip'])

#dictionaryCollection -> pcfgCollection
@ns.route('')
class pcfgCollection(Resource):

    @api.marshal_with(pcfgs_model)
    def get(self):
        """
        Vracia kolekciu pcfg
        """
        #dictionaries -> pcfgs
        pcfgs = getFilesFromFolder(
            PCFG_DIR, FcPcfg, readingFromFolderPostProcces)
        return {'items': pcfgs}


#dictionary -> pcfg
@ns.route('/<id>')
class pcfg(Resource):

    @api.marshal_with(pcfg_model)
    def get(self, id):
        """
        Vráti info o pcfg
        """

        pcfg = FcPcfg.query.filter(FcPcfg.id == id).first()
        if not pcfg:
            abort(500, 'Can\'t open dictionary')

        return pcfg

    @api.marshal_with(simpleResponse)
    def delete(self, id):
        pcfg = FcPcfg.query.filter(FcPcfg.id == id).one()
        # if (dictionary.deleted):
        #     dictionary.deleted = False
        # else:
        #     dictionary.deleted = True
        # db.session.commit()
        pcfgFullPath = os.path.join(PCFG_DIR, pcfg.path)
        print(pcfgFullPath)
        if os.path.exists(pcfgFullPath):
            os.remove(pcfgFullPath)
            deleteUnzipedFolderDirectory(pcfgFullPath)

        return {
            'status': True,
            'message': 'PCFG sucesfully deleted.'
        }, 200


@ns.route('/add')
class pcfgAdd(Resource):
    is_public = True

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Nahrava pcfg na server
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

        uploadedFile = fileUpload(file, PCFG_DIR, ALLOWED_EXTENSIONS)
        if uploadedFile:
            #hc_keyspace = int(shellExec(HASHCAT_PATH + ' --keyspace -a 0 ' + os.path.join(PCFG_DIR, uploadedFile['path']), cwd=HASHCAT_DIR))
            pcfg = FcPcfg(
                name=uploadedFile['filename'], path=uploadedFile['path'], keyspace=int(0))

            try:
                db.session.add(pcfg)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'PCFG with name '
                      + uploadedFile['filename'] + ' already exists.')

            unzipGrammarToPcfgFolder(uploadedFile['filename'])

            return {
                'message': 'PCFG ' + uploadedFile['filename'] + ' successfuly uploaded.',
                'status': True

            }
        else:
            abort(500, 'Wrong file format')
