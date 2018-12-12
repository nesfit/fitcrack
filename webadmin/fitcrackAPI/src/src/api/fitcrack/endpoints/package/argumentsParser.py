'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse, inputs

from settings import SECONDS_PER_JOB
from src.api.apiConfig import api
from src.api.fitcrack.argumentsParser import pagination

packageList_parser = pagination.copy()
packageList_parser.add_argument('name', type=str, required=False, help='filtrovanie balíkov podľa názvu')
packageList_parser.add_argument('status', type=str, required=False, help='filtrovanie podľa stavu package',
                                choices=['ready', 'finished', 'exhausted', 'malformed', 'timeout', 'running', 'validating', 'finishing'])
packageList_parser.add_argument('attack_mode', type=str, required=False, help='filtrovanie podľa typu útoku')
packageList_parser.add_argument('order_by', type=str, required=False, help='zoradenie výsledkov',
                                choices=['name', 'time', 'progress', 'attack_mode', 'status'])
packageList_parser.add_argument('descending', type=inputs.boolean, required=False)
packageList_parser.add_argument('showDeleted', type=inputs.boolean, required=False, default=False)


packageJob_parser = pagination.copy()

verifyHash_argument = reqparse.RequestParser()
verifyHash_argument.add_argument('hashes', type=str, required=True, help='hash na overenie',
                                 default='79c2b46ce2594ecbcb5b73e928345492')
verifyHash_argument.add_argument('hashtype', type=str, required=True,
                                 default='0', help='Kod (code) hashu z /hashcat/hashTypes')

crackingTime_argument = reqparse.RequestParser()
crackingTime_argument.add_argument('hash_type_code', type=str, required=True, help='typ hashu', default='0')
crackingTime_argument.add_argument('boinc_host_ids', type=str, required=True, help='id hostov')
crackingTime_argument.add_argument('attack_settings', required=True)

addPackage_model = api.schema_model('addPackage', {
    "required": ["name"],
    'properties': {
        'name': {
            "type": "string",
            "description": "Názov balíka"
        },
        'comment': {
            "type": "string",
            "description": "Komentár k balíku",
            "default": ''
        },
        'hosts_ids': {
            'description': 'id hostov z /hosts',
            'type': 'array',
            'items': {
                'type': 'integer',
                'minimum': 0
            }
        },
        'seconds_per_job': {
            "description": "Cas pre jednu ulohu. Defualt je " + str(SECONDS_PER_JOB),
            "type": "integer",
            "default": SECONDS_PER_JOB,
            "minimum": 0
        },
        'time_start': {
            "format": "date-time",
            "type": "string",
            "description": "Kedy sa ma package zacat crackovat."
        },
        'time_end': {
            "format": "date-time",
            "type": "string",
            "description": "Kedy ma fitcrack prestat crackovat package."
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
                'left_dictionaries': {
                    'description': 'array of left dictionaries',
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
                    'description': 'array of right dictionaries',
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
            }
        },
        'hash_settings':  {
            'type': 'object',
            'required': ['hash_type'],
            'properties': {
                'hash_type': {
                    'default': '0',
                    'type': 'string',
                    'description': 'Kod (code) hashu z /hashcat/hashTypes'
                },
                'hash_list': {
                    'type': 'array',
                    'items': {
                        'type': 'object',
                        'properties': {
                            'hash': {
                                'type': 'string',
                                'description': 'hash'
                            }
                        }
                    }
                }
            }
        }
    },
    'type': 'object'
})





packageOperation = reqparse.RequestParser()
packageOperation.add_argument('operation', type=str, required=True,  help='operácia s package',
                       choices=["start", "stop", "restart"])

editHostMapping_argument = reqparse.RequestParser()
editHostMapping_argument.add_argument('newHost_ids', type=list, required=True, location='json')


editPackage_argument = reqparse.RequestParser()
editPackage_argument.add_argument('name', type=str, required=True)
editPackage_argument.add_argument('comment', type=str, required=True)
editPackage_argument.add_argument('seconds_per_job', type=int, required=True)
editPackage_argument.add_argument('time_start', type=str, required=True)
editPackage_argument.add_argument('time_end', type=str, required=True)
editPackage_argument.add_argument('startNow', type=str, required=True)
editPackage_argument.add_argument('endNever', type=str, required=True)