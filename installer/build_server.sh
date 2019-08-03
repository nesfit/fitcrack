#!/bin/bash

# Configure and build Fitcrack server
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

# Install gRPC and protocol buffers
if ! [ -d "grpc" ]; then
  echo "Installing gRPC and protocol buffers"

  # Clone, build and install gRPC
  git clone -b $(curl -L https://grpc.io/release) https://github.com/grpc/grpc
  cd grpc
  git submodule update --init
  make
  make install

  # Install prtocol buffers
  cd third_party/protobuf
  make install

  cd $INSTALLER_ROOT
fi

# Rebuiding
echo "Rebuilding gRPC protocol..."
cd server/src/headers
./regenerate_protocol.sh
cd $INSTALLER_ROOT

echo "Building Fitcrack server..."

# Copy server files to BOINC server Root
cp -f server/src/headers/*.h              boinc/sched/
cp -f server/src/headers/AttackModes/*.h  boinc/sched/
cp -f server/src/headers/Generators/*.h   boinc/sched/
cp -f server/src/headers/Database/*.h     boinc/sched/
cp -f server/src/source/*.cpp             boinc/sched/
cp -f server/src/source/AttackModes/*.cpp boinc/sched/
cp -f server/src/source/Generators/*.cpp  boinc/sched/
cp -f server/src/source/Database/*.cpp    boinc/sched/
cp -f server/src/headers/protocol/*.cc    boinc/sched/
cp -f server/src/headers/protocol/*.h     boinc/sched/

rm -f tmp/built 2>/dev/null

cd boinc
chmod +x _autosetup
chmod +x configure


if [ -f /opt/rh/devtoolset-7/enable ]; then
  . /opt/rh/devtoolset-7/enable
fi

./_autosetup
if [[ $? != 0 ]]; then
  echo "Error during autoconfiguration."
  exit
fi

./configure -C --disable-client --disable-manager
if [[ $? != 0 ]]; then
  echo "Error during configuration."
  exit
fi

make
if [[ $? != 0 ]]; then
  echo "Error during compilation."
  exit
fi

cd ..
touch tmp/built
echo "Build successfull."
