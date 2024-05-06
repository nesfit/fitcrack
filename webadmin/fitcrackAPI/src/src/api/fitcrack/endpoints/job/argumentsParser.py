'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''


from flask_restx import reqparse, inputs
from settings import SECONDS_PER_JOB
from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination

jobList_parser = pagination.copy()
jobList_parser.add_argument('name', type=str, required=False, help='filter by name')
jobList_parser.add_argument('status', type=str, required=False, help='filter by state',
                                choices=['ready', 'finished', 'exhausted', 'malformed', 'timeout', 'running', 'finishing'])
jobList_parser.add_argument('attack_mode', type=str, required=False, help='filter by attack type')
jobList_parser.add_argument('order_by', type=str, required=False, help='result ordering',
                                choices=['name', 'time', 'progress', 'attack_mode', 'status'])
jobList_parser.add_argument('descending', type=inputs.boolean, required=False)
jobList_parser.add_argument('showDeleted', type=inputs.boolean, required=False, default=False)
jobList_parser.add_argument('bin', type=int, required=False, default=None)
jobList_parser.add_argument('batch', type=int, required=False, default=None)

jobWorkunit_parser = pagination.copy()

verifyHash_argument = reqparse.RequestParser()
verifyHash_argument.add_argument('hashes', type=str, required=True, help='hash to verify',
                                 default='79c2b46ce2594ecbcb5b73e928345492')
verifyHash_argument.add_argument('hashtype', type=str, required=True,
                                 default='0', help='hash code from /hashcat/hashTypes')

crackingTime_argument = reqparse.RequestParser()
crackingTime_argument.add_argument('hash_list_id', type=int, required=True, help='hash list ID to determine hash type')
crackingTime_argument.add_argument('boinc_host_ids', type=str, required=True, help='host IDs')
crackingTime_argument.add_argument('attack_settings', required=True)

addJob_model = api.schema_model('addJob', {
    "required": ["name", "hash_list_id"],
    'properties': {
        'name': {
            "type": "string",
            "description": "Job name"
        },
        'comment': {
            "type": "string",
            "description": "Optional job comment",
            "default": ''
        },
        'hosts_ids': {
            'description': 'Host IDs from /hosts',
            'type': 'array',
            'items': {
                'type': 'integer',
                'minimum': 0
            }
        },
        'seconds_per_job': {
            "description": "Time (seconds) per one workunit, by default this is " + str(SECONDS_PER_JOB),
            "type": "integer",
            "default": SECONDS_PER_JOB,
            "minimum": 0
        },
        'time_start': {
            "format": "date-time",
            "type": "string",
            "description": "Planned job start time"
        },
        'time_end': {
            "format": "date-time",
            "type": "string",
            "description": "Time limit for cracking"
        },
        'attack_settings': {
            'type': 'object',
            'required': ['attack_mode', 'attack_name'],
            'properties': {
                'attack_mode': {
                    'type': 'integer'
                },
                'attack_name': {
                    'type': 'string'
                },
                'pcfg_grammar': {
                    'description': 'PCFG Grammar',
                    'type': 'object',
                    "properties": {
                        'id': {
                            'type': 'integer'
                        },
                        'keyspace': {
                            'type': 'integer'
                        },
                        'name': {
                            'type': 'string'
                        }
                    },
                },
                'keyspace_limit': {
                    'type': 'integer'
                },
                'left_dictionaries': {
                    'description': 'Array of left dictionaries',
                    'type': 'array',
                    'items': {
                        "type": "object",
                        "properties": {
                            'id': {
                                'type': 'integer'
                            },
                            'keyspace': {
                                'type': 'integer'
                            },
                            'name': {
                                'type': 'string'
                            },
                            'time': {
                                "format": "date-time",
                                "type": "string"
                            },
                        },
                    }
                },
                'right_dictionaries': {
                    'description': 'Array of right dictionaries',
                    'type': 'array',
                    'items': {
                        "type": "object",
                        "properties": {
                            'id': {
                                'type': 'integer'
                            },
                            'keyspace': {
                                'type': 'integer'
                            },
                            'name': {
                                'type': 'string'
                            },
                            'time': {
                                "format": "date-time",
                                "type": "string"
                            },
                        },
                    }
                },
                'rules': {
                    "oneOf": [
                        {"type": "null"},
                        {'type': 'object',
                        'properties': {
                                'id': {
                                    'type': 'integer'
                                },
                                'name': {
                                    'type': 'string'
                                },
                                'time': {
                                    "format": "date-time",
                                    "type": "string"
                                },
                            }
                        }
                    ]
                },
                'rule_left': {
                    'type': 'string'
                },
                'rule_right': {
                    'type': 'string'
                },
                'mask': {
                    'type': 'string'
                },
                'masks': {
                    'type': 'array',
                    'items': {
                        'type': 'string'
                    }
                },
                'attack_submode': {
                    'type': ['integer', 'null']
                },
                'markov_treshold': {
                    'type': ['integer', 'null']
                },
                'markov_id': {
                    'type': ['integer', 'null']
                },
                'charset_ids': [{
                    'type': 'array',
                    'items': {
                        'type': 'integer',
                        'minimum': 0
                    }
                }, 'null'],
                'slow_candidates':  {
                    'type': 'boolean'
                },
            }
        },
        'hash_list_id':  {
            'type': 'integer'
        }
    },
    'type': 'object'
})

jobList_argument = reqparse.RequestParser()
jobList_argument.add_argument('job_ids', type=list, required=True, location='json')

jobOperation = reqparse.RequestParser()
jobOperation.add_argument('operation', type=str, required=True,  help='job action',
                       choices=["start", "stop", "restart", "kill"])

editHostMapping_argument = reqparse.RequestParser()
editHostMapping_argument.add_argument('newHost_ids', type=list, required=True, location='json', help='An array of host IDs')

multiEditHosts_argument = editHostMapping_argument.copy()
multiEditHosts_argument.add_argument('job_ids', type=list, required=True, location='json', help='An array of job IDs')

multiJobOperation_argument = reqparse.RequestParser()
multiJobOperation_argument.add_argument('operation', type=str, required=True,  help='job action', choices=["start", "stop", "kill"])
multiJobOperation_argument.add_argument('job_ids', type=list, required=True, location='json', help='An array of job IDs')

editJob_argument = reqparse.RequestParser()
editJob_argument.add_argument('name', type=str, required=True)
editJob_argument.add_argument('comment', type=str, required=True)
editJob_argument.add_argument('seconds_per_job', type=int, required=True)
editJob_argument.add_argument('time_start', type=str, required=True)
editJob_argument.add_argument('time_end', type=str, required=True)
editJob_argument.add_argument('startNow', type=str, required=True)
editJob_argument.add_argument('endNever', type=str, required=True)

editJob_argument.add_argument('case_permute', type=bool, required=True)
editJob_argument.add_argument('check_duplicates', type=bool, required=True)
editJob_argument.add_argument('min_password_len', type=int, required=True)
editJob_argument.add_argument('max_password_len', type=int, required=True)
editJob_argument.add_argument('min_elem_in_chain', type=int, required=True)
editJob_argument.add_argument('max_elem_in_chain', type=int, required=True)
editJob_argument.add_argument('generate_random_rules', type=int, required=True)

job_permissions_arguments = reqparse.RequestParser()
job_permissions_arguments.add_argument('user_id', type=int, help='user to grant/revoke permissions on', required=True, location='json')
job_permissions_arguments.add_argument('view', type=bool, required=False, location='json', help='grant view permission? when not provided: for new record defaults to false, otherwise leaves unchanged')
job_permissions_arguments.add_argument('modify', type=bool, required=False, location='json', help='grant modify permission? when not provided: for new record defaults to false, otherwise leaves uncFalse')
job_permissions_arguments.add_argument('operate', type=bool, required=False, location='json', help='grant operate permission? when not provided: for new record defaults to false, otherwise leaves unchanged')
