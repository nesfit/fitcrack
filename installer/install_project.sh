#!/bin/bash

# Make Fitcrack project
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

# Adding apache user to BOINC user Group
echo "Adding user $APACHE_USER to $BOINC_GROUP group..."
usermod -a -G $BOINC_GROUP $APACHE_USER

####################################
# Make project and create database #
####################################

SERVER_HOSTNAME=$(hostname)

# Replace "localhost" with "127.0.0.1"
# (as BOINC clients cannot resolve localhost)
BOINC_URL=${BOINC_URL/localhost/127\.0\.0\.1}

# Make project
echo "Making BOINC project."
./boinc/tools/make_project \
 --no_query \
 --url_base $BOINC_URL \
 --srcdir boinc \
 --user_name $BOINC_USER \
 --project_host $SERVER_HOSTNAME \
 --project_root "$BOINC_HOME/projects/$BOINC_PROJECT" \
 --db_host $DB_HOST \
 --db_name $DB_NAME \
 --db_user $DB_USER \
 --db_passwd $DB_PW \
 $BOINC_PROJECT

# Create htaccess file for PROJECT OPS
if [ -z ${OPS_LOGIN+x} ]; then
  read -e -p "Enter user name for accessing ${BOINC_URL}/${BOINC_PROJECT}_ops (default: $BOINC_USER)" OPS_LOGIN
  OPS_LOGIN=${OPS_LOGIN:-$BOINC_USER}
fi

if [ -z ${OPS_PW+x} ]; then
  read -e -p "Enter user name for accessing ${BOINC_URL}/${BOINC_PROJECT}_ops (default: mypassword)" OPS_PW
  OPS_PW=${OPS_PW:-mypassword}
fi

htpasswd -cb $BOINC_PROJECT_DIR/html/ops/.htpasswd "$OPS_LOGIN" "$OPS_PW"

##################################
# Copy Fitcrack apps and daemons #
##################################

# Copy server measure script
cp -f server/server_bin/measureUsage.py $BOINC_PROJECT_DIR/bin/

# Copy pcfg-monitor script
cp -f server/server_bin/pcfg_monitor.py $BOINC_PROJECT_DIR/bin/

# Copy client binaries
mkdir $BOINC_PROJECT_DIR/apps/fitcrack
mkdir $BOINC_PROJECT_DIR/apps/fitcrack/1
cp -Rf server/client_bin/* $BOINC_PROJECT_DIR/apps/fitcrack/1/

# Install server daemons
source installer/install_daemons.sh

# Add app app_versions
MYDIR=$(pwd)
cd $BOINC_PROJECT_DIR
sudo -u $BOINC_USER ./bin/xadd
sudo -u $BOINC_USER ./bin/update_versions -y
cd $MYDIR

#######################################
# Include BOINC HTML in Apache config #
#######################################

# Check if BOINC is included in Apache config
cat $APACHE_CONFIG_FILE | grep $PROJECT_HTTPD_CONF >/dev/null 2>/dev/null

if [[ $? != 0 ]]; then
  echo "Adding $PROJECT_HTTPD_CONF Include to $APACHE_CONFIG_FILE ..."
  echo "IncludeOptional $PROJECT_HTTPD_CONF" >> $APACHE_CONFIG_FILE
  echo "Added."
else
  echo "Reference to $PROJECT_HTTPD_CONF already included in $PROJECT_HTTPD_CONF."
fi

# Restart Apache service
service_restart $APACHE_SERVICE

#############################
# Install Fitcrack database #
#############################
export MYSQL_PWD="$DB_PW"
echo "Creating Fitcrack tables..."
mysql -h $DB_HOST -u $DB_USER -D"$DB_NAME" < server/sql/10_create_tables.sql
if [[ $? != 0 ]]; then
  echo "Error: Unable to create Fitcrack DB tables"
  exit
fi
echo "Fitcrack tables created."

echo "Creating Fitcrack database triggers..."
mysql -h $DB_HOST -u $DB_USER -D"$DB_NAME" < server/sql/20_create_triggers.sql
if [[ $? != 0 ]]; then
  echo "Error: Unable to create Fitcrack DB tables"
  exit
fi
echo "Fitcrack database triggers created."

echo "Inserting initial data..."
mysql -h $DB_HOST -u $DB_USER -D"$DB_NAME" < server/sql/30_insert_data.sql
if [[ $? != 0 ]]; then
  echo "Error: Unable to create Fitcrack DB tables"
  exit
fi
echo "Initial data inserted."

# Install startup scripts
if [ -z ${SERVICE_INSTALL+x} ]; then
  echo "Adding Fitcrack as a service runs the daemons automatically on startup."
  read -e -p "Add Fitcrack as a system service? [y/N] (default: y)" SERVICE_INSTALL
  SERVICE_INSTALL=${SERVICE_INSTALL:-y}
else
  SERVICE_INSTALL='y'
fi

if [ $SERVICE_INSTALL = "y" ]; then
  # Add startup script
  cp -f installer/init/fitcrack /etc/init.d/fitcrack
  chmod +x /etc/init.d/fitcrack
  # Add runlevel symlinks
  case $DISTRO_ID in
    debian|ubuntu)
      update-rc.d fitcrack defaults
    ;;
    centos|redhat|rocky)
      chkconfig --add fitcrack
      chkconfig --level 2345 fitcrack on
    ;;
    suse|linux)
    ;;
  esac
fi
