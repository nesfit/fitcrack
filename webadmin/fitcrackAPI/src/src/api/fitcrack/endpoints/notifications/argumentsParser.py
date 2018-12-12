'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from src.api.fitcrack.argumentsParser import pagination

notifications_parser = pagination.copy()
notifications_parser.add_argument('seen', type=bool, help='mark notifications as seen', required=False, default=True)
