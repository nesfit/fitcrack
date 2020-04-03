#!/bin/bash

# update_server.sh script
# Updates and runs the latest version of server tools
# Author: Lukas Zobal (xzobal00@stud.fit.vutbr.cz)

GREEN='\033[0;32m'
NC='\033[0m'

cd $(dirname ${BASH_SOURCE[0]})

SCRIPT_DIR=$PWD

# Move it to BOINC folder
printf "${GREEN}[1/6] Moving the to BOINC folder...${NC}\n"
cp -a src/headers/*.h              ../boinc/sched/
cp -a src/headers/AttackModes/*.h  ../boinc/sched/
cp -a src/headers/Generators/*.h   ../boinc/sched/
cp -a src/headers/Database/*.h     ../boinc/sched/
cp -a src/headers/Utils/*.h     ../boinc/sched/
cp -a src/source/*.cpp             ../boinc/sched/
cp -a src/source/AttackModes/*.cpp ../boinc/sched/
cp -a src/source/Generators/*.cpp  ../boinc/sched/
cp -a src/source/Database/*.cpp    ../boinc/sched/
cp -a src/source/Utils/*.cpp    ../boinc/sched/

# Make it
printf "${GREEN}[2/6] Making binaries...${NC}\n"
cd ../boinc/sched
if [ "$EUID" -eq 0 ] ; then
  sudo -u boincadm make -j 14
else
  make -j 14
fi

if (( $? != 0 ))
then
  echo "build failed, exiting";
  exit 1
fi

# Remove sources
printf "${GREEN}[3/6] Removing sources...${NC}\n"
find $SCRIPT_DIR/src/headers $SCRIPT_DIR/src/source -name 'protocol' -prune -o \( -name '*.cpp' -o -name '*.h' \) -type f -exec bash -c 'rm $(basename $0)' {} \;

# Stop generator and assimilator
printf "${GREEN}[4/6] Stopping server tools...${NC}\n"

cd /home/boincadm/projects/fitcrack/
./bin/stop


# Move the binaries
printf "${GREEN}[5/6] Moving the binaries...${NC}\n"

cp $SCRIPT_DIR/../boinc/sched/work_generator bin/
cp $SCRIPT_DIR/../boinc/sched/assimilator bin/


# Start the tools
printf "${GREEN}[6/6] Starting the binaries...${NC}\n"
if [[ $(id -un) == "boincadm" ]]
then
  ./bin/start
else
  sudo -u boincadm ./bin/start
fi




printf "${GREEN}Update done.${NC}\n"
