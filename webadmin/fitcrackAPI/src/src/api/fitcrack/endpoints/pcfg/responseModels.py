'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus import fields
from src.api.apiConfig import api

pcfg_model = api.model('PCFG', {
    'id': fields.Integer(readOnly=True, required=False),
    'name': fields.String(readOnly=True, required=False),
    'keyspace': fields.Integer(readOnly=True, required=False),
    'time': fields.DateTime(readOnly=True, required=False),
})

'''
id = Column(Integer, primary_key=True)
name = Column(String(100), nullable=False)
path = Column(String(400), nullable=False)
keyspace = Column(BigInteger, nullable=False)
time_added = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
deleted = Column(Integer, nullable=False, server_default=text("'0'"))
modification_time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
'''

pcfgs_model = api.inherit('PCFGs collection', {
    'items': fields.List(fields.Nested(pcfg_model))
})

pcfgData_model = api.inherit('PCFG with data', {
    'data': fields.String(),
    'status': fields.Boolean()
})
