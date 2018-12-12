'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api

axies_model = api.model('GraphAxies', {
    'x': fields.String(),
    'y': fields.List(fields.String())
})

package_graph_model = api.model('Package graph', {
    'type': fields.String(),
    'axies': fields.Nested(axies_model),
    'data': fields.List(fields.Raw),
    'labels': fields.List(fields.String()),
    'dateFrom': fields.String(),
    'dateTo': fields.String()
})

# pie_value_model = api.model('pie graph value', {
#     'label': fields.String(attribute='0'),
#     'value': fields.Integer(attribute='1')
# })


pie_graph_model = api.model('pie graph', {
    'type': fields.String(),
    'data': fields.List(fields.Raw()),
    'dateFrom': fields.String()
})
