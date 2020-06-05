#!/usr/bin/env bash

make clean
make linux -j
objcopy --strip-unneeded ./bin/runner.bin ../server/client_bin/x86_64-pc-linux-gnu/runner1.bin
objcopy --only-keep-debug ./bin/runner.bin ./bin/runner.bin.debug
rm ./bin/runner.bin

make clean
make windows -j
objcopy --strip-unneeded ./bin/runner.exe ../server/client_bin/windows_x86_64/runner1.exe
objcopy --only-keep-debug ./bin/runner.exe ./bin/runner.exe.debug
rm ./bin/runner.exe