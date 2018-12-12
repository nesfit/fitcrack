'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields

from src.api.apiConfig import api
from src.api.fitcrack.responseModels import pagination

notification_model = api.model('Notification', {
    'job_id': fields.Integer(readOnly=True, required=False, description='id package'),
    'text': fields.String(),
    'type': fields.String(),
    'seen': fields.Boolean(),
    'time': fields.DateTime()
})

page_of_notifications_model = api.inherit('Page of notifications', pagination, {
    'items': fields.List(fields.Nested(notification_model))
})

notification_count_model = api.model('Notification count', {
    'count': fields.Integer()
})
