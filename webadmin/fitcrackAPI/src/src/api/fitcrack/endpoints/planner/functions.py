'''
   * Author : David Brandšteter (xbrand13)
   * Licence: MIT, see LICENSE
'''

#Makes a data template for create_job() method in job endpoint
def create_job_template():
  data = {
      "name": "",
      "comment": "",
      "hosts_ids": [],
      "seconds_per_job": 3600,
      "time_start": "",
      "time_end": "",
      "attack_settings": {
      "attack_mode": 0,
      "attack_name": "dict",
      "rules": {
        "id": None,
        "name": None,
        "count": None,
        "time": None
      },
      "left_dictionaries": [],
      "right_dictionaries": [],
      "rule_left": "",
      "rule_right": "",
      "masks": [
        ""
      ],
      "attack_submode": 0,
      "distribution_mode": 0,
      "markov_treshold": None,
      "markov": None,
      "charset": [],
      "mask": "",
      "keyspace_limit": 0,
      "check_duplicates": True,
      "case_permute": False,
      "min_password_len": 1,
      "max_password_len": 8,
      "min_elem_in_chain": 1,
      "max_elem_in_chain": 8,
      "generate_random_rules": 0,
      "optimized": True
      },
      "hash_list_id": None,
      "valid_only": True
    }
  return data

#Get supported masks
def get_masks():
  masks = {
    "all": {
      "mask": "?a",
      "count": 95
    },
    "lower": {
      "mask": "?l",
      "count": 26
    },
    "digit": {
      "mask": "?d",
      "count": 10
    }
  }
  return masks

#Hashes per second * number of seconds
def compute_keyspace(days, hours, minutes, speed):
  return (speed * ((days*24*3600) + (hours*3600) + (minutes*60)))

#Rules
def decide_on_rules(dicts, rules, keyspace):
  selected_rule = None
  current_keyspace = 0
  for rule in rules:
    current_rule = rule_fit_to_keyspace(dicts, rule, keyspace)
    if (current_rule["fit"] == True and current_rule["current_keyspace"] > current_keyspace):
      selected_rule = rule
      current_keyspace = current_rule["current_keyspace"]

  return selected_rule

def rule_fit_to_keyspace(dicts, rule, keyspace):
  fit = {}
  fit["current_keyspace"] = 0
  fit["fit"] = False

  for dict in dicts:
    fit["current_keyspace"] += rule.count * dict.keyspace
    if fit["current_keyspace"] > keyspace:
      return fit
    
  fit["fit"] = True
  return fit

#Dictionary Attack creation
def make_dict_attack(name, dict, rule, hash_list, hosts_ids):
  data = create_job_template()
  if not hosts_ids:
    hosts_ids = []
  data["hosts_ids"] = hosts_ids
  data["name"] = name + "_" + dict.name
  
  if rule != None:
    data["attack_settings"]["rules"]["id"] = rule.id
    data["attack_settings"]["rules"]["name"] = rule.name
    data["attack_settings"]["rules"]["count"] = rule.count
    data["attack_settings"]["rules"]["time"] = rule.time
  else:
    data["attack_settings"]["rules"] = None

  dictionary = [
      {
      "id": dict.id,
      "name": dict.name,
      "keyspace": dict.keyspace,
      "time": dict.time,
      "hex_dict": dict.hex_dict
    }
  ]

  data["attack_settings"]["left_dictionaries"] = dictionary
  data["hash_list_id"] = hash_list

  return data

def make_bruteforce_attack(name, hash_list, hosts_ids, password_len_min, password_len_max, keyspace):
  masks = generate_masks(password_len_min, password_len_max, keyspace)

  #Check if no masks were able to fit into the keyspace
  if not masks or masks == []:
    return False
  
  data = create_job_template()

  if not hosts_ids:
    hosts_ids = []
  data["hosts_ids"] = hosts_ids
  data["name"] = name + "_BruteForce_Fill"
  data["attack_settings"]["attack_mode"] = 3
  data["attack_settings"]["attack_name"] = "mask"
  data["hash_list_id"] = hash_list

  data["attack_settings"]["masks"] = masks
  return data

def generate_masks(password_len_min, password_len_max, keyspace):
  mask_list = get_masks()
  masks = []
  mask = ""
  next_mask = ""
  current_keyspace = 0
  current_length = 0

  #Decide on first mask
  next_mask = decide_first_mask(keyspace, mask_list)

  #Return False if keyspace is almost empty and no masks fit
  if not next_mask:
    return False
  
  mask = mask + next_mask["mask"]
  current_length += 1
  current_keyspace = next_mask["count"]
  keyspace = keyspace - current_keyspace
  
  #If min length is 1, add it to result
  if current_length >= password_len_min and current_length <= password_len_max:
    masks.append(mask)

  while current_length < password_len_max:
    #Decide on next mask
    next_mask = decide_next_mask(keyspace, current_keyspace, mask_list)
    #Return masks if no more masks fit
    if not next_mask:
      return masks

    mask = mask + next_mask["mask"]
    current_length += 1
    current_keyspace = current_keyspace * next_mask["count"]
    keyspace = keyspace - current_keyspace
    
    #If fits user requirements, add it to result
    if current_length >= password_len_min and current_length <= password_len_max:
      masks.append(mask)
  
  return masks


def decide_first_mask(keyspace, mask_list):
  for mask_name in mask_list:
    if keyspace < mask_list[mask_name]["count"]:
      continue
    else:
      return mask_list[mask_name]
    
  return False

def decide_next_mask(keyspace, current_keyspace, mask_list):
  for mask_name in mask_list:
    if keyspace < current_keyspace * mask_list[mask_name]["count"]:
      continue
    else:
      return mask_list[mask_name]
    
  return False