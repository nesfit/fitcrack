#!/bin/bash

# Ask user to set installer variables
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

#############
# BOINC user
#############
read -e -p "Enter the name for BOINC user (default: boincadm): " BOINC_USER
BOINC_USER=${BOINC_USER:-boincadm}

if ! id "$BOINC_USER" >/dev/null 2>&1; then
  echo "Error: User $APACHE_USER does not exist!"
  exit
fi

###############
# BOINC group
###############
read -e -p "Enter the name of BOINC group (default: $BOINC_USER): " BOINC_GROUP
BOINC_GROUP=${BOINC_GROUP:-$BOINC_USER}

if ! grep -q "^${BOINC_GROUP}:" /etc/group
then
  echo "Error: Group $BOINC_GROUP does not exist!"
  exit
fi

############################
# BOINC user home directory
############################

read -e -p "Enter BOINC user home directory (default: /home/$BOINC_USER): " BOINC_HOME
BOINC_HOME=${BOINC_HOME:-/home/$BOINC_USER}

if ! [ -d "$BOINC_HOME" ]; then
  echo "Selected directory does not exist!"
  exit
fi

#####################
# BOINC project name
#####################

read -e -p "Enter BOINC project name (default: fitcrack): " BOINC_PROJECT
BOINC_PROJECT=${BOINC_PROJECT:-fitcrack}

##########################
# BOINC project directory
#########################

BOINC_PROJECT_DIR="$BOINC_HOME/projects/$BOINC_PROJECT"

##############################
# BOINC project HTTPD CONFIG #
##############################
PROJECT_HTTPD_CONF="$BOINC_PROJECT_DIR/$BOINC_PROJECT.httpd.conf"

#########################################################
# BOINC project URL base (for connecting cracking nodes)
#########################################################

read -e -p "Enter BOINC project URL_BASE (default: http://127.0.0.1): " BOINC_URL
BOINC_URL=${BOINC_URL:-http://127.0.0.1}


#############
# Apache user
#############
read -e -p "Enter the name of Apache user (default: $DEFAULT_APACHE_USER): " APACHE_USER
APACHE_USER=${APACHE_USER:-$DEFAULT_APACHE_USER}

if ! id "$APACHE_USER" >/dev/null 2>&1; then
  echo "Error: User $APACHE_USER does not exist!"
  exit
fi


###############
# Apache group
###############
read -e -p "Enter the name of Apache group (default: $APACHE_USER): " APACHE_GROUP
APACHE_GROUP=${APACHE_GROUP:-$APACHE_USER}

if ! grep -q "^${APACHE_GROUP}:" /etc/group; then
  echo "Error: Group $APACHE_GROUP does not exist!"
  exit
fi

##########################
# Apache config directory
##########################

read -e -p "Enter Apache configuration directory (default: $DEFAULT_APACHE_CONFIG_DIR): " APACHE_CONFIG_DIR
APACHE_CONFIG_DIR=${APACHE_CONFIG_DIR:-$DEFAULT_APACHE_CONFIG_DIR}

if ! [ -d "$APACHE_CONFIG_DIR" ]; then
  echo "Selected directory does not exist!"
  exit
fi

#####################
# Apache config file
#####################

read -e -p "Enter Apache configuration file (default: $DEFAULT_APACHE_CONFIG_FILE): " APACHE_CONFIG_FILE
APACHE_CONFIG_FILE=${APACHE_CONFIG_FILE:-$DEFAULT_APACHE_CONFIG_FILE}

if ! [ -f "$APACHE_CONFIG_FILE" ]; then
  echo "Selected file does not exist!"
  exit
fi

#################
# Apache service
#################
read -e -p "Enter the name of Apache service (default: $DEFAULT_APACHE_SERVICE): " APACHE_SERVICE
APACHE_SERVICE=${APACHE_SERVICE:-$DEFAULT_APACHE_SERVICE}

systemctl | grep $APACHE_SERVICE >/dev/null 2>/dev/null

if [[ $? != 0 ]]; then
  echo "Error: $APACHE_SERVICE not found via systemctl."
  exit
fi


#######################
# Apache document root
#######################

read -e -p "Enter Apache document root (default: $DEFAULT_APACHE_DOCUMENT_ROOT): " APACHE_DOCUMENT_ROOT
APACHE_DOCUMENT_ROOT=${APACHE_DOCUMENT_ROOT:-$DEFAULT_APACHE_DOCUMENT_ROOT}

if ! [ -d "$APACHE_DOCUMENT_ROOT" ]; then
  echo "Selected directory does not exist!"
  exit
fi

#############
# MySQL host
#############

read -e -p "Enter MySQL hostname (default: localhost): " DB_HOST
DB_HOST=${DB_HOST:-localhost}

################
# MySQL db name
################

read -e -p "Enter MySQL database name (default: fitcrack): " DB_NAME
DB_NAME=${DB_NAME:-fitcrack}

##################
# MySQL user name
##################

read -e -p "Enter MySQL user name (default: fitcrack): " DB_USER
DB_USER=${DB_USER:-fitcrack}

######################
# MySQL user password
######################

read -e -p "Enter MySQL user password (default: mypassword): " DB_PW
DB_PW=${DB_PW:-mypassword}

# Check DB connection
mysql -h $DB_HOST -u $DB_USER -p"$DB_PW" -D"$DB_NAME" -e ";" 2>/dev/null

if [[ $? != 0 ]]; then
  echo "Error: Unable to access database $DB_NAME at $DB_HOST as user $DB_USER."
  exit
fi

###############

#echo "============= configuration ============="
#echo "BOINC_USER: $BOINC_USER"
#echo "BOINC_GROUP: $BOINC_GROUP"
#echo "BOINC_HOME: $BOINC_HOME"
#echo "APACHE_USER: $APACHE_USER"
#echo "APACHE_GROUP: $APACHE_GROUP"
#echo "APACHE_CONFIG_FILE: $APACHE_CONFIG_FILE"
#echo "APACHE_DOCUMENT_ROOT: $APACHE_DOCUMENT_ROOT"
#echo "MYSQL SERVER: $APACHE_CONFIG_FILE"
