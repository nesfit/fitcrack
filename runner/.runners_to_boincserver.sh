#!/bin/bash
#fill in BOINC user
cd $(dirname ${BASH_SOURCE[0]})
# FILL path to boinc_server's fitcrack app directory (the one with the versions such as 1/, 2/, etc.)
CLIENT_BIN_DIR=/home/boincadm/projects/fitcrack/apps/fitcrack/
LASTNUM=$(ls $CLIENT_BIN_DIR | sort -g | tail -n1)
PREVDIR=$CLIENT_BIN_DIR/$LASTNUM
NNUM=$((LASTNUM+1))
OUTDIR=$CLIENT_BIN_DIR/$NNUM
cp -a $PREVDIR $OUTDIR
#delete previous
rm ${OUTDIR}/{x86_64-pc-linux-gnu,windows_x86_64}/runner*
#copy new
cp bin/runner.bin ${OUTDIR}/x86_64-pc-linux-gnu/runner${NNUM}.bin
cp bin/runner.exe ${OUTDIR}/windows_x86_64/runner${NNUM}.exe

#alter version.xml
for PLATFORM in x86_64-pc-linux-gnu windows_x86_64
do
	sed -i 's/\(runner[0-9]\+.\(bin\|exe\)\)/runner'$NNUM'.\2/g' $OUTDIR/$PLATFORM/version.xml
done
cd /home/boincadm/projects/fitcrack/apps/fitcrack/../../ && yes | ./bin/update_versions