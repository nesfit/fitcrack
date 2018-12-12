'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

from flask_restplus.errors import abort
from src.api.fitcrack.functions import shellExec



def isComponentRunning(compName):
    out = shellExec("ps -ef | grep " + compName + " | grep -v grep")
    return True
    if len(out.splitlines()) >= 1:
        return True

    return False



