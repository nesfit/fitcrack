'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse

"""Parser for arguments which are used in multiple endpoints"""


pagination = reqparse.RequestParser()
pagination.add_argument('page', type=int, required=False, default=1, help='číslo stránky')
pagination.add_argument('per_page', type=int, required=False, choices=[10, 25, 50, 100],
                        default=25, help='maximálny počet balíkov v odpovedi')
