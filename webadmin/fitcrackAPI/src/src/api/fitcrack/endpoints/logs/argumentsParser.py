'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from datetime import datetime

from flask_restplus import reqparse

newLogs_argument = reqparse.RequestParser()
newLogs_argument.add_argument('log', type=str, required=True)


logs_argument = reqparse.RequestParser()
logs_argument.add_argument('skip_count', type=int, required=False)
logs_argument.add_argument('last_log', type=str, required=False)