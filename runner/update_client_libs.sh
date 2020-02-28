#!/usr/bin/env bash

make clean
make linux
mv ./bin/runner.bin ../server/client_bin/x86_64-pc-linux-gnu/runner1.bin

make clean
make windows
mv ./bin/runner.exe ../server/client_bin/windows_x86_64/runner1.exe