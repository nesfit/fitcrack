'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from itertools import islice

from flask import request, redirect, send_from_directory
from flask_restplus import Resource, abort
from sqlalchemy import exc

from settings import RULE_DIR
from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination
from src.api.fitcrack.endpoints.markov.responseModels import hcStatsCollection_model
from src.api.fitcrack.endpoints.rule.argumentsParser import updateRule_parser, rule_parser
from src.api.fitcrack.endpoints.rule.responseModels import rule_model, ruleData_model
from src.api.fitcrack.functions import fileUpload
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcRule

log = logging.getLogger(__name__)
ns = api.namespace('rule', description='Endpointy ktoré slúžia na pracu s rule subormi.')

ALLOWED_EXTENSIONS = set(['txt', 'rule'])


@ns.route('')
class ruleCollection(Resource):

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Nahrava rule subor na server
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

        uploadedFile = fileUpload(file, RULE_DIR, ALLOWED_EXTENSIONS, suffix='.rule')
        if uploadedFile:
            rule = FcRule(name=uploadedFile['filename'], path=uploadedFile['path'])
            try:
                db.session.add(rule)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Rule with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'File ' + uploadedFile['filename'] + ' successfuly uploaded.',
                'status': True
            }
        else:
            abort(500, 'Wrong file format')

    @api.marshal_with(hcStatsCollection_model)
    def get(self):
        """
        Vracia kolekciu HcStats suborov
        """
        return {'items': FcRule.query.filter(FcRule.deleted == False).all()}


@ns.route('/add')
class ruleAdd(Resource):
    is_public = True

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Nahrava rule subor na server
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

        uploadedFile = fileUpload(file, RULE_DIR, ALLOWED_EXTENSIONS, suffix='.rule')
        if uploadedFile:
            rule = FcRule(name=uploadedFile['filename'], path=uploadedFile['path'])
            try:
                db.session.add(rule)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Rule with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'File ' + uploadedFile['filename'] + ' successfuly uploaded.',
                'status': True
            }
        else:
            abort(500, 'Wrong file format')


@ns.route('/<id>')
class rule(Resource):

    @api.marshal_with(rule_model)
    def get(self, id):
        """
        Vrati info o subore s pravidlami
        """

        rule = FcRule.query.filter(FcRule.id == id).first()

        return rule

    @api.marshal_with(simpleResponse)
    def delete(self, id):
        rule = FcRule.query.filter(FcRule.id == id).one()
        if (rule.deleted):
            rule.deleted = False
        else:
            rule.deleted = True
        db.session.commit()

        ruleFullPath = os.path.join(RULE_DIR, rule.path)
        if os.path.exists(ruleFullPath):
            os.remove(ruleFullPath)
        return {
            'status': True,
            'message': 'Rule file sucesfully deleted.'
        }, 200

@ns.route('/<id>/data')
class ruleData(Resource):
    @api.expect(rule_parser)
    @api.marshal_with(ruleData_model)
    def get(self, id):
        """
        Vráti prvých 25 riadkov zo slovníka
        """
        args = rule_parser.parse_args(request)
        page = args.get('page', 1) - 1
        per_page = args.get('per_page', 25)
        rule = FcRule.query.filter(FcRule.id==id).first()
        if not rule:
            abort(500, 'Can\'t open rules file')

        if args.get('search', None):
            with open(os.path.join(RULE_DIR, rule.path), encoding='latin-1') as file:
                head = ''
                for line in file:
                    if line.find(args['search']) != -1:
                        head += line
        else:
            with open(os.path.join(RULE_DIR, rule.path), encoding='latin-1') as file:
                head = list(islice(file, page * per_page, page * per_page + per_page))

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



@ns.route('/<id>/download')
class downloadRule(Resource):

    def get(self, id):
        """
        Stiahne rule
        """

        rule = FcRule.query.filter(FcRule.id == id).first()
        return send_from_directory(RULE_DIR, rule.path)


@ns.route('/<id>/update')
class updateRule(Resource):

    @api.expect(updateRule_parser)
    @api.marshal_with(simpleResponse)
    def post(self, id):
        """
        Nahradí rule novým stringom
        """

        args = updateRule_parser.parse_args(request)
        newData = args.get('newRule', None)

        rule = FcRule.query.filter(FcRule.id == id).first()
        file = open(os.path.join(RULE_DIR, rule.path), 'r+')

        file.seek(0)
        file.write(newData)
        file.truncate()
        file.close()

        return {
            'message': 'File ' + rule.name + ' successfuly changed.',
            'status': True
        }
