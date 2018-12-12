'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

directory_parser = reqparse.RequestParser()
directory_parser.add_argument('path', type=str, required=True)
