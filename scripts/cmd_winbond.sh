#!/bin/bash

LINK=st25vf010a_flashrom.bin

#permisos
sudo usermod -aG spi $(whoami)

#read
flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -r $LINK

#write
flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=256 -w $LINK

#erase
flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=256 -erase_all
