#!/usr/bin/python3

import psutil
import time
import urllib3.request

server = "http://localhost:5000"

while True:
	cpu = psutil.cpu_percent()
	mem = psutil.virtual_memory().percent

	hdd_read = psutil.disk_io_counters().read_bytes
	hdd_write = psutil.disk_io_counters().write_bytes
	recv = psutil.net_io_counters().bytes_recv
	sent = psutil.net_io_counters().bytes_sent

	time.sleep(60)

	hdd_read = (psutil.disk_io_counters().read_bytes - hdd_read) / 60
	hdd_write = (psutil.disk_io_counters().write_bytes - hdd_write) / 60
	recv = (psutil.net_io_counters().bytes_recv - recv) / 60
	sent = (psutil.net_io_counters().bytes_sent - sent) / 60

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

	http = urllib3.PoolManager()

	url = server + "/serverInfo/saveData" + "?cpu=" + str(cpu) + "&ram=" + str(mem) + "&net_recv=" + str(recv) + "&net_sent=" + str(sent) + "&hdd_read=" + str(hdd_read) + "&hdd_write=" + str(hdd_write)

	response = http.request('GET', url)

