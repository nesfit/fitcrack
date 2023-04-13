'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os

# HashCat settings
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
HASHCAT_EXECUTABLE = 'hashcat.bin'
HASHCAT_HASH_INFO_JSON = 'hash_info.json'
HASHCAT_DIR = ROOT_DIR + '/../hashcat/'
HASHCAT_PATH = HASHCAT_DIR + HASHCAT_EXECUTABLE
HASHCAT_HASH_INFO_PATH = HASHCAT_DIR + HASHCAT_HASH_INFO_JSON
HASHCAT_UTILS_PATH =  ROOT_DIR + '/../hashcat-utils/bin'
RULE_APPLICATOR_PATH = ROOT_DIR + '/../hashcat-utils/cpu_rules.so'
PWD_DIST_PATH =  ROOT_DIR + '/../pwd_dist/pwd_dist.bin'
PRINCE_PROCESSOR_PATH = ROOT_DIR + '/../princeprocessor/pp64.bin'
EXE_OR_BIN = 'bin'
DICTIONARY_DIR = '/usr/share/collections/dictionaries'
PCFG_DIR = '/usr/share/collections/pcfg'
# PCFG Mower
PCFG_MOWER_DIR = ROOT_DIR + '/../pcfg_mower/pcfg_mower.py'
# PCFG Manager
PCFG_MANAGER_DIR = ROOT_DIR + '/../pcfg_manager/pcfg-manager'
PCFG_MANAGER = ROOT_DIR + '/../pcfg_manager'
# PCFG Trainer
PCFG_TRAINER_DIR = ROOT_DIR + '/../pcfg_trainer/pcfg_trainer.py'
PCFG_TRAINER_RULE_DIR = ROOT_DIR + '/../pcfg_trainer/Rules'

HCSTATS_DIR = '/usr/share/collections/markov'
MASKS_DIR = '/usr/share/collections/masks'
CHARSET_DIR = '/usr/share/collections/charsets'
RULE_DIR = '/usr/share/collections/rules'
PROTECTEDFILES_DIR = '/usr/share/collections/encrypted-files'
SERVER_BROWSER_ROOT = '/usr/share/collections/import'

#XtoHashCat settings
XTOHASHCAT_EXECUTABLE = ROOT_DIR + '/../xtohashcat/XtoHashcat.py'
XTOHASHCAT_PATH = ROOT_DIR + '/../xtohashcat/'

# Fitcrack Settings
SECONDS_PER_JOB = 3600
PROJECT_USER = 'boincadm'
PROJECT_NAME = 'fitcrack'
PROJECT_DIR = '/home/' + PROJECT_USER + '/projects/' + PROJECT_NAME

# Webadmin security
ALLOW_TOKEN_SIGNIN = True # Disable this for installations exposed to the internet!!!

# Flask settings
FLASK_SERVER_NAME = 'localhost:5000'
FLASK_DEBUG = True  # Do not use debug mode in production

# Flask-RestX settings
RESTX_SWAGGER_UI_DOC_EXPANSION = 'list'
RESTX_VALIDATE = True
RESTX_MASK_SWAGGER = True
RESTX_ERROR_404_HELP = False

# SQLAlchemy settings

SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://fitcrack:mysqlPASSWORD@localhost/fitcrack'

SQLALCHEMY_TRACK_MODIFICATIONS = False

BOINC_SERVER_URI = 'http://127.0.0.1:80'
