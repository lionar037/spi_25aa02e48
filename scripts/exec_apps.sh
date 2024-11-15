#!/bin/bash




./build/erase_sst25vf  
#./build/read_sst25vf  
echo -e "erase "
./build/write_sst25vf
echo -e "write ... "
./build/read_sst25vf 
echo -e "end read "

hexdump  -C 25vf010a.bin
