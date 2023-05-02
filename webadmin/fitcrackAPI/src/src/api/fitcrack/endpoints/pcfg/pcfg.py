'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''
import logging
from itertools import islice

import os
import shutil

import time
from pathlib import Path

from flask import request, redirect, send_file
from flask_restx import Resource, abort
from sqlalchemy import exc

from settings import PCFG_DIR, HASHCAT_PATH, HASHCAT_DIR, ROOT_DIR, PCFG_TRAINER_DIR, PCFG_TRAINER_RULE_DIR
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.pcfg.argumentsParser import pcfg_parser, pcfgFromFile_parser, \
makePcfgFromDictionary_parser
from src.api.fitcrack.endpoints.pcfg.functions import readingFromFolderPostProcces, \
unzipGrammarToPcfgFolder, deleteUnzipedFolderDirectory, extractNameFromZipfile, \
createPcfgGrammarBin, calculateKeyspace, makePcfgFolder, moveGrammarToPcfgDir

from src.api.fitcrack.endpoints.pcfg.responseModels import pcfgs_model, pcfgData_model, \
    pcfg_model, pcfgTree_model
from src.api.fitcrack.functions import shellExec, fileUpload, allowed_file, getFilesFromFolder, directory_tree
from src.api.fitcrack.responseModels import simpleResponse, file_content
from src.api.fitcrack.argumentsParser import path
from src.database import db
from src.database.models import FcPcfg, FcDictionary

log = logging.getLogger(__name__)
ns = api.namespace(
    'pcfg', description='Endpoint for pcfg operations')


ALLOWED_EXTENSIONS = set(['zip'])

@ns.route('')
class pcfgCollection(Resource):

    @api.marshal_with(pcfgs_model)
    def get(self):
        """
        Returns collection of pcfg
        """
        return {'items': FcPcfg.query.filter(FcPcfg.deleted == False).all()}


@ns.route('/<id>')
class pcfg(Resource):

    def get(self, id):
        """
        Sends zipped PCFG as attachment
        """

        pcfg = FcPcfg.query.filter(FcPcfg.id == id).first()
        if not pcfg:
            abort(404, 'Can\'t find PCFG grammar record in DB')
        path = os.path.join(PCFG_DIR, pcfg.path)
        is_dir = os.path.isdir(path)
        if is_dir:
            makeshift_zip = '/tmp/pcfg/' + pcfg.path
            shutil.make_archive(makeshift_zip, 'zip', path)
            return send_file(makeshift_zip + '.zip', attachment_filename=pcfg.path + '.zip', as_attachment=True)
        else:
            abort(404, 'Can\'t find PCFG grammar directory')

    @api.marshal_with(simpleResponse)
    def delete(self, id):
        """
        Deletes pcfg
        """
        pcfg = FcPcfg.query.filter(FcPcfg.id == id).one()
        pcfg.deleted = True
        db.session.commit()

        pcfgFullPath = os.path.join(PCFG_DIR, pcfg.path)
        if os.path.exists(pcfgFullPath):
            deleteUnzipedFolderDirectory(pcfgFullPath)

        return {
            'status': True,
            'message': 'PCFG sucesfully deleted.'
        }, 200


@ns.route('/<id>/tree')
class pcfgTree(Resource):

    @api.marshal_with(pcfgTree_model)
    @api.response(404, 'PCFG record not found in database')
    @api.response(500, 'PCFG directory not readable from filesystem (zipped or missing)')
    def get(self, id):
        """
        Returns a directory tree of the grammar
        """
        pcfg = FcPcfg.query.filter(FcPcfg.id == id).first()
        if not pcfg:
            abort(404, 'Can\'t find PCFG grammar')
        dirname = Path(pcfg.path).stem
        path = os.path.join(PCFG_DIR, dirname)
        is_dir = os.path.isdir(path)
        if not is_dir:
            abort(500, 'PCFG is not unzipped or doesn\'t exist on the server')
        return directory_tree(path)
        

@ns.route('/<id>/file')
class pcfgFileRead(Resource):

    @api.expect(path)
    @api.marshal_with(file_content)
    def get(self, id):
        """
        Returns requested PCFG file contents
        """
        target = path.parse_args(request).get('path')
        pcfg = FcPcfg.query.filter(FcPcfg.id == id).first()
        if not pcfg:
            abort(404, 'Can\'t find PCFG grammar')
        dirname = Path(pcfg.path).stem

        with open(os.path.join(PCFG_DIR, dirname, target), mode='rb') as file:
            filename = os.path.basename(file.name)
            content = file.read()
            try:
                content = content.decode()
            except UnicodeDecodeError:
                content = 'Binary file HEX dump:\n' + content.hex()

        return {
            'name': filename,
            'path': os.path.join(dirname, target),
            'data': content
        }        


@ns.route('/add')
class pcfgAdd(Resource):

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Upload pcfg on server
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
            unzipGrammarToPcfgFolder(uploadedFile['filename'])
            pcfg_keyspace = calculateKeyspace(uploadedFile['filename'])
            pcfg = FcPcfg(
                name=extractNameFromZipfile(uploadedFile['filename']), path=extractNameFromZipfile(uploadedFile['path']), keyspace=int(pcfg_keyspace))

            try:
                db.session.add(pcfg)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'PCFG with name '
                      + uploadedFile['filename'] + ' already exists.')

            createPcfgGrammarBin(uploadedFile['filename'])

            return {
                'message': 'PCFG ' + uploadedFile['filename'] + ' successfully uploaded.',
                'status': True

            }
        else:
            abort(500, 'Wrong file format')


@ns.route('/makeFromDictionary')
class pcfgMakeFromDictionary(Resource):

    @api.marshal_with(simpleResponse)
    @api.expect(makePcfgFromDictionary_parser)
    def post(self):
        """
        Creates pcfg from the dictionary
        """
        args = makePcfgFromDictionary_parser.parse_args(request)
        dict = FcDictionary.query.filter(FcDictionary.id == args['dictionary_id']).first()
        if not dict:
            abort(500, 'Can not find selected dictionary.')

        if os.path.exists(PCFG_DIR + '/' + extractNameFromZipfile(dict.name)):
            abort(500, 'PCFG with the same as dictionary already exists.')

        makePcfgFolder(dict.name)
        moveGrammarToPcfgDir(dict.name)

        pcfg_keyspace = calculateKeyspace(dict.name)

        pcfg = FcPcfg(
            name=extractNameFromZipfile(dict.name), path=extractNameFromZipfile(dict.name), keyspace=int(pcfg_keyspace))

        try:
            db.session.add(pcfg)
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'PCFG with name '
                  + extractNameFromZipfile(dict.name) + ' already exists.')
        createPcfgGrammarBin(dict.name)
        return {
            'message': 'PCFG ' + dict.name + ' successfully uploaded.',
            'status': True

        }
