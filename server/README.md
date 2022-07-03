# Fitcrack server README

This readme describes the process process of manual Fitcrack server installation. Namely:
* **Download server** (directory accessible over Apache)
* **Upload server** (directory accessible over Apache)
* **Scheduler** (Native BOINC app running on FastCGI over Apache)
* **feeder** (native BOINC daemon)
* **dransitioner** (native BOINC daemon)
* **file_deleter** (native BOINC daemon)
* **work_generator** (Fitcrack-specific BOINC daemon)
* **bitwise_validator** (Fitcrack-specific BOINC daemon)
* **assimilator** (Fitcrack-specific BOINC daemon)
* **trickler** (Fitcrack-specific BOINC daemon)
* **measureUsage.py** (Fitcrack-specific BOINC daemon)
* **pcfg_monitor.py** (Fitcrack-specific BOINC daemon)

(Note: For **Runner** and **Webadmin**, there are separate READMEs)


## Dependencies

On your system, you should install the following dependencies:
* make (3.79+)
* m4 (1.4+)
* libtool (1.5+)
* autoconf (2.58+)
* automake (1.8+)
* GCC (6.3.0+)
* pkg-config (0.15+)
* Perl + libcompress with LZMA
* Python 3
* pip for Python 3
* MySQL (4.0.9+) or MariaDB (10.0+)
* libnotify-dev
* Apache with the following modules:
  * PHP (5+) with XML and MySQL modules
  * CGI
  * WSGI
  * rewrite
* PHP5 with cli support and the GD and MySQL modules
* OpenSSL (0.98+)
* Curl
* Gtest (only if you intend to build the tests in `server/src/tests`)


You can also use distro-specific commands...

### Ubuntu 22.04 LTS commands
```
apt-get install m4 make dh-autoreconf pkg-config git vim apache2 libapache2-mod-php mysql-server mysql-common libmysqlclient-dev zlibc zlib1g zlib1g-dev php php-xml php-mysql php-cli php-gd python-is-python3 python3 python3-mysqldb python3-pymysql python3-pip libapache2-mod-wsgi-py3 libssl-dev libcurl4-openssl-dev apache2-utils pkg-config libnotify-dev curl perl libcompress-raw-lzma-perl

mysql_secure_installation # Set MySQL root password

a2enmod cgi       # enable mod CGI
a2enmod rewrite   # enable mod rewrite
a2enmod wsgi      # enable mod wsgi
systemctl restart apache2
```

### CentOS/RHEL 8 commands
Install deps:
```
yum install -y m4 gcc gcc-c++ make libtool autoconf automake git vim httpd php php-xml php-mysqlnd python3-devel python3 python3-pip python3-mod_wsgi  redhat-rpm-config python3-setuptools mariadb-server mariadb-devel pkgconfig libnotify zlib libcurl-devel openssl-libs openssl-devel

pip3 install mysqlclient

alternatives --set python /usr/bin/python3
```

Configure exceptions for firewalld:
```
firewall-cmd --zone=public --add-service=http --permanent
firewall-cmd --zone=public --add-service=https --permanent
firewall-cmd --zone=public --add-port=5000/tcp --permanent
firewall-cmd --reload
```

Configure services
```
systemctl start httpd.service
systemctl enable httpd.service
systemctl start mariadb
mysql_secure_installation # Set MariaDB root password
systemctl enable mariadb.service
```


## Create necessary users and groups
```
useradd -m -c "BOINC Administrator" boincadm  -s /bin/bash
sudo addgroup boincadm
sudo usermod -a -G boincadm www-data
```

## Create a MySQL database and a user account for Fitcrack server
Create a MySQL database and a user that has full access to it, e.g.:
```
mysql -u root -p
mysql> create database fitcrack;
mysql> GRANT ALL PRIVILEGES ON fitcrack.* TO 'fitcrack'@'localhost' IDENTIFIED BY 'SUPERSECRET';
```

# Copy Fitcrack sources to the BOINC source directory
All `.h` and `.cpp` files from `server/src` must be copied into the `boinc/sched/` directory
before the compilation:
```
cp -f server/src/headers/*.h              boinc/sched/
cp -f server/src/headers/AttackModes/*.h  boinc/sched/
cp -f server/src/headers/Generators/*.h   boinc/sched/
cp -f server/src/headers/Database/*.h     boinc/sched/
cp -f server/src/headers/Utils/*.h        boinc/sched/
cp -f server/src/source/*.cpp             boinc/sched/
cp -f server/src/source/AttackModes/*.cpp boinc/sched/
cp -f server/src/source/Generators/*.cpp  boinc/sched/
cp -f server/src/source/Database/*.cpp    boinc/sched/
cp -f server/src/source/Utils/*.cpp       boinc/sched/
cp -f server/src/headers/protocol/*.cc    boinc/sched/
cp -f server/src/headers/protocol/*.h     boinc/sched/
cp -rf server/src/include/*               boinc/sched/
```

## Configure and build the BOINC server
```
cd boinc
./_autosetup
./configure --disable-client --disable-manager
make
```

## Install libboinc
```
sudo make install
```

## Make and configure the Fitcrack project
Use the `boinc/tools/make_project` script to create your project:
* `--url_base` is the protocol (http/https) and IP/hostname of your server
* `--srcdir` source directory with complied BOINC server
* `--user_name` is the name of the BOINC server user (`boincadm`)
* `--project_host` is the IP or hostname of your server
* `--project_root` is the path to your new project (needs to be accesible by the BOINC server user)
* `--db_user` is the MySQL user name
* `--db_name` is the name of the database for Fitcrack server
* `--db_passwd` is the mysql user password

Example:
```
./boinc/tools/make_project \
  --url_base http://127.0.0.1 \
  --srcdir boinc --user_name boincadm \
  --project-host 127.0.0.1 \
  --project-root /home/boincadm/projects/fitcrack
  --db_host 127.0.0.1 \
  --db_name fitcrack
  --db_user fitcrack \
  --db_passwd SUPERSECRET
```
Note: User `127.0.0.1` insted of `localhost`.


## Allow Apache access to Scheduler CGI scripts
Add the contents of the `/home/boincadm/projects/<your_project_name>/<your_project_name>.httpd.conf` script
to your Apache config and configure access.

Example:
```
sudo bash -c 'cat /home/boincadm/projects/<your_project_name>/<your_project_name>.httpd.conf >> /etc/apache2/apache2.conf'

sudo vim /etc/apache2/apache2.conf
:%s/Allow from all/Allow from all\r        Require all granted/g
:%s/#ServerRoot "\/etc\/apache2"/#ServerRoot "\/etc\/apache2"\rServerName test.local/g
:wq

sudo /usr/sbin/apache2ctl restart
```
Alternatively, you can create a **VirtualHost** for BOINC.


## Secure the BOINC web (ops) administration:
```
htpasswd -cb ~/projects/<your_project_name>/html/ops/.htpasswd <your_username> <your_password>
vim /home/boincadm/projects/<your_project_name>/html/project/project.inc
:%s/<master_url>/http:\/\/<your_server_IP>\//g
:wq
cd /home/boincadm/projects/<your_project_name>/
sudo a2enmod cgi
sudo /usr/sbin/apache2ctl restart
```

## Edit the project config
```
vim /home/boincadm/projects/<your_project_name>/config.xml
:%s/    <\/config>/        <enable_assignment>1<\/enable_assignment>\r        <next_rpc_delay>10<\/next_rpc_delay>\r        <maintenance_delay>60<\/maintenance_delay>\r        <max_wus_in_progress>1<\/max_wus_in_progress>\r        <max_ncpus>1<\/max_ncpus>\r        <prefer_primary_platform>1<\/prefer_primary_platform>\r        <min_sendwork_interval>6<\/min_sendwork_interval>\r    <\/config>\r/g
:%s/example_app/fitcrack/g
:%s/sample_work_generator -d 3/work_generator -d 3 --app fitcrack/g
:%s/sample_assimilator/assimilator/g
:%s/sample_bitwise_validator/bitwise_validator/g
:%s/</daemons>/    <daemon>\r            <cmd>trickler --variety fitcrack</cmd>\r        </daemon>\r    </daemons>/g
:wq
```

## Copy Fitcrack templates and project.xml
```
cd ~
cp templates/* /home/boincadm/projects/<your_project_name>/templates/
cp project.xml /home/boincadm/projects/<your_project_name>/
```


## See the binary-creation info
Copy binaries of your host applications (Runner, Fitcrack, hashcat) to
`/home/boincadm/project/<your_project_name>/app/fitcrack/<version_number>/`

The `<version_number>` directory has to contain subfolder named as the targeted
platform. All platforms detected by BOINC can
be found at https://boinc.berkeley.edu/trac/wiki/BoincPlatforms. Each
subfolder has to contain all of the files even
those platform not-specific (archive etc.). Also there has to be version.xml
with files specified for more info look
at https://boinc.berkeley.edu/trac/wiki/AppVersionNew.
When you have files prepared like this you can call ./bin/update_versions
script which then adds the new version to
to the system and stages it for sending to clients.


## Create the client-side binaries
Create a directory for version 1 of the fitcrack client app:
```
mkdir /home/boincadm/projects/<your_project_name>/apps/fitcrack/1/
```

You can use our pre-build binaries:
```
cp -r client_bin/* /home/boincadm/projects/<your_project_name>/apps/fitcrack/1/
```

Or you can prepare your own.
As described [here](https://boinc.berkeley.edu/trac/wiki/AppVersionNew),
you need to create directories for client platforms you want to support, e.g.:
* `windows_x86_64`
* `x86_64-pc-linux-gnu`

What is needed inside?
* `hashcat1.bin` or `hashcat1.exe` - hashcat compiled for the platform's architecture
* `hashcat_files1.zip` - A ZIP archive with the following (for the corresponding version of hashcat):
  * `modules` - hashcat app's modules
  * `OpenCL` - hashcat's OpenCL cracking kernels
  * `hashcat.hcstat2` - Markov hcstat2 file
  * `hashcat.hctune` - hashcat's hctune settings
  * `hash_info.json` - info about the supported hash algorithms
* `runner1.bin` or `runner1.exe` - Fitcrack runner comppiled for the platform's architecture (See `runner/README.md`)
* `princeprocessor1.bin` or `princeprocessor1.exe` - the [princeprocessor](https://github.com/hashcat/princeprocessor) app for PRINCE attacks compile for the platform's architecture.
* `pcfg-manager1.bin` or `pcfg-manager1.exe` - NESatFIT [PCFG Manager](https://github.com/nesfit/pcfg-manager) compiled for the platform's architecture.
* `version.xml` - version file with the definition of all the above-shown files that should be transferred to the host cracking stations. See example below:
```
<version>
	<file><physical_name>hashcat1.bin</physical_name><copy_file/></file>
	<file><physical_name>hashcat_files1.zip</physical_name><copy_file/></file>
	<file><physical_name>pcfg-manager1.bin</physical_name><copy_file/></file>
	<file><physical_name>princeprocessor1.bin</physical_name><copy_file/></file>
	<file><physical_name>runner1.bin</physical_name>
	    <copy_file/>
	    <main_program/>
	</file>
</version>
```

**Note 1**: We highly recommends to use **statically compiled** binaries
to prevent any dependency problems on hosta stations.
All Fitcrack-related binaries can be compiled statically.

**Note 2:** With changes, the file name must also change. This is why use the
numbering in the filename, e.g.
`hashcat1.bin` stands for hashcat binary for BOINC app version 1.


## Create Fitcrack-specific MySQL tables, triggers, and data

Import the `sql/*` scripts into the  `<your_project_name>` MySQL database, e.g.:
```
mysql -h $DB_HOST -u fitcrack -D"$DB_NAME" < server/sql/10_create_tables.sql
mysql -h $DB_HOST -u $DB_USER -D"$DB_NAME" < server/sql/20_create_triggers.sql
mysql -h $DB_HOST -u $DB_USER -D"$DB_NAME" < server/sql/30_insert_data.sql
```


## Note: If phpmyadmin does not work, use this command (works on Ubuntu 16.04):
```
sudo ln -s /usr/share/phpmyadmin /var/www/html
```


## Start the server
```
cd ~/projects/<your_project_name>/

./bin/xadd
./bin/update_versions
./bin/start
./bin/status
```

Done. Continue with Web-Admin installation guide to use the installed system through web browser.
