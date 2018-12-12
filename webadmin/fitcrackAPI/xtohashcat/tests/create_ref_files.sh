#!/bin/bash

mkdir -p test_ref
mkdir -p test_ref/office
mkdir -p test_ref/7zip
mkdir -p test_ref/winzip
mkdir -p test_ref/rar
mkdir -p test_ref/pdf

# Office
printf "Creating reference output for Office documents ... "
FILES=./test_in/office/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    refPath=test_ref/office/$filename.ref

    out=""
    out=$(../scripts/office2hashcat.py $f 2>/dev/null)

    if [ -n "$out" ]; then
        echo $out > $refPath
        if [[ $filename == *"00_"* ]]; then            
            echo "9700" >> $refPath
        elif [[ $filename == *"03_"* ]]; then
            echo "9800" >> $refPath
        elif [[ $filename == *"07_"* ]]; then
            echo "9400" >> $refPath
        elif [[ $filename == *"10_"* ]]; then
            echo "9500" >> $refPath
        elif [[ $filename == *"13_"* ]] || [[ $filename == *"16_"* ]]; then
            echo "9600" >> $refPath
        fi
    else
        touch $refPath
    fi
done

printf "Done\n"


# 7-Zip
printf "Creating reference output for 7-Zip archives   ... "
FILES=./test_in/7zip/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    refPath=test_ref/7zip/$filename.ref

    out=""
    out=$(../scripts/7z2hashcat.pl $f 2>/dev/null)

    if [ -n "$out" ]; then
        echo $out > $refPath
        echo "11600" >> $refPath
    else
        touch $refPath
    fi
done

printf "Done\n"


# WinZip
printf "Creating reference output for WinZip archives  ... "
FILES=./test_in/winzip/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    refPath=test_ref/winzip/$filename.ref

    out=""
    out=$(../scripts/zip2john $f 2>/dev/null)

    if [ -n "$out" ]; then
        echo $out > $refPath
        echo "13600" >> $refPath
    else
        touch $refPath
    fi
done

printf "Done\n"


# RAR
printf "Creating reference output for RAR archives     ... "
FILES=./test_in/rar/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    refPath=test_ref/rar/$filename.ref

    out=""
    out=$(../scripts/rar2john $f 2>/dev/null)

    if [ -n "$out" ]; then
        echo $out > $refPath
        if [[ $filename == *"3_"* ]]; then            
            echo "12500" >> $refPath
        elif [[ $filename == *"5_"* ]]; then
            echo "13000" >> $refPath
        fi
    else
        touch $refPath
    fi
done

printf "Done\n"


# PDF
printf "Creating reference output for PDF archives     ... "
FILES=./test_in/pdf/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    refPath=test_ref/pdf/$filename.ref

    out=""
    out=$(../scripts/pdf2hashcat.py $f 2>/dev/null)

    if [ -n "$out" ]; then
        echo $out > $refPath
        if [[ $filename == *"1_4_"* ]]; then            
            echo "10500" >> $refPath
        elif [[ $filename == *"1_7_"* ]]; then
            echo "10600" >> $refPath
        elif [[ $filename == *"1_10_"* ]]; then
            echo "10700" >> $refPath
        else
            echo "10400" >> $refPath
        fi
    else
        touch $refPath
    fi
done

printf "Done\n"

echo "All reference outputs successfully created."

