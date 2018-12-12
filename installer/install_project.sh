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

# Make project
echo "Making BOINC project."
./boinc/tools/make_project \
 --url_base $BOINC_URL \
 --srcdir boinc \
 --user_name $BOINC_USER \
 --project_root "$BOINC_HOME/projects/$BOINC_PROJECT" \
 --db_host $DB_HOST \
 --db_name $DB_NAME \
 --db_user $DB_USER \
 --db_passwd $DB_PW \
 $BOINC_PROJECT

# Create htaccess file for PROJECT OPS
read -e -p "Enter user name for accessing ${BOINC_URL}/${BOINC_PROJECT}_ops (default: $BOINC_USER)" OPS_LOGIN
OPS_LOGIN=${OPS_LOGIN:-$BOINC_USER}

read -e -p "Enter user name for accessing ${BOINC_URL}/${BOINC_PROJECT}_ops (default: mypassword)" OPS_PW
OPS_PW=${OPS_PW:-mypassword}

htpasswd -cb $BOINC_PROJECT_DIR/html/ops/.htpasswd "$OPS_LOGIN" "$OPS_PW"

####################################
# Copy Fitcrack apps and templates #
####################################

# Copy server daemon binaries
cp -Rf boinc/sched/work_generator $BOINC_PROJECT_DIR/bin/
cp -Rf boinc/sched/assimilator $BOINC_PROJECT_DIR/bin/
cp -Rf boinc/sched/bitwise_validator $BOINC_PROJECT_DIR/bin/
cp -Rf boinc/sched/trickler $BOINC_PROJECT_DIR/bin/

# Copy client binaries
mkdir $BOINC_PROJECT_DIR/apps/fitcrack
mkdir $BOINC_PROJECT_DIR/apps/fitcrack/1
cp -Rf server/client_bin/* $BOINC_PROJECT_DIR/apps/fitcrack/1/

# Copy Fitcrack communication templates
cp -Rf server/templates/* $BOINC_PROJECT_DIR/templates/


#####################
# !!! IMPORTANT !!! #
#####################
# Set permissions for BOINC user and BOINC group
chmod -R g+rwx $BOINC_HOME
chown -R $BOINC_USER:$BOINC_GROUP $BOINC_HOME/projects

# Add app app_versions
MYDIR=$(pwd)
cd $BOINC_PROJECT_DIR
sudo -u $BOINC_USER ./bin/xadd
sudo -u $BOINC_USER ./bin/update_versions
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
systemctl restart $APACHE_SERVICE

#############################
# Install Fitcrack database #
#############################

echo "Crating Fitcrack tables..."
mysql -h $DB_HOST -u $DB_USER -p"$DB_PW" -D"$DB_NAME" < server/sql/10_create_tables.sql  2>/dev/null
if [[ $? != 0 ]]; then
  echo "Error: Unable to create Fitcrack DB tables"
  exit
fi
echo "Fitcrack tables created."

echo "Crating Fitcrack database triggers..."
mysql -h $DB_HOST -u $DB_USER -p"$DB_PW" -D"$DB_NAME" < server/sql/20_create_triggers.sql  2>/dev/null
if [[ $? != 0 ]]; then
  echo "Error: Unable to create Fitcrack DB tables"
  exit
fi
echo "Fitcrack database triggers created."

echo "Inserting initial data..."
mysql -h $DB_HOST -u $DB_USER -p"$DB_PW" -D"$DB_NAME" < server/sql/30_insert_data.sql  2>/dev/null
if [[ $? != 0 ]]; then
  echo "Error: Unable to create Fitcrack DB tables"
  exit
fi
echo "Initial data inserted."
