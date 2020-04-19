'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.batches.responseModels import batch_model
from src.api.fitcrack.endpoints.dictionary.responseModels import dictionary_model
from src.api.fitcrack.endpoints.pcfg.responseModels import pcfg_model
from src.api.fitcrack.endpoints.markov.responseModels import hcStat_model
from src.api.fitcrack.responseModels import pagination, simpleResponse, job_short_model, \
    boincHost_model


boincResult_model = api.model('boinc result', {
    'id': fields.String(),
    'stderr_out_text': fields.String(),

})

workunit_model = api.model('Workunit', {
    'id': fields.String(),
    'job_id': fields.Integer(),
    'workunit_id': fields.Integer(),
    'host_id': fields.Integer(),
    'boinc_host_id': fields.Integer(),
    'start_index': fields.Integer(),
    'start_index_2': fields.Integer(),
    'hc_keyspace': fields.Integer(),
    'mask_id': fields.Integer(),
    'duplicated': fields.Boolean(),
    'duplicate': fields.Integer(),
    'time': fields.DateTime(),
    'cracking_time': fields.Integer(),
    'cracking_time_str': fields.String(),
    'retry': fields.Boolean(),
    'finished': fields.Boolean(),
    'host': fields.Nested(boincHost_model),
    'result': fields.Nested(boincResult_model),
    'progress': fields.Integer(),
})

mask_model = api.model('Mask', {
    'id': fields.Integer(readOnly=True, required=False),
    'job_id': fields.Integer(readOnly=True, required=False),
    'mask': fields.String(),
    'current_index': fields.Integer(),
    'keyspace': fields.Integer(),
    'hc_keyspace': fields.Integer(),
    'progress': fields.Float(required=False)

})

hash_model = api.model('Hash', {
    'hashText': fields.String(),
    'password': fields.String(),
    'time_cracked': fields.DateTime()
})

page_of_jobs_model = api.inherit('Page of jobs', pagination, {
    'items': fields.List(fields.Nested(job_short_model))
})

page_of_workunits_model = api.inherit('Page of workunits', pagination, {
    'items': fields.List(fields.Nested(workunit_model))
})

verifyHash_model = api.model('Verified hash', {
    'hash': fields.String(required=True, description='Hash being validated'),
    'result': fields.String(required=True, description='Whether the hash is valid'),
    'isInCache': fields.Boolean()
})

verifyHashes_model = api.model('Verified hashes', {
    'items': fields.List(fields.Nested(verifyHash_model)),
    'error': fields.Boolean()
})

host_cracking_time = api.model('host cracking time', {
    'boinc_host_id': fields.Integer(required=True, default=None),
    'hash_type': fields.String(requireed=True, default=None),
    'last_update': fields.DateTime(required=True, default=None),
    'power': fields.Integer(required=True, default=0)
})

crackingTime_model = api.model('Cracking time', {
    'hash_code': fields.String(required=True, description='Hash type', default=None),
    'keyspace': fields.Integer(required=True, default=None),
    'total_power': fields.Integer(required=True, default=None),
    'display_time': fields.String(required=True, default=None),
    'hosts': fields.List(fields.Nested(host_cracking_time))
})

newJob_model = api.inherit('new job', simpleResponse, {
    'job_id': fields.Integer()
})

dictionary_job_model = api.model('Dictionary job', {
    'current_index': fields.Integer(),
    'is_left': fields.Boolean(),
    'dictionary': fields.Nested(dictionary_model)
})
'''
pcfgGrammar_model = api.model('PCFG job', {
    'pcfg': fields.String(pcfg_model)
})
'''


job_big_model = api.model('Job', {
    'id': fields.Integer(readOnly=True, required=False, description='id of the job'),
    'name': fields.String(required=True, description='name of the job'),
    'batch': fields.Nested(batch_model),
    'comment': fields.String(required=False),
    'priority': fields.Integer(),
    'attack_mode': fields.String(required=True),
    'attack_submode': fields.Integer(),
    'attack': fields.String(required=True),
    'status': fields.String(required=False),
    'status_text': fields.String(required=False),
    'status_tooltip': fields.String(required=False),
    'status_type': fields.String(),
    'progress': fields.Float(required=False),
    'time': fields.DateTime(required=False),
    'cracking_time': fields.Float(),
    'cracking_time_str': fields.String(),
    'hash_type_name': fields.String(),
    'hash_type': fields.String(required=True),
    'hash': fields.String(required=True),
    'keyspace': fields.String(required=True),
    'hc_keyspace': fields.String(required=True),
    'indexes_verified': fields.String(required=True, default='0'),
    'current_index': fields.String(required=True, default='0'),
    'current_index_2': fields.String(required=True, default='0'),
    'time_start': fields.DateTime(required=True),
    'time_end': fields.DateTime(required=True),
    'seconds_per_job': fields.String(attribute='seconds_per_workunit',required=True, default='0'),
    'dict1': fields.String(required=True),
    'dictionary1': fields.Nested(dictionary_model),
    'dict2': fields.String(required=True),
    'dictionary2': fields.Nested(dictionary_model),
    'charSet1': fields.Nested(hcStat_model),
    'charSet2': fields.Nested(hcStat_model),
    'charSet3': fields.Nested(hcStat_model),
    'charSet4': fields.Nested(hcStat_model),
    'rulesFile': fields.Nested(hcStat_model),
    'rule_left': fields.String(),
    'rule_right': fields.String(),
    'markov': fields.Nested(hcStat_model),
    'markov_threshold': fields.Integer(),
    'replicate_factor': fields.String(required=True),
    'hosts': fields.List(fields.Nested(boincHost_model)),
    'workunits': fields.List(fields.Nested(workunit_model)),
    'masks': fields.List(fields.Nested(mask_model)),
    'password': fields.String(),
    'hashes': fields.List(fields.Nested(hash_model)),
    'left_dictionaries': fields.List(fields.Nested(dictionary_job_model)),
    'right_dictionaries': fields.List(fields.Nested(dictionary_job_model)),
    'grammar_id': fields.Integer(),
    'grammar_name': fields.String(),
    'grammar_keyspace': fields.Integer()
})

job_nano_model = api.model('Job nano', {
    'id': fields.Integer(readOnly=True, required=False, description='job id'),
    'name': fields.String(required=True, description='job name'),
    'status': fields.String(required=False),
    'status_text': fields.String(required=False),
    'status_tooltip': fields.String(required=False),
    'status_type': fields.String(),
    'progress': fields.Float(required=False),
})

job_nano_list_model = api.inherit('Job nano list', {
    'items': fields.List(fields.Nested(job_nano_model))
})
