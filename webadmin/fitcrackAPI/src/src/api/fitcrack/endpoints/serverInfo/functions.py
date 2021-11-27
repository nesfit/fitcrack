'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import datetime
from flask_restx.errors import abort
from src.api.fitcrack.functions import shellExec
from src.database.models import FcServerUsage


def isComponentRunning(compName):
    out = shellExec("ps -ef | grep " + compName + " | grep -v grep")
    return True
    if len(out.splitlines()) >= 1:
        return True

    return False


def getCpuMemData(fromDate=None, toDate=None):
    data = []
    graphData = FcServerUsage.query

    if toDate:
        date = get_date_or_exception(toDate)
        data.append({'time': toDate})
        graphData = graphData.filter(FcServerUsage.time <= date)

    if fromDate:
        date = get_date_or_exception(fromDate)
        data.append({'time': fromDate})
        graphData = graphData.filter(FcServerUsage.time >= date)

    if toDate and fromDate and get_date_or_exception(toDate) < get_date_or_exception(fromDate):
        abort(500, 'From date is greater than to date.')

    return {
        'items': graphData.all()
    }


def get_date_or_exception(date, abortOnFail=True):
    try:
        date = datetime.datetime.strptime(date, "%Y-%m-%d %H:%M:%S")
    except ValueError:
        abort(500, 'Wrong date fromat')

    return date
