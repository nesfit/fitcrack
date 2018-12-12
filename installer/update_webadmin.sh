#!/bin/bash

# Update WebAdmin
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

echo "                                                          "
echo "=============== Fitcrack WebAdmin update ================="
echo "                                                          "

###################################################
# Update Fitcrack WebAdmin front-end (fitcrackFE) #
###################################################

INSTALL_FRONTEND="N"
if [ -d "$APACHE_DOCUMENT_ROOT/fitcrackFE" ]; then
  echo "WebAdmin front-end is installed in $APACHE_DOCUMENT_ROOT/fitcrackFE."
  read -e -p "Update front-end? [y/N] (default: y): " UPDATE_FRONTEND
  UPDATE_FRONTEND=${UPDATE_FRONTEND:-y}
  if [ $UPDATE_FRONTEND = "y" ]; then
    rm -Rf $APACHE_DOCUMENT_ROOT/fitcrackFE
    INSTALL_FRONTEND="y"
  fi
else
  echo "Unable to find WebAdmin front-end installation."
  exit
fi

# Install front-end
if [ $INSTALL_FRONTEND = "y" ]; then
  echo "Updating Fitcrack WebAdmin front-end..."
  mkdir $APACHE_DOCUMENT_ROOT/fitcrackFE
  cp -Rf webadmin/fitcrackFE/dist/* $APACHE_DOCUMENT_ROOT/fitcrackFE/

  # Set permissions and ownership to Apache user and group
  chmod -R 775 $APACHE_DOCUMENT_ROOT/fitcrackFE
  chown -R $APACHE_USER:$APACHE_GROUP $APACHE_DOCUMENT_ROOT/fitcrackFE

  echo "Front-end files updated."
  echo "Configuring updated front-end..."

  # Set port to backend
  sed -i "s|http://localhost:5000|http://localhost:$BACKEND_PORT|g" $APACHE_DOCUMENT_ROOT/fitcrackFE/static/configuration.js
  echo "Done."

  echo "Updated front-end in $APACHE_DOCUMENT_ROOT/fitcrackFE."
fi


###################################################
# Update Fitcrack WebAdmin back-end (fitcrackAPI) #
###################################################

INSTALL_BACKEND="N"
if [ -d "$APACHE_DOCUMENT_ROOT/fitcrackAPI" ]; then
  echo "WebAdmin backend-end is installed in $APACHE_DOCUMENT_ROOT/fitcrackAPI."
  read -e -p "Update back-end? [y/N] (default: y): " UPDATE_BACKEND
  UPDATE_BACKEND=${UPDATE_BACKEND:-y}
  if [ $UPDATE_BACKEND = "y" ]; then
    rm -Rf $APACHE_DOCUMENT_ROOT/fitcrackAPI
    INSTALL_BACKEND="y"
  fi
else
  echo "Unable to find WebAdmin back-end installation."
  exit
fi

# Install backend-end
if [ $INSTALL_BACKEND = "y" ]; then
  echo "Updating Fitcrack WebAdmin back-end..."
  mkdir $APACHE_DOCUMENT_ROOT/fitcrackAPI
  cp -Rf webadmin/fitcrackAPI/* $APACHE_DOCUMENT_ROOT/fitcrackAPI/

  # Set permissions and ownership to Apache user and group
  chmod -R 775 $APACHE_DOCUMENT_ROOT/fitcrackAPI
  chown -R $APACHE_USER:$APACHE_GROUP $APACHE_DOCUMENT_ROOT/fitcrackAPI

  echo "Back-end files updated in $APACHE_DOCUMENT_ROOT/fitcrackAPI."

  echo "Configuring updated backend-end..."
  # Set credentials
  sed -i "s|PROJECT_USER = '.*|PROJECT_USER = '$BOINC_USER'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
  sed -i "s|PROJECT_NAME = '.*|PROJECT_NAME = '$BOINC_PROJECT'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
  sed -i "s|FLASK_SERVER_NAME = '.*|FLASK_SERVER_NAME = 'localhost:$BACKEND_PORT'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
  sed -i "s|SQLALCHEMY_DATABASE_URI = '.*|SQLALCHEMY_DATABASE_URI = 'mysql+pymysql://$DB_USER:$DB_PW@$DB_HOST/$DB_NAME'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py
  sed -i "s|BOINC_SERVER_URI = '.*|BOINC_SERVER_URI = '$BOINC_URL'|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/settings.py

  # Set port to backend
  sed -i "s|sys.path.insert(0.*|sys.path.insert(0,\"$APACHE_DOCUMENT_ROOT/fitcrackAPI/src/\")|g" $APACHE_DOCUMENT_ROOT/fitcrackAPI/src/wsgi.py
  echo "Done."
fi

##################
# Restart Apache #
##################

echo "WebAdmin update done. Restarting Apache..."
systemctl restart $APACHE_SERVICE
echo "Done."
