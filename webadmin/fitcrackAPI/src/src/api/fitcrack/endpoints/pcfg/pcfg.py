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


'''
@ns.route('/<id>/data')
class pcfgData(Resource):

    @api.expect(pcfg_parser)
    @api.marshal_with(pcfgData_model)
    def get(self, id):
        """
        Vráti prvých 25 riadkov zo slovníka
        """
        args = pcfg_parser.parse_args(request)
        page = args.get('page', 1) - 1
        per_page = args.get('per_page', 25)
        pcfg = FcPcfg.query.filter(FcPcfg.id == id).first()
        if not pcfg:
            abort(500, 'Can\'t open dictionary')

        if args.get('search', None):
            with open(os.path.join(PCFG_DIR, dict.path),  encoding='latin-1') as file:
                head = ''
                for line in file:
                    if line.find(args['search']) != -1:
                        head += line
        else:
            with open(os.path.join(PCFG_DIR, dict.path), encoding='latin-1') as file:
                head = list(islice(file, page * per_page,
                                   page * per_page + per_page))

        if len(head) == 0:
            return {
                'status': False,
                'data': ''
            }
        else:
            return {
                'status': True,
                'data': ''.join(head)
            }

        # return send_from_directory(UPLOAD_DIR,
        #                            dict.name)
'''


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


'''
@ns.route('/fromFile')
class pcfg(Resource):

    @api.marshal_with(simpleResponse)
    @api.expect(pcfgFromFile_parser)
    def post(self):
        """
        Spravi slovník so suboru
        """

        args = pcfgFromFile_parser.parse_args(request)
        files = args.get('files')

        for file in files:
            if not allowed_file(file['name'], ALLOWED_EXTENSIONS):
                abort(500, 'Wrong file format ' + file['name'])
            newName = Path(file['name']).stem + '_' + \
                str(int(time.time())) + Path(file['name']).suffix
            newPath = os.path.join(PCFG_DIR, newName)
            os.symlink(file['path'], newPath)
            hc_keyspace = int(
                shellExec(HASHCAT_PATH + ' --keyspace -a 0 ' + newPath, cwd=HASHCAT_DIR))
            pcfg = FcPcfg(name=file['name'],
                          path=newName, keyspace=hc_keyspace)
            try:
                db.session.add(pcfg)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Dictionary with name '
                      + file['filename'] + ' already exists.')

        return {
            'message': 'Dictionaries successfuly uploaded.',
            'status': True
        }
'''
