#!/usr/bin/python3

import psutil
import time
import urllib3.request
import datetime

server = "http://localhost:5000"
http = urllib3.PoolManager()

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

	cpu /= 60
	mem /= 60
	hdd_read = (psutil.disk_io_counters(nowrap=True).read_bytes - hdd_read) / 60
	hdd_write = (psutil.disk_io_counters(nowrap=True).write_bytes - hdd_write) / 60
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

	url = server + "/serverInfo/saveData" + "?cpu=" + str(cpu) + "&ram=" + str(mem) + "&net_recv=" + str(recv) + "&net_sent=" + str(sent) + "&hdd_read=" + str(hdd_read) + "&hdd_write=" + str(hdd_write)
	try:
        http.request('POST', url)
    except Exception as e:
        print(datetime.datetime.utcnow(), '[WARN]: Unknown exception occured in sending usage data:', e)
