'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields
from src.api.apiConfig import api


hashType = api.model('HashType', {
    'code': fields.String(description='Kód hashu'),
    'name': fields.String(description='Názov hashu'),
    'category': fields.String(description='Kategória hashu'),
    'subcategories': fields.Raw()
})

hashType_list = api.model('HashTypeList', {
    'hashtypes': fields.List(fields.Nested(hashType)),
})




attackMode = api.model('AttackMode', {
    'code': fields.String(description='Kód hashu', attribute='#'),
    'mode': fields.String(description='Názov hashu', attribute='Mode')
})

attackMode_list = api.model('AttackModeList', {
    'attackmodes': fields.List(fields.Nested(attackMode)),
})