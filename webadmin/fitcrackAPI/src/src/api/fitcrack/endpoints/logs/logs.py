'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import logging
from flask_restplus import Resource
from flask import request
from src.api.apiConfig import api
from src.api.fitcrack.endpoints.logs.argumentsParser import newLogs_argument, logs_argument
from src.api.fitcrack.endpoints.logs.functions import log_parser
from src.api.fitcrack.endpoints.logs.responseModels import logsCollection_model
from src.api.fitcrack.functions import shellExec

log = logging.getLogger(__name__)
ns = api.namespace('log', description='Endpoints for log operations.')



@ns.route('')
class logs(Resource):
    @api.expect(logs_argument)
    @api.marshal_with(logsCollection_model)
    def get(self):
        """
        Returns logs.
        """
        args = logs_argument.parse_args(request)
        skip = args.get('skip_count', 0)
        lastLog = args.get('last_log', None)
        while True:
            logs = shellExec('head -n -' + str(skip) + ' /app/log_fitcrack/work_generator.log | tail -n 100')
            if logs == '':
                return {'items': []}
            logs = logs.split('\n')
            results = []
            for log in reversed(logs):
                if log == lastLog:
                    return {'items': results}
                if log.startswith('LOG:') or log.startswith('ERROR:') or log.startswith('WARN:'):
                    results.append(log_parser(log))
            if not lastLog:
                return {'items': results}
            skip += 100







@ns.route('/new')
class logs(Resource):
    @api.expect(newLogs_argument)
    @api.marshal_with(logsCollection_model)
    def get(self):
        """
        Returns new logs.
        """
        args = newLogs_argument.parse_args(request)
        lastLog = args.get('log', 1)
        logs = []
        skip = 0
        while True:
            fewLastLogs = shellExec('head -n -' + str(skip) + ' /app/log_fitcrack/work_generator.log | tail -n 100')
            if fewLastLogs == '':
                return {'items': []}
            fewLastLogs = fewLastLogs.split('\n')
            for log in reversed(fewLastLogs):
                if log.startswith('LOG:') or log.startswith('ERROR:') or log.startswith('WARN:'):
                    if (lastLog == log):
                        return {'items': logs}
                    else:
                        logs.append(log_parser(log))
            skip += 100



# @ns.route('/newCount')
# class logs(Resource):
#     @api.expect(newLogs_argument)
#     def get(self):
#         """
#         vráti nove logy
#         """
#         args = newLogs_argument.parse_args(request)
#         lastLog = args.get('log', 1)
#         fewLastLogs = shellExec('tail -n 100 /app/log_fitcrack/work_generator.log')
#         fewLastLogs = fewLastLogs.split('\n')
#         newLogsCount = 0
#         for log in reversed(fewLastLogs):
#             if (lastLog == log):
#                 return str(newLogsCount)
#             newLogsCount += 1
#
#         return '100+'
