#!/bin/bash

# update_server.sh script
# Updates and runs the latest version of server tools
# Author: Lukas Zobal (xzobal00@stud.fit.vutbr.cz)

GREEN='\033[0;32m'
NC='\033[0m'


# Move it to BOINC folder
printf "${GREEN}[1/6] Moving the to BOINC folder...${NC}\n"
cp src/headers/*.h              /home/boincadm/boinc-src/sched/
cp src/headers/AttackModes/*.h  /home/boincadm/boinc-src/sched/
cp src/headers/Generators/*.h   /home/boincadm/boinc-src/sched/
cp src/headers/Database/*.h     /home/boincadm/boinc-src/sched/
cp src/source/*.cpp             /home/boincadm/boinc-src/sched/
cp src/source/AttackModes/*.cpp /home/boincadm/boinc-src/sched/
cp src/source/Generators/*.cpp  /home/boincadm/boinc-src/sched/
cp src/source/Database/*.cpp    /home/boincadm/boinc-src/sched/

# Make it
printf "${GREEN}[2/6] Making binaries...${NC}\n"
cd ../boinc-src/sched
if [ "$EUID" -eq 0 ] ; then
  sudo -u boincadm make
else
  make
fi

# Remove sources
printf "${GREEN}[3/6] Removing sources...${NC}\n"
rm Package.cpp Package.h Host.h Mask.h Job.h Host.cpp Job.cpp Mask.cpp SqlLoader.h SqlLoader.cpp Config.h Config.cpp AttackDict.h AttackMask.h AttackCombinator.h AttackRules.h AttackMarkov.h AttackDict.cpp AttackMask.cpp AttackCombinator.cpp AttackRules.cpp AttackMarkov.cpp SimpleGenerator.h SimpleGenerator.cpp AttackMode.h AttackBench.h AttackBench.cpp AbstractGenerator.h AbstractGenerator.cpp

# Stop generator and assimilator
printf "${GREEN}[4/6] Stopping server tools...${NC}\n"

cd ../../projects/fitcrack/
./bin/stop


# Move the binaries
printf "${GREEN}[5/6] Moving the binaries...${NC}\n"

cp ../../boinc-src/sched/work_generator bin/
cp ../../boinc-src/sched/assimilator bin/


# Start the tools
printf "${GREEN}[6/6] Starting the binaries...${NC}\n"
./bin/start




printf "${GREEN}Update done.${NC}\n"
