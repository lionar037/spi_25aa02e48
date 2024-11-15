#!/bin/bash

APP = read_sst25vf
FILE =25vf010a.bin

if [[ -e ${FILE} ]]; then
        rm -Rf ${FILE} 
        ls
else
        echo "no existe la ${FILE} spi"
fi


if [[ -a ${APP}  ]]
then
        rm ${APP} 
	ls
else
        echo "no existe la sst25vf_app spi"
fi



rm -Rf build/
cmake -S . -B build
make -C build

#if [[ -a build/${APP} ]]
#then
#	sudo ./build/${APP} 
#	hexdump -C ${FILE} 
#else
#       echo "no se pudo iniciar la app por que no existe app master spi"
#fi



#g++ -o -g app_wr  write_sst25vf.cc spi.cc

