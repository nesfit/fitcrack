#!/bin/bash

# Configure and build Fitcrack server
# This file is part of Fitcrack installer
# Author: Radek Hranicky (ihranicky@fit.vutbr.cz)

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
