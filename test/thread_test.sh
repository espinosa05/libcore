#!/bin/bash

gcc -DTT_PRINT -ggdb -I ../include thread_test.c ../core/out/libcore.a -o thread_test
