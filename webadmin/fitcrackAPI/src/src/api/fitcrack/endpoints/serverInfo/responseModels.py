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


usageinfo = api.model("UsageInfo", {
    "time": fields.DateTime(),
    "cpu": fields.Float(),
    "ram": fields.Float(),
    "hdd_read": fields.Integer(),
    "hdd_write": fields.Integer(),
    "net_sent": fields.Integer(),
    "net_recv": fields.Integer(),
})

usageinfoList = api.model('UsageInfoList', {
    'items': fields.List(fields.Nested(usageinfo)),
})
