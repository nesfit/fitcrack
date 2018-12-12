'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import reqparse



operation = reqparse.RequestParser()
operation.add_argument('operation', type=str, required=True,  help='operácia čo sa má vykonať na serveri',
                       choices=["start", "stop", "restart"])


