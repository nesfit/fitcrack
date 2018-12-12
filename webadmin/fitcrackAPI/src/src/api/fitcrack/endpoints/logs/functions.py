'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from datetime import datetime
import re


def log_parser(log):
    result = {
        'type': '',
        'time': '',
        'host': '',
        'package': '',
        'log': '',
        'raw': log
    }
    if log.startswith('LOG:'):
        result['type'] = 'LOG'
        result['time'] = datetime.strptime(log[5:24], '%d.%m.%Y %H:%M:%S')
    elif log.startswith('ERROR:'):
        result['type'] = 'ERROR'
        result['time'] = datetime.strptime(log[7:26], '%d.%m.%Y %H:%M:%S')
    elif log.startswith('WARN:'):
        result['type'] = 'WARN'
        result['time'] = datetime.strptime(log[6:25], '%d.%m.%Y %H:%M:%S')

    m = re.match(r"(.*)(\[Package #(?P<job_id>\w+)])(\[Host #(?P<host_id>\w+)])? (?P<msg>(.*))", log)
    regexDict = m.groupdict()
    result['package'] = regexDict['job_id']
    result['host'] = regexDict['host_id']
    result['log'] = regexDict['msg']

    return result

