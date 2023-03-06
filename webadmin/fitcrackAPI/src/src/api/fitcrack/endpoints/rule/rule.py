'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
import re
from itertools import islice

from flask import request, redirect, send_file
from flask_restx import Resource, abort
from sqlalchemy import exc

import ctypes
import json
from settings import RULE_DIR, RULE_APPLICATOR_PATH
from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination
from src.api.fitcrack.endpoints.rule.argumentsParser import updateRule_parser, rule_parser
from src.api.fitcrack.endpoints.rule.responseModels import rules_model, rule_model, ruleData_model
from src.api.fitcrack.functions import fileUpload
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcRule

log = logging.getLogger(__name__)
ns = api.namespace('rule', description='Endpoints for work with rule files.')

ALLOWED_EXTENSIONS = set(['txt', 'rule'])

# Define the C function
apply_rule = ctypes.CDLL(RULE_APPLICATOR_PATH).apply_rule_cpu
apply_rule.restype = ctypes.c_int
apply_rule.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p]
RETCODE_COMMENT = -3

@ns.route('')
class ruleCollection(Resource):

    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Uploads rule file on server.
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
            rule_count = 0
            with open(os.path.join(RULE_DIR, uploadedFile['path']), encoding='latin-1') as file:
                for line in file:
                    if re.match('^\s*(\#.*)?$', line) == None:
                        rule_count += 1
            rule = FcRule(name=uploadedFile['filename'], path=uploadedFile['path'], count=rule_count)
            try:
                db.session.add(rule)
                db.session.commit()
            except exc.IntegrityError as e:
                db.session().rollback()
                abort(500, 'Rule with name ' + uploadedFile['filename'] + ' already exists.')
            return {
                'message': 'File ' + uploadedFile['filename'] + ' successfully uploaded.',
                'status': True
            }
        else:
            abort(500, 'Wrong file format')

    @api.marshal_with(rules_model)
    def get(self):
        """
        Returns collection of HcStats files.
        """
        return {'items': FcRule.query.filter(FcRule.deleted == False).all()}


@ns.route('/<id>')
class rule(Resource):

    @api.marshal_with(rule_model)
    def get(self, id):
        """
        Returns information about rule file.
        """

        rule = FcRule.query.filter(FcRule.id == id).first()

        return rule

    @api.marshal_with(simpleResponse)
    def delete(self, id):
        """
        Deletes rule file.
        """
        rule = FcRule.query.filter(FcRule.id == id).one()
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
        Returns first 25 rows from dictionary.
        """
        args = rule_parser.parse_args(request)
        page = args.get('page', 1) - 1
        per_page = args.get('per_page', 25)
        rule = FcRule.query.filter(FcRule.id==id).first()
        if not rule:
            abort(500, 'Can\'t open rules file')
        rule_path = os.path.join(RULE_DIR, rule.path)
        if not os.path.exists(rule_path):
            return {
                'status': False,
                'data': ''
            }

        if args.get('search', None):
            with open(rule_path, encoding='latin-1') as file:
                head = ''
                for line in file:
                    if line.find(args['search']) != -1:
                        head += line
        else:
            with open(rule_path, encoding='latin-1') as file:
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
        Downloads rule.
        """

        rule = FcRule.query.filter(FcRule.id == id).first()
        path = os.path.join(RULE_DIR, rule.path)
        return send_file(path, attachment_filename=rule.path, as_attachment=True)
    
    
@ns.route('/rule/preview')
class passwordsPreview(Resource):
    def post(self):
        request_data = request.get_json()
        dictionary = request_data['dictionaryContent']
        rules = request_data['ruleContent']
        preview = []
        
        for password in dictionary:
            password = password.strip()
            for rule in rules: 
                rule = rule.strip()
                                
                # Apply the rule to the password using the C function
                in_len = len(password.encode('utf-8'))
                final_password = ctypes.create_string_buffer(64)
                #Returns -1 for rule syntax error, -2 for empty rule or password or new password length if OK
                ret_code = apply_rule(rule.encode('utf-8'), len(rule), password.encode('utf-8'), in_len, final_password)
                
                if(ret_code == -1):
                    final_password = ""
                    #if the line in a rule is a comment, specify return code to -3
                    if(len(rule) > 0 and rule[0] == '#'):
                        ret_code = RETCODE_COMMENT
                else:
                    final_password = final_password.value.decode()
                
                #Add element to a preview list
                element = {
                    "finalPassword" : final_password,
                    "retCode" : ret_code
                }                         
                preview.append(element)
                
        return { 'passwordsPreview' : preview }, 200  
