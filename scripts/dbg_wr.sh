#!/bin/bash

g++ -g -o app_wr spi.cc write_sst25vf.cc

#g++ -g -o app_rd spi.cc read_sst25vf.cc

gdb -x debugger.txt ./app_wr

#gdb -x debugger.txt ./app_rd
