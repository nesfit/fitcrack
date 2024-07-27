import platform, time
from typing import Any
import msgpack

from flask_login import current_user
from src.database import db
from sqlalchemy import exc

from src.database.models import FcJob, FcUserPermission
from src.database.models import FcHash, FcMask # for (un)packing directly
from src.database.models import FcRule, FcHcstat, FcDictionary, FcJobDictionary, FcPcfg # for dependencies
from src.database.models import FcHashList


# scalar or relationship fields that get copied over
JOB_EXPORTABLE_COLUMNS = (
  'attack', 'attack_mode', 'attack_submode', 'distribution_mode', 'hash_type',
  'keyspace', 'hc_keyspace', 'name', 'comment', 'seconds_per_workunit', 
  'charset1', 'charset2', 'charset3', 'charset4', 'rule_left', 'rule_right', 
  'markov_threshold', 'case_permute', 'check_duplicates', 
  'min_password_len', 'max_password_len', 'min_elem_in_chain', 
  'max_elem_in_chain', 'generate_random_rules', 'optimized', 'deleted', 'masks',
  'hash_list_id'
)
# relationship fields that get stored as dependencies
JOB_EXPORTABLE_DEPENDENCIES = {
  # as {type}: {value to store for identification}
  'rulesFile': 'name',
  'markov': 'name',
  'pcfg': 'name',
  'left_dictionaries': 'dictionary.name',
  'right_dictionaries': 'dictionary.name'
}
# mapping of dependency names from db columns to tables
DEP_MAP = {
  'rulesFile': 'rule',
  'markov': 'markov',
  'pcfg': 'pcfg',
  'left_dictionaries': 'dictionary',
  'right_dictionaries': 'dictionary'
}
# maps dep type annotations to actual sqlalchemy model classes and columns
ORM_MAP = {
  'rule': (FcRule, 'name'),
  'markov': (FcHcstat, 'name'),
  'pcfg': (FcPcfg, 'name'),
  'dictionary': (FcDictionary, 'name')
}

# Example for this craziness:
# ---------------------------
#
# EXPORTING:
# JOB_EXPORTABLE_DEPENDENCIES says for FcJob column left_dictionaries
# to access (if it is a list then for each element) dictionary.name
# and store its value in the dependency list. This is stored in the form
# 'type/value', type defined by DEP_MAP, ex. here 'dictionary/english.txt'.
# Jobs stored in the export reference these dependencies by their index
# in the dependency list.
#
# Ex: If job requires dicts english.txt and bible.txt, and the dep list is
# ['rule/best64.rule', 'dictionary/bible.txt', 'dictionary/english.txt']
# then the stored job will have 'left_dictionaries': [2, 1]
#
# IMPORTING:
# The function dependency_check uses ORM_MAP to find out which model class
# corresponds to the stored dependecny by its type (dictionary -> FcDictionary)
# and what column to search for the stored value (dictionary -> name). It then
# tries to find and return all the actual dependencies from DB or raises error.
#
#
# EDITS TO MAKE THIS WORK WITH HASH LISTS:
# Now that hash lists are a thing, the transfer function had to change a bit.
# In addition to jobs and dependencies, a new field titled 'hash_lists' is exported.
# This is a mapping from hash-list id (integer; the same id as stored in the database)
# to a serialized hash list. Instead of including a list of hashes with each job, we now
# store just the hash-list id. Of course, we only serialize those hash lists, that are referenced
# from at least one job. This is about it for exporting, but importing poses some challenges.
# In the import process, we first handle hash lists. For convenience to the use, we check whether
# the imported hash list already exists on the server. To that end, first we check whether
# a hash list with the same name already exists. Then we check whether it contains exactly the same
# hashes as the imported hash list. If yes, we don't create a new hash list on the server, but we
# just link to the pre-existing one. We do this by using an import id to server id mapping.
# We just save the hash-list id from the import file corresponds to the hash list id in the server DB.
# If we have to create a new hash list, we do that, but we also save its server id to the mapping.
# Finally, when adding the job, we change out the hash-list id from the one from the import to the
# new one from the mapping.


class JobSerializer:
  """Creates a list of job dicts and a list of dependencies"""
  jobs = [] # jobs as dicts, referencing deps by index
  dependencies = [] # deps as '{type}/{identity value}' (type as per DEP_MAP)
  hash_lists = {} # mapping of hash list id to hash list

  def __init__(self):
    # initialize job list (deps and hash lists can stay cached)
    self.jobs = []

  def add (self, job:FcJob):
    """Adds an FcJob record to the list"""
    result = {}
    # copy columns
    for field in JOB_EXPORTABLE_COLUMNS:
      result[field] = getattr(job, field)
    # add the hash list of the job to the list of hash lists to be serialized, but only if we don't already have the hash list saved
    if not job.hash_list_id in self.hash_lists:
      self.hash_lists[job.hash_list_id] = job.hash_list
    # record deps
    for dep in JOB_EXPORTABLE_DEPENDENCIES:
      dep_list = []
      # get the path to desired value for this dep type
      path = JOB_EXPORTABLE_DEPENDENCIES[dep].split('.')
      # read data
      data = getattr(job, dep)
      # if missing, skip
      if not data or (isinstance(data, list) and len(data) == 0):
        continue
      # normalize to list
      if not isinstance(data, list):
        data = [data]
      # iterate over the list
      for val in data:
        # descend nested values by the path from earlier
        for key in path:
          val = val[key] if isinstance(val, dict) else getattr(val, key)
        tv = '{}/{}'.format(DEP_MAP[dep], val)
        # check if this dep is already recorded, if not, add it
        try:
          dep_index = self.dependencies.index(tv)
        except ValueError:
          self.dependencies.append(tv)
          dep_index = len(self.dependencies) - 1
        # append the index of the dependency to the current dep in the job
        dep_list.append(dep_index)
      result[dep] = dep_list
    self.jobs.append(result)


def orm_pack (obj):
  """Describes to msgpack how to serialize some ORM objects"""
  if isinstance(obj, FcHash):
    obj = obj.hash
  if isinstance(obj, FcMask):
    obj = {
      'mask': obj.mask,
      'keyspace': obj.keyspace,
      'hc_keyspace': obj.hc_keyspace
    }
  if isinstance(obj, FcHashList):
    obj = {
      'hash_type' : obj.hash_type,
      'name' : obj.name,
      'hashes' : obj.hashes
    }
  return obj

def pack (**options):
  """
  Packs system data to portable package file
  """
  # options handling
  job_ids = options.get('jobs')

  # preparation
  package = {
    'packager': platform.node(),
    'timestamp': time.time()
  }

  # reading
  query = FcJob.query.filter_by(deleted=False)
  if job_ids:
    query = query.filter(FcJob.id.in_(job_ids))
  joblist = query.all()

  # transforming
  js = JobSerializer()

  for job in joblist:
    js.add(job)

  package['deps'] = js.dependencies
  package['jobs'] = js.jobs
  package['hash_lists'] = js.hash_lists

  # packing
  packer = msgpack.Packer(use_bin_type=True, default=orm_pack)
  yield packer.pack(package)


class ImportDependencyMissing (Exception):
  """Raised when imported package references unmet dependencies"""

  def __init__(self, missing_deps):
    self.missing = missing_deps

def recreate_hash_list(hash_list) -> FcHashList:
  hash_type = hash_list['hash_type']
  new_hash_list = FcHashList(hash_type=hash_type, name=hash_list['name'])
  hashes_list = []
  for hash in hash_list['hashes']:
    hashes_list.append(FcHash(hash=hash, hash_type=hash_type))
  new_hash_list.hashes = hashes_list
  return new_hash_list

def recreate_mask (data):
  return FcMask(mask=data['mask'], keyspace=data['keyspace'], hc_keyspace=data['hc_keyspace'], current_index=0)

def unpack (package):
  """
  Unpacks system data from exported package and saves to DB
  """
  contents = msgpack.unpack(package,strict_map_key=False)
  # load deps and check for missing
  deps, missing = dependency_check(contents['deps'])
  if len(missing) > 0:
    raise ImportDependencyMissing(missing)
  # start checking for pre-existing hash lists and creating them if need be
  hash_lists_to_add, hash_list_mapping = deduplicate_hash_lists(contents['hash_lists'])
  for msg_id, hash_list in hash_lists_to_add.items():
    new_hash_list = recreate_hash_list(hash_list)
    db.session.add(new_hash_list)
    hash_list_mapping[msg_id] = new_hash_list
  db.session.flush()
  for msg_id in hash_lists_to_add.keys():
    hash_list_mapping[msg_id] = hash_list_mapping[msg_id].id
  # start creating jobs
  for job in contents['jobs']:
    #connect the job to the proper hash list
    job['hash_list_id'] = hash_list_mapping[job['hash_list_id']]
    # transform directly stored object back
    if job.get('masks'):
      job['masks'] = list(map(recreate_mask, job['masks']))
    newjob = FcJob()
    for field in JOB_EXPORTABLE_COLUMNS:
      setattr(newjob, field, job.get(field))
    # manual labor now
    #
    # !!! If you added new DEPS to export, make sure to unpack them like these !!!
    #
    dep_rule = job.get('rulesFile')
    if dep_rule:
      newjob.rulesFile = deps[dep_rule[0]]
    dep_markov = job.get('markov')
    if dep_markov:
      newjob.markov = deps[dep_markov[0]]
    dep_left_dictionaries = job.get('left_dictionaries')
    if dep_left_dictionaries:
      for index in dep_left_dictionaries:
        jd = FcJobDictionary(is_left=1, dictionary=deps[index])
        newjob.left_dictionaries.append(jd)
    dep_right_dictionaries = job.get('right_dictionaries')
    if dep_right_dictionaries:
      for index in dep_right_dictionaries:
        jd = FcJobDictionary(is_left=0, dictionary=deps[index])
        newjob.right_dictionaries.append(jd)
    dep_pcfg = job.get('pcfg')
    if dep_pcfg:
      newjob.pcfg = deps[dep_pcfg[0]]
    # adding values for useless non-null fields
    newjob.indexes_verified = 0
    newjob.current_index_2 = 0
    # add owner
    newjob.permission_records.append(FcUserPermission(user_id=current_user.id, view=1, modify=1, operate=1, owner=1))
    # save
    db.session.add(newjob)
  # end for loop over jobs
  try:
    db.session.commit()
  except exc.IntegrityError as e:
    db.session().rollback()
    raise e
    

def dependency_check (deps):
  missing = []
  records = []
  for dep in deps:
    dtype, dvalue = dep.split('/')
    dcls, col = ORM_MAP[dtype]
    # try to find by value
    dv = {col: dvalue}
    found = dcls.query.filter_by(**dv).filter_by(deleted=False).first()
    if not found:
      missing.append(dep)
    records.append(found)
  return records, missing


def deduplicate_hash_lists(hash_lists:dict[int,dict[str,Any]]):
  new_hash_lists : dict[int,Any] = dict()
  existing_hash_list_mapping : dict[int,int] = dict()
  for msg_id, hash_list in hash_lists.items():
    possible_duplicate = FcHashList.query.filter_by(name=hash_list['name']).filter_by(deleted=False).first()
    if not possible_duplicate:
      new_hash_lists[msg_id] = hash_list
      continue
    set_of_incoming_hashes = set(hash_list['hashes'])
    set_of_hashes_in_possible_duplicate = set(x.hash for x in possible_duplicate.hashes)
    if set_of_incoming_hashes != set_of_hashes_in_possible_duplicate:
      hash_list['name'] = hash_list['name'] + ' (imported from transfer)'
      new_hash_lists[msg_id] = hash_list
      continue
    existing_hash_list_mapping[msg_id] = possible_duplicate.id
  return new_hash_lists, existing_hash_list_mapping
