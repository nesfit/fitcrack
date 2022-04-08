#!/usr/bin/python3

import psutil
import time
import urllib3.request
import datetime
import mysql.connector

DB_HOST = 'localhost'
DB_NAME = 'fitcrack'
DB_USER = 'fitcrack'
DB_PW = 'mypassword'

server = "http://localhost:5000"
http = urllib3.PoolManager()

MINUTES_IN_MONTH = 60 * 24 * 30

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
        print(datetime.datetime.utcnow(),
              '[ERROR]: Could not connect to Fitcrack DB:', err)
        exit(1)

    return fitcrack_db


def main():
    fitcrack_db = connect_db()
    cursor = fitcrack_db.cursor()
    minutes = 0

    while True:
        cpu = 0
        mem = 0

        hdd_read = psutil.disk_io_counters(nowrap=True).read_bytes
        hdd_write = psutil.disk_io_counters(nowrap=True).write_bytes
        recv = psutil.net_io_counters(nowrap=True).bytes_recv
        sent = psutil.net_io_counters(nowrap=True).bytes_sent

        for i in range(60):
            cpu += psutil.cpu_percent()
            mem += psutil.virtual_memory().percent
            time.sleep(1)

        minutes += 1
        cpu /= 60
        mem /= 60
        hdd_read = (psutil.disk_io_counters(nowrap=True).read_bytes - hdd_read) / 60
        hdd_write = (psutil.disk_io_counters(
                nowrap=True).write_bytes - hdd_write) / 60
        recv = (psutil.net_io_counters(nowrap=True).bytes_recv - recv) / 60
        sent = (psutil.net_io_counters(nowrap=True).bytes_sent - sent) / 60

        hdd_read *= 8
        hdd_write *= 8
        recv *= 8
        sent *= 8

        hdd_read /= 1000
        hdd_write /= 1000
        recv /= 1000
        sent /= 1000

        print("CPU: " + str(cpu))
        print("MEM: " + str(mem))

        print("NET_D: " + str(recv))
        print("NET_U: " + str(sent))

        print("HDD_R: " + str(hdd_read))
        print("HDD_W: " + str(hdd_write))

        url = server + "/serverInfo/saveData" + "?cpu=" + str(cpu) + "&ram=" + str(mem) + "&net_recv=" + str(
                recv) + "&net_sent=" + str(sent) + "&hdd_read=" + str(hdd_read) + "&hdd_write=" + str(hdd_write)
        try:
            http.request('POST', url)
        except Exception as e:
            print(datetime.datetime.utcnow(),
                    '[WARN]: Unknown exception occured in sending usage data:', e)

        if minutes == MINUTES_IN_MONTH:
            cursor.execute(
                    "DELETE FROM fc_server_usage WHERE time < (NOW() - INTERVAL 30 DAY)")
            cursor.execute(
                    "DELETE FROM fc_device_info WHERE time < (NOW() - INTERVAL 30 DAY)")
            fitcrack_db.commit()
            minutes = 0


if __name__ == '__main__':
    main()
