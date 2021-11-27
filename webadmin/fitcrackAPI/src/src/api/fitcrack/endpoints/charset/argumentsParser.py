'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse

updateCharset_parser = reqparse.RequestParser()
updateCharset_parser.add_argument('newCharset', type=str, required=True)
