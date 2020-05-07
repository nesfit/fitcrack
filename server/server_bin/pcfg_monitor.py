#!/usr/bin/python3

"""
    Daemon for periodically checking for new running PCFG jobs to run a PCFG Manager for them.
"""

import re
import mysql.connector
import datetime
import subprocess
from psutil import process_iter
from time import sleep


PCFG_DIR = '/usr/share/collections/pcfg/'
CONFIG_PATH = '../config.xml'
EMPTY_HASHLIST = 'empty_hashlist.txt'


def parse_config():
    with open(CONFIG_PATH, 'r') as config:
        lines = config.read().splitlines()
        for line in lines:
            if '<db_user>' in line:
                db_user = re.search('^\s*<db_user>(.+)</db_user>$', line).group(1)
            elif '<db_name>' in line:
                db_name = re.search('^\s*<db_name>(.+)</db_name>$', line).group(1)
            elif '<db_passwd>' in line:
                db_passwd = re.search('^\s*<db_passwd>(.+)</db_passwd>$', line).group(1)
            elif '<db_host>' in line:
                db_host = re.search('^\s*<db_host>(.+)</db_host>$', line).group(1)

    return db_host, db_name, db_user, db_passwd


def connect_db():
    """Parses BOINC config and accesses the Fitcrack DB.

    :return: MySQL object
    """
    try:
        db_host, db_name, db_user, db_passwd = parse_config()
    except FileNotFoundError:
        print(datetime.datetime.utcnow(), '[ERROR]: Config file not found')
        exit(1)
    except UnboundLocalError:
        print(datetime.datetime.utcnow(), '[ERROR]: Failed to parse config.xml')
        exit(1)

    try:
        fitcrack_db = mysql.connector.connect(
            host=db_host,
            user=db_user,
            passwd=db_passwd,
            database=db_name
        )
    except mysql.connector.Error as err:
        print(datetime.datetime.utcnow(), '[ERROR]: Could not connect to Fitcrack DB:', err)
        exit(1)

    return fitcrack_db


def get_running_pcfg_jobs(cursor):
    """Gets list of running PCFG jobs.

    :return [int]:  List of running PCFG Job IDs
    """
    cursor.execute('SELECT id FROM fc_job WHERE status = 10 AND attack_mode = 9')
    jobs = cursor.fetchall()

    return [x[0] for x in jobs]


def get_running_managers():
    """Checks running processes and find the PCFG managers.

    :return [int]: List of Job IDs for which the manager is running
    """
    result = []

    for proc in process_iter():
        try:
            if proc.cmdline() and \
               'pcfg-manager' in proc.cmdline()[0] and \
               'server' in proc.cmdline()[1]:
                result.append(int(proc.cmdline()[proc.cmdline().index('-p') + 1]) - 50050)
        except (ValueError, IndexError):
            continue

    return result


def purge_job(job_id, cursor):
    """Purges the job in case of some nasty crash before."""
    cursor.execute('UPDATE fc_job SET indexes_verified = 0, current_index = 0 WHERE id = %s', (job_id, ))


def job_id_to_port(job_id):
    """Transforms job_id to finite port interval."""
    return (job_id % 1000) + 50050


def get_keyspace(job_id, cursor):
    """Returns job keyspace from database."""
    cursor.execute('SELECT hc_keyspace FROM fc_job WHERE id = %s', (str(job_id),))
    return cursor.fetchone()[0]


def get_grammar_name(job_id, cursor):
    """Returns job keyspace from database."""
    cursor.execute('SELECT fc_pcfg_grammar.name FROM fc_pcfg_grammar '
                   'LEFT JOIN fc_job ON fc_pcfg_grammar.id = fc_job.grammar_id '
                   'WHERE fc_job.id = %s', (job_id,))
    return cursor.fetchone()[0].decode('utf-8')


def run_new_manager(job_id, cursor):
    """Run new PCFG manager for a new running PCFG job."""
    print(datetime.datetime.utcnow(), '[LOG]: Running new PCFG manager for job ID', job_id)
    process = subprocess.Popen(
        ['../bin/pcfg-manager', 'server', '-p', str(job_id_to_port(job_id)), '-m', str(get_keyspace(job_id, cursor)),
         '-r', PCFG_DIR + get_grammar_name(job_id, cursor)])
           

def main():
    # Connect to fitcrack DB
    fitcrack_db = connect_db()
    f_cursor = fitcrack_db.cursor()

    # Prepare empty hashlist
    open(EMPTY_HASHLIST, 'a').close()

    while True:
        pcfg_jobs = get_running_pcfg_jobs(f_cursor)
        running_managers = get_running_managers()

        # Check if there is a missing manager
        missing_managers = []
        for job_id in pcfg_jobs:
            if job_id % 1000 not in running_managers:
                missing_managers.append(job_id)

        for job_id in missing_managers:
            # Run new manager
            purge_job(job_id, f_cursor)
            run_new_manager(job_id, f_cursor)

        # To work around cache, we need to commit always
        fitcrack_db.commit()

        # Sleep before next check
        sleep(2)


if __name__ == '__main__':
    main()
