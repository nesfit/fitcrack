'''
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
'''

#!/usr/bin/python3
import sys


sys.path.insert(0,"/var/www/fitcrackAPI/src/")

from app import app as application
application.debug = False

