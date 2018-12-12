'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import datetime

from flask_restplus import abort
from sqlalchemy import func, Integer, desc

from src.database import db
from src.database.models import FcJobGraph, FcWorkunit, Host


def computePackagesGraph(fromDate=None, toDate=None, packageId=None):
    x = "time"
    y = set()
    data = []
    labels = []

    packagesGraphData = FcJobGraph.query

    if toDate:
        date = get_date_or_exception(toDate)
        packagesGraphData = packagesGraphData.filter(FcJobGraph.time <= date)
        data.append({'time': toDate})

    if fromDate:
        date = get_date_or_exception(fromDate)
        packagesGraphData = packagesGraphData.filter(FcJobGraph.time >= date)
        data.append({'time': fromDate})

    if toDate and fromDate and get_date_or_exception(toDate) < get_date_or_exception(fromDate):
        abort(500, 'From date is greater than to date.')


    if packageId:
        packagesGraphData = packagesGraphData.filter_by(job_id=packageId)

    packagesGraphData.order_by(desc(FcJobGraph.id))
    for packageProgress in packagesGraphData:
        data.append(packageProgress.as_graph())
        if not packageProgress.job.id in y:
            y.add(packageProgress.job.id)
            labels.insert(0, packageProgress.job.name)

    if not packageId and not toDate:
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


def computeHostGraph(fromDate=None, toDate=None, packageId=None):
    x = "time"
    y = []
    data = []
    labels = []
    jobsGraphData = FcWorkunit.query


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

    if packageId:
        jobsGraphData = jobsGraphData.filter_by(job_id=packageId)

    for jobData in jobsGraphData:
        data.append(jobData.as_graph())
        yAx = str(jobData.boinc_host_id) + '_' + str(jobData.job_id)
        if not yAx in y:
            y.append(yAx)
            labels.append(jobData.host.domain_name)

    if not packageId and not toDate:
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


def computeHostPercentageGraph(fromDate=None, packageId=None):
    date = None
    jobsGraphData = FcWorkunit.query

    if fromDate:
        try:
            date = datetime.datetime.strptime(fromDate, "%Y-%m-%d %H:%M:%S")
        except ValueError:
            abort(500, 'Wrong date fromat')
        jobsGraphData = jobsGraphData.filter(FcWorkunit.time >= date)

    # TODO: Zakomponovat datum

    jobsGraphData = db.session.query(Host.domain_name.label('label'),
                                     func.cast(func.sum(FcWorkunit.hc_keyspace), Integer()).label('value')).filter(
        FcWorkunit.job_id == packageId).group_by(FcWorkunit.boinc_host_id).join(FcWorkunit.host).all()

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