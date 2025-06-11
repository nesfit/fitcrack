'''
   * Author : David Brandšteter (xbrand13)
   * Licence: MIT, see LICENSE
'''

from flask_restx import reqparse, inputs

def list_of_ints(arg):
    if not arg:
        return
    else:
        return list(map(int, arg.split(',')))

planner_parser = reqparse.RequestParser()
planner_parser.add_argument('days', type=int, required=True)
planner_parser.add_argument('hours', type=int, required=True)
planner_parser.add_argument('minutes', type=int, required=True)
planner_parser.add_argument('cracking_speed', type=int, required=True)

planner_parser.add_argument('fill', type=inputs.boolean, required=True)

planner_parser.add_argument('name', type=str, required=True, nullable=False)
planner_parser.add_argument('hashList', type=int, required=True)
planner_parser.add_argument('hostsIds', type=list_of_ints, required=False)
planner_parser.add_argument('password_len_min', type=int, required=False)
planner_parser.add_argument('password_len_max', type=int, required=False)
