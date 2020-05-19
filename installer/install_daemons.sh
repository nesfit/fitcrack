#!/bin/bash

# Instal server daemons
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

# Copy server daemon binaries
cp -Rf boinc/sched/work_generator $BOINC_PROJECT_DIR/bin/
cp -Rf boinc/sched/assimilator $BOINC_PROJECT_DIR/bin/
cp -Rf boinc/sched/bitwise_validator $BOINC_PROJECT_DIR/bin/
cp -Rf boinc/sched/trickler $BOINC_PROJECT_DIR/bin/
cp -Rf webadmin/fitcrackAPI/pcfg_manager/pcfg-manager $BOINC_PROJECT_DIR/bin/

##########################
# Configure pcfg monitor #
##########################

sed -i "s|DB_HOST = '.*|DB_HOST = '$DB_HOST'|g" $BOINC_PROJECT_DIR/bin/pcfg_monitor.py
sed -i "s|DB_NAME = '.*|DB_NAME = '$DB_NAME'|g" $BOINC_PROJECT_DIR/bin/pcfg_monitor.py
sed -i "s|DB_USER = '.*|DB_USER = '$DB_USER'|g" $BOINC_PROJECT_DIR/bin/pcfg_monitor.py
sed -i "s|DB_PW = '.*|DB_PW = '$DB_PW'|g" $BOINC_PROJECT_DIR/bin/pcfg_monitor.py

# Copy Fitcrack communication templates
cp -Rf server/templates/* $BOINC_PROJECT_DIR/templates/

#####################
# !!! IMPORTANT !!! #
#####################
# Set permissions for BOINC user and BOINC group
chmod -R g+rwx $BOINC_HOME
chown -R $BOINC_USER:$BOINC_GROUP $BOINC_HOME/projects
