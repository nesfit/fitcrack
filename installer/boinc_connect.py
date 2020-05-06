#!/usr/bin/python3
 
import os
import sys
import argparse
import subprocess
import time

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("-u", "--url", type=str, default="http://127.0.0.1/fitcrack")
    args = parser.parse_args()    
    subprocess.call(['boinccmd', '--project', args.url, 'detach'], cwd='/var/lib/boinc', stderr=subprocess.DEVNULL)
    output = subprocess.check_output(['boinccmd', '--create_account', args.url, 'fitcrack@test.cz', 'fitcrack@test.cz', 'fitcrack@test.cz'], cwd='/var/lib/boinc')
    key = output.split()[-1]
    ret = subprocess.call(['boinccmd', '--project_attach', args.url, key], cwd='/var/lib/boinc')
    subprocess.call(['boinccmd', '--project', args.url, 'update'], cwd='/var/lib/boinc')
    sys.exit(ret)
    