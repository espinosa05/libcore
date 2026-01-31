#!/bin/bash
set -x

gcc -c wm_test.c -I ../include/
gcc wm_test.o ../core/out/libcore.a -o wm_test -lvulkan -lxcb -lxcb-icccm -lxcb-keysyms
./wm_test
