#!/usr/bin/env bash

cd $(dirname ${BASH_SOURCE[0]})

mkdir -p testdata

HASHCAT_SRC_DIR=../../client_bin
if grep Microsoft /proc/version >/dev/null
then
	HASHCAT_SRC_DIR+=/windows_x86_64
	HASHCAT_BINARY=hashcat64_v510_1.exe
else
	HASHCAT_SRC_DIR+=/x86_64-pc-linux-gnu
	HASHCAT_BINARY=hashcat64_v510_1.bin
fi

TMPDIR=/tmp/fitcrackServerTestHashcatDir
mkdir -p $TMPDIR
cp -a $HASHCAT_SRC_DIR/$HASHCAT_BINARY $TMPDIR
unzip -o $HASHCAT_SRC_DIR/hashcat_files_v510_1.zip -d $TMPDIR > /dev/null
( cd $TMPDIR && ./$HASHCAT_BINARY --stdout -a 3 a??b?dc?hd?He??f --markov-disable ) | sed 's/?/??/g' | tr -d '\r'