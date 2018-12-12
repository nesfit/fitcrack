'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from settings import HASHCAT_PATH
from src.api.fitcrack.functions import shellExec, getStringBetween, parserAsciiTable
import os


def getTable(tableName):
    out = shellExec( HASHCAT_PATH + " --help")

    twoNewLines = os.linesep + os.linesep
    hashtypes = getStringBetween(out, "- [ " + tableName + " ] -" + twoNewLines, twoNewLines)

    hashtypes = parserAsciiTable(hashtypes)

    return hashtypes


