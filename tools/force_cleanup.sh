#!/bin/bash

# Completely purge Fitcrack
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

# Get functions for sysvinit/systemd
source installer/services.sh

echo "                                                              "
echo "=============== COMPLETE CLEANUP ================="
echo "                                                              "

###################
# Project cleanup #
###################

rm -rf $BOINC_PROJECT_DIR
echo "Project directory deleted."

if ! [ -z "$PROJECT_HTTPD_CONF" ]; then
  echo "Removing BOINC references from $APACHE_CONFIG_FILE ..."

  sed -i "s|IncludeOptional $PROJECT_HTTPD_CONF.*||g" $APACHE_CONFIG_FILE 2>/dev/null

  if [[ $? != 0 ]]; then
    echo "Failed to remove."
  else
    echo "Removed."
  fi
fi
echo "Project removed. Restarting Apache..."
service_restart $APACHE_SERVICE


# Remove startup scripts (if exist)
if [ -f "/etc/init.d/fitcrack" ]; then
  echo "Removing fitcrack system service..."
  # Remove runlevel symlinks
  case $DISTRO_ID in
    debian|ubuntu)
      update-rc.d fitcrack remove
    ;;
    centos|redhat)
      chkconfig --level 2345 fitcrack off
      chkconfig --del fitcrack
    ;;
    suse|linux)
    ;;
  esac
  # Remove startup script
  rm -f /etc/init.d/fitcrack
fi

####################
# Database cleanup #
####################

DROPPED="N"

export MYSQL_PWD="$DB_PW"
echo "Dropping original database..."
mysql -h $DB_HOST -u $DB_USER -e "DROP DATABASE $DB_NAME;"
if [[ $? != 0 ]]; then
  echo "Error: Unable to drop database $DB_NAME as user $DB_USER."
else
  echo "Original database dropped."
  DROPPED="y"
fi

if [ $DROPPED = "y" ]; then
  echo "Creating an empty database..."
  mysql -h $DB_HOST -u $DB_USER -e "CREATE DATABASE $DB_NAME;"
  if [[ $? != 0 ]]; then
    echo "Error: Unable to create empty database $DB_HOST as user $DB_USER."
    echo "Try manually as root."
    exit
  else
    echo "Empty database created."
  fi
fi


####################
# WebAdmin cleanup #
####################
rm -Rf $APACHE_DOCUMENT_ROOT/fitcrackFE
rm -Rf $APACHE_DOCUMENT_ROOT/fitcrackAPI

if [ -f $APACHE_CONFIG_DIR/sites-enabled/fitcrackFE.conf ]; then
  rm -Rf $APACHE_CONFIG_DIR/sites-enabled/fitcrackFE.conf
fi

if [ -f $APACHE_CONFIG_DIR/sites-enabled/fitcrackAPI.conf ]; then
  rm -Rf $APACHE_CONFIG_DIR/sites-enabled/fitcrackAPI.conf
fi

if [ -f $APACHE_CONFIG_DIR/sites-available/fitcrackFE.conf ]; then
  rm -Rf $APACHE_CONFIG_DIR/sites-available/fitcrackFE.conf
fi

if [ -f $APACHE_CONFIG_DIR/sites-available/fitcrackAPI.conf ]; then
  rm -Rf $APACHE_CONFIG_DIR/sites-available/fitcrackAPI.conf
fi

echo "WebAdmin Uninstalled. Restarting Apache..."
service_restart $APACHE_SERVICE


#######################
# Collections cleanup #
#######################

rm -rf /usr/share/collections/charsets
rm -rf /usr/share/collections/dictionaries
rm -rf /usr/share/collections/markov
rm -rf /usr/share/collections/encrypted-files
rm -rf /usr/share/collections/masks
rm -rf /usr/share/collections/rules
rm -rf /usr/share/collections/pcfg
