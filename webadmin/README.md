# Installation

## Install dependencies

On Debian/Ubuntu systems:
```
apt install -y apache2 libapache2-mod-wsgi-py3 python3 python3-pip
a2enmod rewrite   # enable mod rewrite
a2enmod wsgi      # enable mod wsgi
systemctl restart apache2
```

On Centos/RHEL:
```
yum install -y python3-devel python3 python3-pip python3-mod_wsgi
pip3 install mysqlclient
```


## Install backend dependencies

Install backend dependencies
```  
cd /var/www/fitcrackAPI/src
sudo pip3 install -r requirements.txt
```


## Build tools used by WebAdmin
```
cd webadmin/fitcrackAPI/hashcat-utils/src && make
cd webadmin/fitcrackAPI/xtohashcat/scripts && make
cd webadmin/fitcrackAPI/pwd_dist && make
```


## Build the WebAdmin frontend for production

Install latest [node.js](https://nodejs.org/en/download/), e.g.:
```
wget https://nodejs.org/dist/v16.15.0/node-v16.15.0-linux-x64.tar.xz \
&& tar -xJvf node-v16.15.0-linux-x64.tar.xz -C /usr/local/lib/nodejs
```

Install frontend dependencies:
```
export PATH=/usr/local/lib/nodejs/node-v16.15.0-linux-x64/bin:$PATH
cd webadmin/fitcrackFE
npm run ci
```

Build frontend for production:
```
npm run build
```

New production build is in `fitcrakFE/dist`


## Copy backend and frontend files to Apache directories

Copy backend sources to `/var/www`

```
cp -a fitcrackAPI/. /var/www/fitcrackAPI
```

Copy frontend files to `var/www/html`

```  
cp -a fitcrackFE/dist/. /var/www/html
```  


## Register webadmin with Apache

Change `/etc/apache2/sites-available/000-default.conf` to:

```
Listen 5000
<VirtualHost *:5000>

	 WSGIDaemonProcess fitcrack user=boincadm group=boincadm threads=5
	 WSGIScriptAlias / /var/www/fitcrackAPI/src/wsgi.py

	<Directory /var/www/fitcrackAPI/src/>
		 WSGIProcessGroup fitcrack
		 WSGIApplicationGroup %{GLOBAL}
		 WSGIScriptReloading On
		 Require all granted
	</Directory>
</VirtualHost>

Listen 80
<VirtualHost *:80>
	DocumentRoot /var/www/html
	<Directory /var/www/html/>
		RewriteEngine On
		RewriteBase /
		RewriteRule ^index\.html$ - [L]
		RewriteCond %{REQUEST_FILENAME} !-f
		RewriteCond %{REQUEST_FILENAME} !-d
		RewriteRule . /index.html [L]
	</Directory>
</VirtualHost>
```
Or you can create separate **VirtualHosts** for backend and frontend
if you want.


Restart apache

```
service apache2 restart
```
Go to `http://YOUR-HOSTNAME`, e.g. `http://localhost`.


# Configuration

To configure Frontend, change `var/www/html/static/configuration.js`. You can set backend URL here. The default is set to:
```
window.serverAddress = 'http://localhost:5000'
```

Backend configuration is stored in `var/www/fitcrackAPI/src/settings.py`. You will probably need to change Database URI. The default is:
```
SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://root@localhost/fitcrack'
```

Sytax if database URI is:
```
mysql+pymysql://[user]:[password]@[server]/[database]
```


## Install collections
Fitcrack server works with various resources (dictionaries, mask files, etc.).
Those are by default stored under the `/usr/share/collections` directory.
You should thus create it populate it:
```
mkdir /usr/share/collections
mkdir /usr/share/collections/import
cp -R collections/charsets /usr/share/collections/
cp -R collections/dictionaries /usr/share/collections/
cp -R collections/markov /usr/share/collections/
cp -R collections/encrypted-files /usr/share/collections/
cp -R collections/masks /usr/share/collections/
cp -R collections/rules /usr/share/collections/
cp collections/pcfg.tar.gz /usr/share/collections/

cd /usr/share/collections/
tar xzvf pcfg.tar.gz
rm pcfg.tar.gz

chown -R www-data:www-data /usr/share/collections
chmod -R 777 /usr/share/collections
```
(You can also set stricter permissions if both BOINC server user
and Apache server user have access to it.)


# Logs

For logs check `var/log/apache2/error.log`.
On CentOS/RHEL system, it is `var/log/httpd/error.log`.
