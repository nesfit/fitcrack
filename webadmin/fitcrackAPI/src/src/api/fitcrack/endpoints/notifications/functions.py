'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from sqlalchemy import desc

from src.api.fitcrack.lang import job_status_text_info_to_code_dict, status_to_code
from src.database import db
from src.database.models import FcNotification


def getNotifications(userID, page, per_page, markAsSeen):
    notifications = []
    DBnotifications = FcNotification.query.filter(FcNotification.user_id == userID).order_by(
        desc(FcNotification.time)).paginate(page, per_page, error_out=True)
    for notif in DBnotifications.items:
        notifications.append(
            {
                'job_id': notif.source_id,
                'title': notif.source.name,
                'text': job_status_text_info_to_code_dict[notif.new_value],
                'type': getNotifType(notif.new_value),
                'seen': notif.seen,
                'time': notif.time
            }
        )
        if markAsSeen:
            notif.seen = True

    db.session.commit()
    DBnotifications.items = notifications
    return DBnotifications


def getNotifType(code):
    if code == status_to_code['ready'] or code == status_to_code['running'] or code == status_to_code['finishing']:
        return 'info'
    if code == status_to_code['finished']:
        return 'success'
    if code == status_to_code['timeout']:
        return 'warning'
    if code == status_to_code['exhausted'] or code == status_to_code['malformed']:
        return 'error'
