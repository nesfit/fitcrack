#!/bin/bash

# Remove previous installation
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

echo "                                                              "
echo "=============== REMOVE PREVIOUS INSTALLATION ================="
echo "                                                              "

###################
# Project cleanup #
###################
if [ -d "$BOINC_PROJECT_DIR" ]; then
  read -e -p "Remove project $BOINC_PROJECT_DIR ? [y/N] (default: N): " DELETE_PROJECT_DIR
  DELETE_PROJECT_DIR=${DELETE_PROJECT_DIR:-N}

  if [ $DELETE_PROJECT_DIR = "y" ]; then
    read -e -p "All data will be lost! Are you sure? [y/N] (default: N): " CONFIRMED
    CONFIRMED=${CONFIRMED:-N}

    if [ $CONFIRMED = "y" ]; then
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
      systemctl restart $APACHE_SERVICE
    fi
  fi

fi

####################
# Database cleanup #
####################
read -e -p "Delete database $DB_NAME at $DB_HOST ? [y/N] (default: N): " DELETE_DB
DELETE_DB=${DELETE_DB:-N}
DB_ROOT_CLEANUP="N"
DROPPED="N"

if [ $DELETE_DB = "y" ]; then
  read -e -p "All data will be lost! Are you sure? [y/N] (default: N): " CONFIRMED
  CONFIRMED=${CONFIRMED:-N}

  if [ $CONFIRMED = "y" ]; then
    # Drop project database

    echo "Dropping original database..."
    mysql -h $DB_HOST -u $DB_USER -p"$DB_PW" -e "DROP DATABASE $DB_NAME;" 2>/dev/null
    if [[ $? != 0 ]]; then
      echo "Error: Unable to drop database $DB_NAME as user $DB_USER."
      read -e -p "Try again as database root? [y/N] (default: N): " DB_ROOT_CLEANUP
      DB_ROOT_CLEANUP=${DB_ROOT_CLEANUP:-N}
    else
      echo "Original database dropped."
      DROPPED="y"
    fi

    if [ $DROPPED = "y" ]; then
      echo "Creating an empty database..."
      mysql -h $DB_HOST -u $DB_USER -p"$DB_PW" -e "CREATE DATABASE $DB_NAME;" 2>/dev/null
      if [[ $? != 0 ]]; then
        echo "Error: Unable to create empty database $DB_HOST as user $DB_USER."
        echo "Try manually as root."
        exit
      else
        echo "Empty database created."
      fi
    fi
  fi
fi

if [ $DB_ROOT_CLEANUP = "y" ]; then
    read -e -p "Enter database root password: " DB_ROOT_PW;

    echo "Dropping original database..."
    mysql -h $DB_HOST -u root -p"$DB_ROOT_PW" -e "DROP DATABASE $DB_NAME;" 2>/dev/null
    if [[ $? != 0 ]]; then
      echo "Error: Unable to drop database $DB_NAME as root. Is the password correct?"
      exit
    else
      echo "Original database dropped."
    fi

    echo "Creating an empty database..."
    mysql -h $DB_HOST -u root -p"$DB_ROOT_PW" -e "CREATE DATABASE $DB_NAME;" 2>/dev/null
    if [[ $? != 0 ]]; then
      echo "Error: Unable to drop database $DB_NAME as root. Is the password correct?"
      exit
    else
      echo "Empty database created."
    fi
fi

####################
# WebAdmin cleanup #
####################

read -e -p "Uninstall WebAdmin ? [y/N] (default: N): " UNINSTALL_WEBADMIN
UNINSTALL_WEBADMIN=${UNINSTALL_WEBADMIN:-N}

if [ $UNINSTALL_WEBADMIN = "y" ]; then
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
  systemctl restart $APACHE_SERVICE
fi



#######################
# Collections cleanup #
#######################

read -e -p "Remove common collections (dictionaries, etc.) ? [y/N] (default: N): " REMOVE_COLLECTIONS
REMOVE_COLLECTIONS=${REMOVE_COLLECTIONS:-N}

if [ $REMOVE_COLLECTIONS = "y" ]; then
  rm -rf /usr/share/collections/charsets
  rm -rf /usr/share/collections/dictionaries
  rm -rf /usr/share/collections/markov
  rm -rf /usr/share/collections/encrypted-files
  rm -rf /usr/share/collections/masks
  rm -rf /usr/share/collections/rules
fi
