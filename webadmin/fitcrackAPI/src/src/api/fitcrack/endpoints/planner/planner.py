'''
   * Author : David Brandšteter (xbrand13)
   * Licence: MIT, see LICENSE
'''
import logging
from flask_login import current_user
from src.database import db
from flask import request
from flask_restx import Resource, abort
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.planner.argumentsParser import planner_parser
from src.database.models import FcDictionary, FcBatch, FcJob, FcRule
from src.api.fitcrack.endpoints.job.functions import create_job
from sqlalchemy import exc

from src.api.fitcrack.endpoints.planner.functions import decide_on_rules, make_dict_attack, make_bruteforce_attack, compute_keyspace


log = logging.getLogger(__name__)
ns = api.namespace('planner', description='Endpoints for automatic planner')

@ns.route('')
class planner(Resource):

    @api.expect(planner_parser)
    @api.response(201, 'Created')
    @api.response(200, 'Updated')
    @api.response(400, 'Failed')
    @api.response(500, 'Failed')
    def post(self):
        """
        Generates a Batch of planned attacks.
        """
        args = planner_parser.parse_args(request)

        #Compute dictionary keyspace and select dictionaries
        keyspace = compute_keyspace(args['days'], args['hours'], args['minutes'], args['cracking_speed'])
        dictionaries = FcDictionary.query.filter(FcDictionary.keyspace <= keyspace).all()

        #Args
        is_new = True
        final_jobs = []
        name = args['name']
        hash_list = args['hashList']
        hosts_ids = args['hostsIds']
        fill = args['fill']
        password_len_min = args['password_len_min']
        password_len_max = args['password_len_max']

        #Pick effective rules
        rules = FcRule.query.all()
        rule = decide_on_rules(dictionaries, rules, keyspace)

        #Dictionary attacks
        for dict in dictionaries:
            if rule != None:
                attack_keyspace = dict.keyspace * rule.count
            else:
                attack_keyspace = dict.keyspace

            if keyspace > attack_keyspace:
                data = make_dict_attack(name, dict, rule, hash_list, hosts_ids)
                job = create_job(data)
                final_jobs.append(job.id)
                keyspace = keyspace - attack_keyspace
            else:
                continue

        #Brute-force Fill
        if fill == True:
            data = make_bruteforce_attack(name, hash_list, hosts_ids, password_len_min, password_len_max, keyspace)
            if data:
                job = create_job(data)
                final_jobs.append(job.id)

        if final_jobs == []:
            abort(400, 'Couldn\'t plan any jobs. Time is too short or cracking speed is too low.')

        #Batch creation
        batch = FcBatch()
        batch.name = name
        batch.jobs = FcJob.query.filter(FcJob.id.in_(final_jobs)).all()
        batch.creator_id = current_user.id
        for index, job_id in enumerate(final_jobs):
            job = FcJob.query.filter_by(id=job_id).one_or_none()
            if job:
                job.queue_position = index

        try:
            if is_new:
                db.session.add(batch)
            db.session.commit()
        except exc.IntegrityError as e:
            db.session().rollback()
            abort(500, 'Couldn\'t create batch.')   
        
        return {
            'message': 'batch \"' + batch.name + '\" successfully created.',
            'status': True,
            'batch_id': batch.id,
            'unused_keyspace': keyspace
        }
