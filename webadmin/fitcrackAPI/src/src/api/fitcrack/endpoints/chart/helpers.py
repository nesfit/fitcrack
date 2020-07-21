'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from datetime import datetime

class DateFormatError (Exception):
  """Raised when date parsing encounters incorrect datetime string"""

class DatePrecedenceError (Exception):
  """Raised when requested end date bound precedes the begin bound"""

def parse_date (date):
  try:
    if date.count(':') == 1:
      date += ':00'
    return datetime.strptime(date, "%Y-%m-%dT%H:%M:%S")
  except ValueError:
    raise DateFormatError()

def set_bounds_filter (query, model, bounds_args):
  begin, end = bounds_args['from'], bounds_args['to']
  # set bounds
  begin_date, end_date = None, None
  if begin:
    begin_date = parse_date(begin)
    query = query.filter(model.time >= begin_date)
  if end:
    end_date = parse_date(end)
    query = query.filter(model.time <= end_date)
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