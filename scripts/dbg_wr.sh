#!/bin/bash

g++ -g -o app_wr spi.cpp write_sst25vf.cpp


gdb -x debugger.txt ./app_wr

#gdb -x debugger.txt ./app_rd
