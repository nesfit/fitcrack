'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields
from src.api.apiConfig import api


hashType = api.model('HashType', {
    'code': fields.String(description='Code of the hash'),
    'name': fields.String(description='Name of the hash'),
    'category': fields.String(description='Category of the hash')
})

hashType_list = api.model('HashTypeList', {
    'hashtypes': fields.List(fields.Nested(hashType)),
})




attackMode = api.model('AttackMode', {
    'code': fields.String(description='Code of the hash', attribute='#'),
    'mode': fields.String(description='Name of the hash', attribute='Mode')
})

attackMode_list = api.model('AttackModeList', {
    'attackmodes': fields.List(fields.Nested(attackMode)),
})
