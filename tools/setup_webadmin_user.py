#!/usr/bin/env python3
#
# Updates the WebAdmin's default user from environment values
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

import os
import mysql.connector
from mysql.connector import Error
from werkzeug.security import generate_password_hash

mysql_host = os.environ['DB_HOST']
mysql_database = os.environ['DB_NAME']
mysql_user = os.environ['DB_USER']
mysql_pw = os.environ['DB_PW']

wa_user = os.environ['WEBADMIN_USER']
wa_pw = os.environ['WEBADMIN_PW']
wa_email = os.environ['WEBADMIN_EMAIL']

pw_hash = generate_password_hash(wa_pw)

try:
    connection = mysql.connector.connect(host=mysql_host, database=mysql_database, user=mysql_user, password=mysql_pw)
    if connection.is_connected():
        db_Info = connection.get_server_info()
        print("Connected to MySQL Server version ", db_Info)
        cursor = connection.cursor()
        query = "UPDATE `fc_user` SET username='" + wa_user + "', password='" + pw_hash + "', mail='" + wa_email + "' WHERE id=1";
        cursor.execute(query)
        connection.commit()
        print("Webadmin default user updated.")

except Error as e:
    print("Error while connecting to MySQL", e)
