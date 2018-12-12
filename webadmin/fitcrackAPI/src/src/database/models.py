'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

# coding: utf-8
import base64
import datetime
import math

from flask_login import UserMixin, AnonymousUserMixin
from sqlalchemy import BigInteger, Column, DateTime, Float, Integer, SmallInteger, String, Text, text, ForeignKey, \
    Numeric, func, LargeBinary, select, and_
from sqlalchemy.ext.hybrid import hybrid_property
from sqlalchemy.orm import relationship
from werkzeug.security import check_password_hash, generate_password_hash

from src.api.fitcrack.attacks.hashtypes import getHashById
from src.api.fitcrack.functions import getStringBetween
from src.api.fitcrack.lang import package_status_text_to_code_dict, host_status_text_to_code_dict, \
    package_status_text_info_to_code_dict
from src.database import db

Base = db.Model


class FcBenchmark(Base):
    __tablename__ = 'fc_benchmark'

    id = Column(Integer, primary_key=True)
    boinc_host_id = Column(BigInteger, nullable=False)
    hash_type = Column(Integer)
    power = Column(BigInteger, nullable=False, server_default=text("'0'"))
    last_update = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))

    def as_dict(self):
        return {
            'boinc_host_id': getattr(self, 'boinc_host_id'),
            'hash_type': getattr(self, 'hash_type'),
            'power': getattr(self, 'power'),
            'last_update': getattr(self, 'last_update').isoformat()
        }

class FcHashcache(Base):
    __tablename__ = 'fc_hashcache'

    id = Column(BigInteger, primary_key=True)
    hash_type = Column(Integer)
    hash = Column(String(200, collation='utf8_bin'))
    result = Column(Text(collation='utf8_bin'))
    added = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))

    @hybrid_property
    def hash_type_name(self):
        return getHashById(str(self.hash_type))['name']

    @hybrid_property
    def password(self):
       if self.result:
           return base64.b64decode(self.result).decode("utf-8")
       return None

class FcHost(Base):
    __tablename__ = 'fc_host'

    id = Column(BigInteger, primary_key=True)
    boinc_host_id = Column(ForeignKey('host.id', ondelete='CASCADE', onupdate='CASCADE'), nullable=False)
    power = Column(BigInteger, nullable=False, server_default=text("'0'"))
    job_id = Column(BigInteger, ForeignKey('fc_job.id'), nullable=False)
    status = Column(SmallInteger, nullable=False, server_default=text("'0'"))
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))

    @hybrid_property
    def status_text(self):
        return host_status_text_to_code_dict.get(self.status)

    #job = relationship("FcWorkunit", back_populates="hosts")
    # workunits = relationship("FcWorkunit", back_populates="host")
    boinc_host = relationship("Host", uselist=False)



class FcMask(Base):
    __tablename__ = 'fc_mask'

    id = Column(BigInteger, primary_key=True)
    job_id = Column(BigInteger, ForeignKey('fc_job.id'), nullable=False)
    mask = Column(String(30, 'utf8_bin'))
    current_index = Column(BigInteger, nullable=False)
    keyspace = Column(BigInteger, nullable=False)
    hc_keyspace = Column(BigInteger, nullable=False)

    job = relationship("FcJob", back_populates="masks")

    @hybrid_property
    def progress(self):
        if self.hc_keyspace == 0:
            return 0
        if self.job.status == 1:
            return 100
        return round((self.current_index / self.hc_keyspace) * 100, 2)


class FcDictionary(Base):
    __tablename__ = 'fc_dictionary'

    id = Column(Integer, primary_key=True)
    name = Column(String(100), nullable=False)
    path = Column(String(400), nullable=False)
    keyspace = Column(BigInteger, nullable=False)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))
    modification_time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))


class FcHcstat(Base):
    __tablename__ = 'fc_hcstats'

    id = Column(Integer, primary_key=True)
    name = Column(String(100), nullable=False)
    path = Column(String(400), nullable=False)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))


class FcMasksSet(Base):
    __tablename__ = 'fc_masks_set'

    id = Column(Integer, primary_key=True)
    name = Column(String(100), nullable=False)
    path = Column(String(400), nullable=False)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))


class FcCharset(Base):
    __tablename__ = 'fc_charset'

    id = Column(Integer, primary_key=True)
    name = Column(String(100), nullable=False)
    path = Column(String(400), nullable=False)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))


class FcRule(Base):
    __tablename__ = 'fc_rule'

    id = Column(Integer, primary_key=True)
    name = Column(String(100), nullable=False)
    path = Column(String(400), nullable=False)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))



class FcJobDictionary(Base):
    __tablename__ = 'fc_job_dictionary'

    id = Column(BigInteger, primary_key=True)
    job_id = Column(BigInteger, ForeignKey('fc_job.id'), nullable=False)
    dictionary_id = Column(BigInteger, ForeignKey('fc_dictionary.id'), nullable=False)
    current_index = Column(BigInteger, nullable=False, server_default=text("'0'"))
    is_left = Column(Integer, nullable=False, server_default=text("'1'"))
    dictionary = relationship("FcDictionary")



class FcJob(Base):
    __tablename__ = 'fc_job'

    id = Column(BigInteger, primary_key=True)
    token = Column(String(64, 'utf8_bin'))
    attack = Column(String(20, 'utf8_bin'), nullable=False)
    attack_mode = Column(Integer, nullable=False)
    attack_submode = Column(Integer, nullable=False, server_default=text("'0'"))
    hash_type = Column(Integer, nullable=False)
    hash = Column(String(200, collation='utf8_bin'), nullable=False)
    status = Column(SmallInteger, nullable=False, server_default=text("'0'"))
    result = Column(Text(collation='utf8_bin'))
    keyspace = Column(BigInteger, nullable=False)
    hc_keyspace = Column(BigInteger, nullable=False)
    indexes_verified = Column(BigInteger, nullable=False)
    current_index = Column(BigInteger, nullable=False, server_default=text("'0'"))
    current_index_2 = Column(BigInteger, nullable=False)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    name = Column(Text(collation='utf8_bin'), nullable=False)
    comment = Column(Text(collation='utf8_bin'), nullable=False)
    time_start = Column(DateTime)
    time_end = Column(DateTime)
    cracking_time = Column(Float(asdecimal=True), nullable=False, server_default=text("'0'"))
    seconds_per_workunit = Column(BigInteger, nullable=False, server_default=text("'3600'"))
    config = Column(String(400, collation='utf8_bin'), nullable=False)
    dict1 = Column(String(100, 'utf8_bin'), ForeignKey('fc_dictionary.path'), nullable=False)
    dict2 = Column(String(100, 'utf8_bin'), ForeignKey('fc_dictionary.path'), nullable=False)
    charset1 = Column(String(100, 'utf8_bin'), ForeignKey('fc_charset.name'))
    charset2 = Column(String(100, 'utf8_bin'), ForeignKey('fc_charset.name'))
    charset3 = Column(String(100, 'utf8_bin'), ForeignKey('fc_charset.name'))
    charset4 = Column(String(100, 'utf8_bin'), ForeignKey('fc_charset.name'))
    rules = Column(String(100, 'utf8_bin'), ForeignKey('fc_rule.name'))
    rule_left = Column(String(255, 'utf8_bin'))
    rule_right = Column(String(255, 'utf8_bin'))
    markov_hcstat = Column(String(100, 'utf8_bin'), ForeignKey('fc_hcstats.name'))
    markov_threshold = Column(Integer, nullable=False, server_default=text("'0'"))
    replicate_factor = Column(Integer, nullable=False, server_default=text("'1'"))
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))

    workunits = relationship("FcWorkunit")
    masks = relationship('FcMask')


    charSet1 = relationship("FcCharset",
                            primaryjoin="FcJob.charset1==FcCharset.name")
    charSet2 = relationship("FcCharset",
                            primaryjoin="FcJob.charset2==FcCharset.name")
    charSet3 = relationship("FcCharset",
                            primaryjoin="FcJob.charset3==FcCharset.name")
    charSet4 = relationship("FcCharset",
                            primaryjoin="FcJob.charset4==FcCharset.name")

    rulesFile = relationship("FcRule",
                             primaryjoin="FcJob.rules==FcRule.name")

    markov = relationship("FcHcstat",
                          primaryjoin="FcJob.markov_hcstat==FcHcstat.name")



    hosts = relationship("Host", secondary="fc_host_activity",
                         primaryjoin="FcJob.id == FcHostActivity.job_id",
                         secondaryjoin="FcHostActivity.boinc_host_id == Host.id",
                         viewonly=True)

    left_dictionaries = relationship("FcJobDictionary", primaryjoin=and_(FcJobDictionary.job_id == id, FcJobDictionary.is_left == True))
    right_dictionaries = relationship("FcJobDictionary", primaryjoin=and_(FcJobDictionary.job_id == id, FcJobDictionary.is_left == False))

    @hybrid_property
    def hash_type_name(self):
        return getHashById(str(self.hash_type))['name']

    @hybrid_property
    def cracking_time_str(self):
        try:
            return str(datetime.timedelta(seconds=math.floor(self.cracking_time)))
        except OverflowError:
            return 'really long'


    @hybrid_property
    def progress(self):
        if self.hc_keyspace == 0:
            return 100
        elif self.status == 1:
            return 100
        elif self.status == 2:
            return 100
        elif self.attack_mode == 1:
            return round((self.indexes_verified / self.keyspace) * 100)
        else:
            return round((self.indexes_verified / self.hc_keyspace) * 100)

    @progress.expression
    def progress(cls):
        return func.round((cls.indexes_verified / func.ifnull(cls.hc_keyspace, 0)) * 100)

    @hybrid_property
    def status_text(self):
        return package_status_text_to_code_dict.get(int(self.status))

    @hybrid_property
    def status_tooltip(self):
        return package_status_text_info_to_code_dict.get(int(self.status))

    @hybrid_property
    def status_type(self):
        if int(self.status) == 0 or int(self.status) == 10 or int(self.status) == 12:
            return 'info'
        if int(self.status) == 1:
            return 'success'
        if int(self.status) == 0 or int(self.status) == 4:
            return 'warning'
        if int(self.status) == 1 or int(self.status) == 2 or int(self.status) == 3:
            return 'error'

    hashes = relationship("FcHash", back_populates="job")

class FcSetting(Base):
    __tablename__ = 'fc_settings'

    id = Column(Integer, primary_key=True)
    delete_finished_workunits = Column(Integer, nullable=False, server_default=text("'0'"))
    default_seconds_per_workunit = Column(Integer, nullable=False, server_default=text("'3600'"))
    default_replicate_factor = Column(Integer, nullable=False, server_default=text("'1'"))
    default_verify_hash_format = Column(Integer, nullable=False, server_default=text("'1'"))
    default_check_hashcache = Column(Integer, nullable=False, server_default=text("'1'"))
    default_workunit_timeout_factor = Column(Integer, nullable=False, server_default=text("'2'"))
    default_bench_all = Column(Integer, nullable=False, server_default=text("'1'"))

class FcJobGraph(Base):
    __tablename__ = 'fc_job_graph'

    id = Column(BigInteger, primary_key=True)
    progress = Column(Numeric(4, 2), nullable=False)
    job_id = Column(ForeignKey('fc_job.id'), nullable=False, index=True)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))

    job = relationship('FcJob')

    def as_graph(self):
        return {
            'time': str(getattr(self, 'time')),
            getattr(self.job, 'id'): round(getattr(self, 'progress'))
        }


class Host(Base):
    __tablename__ = 'host'

    id = Column(Integer, primary_key=True)
    create_time = Column(Integer, nullable=False)
    userid = Column(Integer, ForeignKey('user.id'))
    rpc_seqno = Column(Integer, nullable=False)
    rpc_time = Column(Integer, nullable=False)
    total_credit = Column(Float(asdecimal=True), nullable=False, index=True)
    expavg_credit = Column(Float(asdecimal=True), nullable=False, index=True)
    expavg_time = Column(Float(asdecimal=True), nullable=False)
    timezone = Column(Integer, nullable=False)
    domain_name = Column(String(254))
    serialnum = Column(String(254))
    last_ip_addr = Column(String(254))
    nsame_ip_addr = Column(Integer, nullable=False)
    on_frac = Column(Float(asdecimal=True), nullable=False)
    connected_frac = Column(Float(asdecimal=True), nullable=False)
    active_frac = Column(Float(asdecimal=True), nullable=False)
    cpu_efficiency = Column(Float(asdecimal=True), nullable=False)
    duration_correction_factor = Column(Float(asdecimal=True), nullable=False)
    p_ncpus = Column(Integer, nullable=False)
    p_vendor = Column(String(254))
    p_model = Column(String(254))
    p_fpops = Column(Float(asdecimal=True), nullable=False)
    p_iops = Column(Float(asdecimal=True), nullable=False)
    p_membw = Column(Float(asdecimal=True), nullable=False)
    os_name = Column(String(254))
    os_version = Column(String(254))
    m_nbytes = Column(Float(asdecimal=True), nullable=False)
    m_cache = Column(Float(asdecimal=True), nullable=False)
    m_swap = Column(Float(asdecimal=True), nullable=False)
    d_total = Column(Float(asdecimal=True), nullable=False)
    d_free = Column(Float(asdecimal=True), nullable=False)
    d_boinc_used_total = Column(Float(asdecimal=True), nullable=False)
    d_boinc_used_project = Column(Float(asdecimal=True), nullable=False)
    d_boinc_max = Column(Float(asdecimal=True), nullable=False)
    n_bwup = Column(Float(asdecimal=True), nullable=False)
    n_bwdown = Column(Float(asdecimal=True), nullable=False)
    credit_per_cpu_sec = Column(Float(asdecimal=True), nullable=False)
    venue = Column(String(254), nullable=False)
    nresults_today = Column(Integer, nullable=False)
    avg_turnaround = Column(Float(asdecimal=True), nullable=False)
    host_cpid = Column(String(254))
    external_ip_addr = Column(String(254))
    max_results_day = Column(Integer, nullable=False)
    error_rate = Column(Float(asdecimal=True), nullable=False, server_default=text("'0'"))
    product_name = Column(String(254), nullable=False)
    gpu_active_frac = Column(Float(asdecimal=True), nullable=False)
    p_ngpus = Column(Integer, nullable=False)
    p_gpu_fpops = Column(Float(asdecimal=True), nullable=False)

    #workunits = relationship("FcWorkunit", back_populates="host", order_by="desc(FcWorkunit.id)")
    #fc_host = relationship("FcHost", uselist=False, back_populates="boinc_host")
    user = relationship("User", back_populates="hosts")

    workunits = relationship("FcWorkunit", secondary="fc_host_activity",
                        primaryjoin="Host.id == FcHostActivity.boinc_host_id",
                        secondaryjoin="FcHostActivity.job_id == FcWorkunit.id",
                        viewonly=True, order_by="desc(FcWorkunit.id)")

    last_active = relationship("FcHostStatus", uselist=False)

    #job = relationship("FcWorkunit", back_populates="hosts")

    @hybrid_property
    def deleted(self):
        return True if \
            FcHostStatus.query.filter(FcHostStatus.boinc_host_id == self.id, FcHostStatus.deleted == True).first() \
            else False


    @deleted.expression
    def deleted(cls):
        return select([FcHostStatus.deleted]).where(and_(cls.id == FcHostStatus.boinc_host_id, FcHostStatus.deleted == 1)).as_scalar()


class FcWorkunit(Base):
    __tablename__ = 'fc_workunit'

    id = Column(BigInteger, primary_key=True)
    job_id = Column(BigInteger, ForeignKey('fc_job.id'), nullable=False)
    workunit_id = Column(BigInteger, nullable=False)
    host_id = Column(BigInteger, ForeignKey('fc_host.id'), nullable=False)
    boinc_host_id = Column(Integer, ForeignKey('host.id'), nullable=False)
    start_index = Column(BigInteger, nullable=False)
    start_index_2 = Column(BigInteger, nullable=False)
    hc_keyspace = Column(BigInteger, nullable=False)
    progress = Column(Float(asdecimal=True), nullable=False, server_default=text("'0'"))
    mask_id = Column(BigInteger, nullable=False)
    dictionary_id = Column(BigInteger, nullable=False)
    duplicated = Column(Integer, nullable=False, server_default=text("'0'"))
    duplicate = Column(BigInteger, nullable=False, server_default=text("'0'"))
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    cracking_time = Column(Float(asdecimal=True), nullable=False, server_default=text("'0'"))
    retry = Column(Integer, nullable=False, server_default=text("'0'"))
    finished = Column(Integer, nullable=False, server_default=text("'0'"))

    job = relationship("FcJob", back_populates="workunits")
    # host = relationship("FcHost", back_populates="workunits")
    host = relationship("Host", back_populates="workunits")

    result = relationship('Result',  uselist=False)

    def as_graph(self):
        return {
            'time': str(getattr(self, 'time')),
            str(getattr(self, 'boinc_host_id'))
            + '_' + str(getattr(self, 'job_id')): int(getattr(self, 'hc_keyspace'))
        }

    @hybrid_property
    def cracking_time_str(self):
        return str(datetime.timedelta(seconds=math.floor(self.cracking_time)))



class FcHostActivity(Base):
    __tablename__ = 'fc_host_activity'

    id = Column(BigInteger, primary_key=True)
    boinc_host_id = Column(Integer, ForeignKey('host.id'), nullable=False)
    job_id = Column(BigInteger, ForeignKey('fc_job.id'), nullable=False)

    boinc_host = relationship("Host")

    @hybrid_property
    def host(self):
        return self.boinc_host.fc_host




class FcNotification(Base):
    __tablename__ = 'fc_notification'

    id = Column(Integer, primary_key=True)
    user_id = Column(ForeignKey('fc_user.id', ondelete='CASCADE', onupdate='CASCADE'), index=True)
    source_type = Column(Integer, server_default=text("'0'"))
    source_id = Column(ForeignKey('fc_job.id', ondelete='CASCADE', onupdate='CASCADE'), index=True)
    old_value = Column(SmallInteger)
    new_value = Column(SmallInteger)
    seen = Column(Integer, server_default=text("'0'"))
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))

    source = relationship('FcJob')
    user = relationship('FcUser')


class FcRole(Base):
    __tablename__ = 'fc_role'

    id = Column(Integer, primary_key=True)
    name = Column(String(50), nullable=False)
    MANAGE_USERS = Column(Integer, nullable=False, server_default=text("'0'"))
    ADD_NEW_JOB = Column(Integer, nullable=False, server_default=text("'0'"))
    UPLOAD_DICTIONARIES = Column(Integer, nullable=False, server_default=text("'0'"))
    VIEW_ALL_JOBS = Column(Integer, nullable=False, server_default=text("'0'"))
    EDIT_ALL_JOBS = Column(Integer, nullable=False, server_default=text("'0'"))
    OPERATE_ALL_JOBS = Column(Integer, nullable=False, server_default=text("'0'"))
    ADD_USER_PERMISSIONS_TO_JOB = Column(Integer, nullable=False, server_default=text("'0'"))


class FcUser(UserMixin, Base):
    __tablename__ = 'fc_user'

    id = Column(Integer, primary_key=True)
    username = Column(String(64), nullable=False)
    password = Column(String(128), nullable=False)
    mail = Column(String(200), nullable=False)
    role_id = Column(ForeignKey('fc_role.id'), nullable=False, index=True)
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))

    def set_password(self, password):
        setattr(self, 'password', generate_password_hash(password))

    def check_password(self, password):
        return check_password_hash(getattr(self, 'password'), password)

    role = relationship('FcRole')

    def as_dict(self):
        return {
            'id': getattr(self, 'id'),
            'username': getattr(self, 'login'),
            'mail': getattr(self, 'mail'),
            'role': getattr(self, 'role')
        }

    stuffs = []

    def addStuff(self, stuff):
        self.stuffs.append(stuff)


class AnonUser(AnonymousUserMixin):
    id = 0
    username = 'Anon'
    stuffs = []

    def addStuff(self, stuff):
        self.stuffs.append(stuff)


class FcUserPermission(Base):
    __tablename__ = 'fc_user_permissions'

    id = Column(Integer, primary_key=True)
    job_id = Column(ForeignKey('fc_job.id', ondelete='CASCADE', onupdate='CASCADE'), nullable=False, index=True)
    user_id = Column(ForeignKey('fc_user.id', ondelete='CASCADE', onupdate='CASCADE'), nullable=False, index=True)
    view = Column(Integer, nullable=False, server_default=text("'0'"))
    modify = Column(Integer, nullable=False, server_default=text("'0'"))
    operate = Column(Integer, nullable=False, server_default=text("'0'"))

    job = relationship('FcJob')
    user = relationship('FcUser')


class User(Base):
    __tablename__ = 'user'

    id = Column(Integer, primary_key=True)
    create_time = Column(Integer, nullable=False)
    email_addr = Column(String(254), nullable=False, unique=True)
    name = Column(String(254), index=True)
    authenticator = Column(String(254), unique=True)
    country = Column(String(254))
    postal_code = Column(String(254))
    total_credit = Column(Float(asdecimal=True), nullable=False, index=True)
    expavg_credit = Column(Float(asdecimal=True), nullable=False, index=True)
    expavg_time = Column(Float(asdecimal=True), nullable=False)
    global_prefs = Column(LargeBinary)
    project_prefs = Column(LargeBinary)
    teamid = Column(Integer, nullable=False, index=True)
    venue = Column(String(254), nullable=False)
    url = Column(String(254))
    send_email = Column(SmallInteger, nullable=False)
    show_hosts = Column(SmallInteger, nullable=False)
    posts = Column(SmallInteger, nullable=False)
    seti_id = Column(Integer, nullable=False)
    seti_nresults = Column(Integer, nullable=False)
    seti_last_result_time = Column(Integer, nullable=False)
    seti_total_cpu = Column(Float(asdecimal=True), nullable=False)
    signature = Column(String(254))
    has_profile = Column(SmallInteger, nullable=False)
    cross_project_id = Column(String(254), nullable=False)
    passwd_hash = Column(String(254), nullable=False)
    email_validated = Column(SmallInteger, nullable=False)
    donated = Column(SmallInteger, nullable=False)
    login_token = Column(String(32), nullable=False)
    login_token_time = Column(Float(asdecimal=True), nullable=False)

    hosts = relationship('Host')


class FcEncryptedFile(Base):
    __tablename__ = 'fc_protected_file'

    id = Column(Integer, primary_key=True)
    name = Column(String(100), nullable=False)
    path = Column(String(400), nullable=False)
    hash = Column(String(1000), nullable=False)
    hash_type = Column(String(50), nullable=False)
    time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))

    @hybrid_property
    def hash_type_name(self):
        return getHashById(str(self.hash_type))['name']



class FcHostStatus(Base):
    __tablename__ = 'fc_host_status'

    id = Column(BigInteger, primary_key=True)
    boinc_host_id = Column(Integer, ForeignKey(Host.id), nullable=False)
    last_seen = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    deleted = Column(Integer, nullable=False, server_default=text("'0'"))

    @hybrid_property
    def seconds_delta(self):
        # TODO: Overflow?
        now = datetime.datetime.utcnow()
        then = self.last_seen
        delta = now - then
        total_seconds = math.floor(delta.total_seconds())
        return str(total_seconds)

    @hybrid_property
    def online(self):
        now = datetime.datetime.utcnow()
        then = self.last_seen
        delta = now - then
        total_seconds = math.floor(delta.total_seconds())
        return True if total_seconds <= 60 else False



class FcHash(Base):
    __tablename__ = 'fc_hash'

    id = Column(BigInteger, primary_key=True)
    job_id = Column(BigInteger, ForeignKey(FcJob.id), nullable=False)
    hash_type = Column(Integer, nullable=False)
    hash = Column(LargeBinary, nullable=False)
    result = Column(String(400, collation='utf8_bin'))
    added = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP"))
    time_cracked = Column(DateTime)

    job = relationship("FcJob", back_populates="hashes")


    @hybrid_property
    def hashText(self):
        try:
            return self.hash.decode("utf-8")
        except UnicodeDecodeError:
            return 'BASE64:' + base64.encodebytes(self.hash).decode("utf-8")

    @hybrid_property
    def password(self):
        try:
            if self.result:
                return bytes.fromhex(self.result.rstrip()).decode('utf-8')
            else:
                return None
        except (UnicodeDecodeError, ValueError) as e:
            return self.result

    @hybrid_property
    def hash_type_name(self):
        return getHashById(str(self.hash_type))['name']


class Result(Base):
    __tablename__ = 'result'


    id = Column(Integer, primary_key=True)
    create_time = Column(Integer, nullable=False)
    workunitid = Column(Integer, ForeignKey('fc_workunit.workunit_id'))
    server_state = Column(Integer, nullable=False)
    outcome = Column(Integer, nullable=False)
    client_state = Column(Integer, nullable=False)
    hostid = Column(Integer, nullable=False)
    userid = Column(Integer, nullable=False)
    report_deadline = Column(Integer, nullable=False)
    sent_time = Column(Integer, nullable=False)
    received_time = Column(Integer, nullable=False)
    name = Column(String(254), nullable=False, unique=True)
    cpu_time = Column(Float(asdecimal=True), nullable=False)
    xml_doc_in = Column(LargeBinary)
    xml_doc_out = Column(LargeBinary)
    stderr_out = Column(LargeBinary)
    batch = Column(Integer, nullable=False)
    file_delete_state = Column(Integer, nullable=False, index=True)
    validate_state = Column(Integer, nullable=False)
    claimed_credit = Column(Float(asdecimal=True), nullable=False)
    granted_credit = Column(Float(asdecimal=True), nullable=False)
    opaque = Column(Float(asdecimal=True), nullable=False)
    random = Column(Integer, nullable=False)
    app_version_num = Column(Integer, nullable=False)
    appid = Column(Integer, nullable=False)
    exit_status = Column(Integer, nullable=False)
    teamid = Column(Integer, nullable=False)
    priority = Column(Integer, nullable=False)
    mod_time = Column(DateTime, nullable=False, server_default=text("CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP"))
    elapsed_time = Column(Float(asdecimal=True), nullable=False)
    flops_estimate = Column(Float(asdecimal=True), nullable=False)
    app_version_id = Column(Integer, nullable=False)
    runtime_outlier = Column(Integer, nullable=False)
    size_class = Column(SmallInteger, nullable=False, server_default=text("'-1'"))
    peak_working_set_size = Column(Float(asdecimal=True), nullable=False)
    peak_swap_size = Column(Float(asdecimal=True), nullable=False)
    peak_disk_usage = Column(Float(asdecimal=True), nullable=False)

    @hybrid_property
    def stderr_out_text(self):
        try:
            return getStringBetween(self.stderr_out.decode("utf-8"), '<stderr_txt>', '</stderr_txt>' )
        except:
            return self.stderr_out.decode("utf-8")


