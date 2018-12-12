#!/bin/bash

# WebAdmin installer
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

##################################
# Install backend prerequisities #
##################################

echo "Installing back-end requirements..."
pip3 install -r webadmin/fitcrackAPI/src/requirements.txt
echo "Done."

####################################
# Prepare Apache for configuration #
####################################

echo "Checking Apache..."
if ! [ -d $APACHE_CONFIG_DIR/sites-enabled ]; then
  echo "Virtualhost directories do not exists, creating..."
  echo "Creating: $APACHE_CONFIG_DIR/sites-available"
  echo "Creating: $APACHE_CONFIG_DIR/sites-enabled"
  mkdir $APACHE_CONFIG_DIR/sites-available
  mkdir $APACHE_CONFIG_DIR/sites-enabled
  echo "Done."

  cat $APACHE_CONFIG_FILE | grep sites-enabled >/dev/null 2>/dev/null

  if [[ $? != 0 ]]; then
    echo "Adding IncludeOptional to sites-enabled."
    echo "IncludeOptional $APACHE_CONFIG_DIR/sites-enabled/*.conf" >> $APACHE_CONFIG_FILE
    echo "Added."
  fi
fi
echo "Apache is prepared for configuration."

###################
# Configure ports #
###################

read -e -p "Enter a port for front-end to listen on (default: 80): " FRONTEND_PORT
FRONTEND_PORT=${FRONTEND_PORT:-80}


#########################
# Get front-end TCP port #
#########################

if lsof -Pi :$FRONTEND_PORT -sTCP:LISTEN -t >/dev/null ; then
  FRONTEND_PORT_FREE="N"
else
  FRONTEND_PORT_FREE="y"
fi

if [ $FRONTEND_PORT_FREE = "N" ]; then
  echo "Port $FRONTEND_PORT is already used."

  if [ $FRONTEND_PORT -eq 80 ] && [ -f "$APACHE_CONFIG_DIR/sites-available/000-default.conf" ] && [ -f "$APACHE_CONFIG_DIR/sites-enabled/000-default.conf" ]; then
    echo "Probably caused by: $APACHE_CONFIG_DIR/sites-available/000-default.conf"
    read -e -p "Replace default site? [y/N] (default: N): " REPLACE_DEFAULT
    REPLACE_DEFAULT=${REPLACE_DEFAULT:-N}

    if [ $REPLACE_DEFAULT = "y" ]; then
      rm -f $APACHE_CONFIG_DIR/sites-enabled/000-default.conf
      rm -f $APACHE_CONFIG_DIR/sites-available/000-default.conf
      FRONTEND_PORT_FREE="y"
    fi
  else
    read -e -p "Ignore and configure anyway? [y/N] (default: N) " FORCE_CONFIGURE
    FORCE_CONFIGURE=${FORCE_CONFIGURE:-N}

    if [ $FORCE_CONFIGURE = "y" ]; then
      FRONTEND_PORT_FREE="y"
    fi
  fi
fi

###########################
# Front-end Apache config #
###########################

if [ $FRONTEND_PORT_FREE = "N" ]; then
  echo "Resolve the problem or try again with another port number!"
  exit
else
  echo "Creating front-end config: $APACHE_CONFIG_DIR/sites-available/fitcrackFE.conf"

  ADD_LISTEN_DIRECTIVE="y"
  cat $APACHE_CONFIG_FILE | grep "^Listen $FRONTEND_PORT" >/dev/null
  if [[ $? = 0 ]]; then
    ADD_LISTEN_DIRECTIVE="N"
  fi
  if [ -f $APACHE_CONFIG_DIR/ports.conf ]; then
    cat $APACHE_CONFIG_DIR/ports.conf | grep "^Listen $FRONTEND_PORT" >/dev/null
    if [[ $? = 0 ]]; then
      ADD_LISTEN_DIRECTIVE="N"
    fi
  fi

  FE_CONFIG_FILE=$APACHE_CONFIG_DIR/sites-available/fitcrackFE.conf
  echo "# Fitcrack WebAdmin front-end config" > $FE_CONFIG_FILE

  if [ $ADD_LISTEN_DIRECTIVE = "y" ]; then
    echo "Listen $FRONTEND_PORT" >> $FE_CONFIG_FILE
  fi

  echo "<VirtualHost *:$FRONTEND_PORT>" >> $FE_CONFIG_FILE
  echo "  DocumentRoot $APACHE_DOCUMENT_ROOT/fitcrackFE" >> $FE_CONFIG_FILE
  echo "  <Directory $APACHE_DOCUMENT_ROOT/fitcrackFE/>" >> $FE_CONFIG_FILE
  echo "    RewriteEngine On" >> $FE_CONFIG_FILE
  echo "    RewriteBase /" >> $FE_CONFIG_FILE
  echo "    RewriteRule ^index\.html$ - [L]" >> $FE_CONFIG_FILE
  echo "    RewriteCond %{REQUEST_FILENAME} !-f" >> $FE_CONFIG_FILE
  echo "    RewriteCond %{REQUEST_FILENAME} !-d" >> $FE_CONFIG_FILE
  echo "    RewriteRule . /index.html [L]" >> $FE_CONFIG_FILE
  echo "  </Directory>" >> $FE_CONFIG_FILE
  echo "</VirtualHost>" >> $FE_CONFIG_FILE

  echo "Creating a symlink: $APACHE_CONFIG_DIR/sites-enabled/fitcrackFE.conf"
  ln -sf $APACHE_CONFIG_DIR/sites-available/fitcrackFE.conf $APACHE_CONFIG_DIR/sites-enabled/fitcrackFE.conf
  echo "Front-end Apache configuration done"
fi

#########################
# Get back-end TCP port #
#########################

read -e -p "Enter a port for back-end to listen on (default: 5000): " BACKEND_PORT
BACKEND_PORT=${BACKEND_PORT:-5000}

if lsof -Pi :$BACKEND_PORT -sTCP:LISTEN -t >/dev/null ; then
  BACKEND_PORT_FREE="N"
else
  BACKEND_PORT_FREE="y"
fi

if [ $BACKEND_PORT_FREE = "N" ]; then
  echo "Port $BACKEND_PORT_FREE is already used."
  read -e -p "Ignore and configure anyway? [y/N] (default: N) " FORCE_CONFIGURE
  FORCE_CONFIGURE=${FORCE_CONFIGURE:-N}

  if [ $FORCE_CONFIGURE = "y" ]; then
    BACKEND_PORT_FREE="y"
  fi
fi

###########################
# Back-end Apache config #
###########################

if [ $BACKEND_PORT_FREE = "N" ]; then
  echo "Resolve the problem or try again with another port number!"
  exit
else
  echo "Creating back-end config: $APACHE_CONFIG_DIR/sites-available/fitcrackAPI.conf"

  ADD_LISTEN_DIRECTIVE="y"
  cat $APACHE_CONFIG_FILE | grep "^Listen $BACKEND_PORT" >/dev/null
  if [[ $? = 0 ]]; then
    ADD_LISTEN_DIRECTIVE="N"
  fi
  if [ -f $APACHE_CONFIG_DIR/ports.conf ]; then
    cat $APACHE_CONFIG_DIR/ports.conf | grep "^Listen $BACKEND_PORT" >/dev/null
    if [[ $? = 0 ]]; then
      ADD_LISTEN_DIRECTIVE="N"
    fi
  fi

  BE_CONFIG_FILE=$APACHE_CONFIG_DIR/sites-available/fitcrackAPI.conf
  echo "# Fitcrack WebAdmin back-end config" > $BE_CONFIG_FILE

  if [ $ADD_LISTEN_DIRECTIVE = "y" ]; then
    echo "Listen $BACKEND_PORT" >> $BE_CONFIG_FILE
  fi

  echo "<VirtualHost *:$BACKEND_PORT>" >> $BE_CONFIG_FILE
  echo "  WSGIDaemonProcess fitcrack user=$APACHE_USER group=$APACHE_USER threads=5" >> $BE_CONFIG_FILE
  echo "  WSGIScriptAlias / $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/wsgi.py" >> $BE_CONFIG_FILE
  echo "  <Directory $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/>" >> $BE_CONFIG_FILE
  echo "    WSGIProcessGroup fitcrack" >> $BE_CONFIG_FILE
  echo "    WSGIApplicationGroup %{GLOBAL}" >> $BE_CONFIG_FILE
  echo "    WSGIScriptReloading On" >> $BE_CONFIG_FILE
  echo "    Require all granted" >> $BE_CONFIG_FILE
  echo "  </Directory>" >> $BE_CONFIG_FILE
  echo "</VirtualHost>" >> $BE_CONFIG_FILE

  echo "Creating a symlink: $APACHE_CONFIG_DIR/sites-enabled/fitcrackAPI.conf"
  ln -sf $APACHE_CONFIG_DIR/sites-available/fitcrackAPI.conf $APACHE_CONFIG_DIR/sites-enabled/fitcrackAPI.conf
  echo "Front-end Apache configuration done"
fi

##########################################################
# Install Fitcrack WebAdmin front-end (fitcrackFE) files #
##########################################################

if [ -d "$APACHE_DOCUMENT_ROOT/fitcrackFE" ]; then
  echo "Front-end already seems to be installed in $APACHE_DOCUMENT_ROOT/fitcrackFE."
  read -e -p "Reinstall front-end? [y/N] (default: N): " REINSTALL_FRONTEND
  REINSTALL_FRONTEND=${REINSTALL_FRONTEND:-N}
  if [ $REINSTALL_FRONTEND = "y" ]; then
    rm -Rf $APACHE_DOCUMENT_ROOT/fitcrackFE
    INSTALL_FRONTEND="y"
  else
    INSTALL_FRONTEND="N"
  fi
else
  INSTALL_FRONTEND="y"
fi

# Install front-end
if [ $INSTALL_FRONTEND = "y" ]; then
  echo "Installing Fitcrack WebAdmin front-end..."
  mkdir $APACHE_DOCUMENT_ROOT/fitcrackFE
  cp -Rf webadmin/fitcrackFE/dist/* $APACHE_DOCUMENT_ROOT/fitcrackFE/

  # set permissions to Apache user and group

  # Set permissions and ownership to Apache user and group
  chmod -R 775 $APACHE_DOCUMENT_ROOT/fitcrackFE
  chown -R $APACHE_USER:$APACHE_GROUP $APACHE_DOCUMENT_ROOT/fitcrackFE

  echo "Installed to $APACHE_DOCUMENT_ROOT/fitcrackFE."
fi

#########################################################
# Install Fitcrack WebAdmin back-end (fitcrackAPI) file #
#########################################################

if [ -d "$APACHE_DOCUMENT_ROOT/fitcrackAPI" ]; then
  echo "Back-end already seems to be installed in $APACHE_DOCUMENT_ROOT/fitcrackAPI."
  read -e -p "Reinstall back-end? [y/N] (default: N): " REINSTALL_BACKEND
  REINSTALL_BACKEND=${REINSTALL_BACKEND:-N}
  if [ $REINSTALL_BACKEND = "y" ]; then
    rm -Rf $APACHE_DOCUMENT_ROOT/fitcrackAPI
    INSTALL_BACKEND="y"
  else
    INSTALL_BACKEND="N"
  fi
else
  INSTALL_BACKEND="y"
fi

# Install front-end
if [ $INSTALL_BACKEND = "y" ]; then
  echo "Installing Fitcrack WebAdmin back-end..."
  mkdir $APACHE_DOCUMENT_ROOT/fitcrackAPI
  cp -Rf webadmin/fitcrackAPI/* $APACHE_DOCUMENT_ROOT/fitcrackAPI/

  # Set permissions and ownership to Apache user and group
  chmod -R 775 $APACHE_DOCUMENT_ROOT/fitcrackAPI
  chown -R $APACHE_USER:$APACHE_GROUP $APACHE_DOCUMENT_ROOT/fitcrackAPI

  echo "Installed to $APACHE_DOCUMENT_ROOT/fitcrackAPI."
fi

#######################
# Configure front-end #
#######################

echo "Configuring front-end..."
# Set port to backend
sed -i "s|http://localhost:5000|http://localhost:$BACKEND_PORT|g" $APACHE_DOCUMENT_ROOT/fitcrackFE/static/configuration.js
echo "Done."

#######################
# Configure back-end #
#######################

echo "Configuring backend-end..."
# Set credentials
sed -i "s|PROJECT_USER = '.*|PROJECT_USER = '$BOINC_USER'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
sed -i "s|PROJECT_NAME = '.*|PROJECT_NAME = '$BOINC_PROJECT'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
sed -i "s|FLASK_SERVER_NAME = '.*|FLASK_SERVER_NAME = 'localhost:$BACKEND_PORT'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
sed -i "s|SQLALCHEMY_DATABASE_URI = '.*|SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://$DB_USER:$DB_PW@$DB_HOST/$DB_NAME'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
sed -i "s|BOINC_SERVER_URI = '.*|BOINC_SERVER_URI = '$BOINC_URL'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py

# Set port to backend
sed -i "s|sys.path.insert(0.*|sys.path.insert(0,\"$APACHE_DOCUMENT_ROOT/fitcrackAPI/src/\")|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/wsgi.py
echo "Done."

##################
# Restart Apache #
##################

echo "WebAdmin installed. Restarting Apache..."
systemctl restart $APACHE_SERVICE
echo "Done."
