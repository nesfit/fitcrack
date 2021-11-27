'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import fields

from src.api.apiConfig import api

time_series_dataset = api.model('Time series dataset', {
  'label': fields.String(),
  'data': fields.Raw(),
  'meta': fields.Raw()
})

time_series_model = api.model('Time series chart data', {
  'datasets': fields.List(fields.Nested(time_series_dataset))
})

doughnut_model = api.model('Doughnut chart data', {
  'labels': fields.List(fields.String()),
  'data': fields.List(fields.Integer()),
  'meta': fields.Raw()
})
