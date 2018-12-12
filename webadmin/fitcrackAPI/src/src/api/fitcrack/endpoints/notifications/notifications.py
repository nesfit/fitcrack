'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging

from flask import request
from flask_login import current_user, login_required
from flask_restplus import Resource

from src.api.apiConfig import api
from src.api.fitcrack.endpoints.notifications.argumentsParser import notifications_parser
from src.api.fitcrack.endpoints.notifications.functions import getNotifications
from src.api.fitcrack.endpoints.notifications.responseModels import page_of_notifications_model, \
    notification_count_model
from src.database.models import FcNotification

log = logging.getLogger(__name__)
ns = api.namespace('notifications', description='Endpointy ktoré slúžia na vypisovanie grafov')


@ns.route('')
class notifications(Resource):

    @api.expect(notifications_parser)
    @api.marshal_with(page_of_notifications_model)
    def get(self):
        """
        Vráti uživateľove notifikácie
        """
        args = notifications_parser.parse_args(request)
        page = args.get('page', 1)
        per_page = args.get('per_page', 10)
        markAsSeen = args.get('seen', True)

        notifications = getNotifications(current_user.id, page, per_page, markAsSeen)
        # notifications = getNotifications(7, page, per_page, markAsSeen)
        return notifications

@ns.route('/count')
class notifications(Resource):

    @api.marshal_with(notification_count_model)
    def get(self):
        """
        Vráti počet neprečítaných notifikácií
        """

        count = FcNotification.query.filter(FcNotification.user_id == current_user.id).filter(
            FcNotification.seen == False).count()

        # count = FcNotification.query.filter(FcNotification.user_id == 7).filter(
        #     FcNotification.seen == False).count()
        return {'count': count}
