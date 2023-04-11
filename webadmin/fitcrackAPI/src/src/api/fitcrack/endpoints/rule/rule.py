'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
import re
from itertools import islice
from pathlib import Path

from flask import request, redirect, send_file
from flask_restx import Resource, abort
from sqlalchemy import exc
from werkzeug.utils import secure_filename

import ctypes
import json
from settings import RULE_DIR, RULE_APPLICATOR_PATH
from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination
from src.api.fitcrack.endpoints.rule.argumentsParser import updateRule_parser, rule_parser
from src.api.fitcrack.endpoints.rule.responseModels import rules_model, rule_model, ruleData_model, previewPasswords_model, randomRule_model
from src.api.fitcrack.functions import fileUpload, allowed_file
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcRule

log = logging.getLogger(__name__)
ns = api.namespace('rule', description='Endpoints for work with rule files.')

ALLOWED_EXTENSIONS = set(['txt', 'rule'])
RETCODE_COMMENT = -3

# Define the C function for applying rules
apply_rule = ctypes.CDLL(RULE_APPLICATOR_PATH).apply_rule_cpu
apply_rule.restype = ctypes.c_int
apply_rule.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int, ctypes.c_char_p]

# Define the C function for generating random rule
gen_random_rule = ctypes.CDLL(RULE_APPLICATOR_PATH).generate_random_rule
gen_random_rule.restype = ctypes.c_int
gen_random_rule.argtypes = [ctypes.c_char_p, ctypes.c_uint32, ctypes.c_uint32]


def countRules(filePath):
    '''
    Function which counts rules
    '''
    ruleCount = 0
    with open(os.path.join(RULE_DIR, filePath), encoding='latin-1') as file:
        for line in file:
            if re.match('^\s*(\#.*)?$', line) == None:
                ruleCount += 1
    return ruleCount


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
            ruleCount = countRules(uploadedFile['path']) #count the number of rules
            rule = FcRule(name=uploadedFile['filename'], path=uploadedFile['path'], count=ruleCount)
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
        
    @api.marshal_with(simpleResponse)
    def put(self, id):
        """
        Updates the rule file
        """
        newFile = request.files['file']
        fileRecord = FcRule.query.filter(FcRule.id == id).first() #the record of rule file in database
        oldFilePath = Path(RULE_DIR) / fileRecord.path
        newFilePath = oldFilePath
        nameChanged = False
        
        if newFile.filename != fileRecord.name: #if the name of file changed
            # check if the file extension is allowed (.txt or .rule)
            if allowed_file(newFile.filename, ALLOWED_EXTENSIONS):
                databaseFileName = secure_filename(newFile.filename)
                filePart = Path(databaseFileName).stem
                # create a new path, rule file has always ".rule" suffix on the server
                serverFileName = filePart + ".rule"
                newFilePath = Path(RULE_DIR) / serverFileName
                
                # check if new filePath doesn't already exist
                if Path(newFilePath).exists():
                    abort(500, "File with name " + serverFileName + " already exists on server. Path: " + str(newFilePath))
            else:
                abort(500, "Can't update file " + fileRecord.name + ". The file extension is not allowed.")
                
            os.rename(oldFilePath, newFilePath)
            fileRecord.name = databaseFileName
            fileRecord.path = serverFileName
            nameChanged = True
        
        # update the content of rule file
        with open(newFilePath, "wb") as origFile:
            origFile.write(newFile.read())
        
        # change record in database
        fileRecord.count = countRules(fileRecord.path)
        db.session.commit()
        
        ''' TODO if there should be unique name in database, then I need to update content of file afterwards
        try:
            db.session.commit()
        except exc.IntegrityError as e: #TODO should name be unique? add to models.py
            db.session().rollback()
            abort(500, 'Rule with name ' + databaseFileName + ' already exists.')
        '''
        
        # return success    
        if(nameChanged):
            returnMessage = "Renamed rule file to " + fileRecord.name + " and successfully updated."
        else:
            returnMessage = "Rule file " + fileRecord.name + " successfully updated."
        return {
            'status': True,
            'message': returnMessage
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
    
    
@ns.route('/randomRule')
class generateRandomRule(Resource):
    @api.marshal_with(randomRule_model)
    def get(self):
        """
        Returns generated random rule.
        """
        min_function_num = 3
        max_function_num = 8
        random_rule_buf = ctypes.create_string_buffer(256)
        
        # generate random rule using the C function
        ret_code = gen_random_rule(random_rule_buf, min_function_num, max_function_num)
        
        return { "randomRule": random_rule_buf.value.decode() }
        
        
        
    
    
@ns.route('/preview')
class passwordsPreview(Resource):
    @api.marshal_with(previewPasswords_model)
    def post(self):
        """
        Returns passwords after rules mangling.
        """
        request_data = request.get_json()
        dictionary = request_data['passwordsList']
        rules = request_data['rules']
        preview = []
        final_password_buf = ctypes.create_string_buffer(64)
        
        counter = 0
        for password in dictionary:
            password = password.strip()
            for rule in rules: 
                counter += 1
                rule = rule.strip()
                                
                # Apply the rule to the password using the C function
                in_len = len(password.encode('latin-1'))
                
                #Returns -1 for rule syntax error, -2 for empty rule or password or new password length if OK
                ret_code = apply_rule(rule.encode('latin-1'), len(rule), password.encode('latin-1'), in_len, final_password_buf)
                
                if(ret_code == -1):
                    final_password_str = ""
                    #if the line in a rule is a comment, specify return code to -3
                    if(len(rule) > 0 and rule[0] == '#'):
                        ret_code = RETCODE_COMMENT
                else:
                    final_password_str = final_password_buf.value.decode('latin-1')
                
                #Add element to a preview list
                element = {
                    "finalPassword" : final_password_str,
                    "retCode" : ret_code
                }                         
                preview.append(element) 
                
        #Return success
        return { 
            'passwordsPreview' : preview,
            'status': True,
            'message': "Passwords were successfully mangled."
        }, 200
                
                
        