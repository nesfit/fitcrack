'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

import os

from settings import HASHCAT_PATH, DICTIONARY_DIR, HASHCAT_DIR
from src.api.fitcrack.functions import shellExec


def readingFromFolderPostProcces(DictionaryModel):
    DictionaryModel.keyspace = int(shellExec(HASHCAT_PATH + ' --keyspace -a 0 "' + os.path.join(DICTIONARY_DIR, DictionaryModel.path + '"'),
                                             cwd=HASHCAT_DIR))
    return DictionaryModel
