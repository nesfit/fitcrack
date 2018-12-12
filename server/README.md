# Fitcrack server README

This readme describes a non-trivial process of manual Fitcrack server instllation.
We **highly recommend** to use the [installer](../README.md) instead.

# Install necessary packages

* make (3.79+)
* m4 (1.4+)
* libtool (1.5+)
* autoconf (2.58+)
* automake (1.8+)
* GCC (6.3.0+)
* pkg-config (0.15+)
* Python 2 (2.2+)
* Python 3
* pip for Python 3
* MySQL (4.0.9+) or MariaDB (10.0+)
* libnotify-dev
* Apache with the following modules:
  * PHP (5+) with XML and MySQL modules
  * CGI
  * WSGI (for WebAdmin)
  * rewrite
* PHP5 with cli support and the GD and MySQL modules
* OpenSSL (0.98+)

Or use the distro-specific packages below...

## Ubuntu 16.04 LTS commands
```
# Note: You might want to use newer version of php* packages
sudo apt install m4 make dh-autoreconf pkg-config git vim libapache2-mod-php5 libmysqlclient-dev php5-mysql php5-cli php5-gd python python-mysqldb libssl-dev libcurl4-openssl-dev apache2-utils libboost1.62-all-dev
sudo apt install mysql-server-5.5
sudo apt install phpmyadmin (Configure database with dbconfig-common - YES)
```

## Ubuntu 18.04 LTS / Debian 6 commands
```
apt-get install m4 make dh-autoreconf pkg-config git vim apache2 libapache2-mod-php mysql-server mysql-common libmysqlclient-dev zlibc zlib1g zlib1g-dev php php-xml php-mysql php-cli php-gd python python python3 python-mysqldb python3-pymysql python3-pip libapache2-mod-wsgi-py3 libssl-dev libcurl4-openssl-dev apache2-utils libboost1.62-all-dev pkg-config libnotify-dev

mysql_secure_installation # Set MySQL root password

a2enmod cgi       # enable mod CGI
a2enmod rewrite   # enable mod rewrite
a2enmod wsgi      # enable mod wsgi
systemctl restart apache2
```

## CentOS/RHEL 7 commands
```
yum install -y https://$(rpm -E '%{?centos:centos}%{!?centos:rhel}%{rhel}').iuscommunity.org/ius-release.rpm
yum install -y epel-release centos-release-scl
yum install -y devtoolset-7 m4 libtool autoconf automake  git vim httpd php php-mysql mod_wsgi mariadb-server mariadb-devel zlib libcurl-devel openssl-libs python python36 python36u-mod_wsgi python36u-setuptools  MySQL-python python2-PyMySQL  boost* pkgconfig libnotify
alternatives --install /usr/bin/python3 python3 /usr/bin/python3.6 60
easy_install-3.6 pip
systemctl start httpd.service
systemctl enable httpd.service
systemctl start mariadb
mysql_secure_installation # Set MariaDB root password
systemctl enable mariadb.service
```


# Create necessary users and groups
```
useadd -s /bin/bash -m boincadm
sudo addgroup boincadm
sudo usermod -a -G boincadm www-data
```

# Get BOINC server resources
* Copy the source directory ``../boinc/`` to  ``/home/boincadm/boinc-src/``
* Or get the sources from GIT: ``cd ~ && git clone https://github.com/BOINC/boinc.git boinc-src``

# Copy fitcrack sporces to BOINC source directory
```
./update_server.sh
```

# Apply Fitcrack patches to BOINC Makefiles
```
cd src/patches
./patch_makefile.sh
```

# Configure and build BOINC
```
cd ~/boinc-src
./_autosetup
./configure --disable-client --disable-manager
make
```

# Install libboinc
```
sudo make install
```

# Make and configure the Fitcrack project
```
cd ~/boinc-src/tools

# Create the project:
# --url_base is the IP address of your server
# --test_app will be the name of the application,
# --db_user is the mysql username (root)
# --db_passwd is the mysql root password

./make_project --url_base http://*your_server_ip* --db_user root --db_passwd *root_password* --test_app fitcrack
```

# Allow Apache access to Scheduler CGI scripts
```
sudo bash -c 'cat /home/boincadm/projects/*your_project_name*/*your_project_name*.httpd.conf >> /etc/apache2/apache2.conf'

sudo vim /etc/apache2/apache2.conf
:%s/Allow from all/Allow from all\r        Require all granted/g
:%s/#ServerRoot "\/etc\/apache2"/#ServerRoot "\/etc\/apache2"\rServerName test.local/g
:wq

sudo /usr/sbin/apache2ctl restart
```

# Secure the BOINC web (ops) administration:
```
htpasswd -cb ~/projects/*your_project_name*/html/ops/.htpasswd *your_username* *your_password*
vim /home/boincadm/projects/*your_project_name*/html/project/project.inc
:%s/<master_url>/http:\/\/*your_server_IP*\//g
:wq
cd /home/boincadm/projects/*your_project_name*/
sudo a2enmod cgi
sudo /usr/sbin/apache2ctl restart
```

# Edit project config
```
vim /home/boincadm/projects/*your_project_name*/config.xml
:%s/    <\/config>/        <enable_assignment>1<\/enable_assignment>\r        <next_rpc_delay>10<\/next_rpc_delay>\r        <maintenance_delay>60<\/maintenance_delay>\r        <max_wus_in_progress>1<\/max_wus_in_progress>\r        <max_ncpus>1<\/max_ncpus>\r        <prefer_primary_platform>1<\/prefer_primary_platform>\r        <min_sendwork_interval>6<\/min_sendwork_interval>\r    <\/config>\r/g
:%s/example_app/fitcrack/g
:%s/sample_work_generator -d 3/work_generator -d 3 --app fitcrack/g
:%s/sample_assimilator/assimilator/g
:%s/sample_bitwise_validator/bitwise_validator/g
:%s/</daemons>/    <daemon>\r            <cmd>trickler --variety fitcrack</cmd>\r        </daemon>\r    </daemons>/g
:wq
```

# Copy Fitcrack templates and project.xml
```
cd ~
cp templates/* /home/boincadm/projects/*your_project_name*/templates/
cp project.xml /home/boincadm/projects/*your_project_name*/
```

# Delete default application versions:
```
rm -r ~/projects/*your_project_name*/apps/example_app/*
```

# See the binary-creation info:
```
# Copy binaries of your host aplication to
# /home/boincadm/project/*your_project_name*/app/fitcrack/*version_number*/
# *version_number* directory has to contain subfolder named as targeted
# platform. All platforme detected by BOINC can
# be found at https://boinc.berkeley.edu/trac/wiki/BoincPlatforms. Each
# subfolder has to contain all of the files even
# those platform not-specific (archive etc.). Also there has to be version.xml
# with files specified for more info look
# at https://boinc.berkeley.edu/trac/wiki/AppVersionNew.
# When you have files prepared like this you can call ./bin/update_versions
# script which then adds the new version to
# to the system and stages it for sending to clients.
```

# Create the client-side binaries
```
mkdir /home/boincadm/projects/*your_project_name*/apps/fitcrack/1/
cp -r client_bin/* /home/boincadm/projects/*your_project_name*/apps/fitcrack/1/
```

# Create Fitcrack-specific MySQL tables
```
# import the sql/*` scripts into MySQL *your_project_name* database
```
# Note: If phpmyadmin does not work, use this command (works on Ubuntu 16.04):
```
sudo ln -s /usr/share/phpmyadmin /var/www/html
```

# Start the server
```
cd ~/projects/*your_project_name*/

./bin/xadd
./bin/update_versions
./bin/start
./bin/status
```

Done. Continue with Web-Admin installation guide to use the installed system through web browser.
