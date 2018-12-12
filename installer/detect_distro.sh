#!/bin/bash

# Detect Linux distro and set distro-specific defaults
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

# Get distro ID
if [ -f /etc/os-release ]; then
    # New Debian/Ubuntu/CentOS, freedesktop.org and systemd
    . /etc/os-release
elif type lsb_release >/dev/null 2>&1; then
    # linuxbase.org
    ID=$(lsb_release -si)
elif [ -f /etc/lsb-release ]; then
    # For some versions of Debian/Ubuntu without lsb_release command
    . /etc/lsb-release
elif [ -f /etc/debian_version ]; then
    # Older Debian/Ubuntu/etc.
    ID="debian"
elif [ -f /etc/SuSe-release ]; then
    # Older SuSE/etc.
    ID="suse"
elif [ -f /etc/redhat-release ]; then
    # Older Red Hat, CentOS, etc.
    ID="redhat"
else
    # Fall back to uname, e.g. "Linux <version>", also works for BSD, etc.
    OS=$(uname -s)
    VER=$(uname -r)
fi

# Lowercase the ID
ID=`echo "$ID" | tr '[:upper:]' '[:lower:]'`

# Print distro info
echo "Distro '$ID' detected. Setting default variables."

# Set defaults
case $ID in
    debian|ubuntu)
        DEFAULT_APACHE_USER="www-data"
        DEFAULT_APACHE_CONFIG_DIR="/etc/apache2"
        DEFAULT_APACHE_CONFIG_FILE="/etc/apache2/apache2.conf"
        DEFAULT_APACHE_DOCUMENT_ROOT="/var/www/html"
        DEFAULT_APACHE_SERVICE="apache2"
        ;;
    centos|redhat)
        DEFAULT_APACHE_USER="apache"
        DEFAULT_APACHE_CONFIG_DIR="/etc/httpd"
        DEFAULT_APACHE_CONFIG_FILE="/etc/httpd/conf/httpd.conf"
        DEFAULT_APACHE_DOCUMENT_ROOT="/var/www/html"
        DEFAULT_APACHE_SERVICE="httpd"
        ;;
    suse|linux)
        DEFAULT_APACHE_USER="wwwrun"
        DEFAULT_APACHE_CONFIG_DIR="/etc/apache2"
        DEFAULT_APACHE_CONFIG_FILE="/etc/apache2/httpd.conf"
        DEFAULT_APACHE_DOCUMENT_ROOT="/var/www/html"
        DEFAULT_APACHE_SERVICE="httpd"
        ;;
esac
