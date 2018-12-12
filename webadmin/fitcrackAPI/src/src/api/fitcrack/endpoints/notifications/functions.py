'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from sqlalchemy import desc

from src.api.fitcrack.lang import package_status_text_to_code_dict
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
                'text': 'Job ' + notif.source.name + ' ' + package_status_text_to_code_dict[notif.new_value],
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
    if code == 0 or code == 10 or code == 12:
        return 'info'
    if code == 1:
        return 'success'
    if code == 0 or code == 4:
        return 'warning'
    if code == 1 or code == 2 or code == 3:
        return 'error'
