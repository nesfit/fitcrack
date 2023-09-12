'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

"""response models which are used in multiple endpoints"""

from flask_restx import fields

from src.api.apiConfig import api

pagination = api.model('A page of results', {
    'page': fields.Integer(description='Number of this page of results'),
    'pages': fields.Integer(description='Total number of pages of results'),
    'per_page': fields.Integer(description='Number of items per page of results'),
    'total': fields.Integer(description='Total number of results'),
})

simpleResponse = api.model('simpleResponse', {
    'status': fields.Boolean(description="True if success, False if failed."),
    'message': fields.String(description="Some details...")
})

file_content = api.model('File info and content data', {
    'name': fields.String(),
    'path': fields.String(),
    'data': fields.String()
})

host_short_model = api.model('Host short', {
    'id': fields.Integer(readOnly=True, required=False),
    'boinc_host_id': fields.Integer(readOnly=True, required=False),
    'power': fields.Integer(readOnly=True, required=False),
    'job_id': fields.Integer(readOnly=True, required=False),
    'status': fields.Integer(readOnly=True, required=False),
    'status_text': fields.String(),
    'time': fields.DateTime(readOnly=True, required=False),
})

user_model = api.model('Fc User', {
    'name': fields.String(),
})


boincHostActivity_model = api.model('Host activity', {
    'last_seen': fields.String(),
    'seconds_delta': fields.String(),
    'online': fields.Boolean()
})

job_permissions = api.model('Job permissions object', {
    'view': fields.Boolean(),
    'edit': fields.Boolean(),
    'operate': fields.Boolean(),
    'owner': fields.Boolean()
})

job_nano_model = api.model('Job nano', {
    'id': fields.Integer(readOnly=True, required=False, description='id of the job'),
    'name': fields.String(required=True, description='job name'),
    'status': fields.Integer(readOnly=True, required=False, description='state of the job'),
})

device_info_model = api.model('Compute device info log', {
    # id = Column(BigInteger, primary_key=True)
    'id': fields.Integer(readOnly=True, required=False, description='id of the log'),
    # device_id = Column(BigInteger, ForeignKey('fc_device.id'), nullable=False)
    # workunit_id = Column(BigInteger, ForeignKey('fc_workunit.id'), nullable=False)
    'workunit_id': fields.Integer(readOnly=True, required=False, description='id of the workunit being processed'),
    # speed = Column(BigInteger, nullable=False)
    # temperature = Column(Integer, nullable=False)
    # utilization = Column(Integer, nullable=False)
    'speed': fields.Integer(readOnly=True, required=False, description='computing speed'),
    'temperature': fields.Integer(readOnly=True, required=False, description='device temperature'),
    'utilization': fields.Integer(readOnly=True, required=False, description='device utilization'),
    # time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    'time': fields.DateTime()
})

device_model = api.model('Compute device', {
    # id = Column(BigInteger, primary_key=True)
    'id': fields.Integer(readOnly=True, required=False, description='id of the device'),
    # boinc_host_id = Column(BigInteger, ForeignKey('host.id'), nullable=False)
    # hc_id = Column(BigInteger, nullable=False)
    'hc_id': fields.Integer(readOnly=True, required=False, description='hashcat device identifier'),
    # name = Column(String(255), nullable=False)
    'name': fields.String(description='device name'),
    # type = Column(String(255), nullable=False)
    'type': fields.String(description='device type'),

    # host = relationship("Host")
    ###'host': fields.Nested(host_short_model),
    # device_info = relationship("FcDeviceInfo", back_populates="device")
    'device_info': fields.List(fields.Nested(device_info_model))
})

boincHost_model = api.model('Host boinc', {
    'id': fields.Integer(readOnly=True, required=False),
    'domain_name': fields.String(),
    'ip_address': fields.String(attribute='last_ip_addr'),
    'p_model': fields.String(),
    'os_name': fields.String(),
    'user': fields.Nested(user_model),
    'last_active': fields.Nested(boincHostActivity_model),
    'deleted': fields.Boolean(),
    'jobs': fields.Nested(job_nano_model)
})

boincResult_model = api.model('boinc result', {
    'id': fields.String(),
    'stderr_out_text': fields.String(),
})

workunit_model = api.model('Workunit', {
    'id': fields.String(),
    'job_id': fields.Integer(),
    'workunit_id': fields.Integer(),
    'host_id': fields.Integer(),
    'boinc_host_id': fields.Integer(),
    'start_index': fields.Integer(),
    'start_index_2': fields.Integer(),
    'start_index_real': fields.Integer(),
    'hc_keyspace': fields.Integer(),
    'keyspace': fields.Integer(),
    'mask_id': fields.Integer(),
    'duplicated': fields.Boolean(),
    'duplicate': fields.Integer(),
    'time': fields.DateTime(),
    'cracking_time': fields.Integer(),
    'cracking_time_str': fields.String(),
    'retry': fields.Boolean(),
    'finished': fields.Boolean(),
    'host': fields.Nested(boincHost_model),
    'result': fields.Nested(boincResult_model),
    'progress': fields.Integer(),
    'speed': fields.Integer()
})

job_short_model = api.model('Job short', {
    'id': fields.Integer(readOnly=True, required=False, description='id job'),
    'name': fields.String(required=True, description='name of the job'),
    'comment': fields.String(required=False),
    'permissions': fields.Nested(job_permissions),
    'batch_id': fields.Integer(), # to know if job is enqueued and where
    'queue_position': fields.Integer(),
    'attack_mode': fields.String(required=True),
    'attack': fields.String(required=True),
    'host_count': fields.Integer(),
    'keyspace': fields.String(required=True),
    'hc_keyspace': fields.String(required=True),
    'status': fields.String(required=False),
    'status_text': fields.String(required=False),
    'status_tooltip': fields.String(required=False),
    'status_type': fields.String(),
    'progress': fields.Float(required=False),
    'time': fields.DateTime(required=False),
    'workunit_sum_time': fields.Float(),
    'hash_type': fields.String(required=True),
    'time_start': fields.DateTime(required=True),
    'time_end': fields.DateTime(required=True),
    'deleted': fields.Boolean()
})

job_micro_model = api.model('Job micro', {
    'id': fields.Integer(readOnly=True, required=False, description='job id'),
    'name': fields.String(required=True, description='job name'),
    'permissions': fields.Nested(job_permissions),
    'queue_position': fields.Integer(),
    'attack': fields.String(),
    'status': fields.String(required=False),
    'status_text': fields.String(required=False),
    'status_tooltip': fields.String(required=False),
    'status_type': fields.String(),
    'progress': fields.Float(required=False),
})