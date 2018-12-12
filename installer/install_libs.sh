#!/bin/bash

# Install BOINC libraries
rm -f tmp/libs 2>/dev/null
cd boinc
make install

if [[ $? != 0 ]]; then
  echo "Error installing BOINC libraries."
  exit
fi

cd ..
touch tmp/libs
echo "BOINC libraries installed."
