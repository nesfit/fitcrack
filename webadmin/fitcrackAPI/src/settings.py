'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os

# HashCat settings
ROOT_DIR = os.path.dirname(os.path.abspath(__file__))
HASHCAT_EXECUTABLE = 'hashcat64.bin'
HASHCAT_DIR = ROOT_DIR + '/../hashcat-5.1.0/'
HASHCAT_PATH = HASHCAT_DIR + HASHCAT_EXECUTABLE
HASHCAT_UTILS_PATH =  ROOT_DIR + '/../hashcat-utils/bin'
MASK_PROCESSOR_PATH = ROOT_DIR + '/../maskprocessor/mp64.bin'
PRINCE_PROCESSOR_PATH = ROOT_DIR + '/../princepreprocessor/pp64.bin'
EXE_OR_BIN = 'bin'
DICTIONARY_DIR = '/usr/share/collections/dictionaries'
PCFG_DIR = '/usr/share/collections/pcfg'
# PCFG Mower
#PCFG_MOWER_DIR = ROOT_DIR + '/src/api/fitcrack/endpoints/pcfg/pcfg_mower/pcfg_mower.py'
PCFG_MOWER_DIR = ROOT_DIR + '/../pcfg_mower/pcfg_mower.py'
# PCFG Manager
#PCFG_MANAGER_DIR = ROOT_DIR + '/src/api/fitcrack/endpoints/pcfg/pcfg_manager/pcfg-manager'
PCFG_MANAGER_DIR = ROOT_DIR + '/../pcfg_manager/pcfg-manager'
#PCFG_MANAGER = ROOT_DIR + '/src/api/fitcrack/endpoints/pcfg/pcfg_manager'
PCFG_MANAGER = ROOT_DIR + '/../pcfg_manager'
# PCFG Trainer
PCFG_TRAINER_DIR = ROOT_DIR + '/../pcfg_trainer/pcfg_trainer.py'
PCFG_TRAINER_RULE_DIR = ROOT_DIR + '/../pcfg_trainer/Rules'

HCSTATS_DIR = '/usr/share/collections/markov'
MASKS_DIR = '/usr/share/collections/masks'
CHARSET_DIR = '/usr/share/collections/charsets'
RULE_DIR = '/usr/share/collections/rules'
PROTECTEDFILES_DIR = '/usr/share/collections/encrypted-files'
SERVER_BROWSER_ROOT = '/usr/share/collections/'

#XtoHashCat settings
XTOHASHCAT_EXECUTABLE = ROOT_DIR + '/../xtohashcat/XtoHashcat.py'
XTOHASHCAT_PATH = ROOT_DIR + '/../xtohashcat/'

#Hashvalidator settings
HASHVALIDATOR_PATH = ROOT_DIR + '/../hashvalidator/hashValidator'

# Fitcrack Settings
SECONDS_PER_JOB = 3600
PROJECT_USER = 'root'
PROJECT_NAME = 'fitcrack'
PROJECT_DIR = '/home/' + PROJECT_USER + '/projects/' + PROJECT_NAME

# Flask settings
FLASK_SERVER_NAME = 'localhost:5000'
FLASK_DEBUG = True  # Do not use debug mode in production

# Flask-Restplus settings
RESTPLUS_SWAGGER_UI_DOC_EXPANSION = 'list'
RESTPLUS_VALIDATE = True
RESTPLUS_MASK_SWAGGER = True
RESTPLUS_ERROR_404_HELP = False

# SQLAlchemy settings

SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://root@localhost/fitcrack'

SQLALCHEMY_TRACK_MODIFICATIONS = False

BOINC_SERVER_URI = 'http://localhost'
