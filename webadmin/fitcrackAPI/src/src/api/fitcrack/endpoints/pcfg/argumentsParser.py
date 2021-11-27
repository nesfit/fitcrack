'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from src.api.fitcrack.argumentsParser import pagination
from flask_restx import reqparse

pcfg_parser = pagination.copy()
pcfg_parser.add_argument('search', type=str, required=False)

pcfgFromFile_parser = reqparse.RequestParser()
pcfgFromFile_parser.add_argument('files', type=list, required=True, location='json')

makePcfgFromDictionary_parser = reqparse.RequestParser()
makePcfgFromDictionary_parser.add_argument('dictionary_id', type=int, required=True)
makePcfgFromDictionary_parser.add_argument('name', type=str, required=True)
