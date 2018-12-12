#!/bin/bash
make clean && make -j8 linux && make cleanobj && make -j8 windows
