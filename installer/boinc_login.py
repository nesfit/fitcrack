#!/usr/bin/python3
 
import os
import sys
import argparse
import subprocess
import time

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--url", type=str, default="http://127.0.0.1/fitcrack")
    parser.add_argument("--email", type=str, default="fitcrack@test.cz")
    parser.add_argument("--username", type=str, default="fitcrack")
    parser.add_argument("--password", type=str, default="mypassword")
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

    # login to boinc server
    # load key from /var/lib/boinc/fitcrack.key
    with open(os.path.join(boinc_dir, 'fitcrack.key'), 'r') as f:
        key = f.read()
    ret = subprocess.call(['boinccmd', '--project_attach', args.url, key], cwd=boinc_dir)
    subprocess.call(['boinccmd', '--project', args.url, 'update'], cwd=boinc_dir)
    sys.exit(ret)    