#!/bin/bash

# Manage services for both sysvinit and systemd
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

SUITE=sysvinit
pidof systemd >/dev/null 2>/dev/null && SUITE=systemd

# Usage: service_<operation> <servicename>

###########################
# Check if service exists #
###########################

service_exists() {
  if [ $SUITE = "systemd" ]; then
    systemctl 2>&1 | grep $1 >/dev/null 2>/dev/null
  else
    service --status-all 2>&1 | grep $1 >/dev/null 2>/dev/null
  fi
}

##########################
# Display service status #
##########################

service_status() {
  if [ $SUITE = "systemd" ]; then
    systemctl status $1
  else
    service $1 status
  fi
}



#################
# Start service #
#################

service_start() {
  if [ $SUITE = "systemd" ]; then
    systemctl start $1
  else
    service $1 start
  fi
}


################
# Stop service #
################

service_stop() {
  if [ $SUITE = "systemd" ]; then
    systemctl stop $1
  else
    service $1 stop
  fi
}


##################
# Retart service #
##################

service_restart() {
  if [ $SUITE = "systemd" ]; then
    systemctl restart $1
  else
    service $1 restart
  fi
}


##################
# Reload service #
##################

service_reload() {
  if [ $SUITE = "systemd" ]; then
    systemctl reload $1
  else
    # sysvinit does not support reload
    service $1 restart
  fi
}

