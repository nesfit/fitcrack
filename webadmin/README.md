# Instalation

Install dependencies

    sudo apt-get install -y apache2 libapache2-mod-wsgi-py3 python3 python3-pip 

Active apache2 mods

    sudo a2enmod wsgi 
    sudo a2enmod rewrite


Copy backend sources to `var/www`

    sudo cp -a fitcrackAPI/. /var/www/fitcrackAPI


Copy frontend files to `var/www/html`
    
    sudo cp -a fitcrackFE/dist/. /var/www/html


Install backend dependencies

    cd /var/www/fitcrackAPI/src
    sudo pip3 install -r requirements.txt 


Change `/etc/apache2/sites-available/000-default.conf` to:

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


Restart apache

    sudo service apache2 restart


Go to `http://localhost`




# Configuration

To configure Frontend change `var/www/html/static/configuration.js`. You can set backend URL here. The default is set to:
    
    window.serverAddress = 'http://localhost:5000'


Backend configuration is stored in `var/www/fitcrackAPI/src/settings.py`. You will probably need to change Database URI. The default is:

    SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://root@localhost/fitcrack'

Sytax if database URI is:

    mysql+pymysql://[user]:[password]@[server]/[database]




# Logs

For logs check `var/log/apache2/error.log`.




# Frontend development

Install latest [node.js](https://nodejs.org/en/download/) with npm. Than go to the frontend folder and install frontend dependencies:

    npm install

Run develop server with hot reload at localhost

    npm run dev

Build application for production 

    npm run build

New production build is in `fitcrakFE/dist`
