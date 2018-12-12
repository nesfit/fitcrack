'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

hcStat_model = api.model('HcStat', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'time': fields.DateTime(readOnly=True, required=False),
})
hcStatsCollection_model = api.inherit('HcStats collection', {
    'items': fields.List(fields.Nested(hcStat_model))
})
