'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask import request
from flask_restx import Resource, abort
from sqlalchemy import exc

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.settings.argumentsParser import settings_arguments
from src.api.fitcrack.endpoints.settings.responseModels import settings_model
from src.api.fitcrack.responseModels import simpleResponse
from src.database import db
from src.database.models import FcSetting

log = logging.getLogger(__name__)
ns = api.namespace('settings', description='Endpoints for manipulating system settings.')

@ns.route('')
class settings(Resource):

    @api.marshal_with(settings_model)
    def get(self):
        """
        Returns all system settings.
        """
        settings = FcSetting.query.first() # single row table
        return settings

    @api.expect(settings_arguments)
    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Sets all system settings.
        """
        args = settings_arguments.parse_args(request)
        spw = args['default_seconds_per_workunit'] 
        wtf = args['workunit_timeout_factor']
        hta = args['hwmon_temp_abort']
        dba = args['bench_all']
        dca = args['distribution_coefficient_alpha']
        tpmin = args['t_pmin']
        ruw = args['ramp_up_workunits']
        rdc = args['ramp_down_coefficient']
        vhf = args['verify_hash_format']
        aahtrj = args['auto_add_hosts_to_running_jobs']

        settings = FcSetting.query.first()
        if (spw is not None): settings.default_seconds_per_workunit = spw
        if (wtf is not None): settings.workunit_timeout_factor = wtf
        if (hta is not None): settings.hwmon_temp_abort = hta
        if (dba is not None): settings.bench_all = dba
        if (dca is not None): settings.distribution_coefficient_alpha = dca
        if (tpmin is not None): settings.t_pmin = tpmin
        if (ruw is not None): settings.ramp_up_workunits = ruw
        if (rdc is not None): settings.ramp_down_coefficient = rdc
        if (vhf is not None): settings.verify_hash_format = vhf
        if (aahtrj is not None): settings.auto_add_hosts_to_running_jobs = aahtrj
        db.session.commit()

        return {
            'status': True,
            'message': 'Settings saved.'
        }