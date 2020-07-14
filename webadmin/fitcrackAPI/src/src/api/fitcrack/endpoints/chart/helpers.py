'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from datetime import datetime
from src.database.models import FcJobGraph

class DateFormatError (Exception):
  """Raised when date parsing encounters incorrect datetime string"""

class DatePrecedenceError (Exception):
  """Raised when requested end date bound precedes the begin bound"""

def parse_date (date):
  try:
    return datetime.strptime(date, "%Y-%m-%dT%H:%M:%S")
  except ValueError:
    raise DateFormatError()

def set_bounds_filter (query, bounds_args):
  begin, end = bounds_args['from'], bounds_args['to']
  # set bounds
  begin_date, end_date = None, None
  if begin:
    begin_date = parse_date(begin)
    query = query.filter(FcJobGraph.time >= begin_date)
  if end:
    end_date = parse_date(end)
    query = query.filter(FcJobGraph.time <= end_date)
  # check if begin is before end
  if begin_date and end_date and end_date < begin_date:
    raise DatePrecedenceError()
  return query

def build_job_datasets (datapoints): # job:obj, time:date, progress:int
  datasets = {}
  for d in datapoints:
    if not d.job.id in datasets:
      datasets[d.job.id] = {'label': d.job.name, 'data': {}, 'meta': {'id': d.job.id}}
    datasets[d.job.id]['data'][d.time.isoformat()] = int(d.progress)
  return [val for val in datasets.values()]

def build_wu_datasets (datapoints): # host:obj, time:date, hc_keyspace:int
  datasets = {}
  for d in datapoints:
    if not d.host.id in datasets:
      datasets[d.host.id] = {'label': d.host.domain_name, 'data': {}, 'meta': {'id': d.host.id}}
    datasets[d.host.id]['data'][d.time.isoformat()] = int(d.hc_keyspace)
  return [val for val in datasets.values()]

def build_doughnut (data): # (hostname, keyspace)
  labels = []
  values = []
  for d in data:
    labels.append(d[0])
    values.append(d[1])
  return labels, values

#####

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
