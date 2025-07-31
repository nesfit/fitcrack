#
# File: Dockerfile
#
#####################################################
#   FITCRACK DISTRIBUTED PASSWORD CRACKING SYSTEM   #
#####################################################
#
# Dockerfile for building Fitcrack server docker image
# Copyright: Fitcrack Team <ihranicky@fit.vutbr.cz>
# https://fitcrack.fit.vutbr.cz/
#

# Base image
FROM ubuntu:22.04

LABEL maintainer="Fitcrack Team <ihranicky@fit.vutbr.cz>"

USER root
WORKDIR /tmp

# Just testing input .env values passed from .env --> docker-compose --> Dockerfile
ARG COMPILER_THREADS

RUN apt-get -y update && apt install -yq curl wget vim
RUN apt install -y iputils-ping
RUN apt install -y net-tools && apt install -y telnet

# Configure timezone
ENV TZ UTC
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt install -yq sudo

# noninteractive keyboard configuration
COPY ./tools/keyboard /etc/default/keyboard
RUN export DEBIAN_FRONTEND=noninteractive
RUN echo 'debconf debconf/frontend select Noninteractive' | debconf-set-selections
RUN apt-get install -y -q
RUN apt-get install -y -q keyboard-configuration

# MySQL database server
RUN apt install -y mysql-server

# Install Node.JS for frontend compilation
RUN mkdir -p /usr/local/lib/nodejs
RUN apt install -y wget xz-utils
RUN wget https://nodejs.org/dist/v16.15.0/node-v16.15.0-linux-x64.tar.xz \
&& tar -xJvf node-v16.15.0-linux-x64.tar.xz -C /usr/local/lib/nodejs


######################################################################################
############################# BOINC Server dependencies ##############################

# Add BOINC server user and fitcrack group
RUN useradd -m -c "BOINC Administrator" boincadm  -s /bin/bash
RUN groupadd fitcrack
RUN usermod -a -G fitcrack boincadm

# Dependencies for building BOINC Server
RUN apt install -yq build-essential
RUN apt install -yq m4 dh-autoreconf pkg-config git \
                    libmysqlclient-dev zlib1g zlib1g-dev \
                    libcurl4-openssl-dev

# Install MySQL client
RUN apt install -yq mysql-client

# Perl
RUN apt install -yq perl libcompress-raw-lzma-perl

# PHP and modules
RUN apt install -yq php php-xml php-mysql php-cli php-gd

# Python3 with MySQL connectors
RUN apt install -yq python3 python3-pymysql python3-pip python3-mysqldb
RUN pip3 install urllib3==1.26.15
RUN ln -s /usr/bin/python3 /usr/bin/python

# Apache with necessary mods
RUN apt install -yq apache2 apache2-utils libapache2-mod-php libapache2-mod-wsgi-py3
RUN a2enmod cgi
RUN a2enmod rewrite
RUN a2enmod wsgi
RUN service apache2 restart

######################################################################################
################################# BOINC Server build #################################

COPY . /srv/fitcrack/
WORKDIR /srv/fitcrack

# Apply fitcrack specific changes to boinc
# First check if the patch is not already applied locally (e.g. installer was used before)
RUN if ! patch -p0 --dry-run --silent < installer/fitcrack_changes_in_boinc.patch 2>/dev/null; then \
    echo "Patch already applied or failed to apply in dry run. Skipping."; \
else \
    patch -p0 < installer/fitcrack_changes_in_boinc.patch; \
fi


# Copy server files to BOINC server Root
RUN cp -f server/src/headers/*.h              boinc/sched/
RUN cp -f server/src/headers/AttackModes/*.h  boinc/sched/
RUN cp -f server/src/headers/Generators/*.h   boinc/sched/
RUN cp -f server/src/headers/Database/*.h     boinc/sched/
RUN cp -f server/src/headers/Utils/*.h     boinc/sched/
RUN cp -f server/src/source/*.cpp             boinc/sched/
RUN cp -f server/src/source/AttackModes/*.cpp boinc/sched/
RUN cp -f server/src/source/Generators/*.cpp  boinc/sched/
RUN cp -f server/src/source/Database/*.cpp    boinc/sched/
RUN cp -f server/src/source/Utils/*.cpp    boinc/sched/
RUN cp -f server/src/headers/protocol/*.cc    boinc/sched/
RUN cp -f server/src/headers/protocol/*.h     boinc/sched/
RUN cp -rf server/src/include/*               boinc/sched/
RUN rm -f tmp/built 2>/dev/null

# Build Fitcrack server
WORKDIR /srv/fitcrack/boinc
RUN chmod +x _autosetup && ./_autosetup
RUN chmod +x configure && ./configure --disable-client --disable-manager
RUN make -j$COMPILER_THREADS

# Prepare project creation script
WORKDIR /srv/fitcrack/

# Build runner
WORKDIR /srv/fitcrack/runner
RUN apt-get install -y g++-mingw-w64-x86-64
RUN chmod +x ./update_client_bins.sh
RUN bash ./update_client_bins.sh

WORKDIR /srv/fitcrack/

######################################################################################
################################# Webadmin backend ###################################

RUN pip3 install -r webadmin/fitcrackAPI/src/requirements.txt

RUN cd webadmin/fitcrackAPI/hashcat-utils/src && make
RUN cd webadmin/fitcrackAPI/xtohashcat/scripts && make
RUN cd webadmin/fitcrackAPI/pwd_dist && make

######################################################################################
################################# Webadmin frontend ###################################

WORKDIR /srv/fitcrack/webadmin/fitcrackFE
RUN export PATH=/usr/local/lib/nodejs/node-v16.15.0-linux-x64/bin:$PATH \
&& npm ci && npm run build

######################################################################################

WORKDIR /srv/fitcrack/
COPY tools/entrypoint-fitcrack.sh /srv/fitcrack/

# Copy database init-script
RUN mkdir /srv/db_init

# ports and env values
EXPOSE 80
EXPOSE 5000

# Entrypoint
ENTRYPOINT ["/srv/fitcrack/entrypoint-fitcrack.sh"]

HEALTHCHECK --interval=5s --timeout=20s CMD ps -ef | grep apache2 | grep -v grep || exit 1
