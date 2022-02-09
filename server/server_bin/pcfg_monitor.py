#!/usr/bin/python3

"""
    Daemon for periodically checking for new running PCFG jobs to run a PCFG Manager for them.
"""

import re
import mysql.connector
import datetime
import time
import subprocess
from psutil import process_iter
from time import sleep

DB_HOST = 'localhost'
DB_NAME = 'fitcrack'
DB_USER = 'fitcrack'
DB_PW = 'mypassword'

PCFG_DIR = '/usr/share/collections/pcfg/'

def connect_db():
    """Accesses the Fitcrack DB.

    :return: MySQL object
    """
    try:
        fitcrack_db = mysql.connector.connect(
            host=DB_HOST,
            user=DB_USER,
            passwd=DB_PW,
            database=DB_NAME
        )
    except mysql.connector.Error as err:
        print(datetime.datetime.utcnow(), '[ERROR]: Could not connect to Fitcrack DB:', err)
        exit(1)

    return fitcrack_db


def get_running_pcfg_jobs(cursor):
    """Gets list of running PCFG jobs.

    :return [int]:  List of running PCFG Job IDs
    """
    cursor.execute('SELECT id FROM fc_job WHERE status = 10 AND attack_mode = 9 AND current_index < hc_keyspace')
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
        except Exception as e:
            print(datetime.datetime.utcnow(), '[WARN]: Unknown exception occured in process parsing:', e)
            continue

    return result


def cancel_wus(job_id, cursor):
    """Cancel running BOINC WUs.

    :note: This is a duplicated 'cancel_jobs' BOINC function from backend_lib.h
    """
    cursor.execute('SELECT id, workunit_id FROM fc_workunit WHERE job_id = %s AND finished = 0', (job_id, ))
    wus = cursor.fetchall()

    for wu_id, fitcrack_wu_id in wus:
        # Cancel results
        cursor.execute('UPDATE result SET server_state = 5, outcome = 5 WHERE server_state <= 2 AND workunitid = %s', (wu_id, ))

        # Cancel jobs
        cursor.execute('UPDATE workunit SET error_mask = error_mask|16, transition_time = %s WHERE id = %s', (int(time.time()), wu_id, ))
        cursor.execute('UPDATE fc_workunit SET finished = 1 WHERE id = %s', (fitcrack_wu_id, ))


def purge_job(job_id, cursor):
    """Purges the job in case of some nasty crash before."""
    cursor.execute('UPDATE fc_job SET status = 0 WHERE id = %s', (job_id, ))
    sleep(2)
    cancel_wus(job_id, cursor)
    sleep(2)
    cursor.execute('UPDATE fc_job SET status = 10, indexes_verified = 0, current_index = 0 WHERE id = %s', (job_id, ))


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
    return cursor.fetchone()[0].encode('utf-8').decode('utf-8')


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
