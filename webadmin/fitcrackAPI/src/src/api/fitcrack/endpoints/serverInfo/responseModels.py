'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields
from src.api.apiConfig import api


subsystem = api.model('Subsystem', {
    "name": fields.String(),
    "status": fields.String()
})


serverinfo = api.model('ServerInfo', {
    'subsystems': fields.List(fields.Nested(subsystem)),
})




