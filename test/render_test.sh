#!/bin/bash
set -x

gcc -c render_test.c -I ../include/ -I ../../libkiek/include/
gcc render_test.o ../../libkiek/kiek/out/libkiek.a ../core/out/libcore.a -o render_test -lvulkan -lxcb -lxcb-icccm -lxcb-keysyms
./render_test
