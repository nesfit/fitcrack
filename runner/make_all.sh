#!/bin/bash
cd $(dirname ${BASH_SOURCE[0]})
make clean && make -j8 linux && make cleanobj && make -j8 windows
