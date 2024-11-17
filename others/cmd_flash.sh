#!/bin/bash

./app

#sudo flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -erase_all

sudo flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -E


flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -r  file.bin

hexdump -C file.bin


