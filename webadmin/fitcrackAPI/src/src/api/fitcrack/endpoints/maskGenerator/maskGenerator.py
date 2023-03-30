'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

import os
from flask import request, redirect, send_file
from flask_restx import Resource, abort
from sqlalchemy import exc

from settings import MASKS_DIR, ROOT_DIR
from src.api.apiConfig import api
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
            'message': 'Hello',
            'status': True
        }
