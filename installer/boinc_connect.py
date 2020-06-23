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

    boinc_dir = None
    if os.path.exists('/var/lib/boinc'):
        # Almost all distros..
        boinc_dir = '/var/lib/boinc'
    elif os.path.exists('/var/lib/boinc-client'):
        # Debian
        boinc_dir = '/var/lib/boinc-client'
    else:
        print("Unable to find boinc folder", file=sys.stderr)
        sys.exit(1)

    subprocess.call(['boinccmd', '--project', args.url, 'detach'], cwd=boinc_dir, stderr=subprocess.DEVNULL)
    output = subprocess.check_output(['boinccmd', '--create_account', args.url, 'fitcrack@test.cz', 'fitcrack@test.cz', 'fitcrack@test.cz'], cwd=boinc_dir)
    key = output.split()[-1]
    ret = subprocess.call(['boinccmd', '--project_attach', args.url, key], cwd=boinc_dir)
    subprocess.call(['boinccmd', '--project', args.url, 'update'], cwd=boinc_dir)
    sys.exit(ret)
    