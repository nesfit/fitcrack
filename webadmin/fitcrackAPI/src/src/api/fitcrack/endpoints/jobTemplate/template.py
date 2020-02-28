'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
import json

from flask import request

from flask_restplus import Resource
from flask_restplus import reqparse

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.jobTemplate.responseModels import templates_model, template_data
from src.api.fitcrack.responseModels import simpleResponse

from src.database import db
from src.database.models import FcTemplate

log = logging.getLogger(__name__)
ns = api.namespace('template', description='Endpoints for job templates.')

add_model = api.schema_model('add_template', {'properties': {'template': {'type': 'string'}}})

#add_template_args = reqparse.RequestParser()
#add_template_args.add_argument('template', type=str, help='name of the template', required=True, location='json')

@ns.route('')
class template(Resource):

    @api.expect(add_model)
    @api.marshal_with(simpleResponse)
    def post(self):
        """
        Add a job template.
        """

        data = request.json
        template = FcTemplate(name=data['template'], template=data)
        try:
            db.session.add(template)
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t add template.')
        return {
            'status': True,
            'message': 'Template ' + data['template'] + ' added.'
        }

    @api.marshal_with(templates_model)
    def get(self):
        """
        Returns collection of job templates.
        """
        return {'items': FcTemplate.query.all()}


@ns.route('/<id>')
class templateData(Resource):

    @api.marshal_with(template_data)
    def get(self, id):
        """
        Returns a template with its job settings.
        """
        template = FcTemplate.query.get(id)
        if template is None:
            abort(404, 'No such template')
        else:
            return {
                'template': json.dumps(template.template) # uhm...
            }
