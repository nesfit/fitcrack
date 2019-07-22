'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from src.api.fitcrack.argumentsParser import pagination
from flask_restplus import reqparse

pcfg_parser = pagination.copy()
pcfg_parser.add_argument('search', type=str, required=False)

pcfgFromFile_parser = reqparse.RequestParser()
pcfgFromFile_parser.add_argument('files', type=list, required=True, location='json')

#dictionary_parser -> pcfg_parser
#dictionaryFromFile_parser -> pcfgFromFile_parser
