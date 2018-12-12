'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

makeMarkovFromDictionary_parser = reqparse.RequestParser()
makeMarkovFromDictionary_parser.add_argument('dictionary_id', type=int, required=True)
makeMarkovFromDictionary_parser.add_argument('name', type=str, required=True)
