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
# Updated Fitcrack server image for Ubuntu 24.04 + BOINC server 1.6.1
# BOINC server_release/1.6/1.6.1 = bb54b14b9152a7eb2ecc816ad120214e786beaab
#

FROM ubuntu:24.04

LABEL maintainer="Fitcrack Team"

ARG DEBIAN_FRONTEND=noninteractive
ARG COMPILER_THREADS=4
ARG TZ=UTC
ARG NODE_VERSION=16.15.0

USER root
SHELL ["/bin/bash", "-o", "pipefail", "-c"]

ENV TZ=${TZ}
ENV PATH=/usr/local/lib/nodejs/node-v${NODE_VERSION}-linux-x64/bin:${PATH}

WORKDIR /tmp

RUN ln -snf /usr/share/zoneinfo/${TZ} /etc/localtime \
    && echo ${TZ} > /etc/timezone \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
        apache2 \
        apache2-utils \
        autoconf \
        automake \
        build-essential \
        ca-certificates \
        curl \
        g++-mingw-w64-x86-64 \
        git \
        iputils-ping \
        libapache2-mod-php \
        libapache2-mod-wsgi-py3 \
        libcurl4-openssl-dev \
        libfcgi-dev \
        libmysqlclient-dev \
        libssl-dev \
        libtool \
        libzip-dev \
        m4 \
        mysql-client \
        mysql-server \
        net-tools \
        nlohmann-json3-dev \
        patch \
        perl \
        php \
        php-cli \
        php-gd \
        php-mysql \
        php-xml \
        pkg-config \
        python3 \
        python3-dev \
        python3-mysqldb \
        python3-pip \
        python3-pymysql \
        python3-setuptools \
        python3-wheel \
        sudo \
        telnet \
        tzdata \
        vim \
        wget \
        xz-utils \
        zlib1g \
        zlib1g-dev \
    && rm -rf /var/lib/apt/lists/*

RUN ln -sf /usr/bin/python3 /usr/bin/python
RUN a2enmod cgi rewrite wsgi

RUN mkdir -p /usr/local/lib/nodejs \
    && wget -q https://nodejs.org/dist/v${NODE_VERSION}/node-v${NODE_VERSION}-linux-x64.tar.xz \
    && tar -xJf node-v${NODE_VERSION}-linux-x64.tar.xz -C /usr/local/lib/nodejs \
    && rm -f node-v${NODE_VERSION}-linux-x64.tar.xz

RUN useradd -m -c "BOINC Administrator" boincadm -s /bin/bash \
    && groupadd fitcrack \
    && usermod -a -G fitcrack boincadm

COPY . /srv/fitcrack/
WORKDIR /srv/fitcrack

# Apply Fitcrack BOINC patch and copy Fitcrack scheduler sources into BOINC
RUN patch -p0 < installer/fitcrack_changes_in_boinc.patch \
    && cp -f server/src/headers/*.h boinc/sched/ \
    && cp -f server/src/headers/AttackModes/*.h boinc/sched/ \
    && cp -f server/src/headers/Generators/*.h boinc/sched/ \
    && cp -f server/src/headers/Database/*.h boinc/sched/ \
    && cp -f server/src/headers/Utils/*.h boinc/sched/ \
    && cp -f server/src/source/*.cpp boinc/sched/ \
    && cp -f server/src/source/AttackModes/*.cpp boinc/sched/ \
    && cp -f server/src/source/Generators/*.cpp boinc/sched/ \
    && cp -f server/src/source/Database/*.cpp boinc/sched/ \
    && cp -f server/src/source/Utils/*.cpp boinc/sched/ \
    && cp -f server/src/headers/protocol/*.cc boinc/sched/ \
    && cp -f server/src/headers/protocol/*.h boinc/sched/ \
    && cp -rf server/src/include/* boinc/sched/

RUN python3 -m pip install --break-system-packages urllib3==1.26.15 mysqlclient

WORKDIR /srv/fitcrack/boinc
RUN chmod +x _autosetup \
    && ./_autosetup \
    && ./configure --disable-client --disable-manager --enable-boinczip \
    && make -j${COMPILER_THREADS}

WORKDIR /srv/fitcrack/runner
RUN chmod +x ./update_client_bins.sh \
    && bash ./update_client_bins.sh

WORKDIR /srv/fitcrack/

######################################################################################
################################# Webadmin backend ###################################

RUN pip3 install --break-system-packages -r webadmin/fitcrackAPI/src/requirements.txt

RUN cd webadmin/fitcrackAPI/hashcat-utils/src && make
RUN cd webadmin/fitcrackAPI/xtohashcat/scripts && make
RUN cd webadmin/fitcrackAPI/pwd_dist && make

######################################################################################
################################# Webadmin frontend ###################################

WORKDIR /srv/fitcrack/webadmin/fitcrackFE
RUN npm ci && npm run build

WORKDIR /srv/fitcrack
COPY tools/entrypoint-fitcrack.sh /srv/fitcrack/
RUN chmod +x /srv/fitcrack/entrypoint-fitcrack.sh \
    && mkdir -p /srv/db_init

EXPOSE 80 5000

HEALTHCHECK --interval=5s --timeout=20s CMD ps -ef | grep apache2 | grep -v grep || exit 1

ENTRYPOINT ["/srv/fitcrack/entrypoint-fitcrack.sh"]
