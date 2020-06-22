'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import datetime

from flask_restplus import abort
from sqlalchemy import func, Integer, desc

from src.database import db
from src.database.models import FcJobGraph, FcWorkunit, Host

def computeJobsGraph(fromDate=None, toDate=None, jobId=None):
    x = "time"
    y = set()
    data = []
    labels = []

    jobsGraphData = FcJobGraph.query

    if toDate:
        date = get_date_or_exception(toDate)
        jobsGraphData = jobsGraphData.filter(FcJobGraph.time <= date)
        data.append({'time': toDate})

    if fromDate:
        date = get_date_or_exception(fromDate)
        jobsGraphData = jobsGraphData.filter(FcJobGraph.time >= date)
        data.append({'time': fromDate})

    if toDate and fromDate and get_date_or_exception(toDate) < get_date_or_exception(fromDate):
        abort(500, 'From date is greater than to date.')


    if jobId:
        jobsGraphData = jobsGraphData.filter_by(job_id=jobId)

    jobsGraphData.order_by(desc(FcJobGraph.id))
    for jobProgress in jobsGraphData:
        data.append(jobProgress.as_graph())
        if not jobProgress.job.id in y:
            y.add(jobProgress.job.id)
            labels.insert(0, jobProgress.job.name)

    if not jobId and not toDate:
        data.append({'time': datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")})

    if len(y) == 0:
        y.add(None)

    return {
        'type': 'line',
        'data': data,
        'axies': {
            'x': x,
            'y': y
        },
        'labels': labels,
        'dateFrom': fromDate,
        'dateTo': toDate
    }


def computeHostGraph(fromDate=None, toDate=None, jobIds=None):
    x = "time"
    y = []
    data = []
    labels = []
    jobsGraphData = FcWorkunit.query

    if jobIds and not isinstance(jobIds, list):
        jobIds = [jobIds]

    if toDate:
        date = get_date_or_exception(toDate)
        data.append({'time': toDate})
        jobsGraphData = jobsGraphData.filter(FcWorkunit.time <= date)

    if fromDate:
        date = get_date_or_exception(fromDate)
        data.append({'time': fromDate})
        jobsGraphData = jobsGraphData.filter(FcWorkunit.time >= date)


    if toDate and fromDate and get_date_or_exception(toDate) < get_date_or_exception(fromDate) :
        abort(500, 'From date is greater than to date.')

    if jobIds:
        jobsGraphData = jobsGraphData.filter(FcWorkunit.job_id.in_(jobIds))

    for jobData in jobsGraphData:
        data.append(jobData.as_graph())
        yAx = str(jobData.boinc_host_id) + '_' + str(jobData.job_id)
        if not yAx in y:
            y.append(yAx)
            labels.append(jobData.host.domain_name)

    if not jobIds and not toDate:
        data.append({'time': datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")})

    if len(y) == 0:
        y.append(None)

    return {
        'type': 'line',
        'data': data,
        'axies': {
            'x': x,
            'y': y
        },
        'labels': labels,
        'dateFrom': fromDate,
        'dateTo': toDate
    }


def computeHostPercentageGraph(fromDate=None, jobIds=None):
    date = None
    jobsGraphData = FcWorkunit.query

    if jobIds and not isinstance(jobIds, list):
        jobIds = [jobIds]

    if fromDate:
        try:
            date = datetime.datetime.strptime(fromDate, "%Y-%m-%d %H:%M:%S")
        except ValueError:
            abort(500, 'Wrong date fromat')
        jobsGraphData = jobsGraphData.filter(FcWorkunit.time >= date)

    # TODO: implement dates

    jobsGraphData = db.session.query(Host.domain_name.label('label'),
                                     func.cast(func.sum(FcWorkunit.hc_keyspace), Integer()).label('value')).filter(
        FcWorkunit.job_id.in_(jobIds)).group_by(FcWorkunit.boinc_host_id).join(FcWorkunit.host).all()

    data = []
    for tup in jobsGraphData:
        data.append({
            'label': tup[0],
            'value': tup[1]
        })

    return {
        'type': 'pie',
        'data': data,
        'dateFrom': fromDate,
    }



def get_date_or_exception(date, abortOnFail=True):
    try:
        date = datetime.datetime.strptime(date, "%Y-%m-%d %H:%M:%S")
    except ValueError:
        abort(500, 'Wrong date fromat')

    return date
