#!/bin/bash

COLOR_OFF=0

RED='\033[;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if [[ $COLOR_OFF -eq 1 ]]; then
    RED=''
    GREEN=''
    NC=''
fi

PASSED_TESTS=0
FAILED_TESTS=0
QUIET=0

if [ "$1" = "-q" ]; then
    QUIET=1
fi


function condition {
    if [[ "$OK" == "$1" && $? -eq "$2" ]]; then
        if [ $QUIET -eq 0 ]; then
            printf "$3"
            printf "$4"
            printf "test ${GREEN}PASSED${NC}\n\n"
        fi
        ((PASSED_TESTS++))
    else
        printf "$3"
        printf "$4"
        printf "test ${RED}FAILED${NC}\n\n"
        ((FAILED_TESTS++))
    fi
}


function retval {
    if [ $1 -eq $2 ]; then
        if [ $QUIET -eq 0 ]; then
            printf "$3"
            printf " ${GREEN}PASSED${NC}\n"
        fi
        ((PASSED_TESTS++))
    else
        printf "$3"
        printf " ${RED}FAILED${NC}\n"
        ((FAILED_TESTS++))
    fi
}


function compare {
    cmp $1 $2
    if [ $? -eq 0 ]; then
        if [ $QUIET -eq 0 ]; then
            printf "$3"
            printf " ${GREEN}PASSED${NC}\n"
        fi
        ((PASSED_TESTS++))
    else
        printf "$3"
        printf " ${RED}FAILED${NC}\n"
        ((FAILED_TESTS++))
    fi
}

# Create reference output
cd tests
./create_ref_files.sh
cd ..

# Create test_out directory tree
mkdir -p tests/test_out
mkdir -p tests/test_out/office
mkdir -p tests/test_out/pdf
mkdir -p tests/test_out/winzip
mkdir -p tests/test_out/7zip
mkdir -p tests/test_out/rar

printf "XtoHashcat tests started.\n"
printf "Testing XtoHashcat format auto-detection...\n"

printf "\n________ MS OFFICE ________\n"

FILES=./tests/test_in/office/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    outPath=tests/test_out/office/$filename.out
    refPath=tests/test_ref/office/$filename.ref

    ./XtoHashcat.py $f 1>$outPath 2>/dev/null
    lastRetVal=$?

    if [[ $filename == *"_0"* ]]; then
        retr=1
    else
        retr=0
    fi

    retval $lastRetVal $retr "OFFICE_($filename)_RET"
    compare $outPath $refPath "OFFICE_($filename)_OUT"
done

printf "\n_________ 7-ZIP __________\n"

FILES=./tests/test_in/7zip/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    outPath=tests/test_out/7zip/$filename.out
    refPath=tests/test_ref/7zip/$filename.ref

    ./XtoHashcat.py $f 1>$outPath 2>/dev/null
    lastRetVal=$?

    if [[ $filename == *"_0"* ]]; then
        retr=1
    else
        retr=0
    fi

    retval $lastRetVal $retr "7ZIP_($filename)_RET"
    compare $outPath $refPath "7ZIP_($filename)_OUT"
done

# TODO: RAR/ZIP tests needs conversion from john2hashcat
#
#printf "\n__________ RAR ___________\n"
#
#FILES=./tests/test_in/rar/*
#for f in $FILES
#do
#    filename=$(basename "$f")
#    filename="${filename%.*}"
#    outPath=tests/test_out/rar/$filename.out
#    refPath=tests/test_ref/rar/$filename.ref
#
#    ./XtoHashcat.py $f 1>$outPath 2>/dev/null
#    lastRetVal=$?
#    
#    if [[ $filename == *"_0"* ]]; then
#        retr=1
#    else
#        retr=0
#    fi
#
#    retval $lastRetVal $retr "RAR_($filename)_RET"
#    compare $outPath $refPath "RAR_($filename)_OUT"
#done
#
#printf "\n_________ WINZIP _________\n"
#
#FILES=./tests/test_in/winzip/*
#for f in $FILES
#do
#    filename=$(basename "$f")
#    filename="${filename%.*}"
#    outPath=tests/test_out/winzip/$filename.out
#    refPath=tests/test_ref/winzip/$filename.ref
#
#    ./XtoHashcat.py $f 1>$outPath 2>/dev/null
#    lastRetVal=$?
#    
#    if [[ $filename == *"_0"* ]]; then
#        retr=1
#    else
#        retr=0
#    fi
#
#    retval $lastRetVal $retr "WINZIP_($filename)_RET"
#    compare $outPath $refPath "WINZIP_($filename)_OUT"
#done

printf "\n__________ PDF ___________\n"

FILES=./tests/test_in/pdf/*
for f in $FILES
do
    filename=$(basename "$f")
    filename="${filename%.*}"
    outPath=tests/test_out/pdf/$filename.out
    refPath=tests/test_ref/pdf/$filename.ref

    ./XtoHashcat.py $f 1>$outPath 2>/dev/null
    lastRetVal=$?
    
    if [[ $filename == *"_0"* ]]; then
        retr=1
    else
        retr=0
    fi

    retval $lastRetVal $retr "PDF_($filename)_RET"
    compare $outPath $refPath "PDF_($filename)_OUT"
done

printf "___________________________\n"

printf "${GREEN}PASSED TESTS:${NC} $PASSED_TESTS\n"
printf "${RED}FAILED TESTS:${NC} $FAILED_TESTS\n"
