'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''
import json
from settings import HASHCAT_HASH_INFO_PATH

hash_info_json = None


def loadHashInfoIfNeeded():
  global hash_info_json
  if not hash_info_json:
    with open(HASHCAT_HASH_INFO_PATH) as hash_info:
      hash_info_json = json.load(hash_info)


def getHashNameById(code):
  global hash_info_json
  loadHashInfoIfNeeded()
  str_code = str(code)
  if str_code in hash_info_json:
    return hash_info_json[str_code]['name']

  return "unknown"


def getHashTypes():
  global hash_info_json
  loadHashInfoIfNeeded()
  hashtypes = []

  for h in hash_info_json:
    hashtype = {}
    hashtype['code']     = h
    hashtype['category'] = hash_info_json[h]['category']
    hashtype['name']     = hash_info_json[h]['name']

    hashtypes.append(hashtype)

  return hashtypes
