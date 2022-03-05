# Fitcrack (Distributed password cracking system)

Note: A detailed documentation and other information can be found at [https://fitcrack.fit.vutbr.cz](https://fitcrack.fit.vutbr.cz)

This README describes how to install and run Fitcrack distributed password cracking system.

## Step-by-step: Install on Ubuntu 20.04 LTS

Open a **root** terminal, go to the directory with Fitcrack sources and proceed as follows.

### Install prerequisities
```
apt-get install m4 make dh-autoreconf pkg-config git vim apache2 libapache2-mod-php mysql-server mysql-common libmysqlclient-dev zlibc zlib1g zlib1g-dev php php-xml php-mysql php-cli php-gd python-is-python3 python3 python3-mysqldb python3-pymysql python3-pip libapache2-mod-wsgi-py3 libssl-dev libcurl4-openssl-dev apache2-utils pkg-config libnotify-dev curl perl libcompress-raw-lzma-perl

mysql_secure_installation # Set MySQL root password

a2enmod cgi       # enable mod CGI
a2enmod rewrite   # enable mod rewrite
a2enmod wsgi      # enable mod wsgi
systemctl restart apache2
```
### Setup user and Database
```
useradd -m -c "BOINC Administrator" boincadm  -s /bin/bash
mysql -u root -p
mysql> create database fitcrack;
mysql> CREATE USER 'fitcrack'@'localhost' IDENTIFIED BY 'mypassword';
mysql> GRANT ALL ON fitcrack.* TO 'fitcrack'@'localhost';
mysql> SET GLOBAL log_bin_trust_function_creators = 1;
```

### Install Fitcrack
```
./install_fitcrack.sh
```
You can read about the "expose to internet" option [here](#about-webadmin-security).

<a name="oper"></a>
## Operating the server

The **default** login credentials to the WebAdmin are:
* login: ``fitcrack``
* password: ``FITCRACK``

### About WebAdmin security
When WebAdmin authenticates with the backend, cross-origin cookies are used. As browsers tightened their security policies, this method now **requires the connection to be encrypted (HTTPS)**.

To get around this when you just want to test out fitcrack, we introduced an alternative (localstorage workaround) authentication method using [JWT](https://jwt.io). This enables auth over HTTP, but it **is not secure** and should **never ever** be used for installations accessible over the internet.

When you set the installation as exposed to the internet during installation, the JWT login flow will be disabled. Which one is used is completely transparent to the user, just remeber that you'll need HTTPS to use WebAdmin then.


To change this decision later, modify [this line](webadmin/fitcrackAPI/src/settings.py#L46) in `settings.py`.

### Server daemons

At the end of the installation, the installer asks you if you wish to start
the server daemons. If you decide not to, you can do that manually as described below:

## Running the server

Login to the server as BOINC user (e.g. `boincadm`) and enter the project directory:
```
boincadm@myserver:~$ cd projects/fitcrack
```

### Get server status
```
boincadm@myserver:~/projects/fitcrack$ ./bin/status
```

### Start Fitcrack server
```
boincadm@myserver:~/projects/fitcrack$ ./bin/start
```

### Stop Fitcrack server
```
boincadm@myserver:~/projects/fitcrack$ ./bin/stop
```

#### Advanced: Host specific configuration
Sometimes you need to configure specific behavior for individual hosts.
Fitcrack allows you to define additional **host-specific hashcat arguments**.
This gives you options to select what GPUs to use for cracking,
configure the workload profile for fine-tuning of the performance, etc.
Simply create a file `/etc/fitcrack.conf` (Linux hosts) or 
`C:\ProgramData\BOINC\fitcrack.conf` (Windows hosts).
If the file exists and is readable for the BOINC client user, the
**Runner** subsystem will append the contents to **hashcat's arguments**.

##### Example
```
$ echo '-w 4 -d 1,2 --force' > /etc/fitcrack.conf
```
Hashcat will use OpenCL devices 1 and 2. The workload profile will be set to level 4 (Nightmare).
The cracking session will be forced and all warnings ignored.

