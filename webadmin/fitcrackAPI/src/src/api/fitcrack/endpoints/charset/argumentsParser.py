'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

updateCharset_parser = reqparse.RequestParser()
updateCharset_parser.add_argument('newCharset', type=str, required=True)
