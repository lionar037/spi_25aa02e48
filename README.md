# codigo para leer y escribir memoria ST25VF010A de microchip

# ejecutar:

git clone url...

git pull 

make -j4

make run

# genera un archivo "25vf010a.bin"

# leer con :

hexdump -C files/25vf010a.bin

#software para leer y escribir memorias WINBOND 25Q32 etc...

sudo apt install -y libpci-dev
sudo apt install -y libusb-1.0
sudo apt install -y libusb-dev
sudo apt-get install build-essential -y
sudo apt-get install -y libusb-1.0-0-dev

sudo apt isntall -y meson

# comandos para memoria WINBOND 25Q16...

LINK=files/st25vf010a_flashrom.bin

#permisos
sudo usermod -aG spi $(whoami)

#read
flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -r $LINK

#write
flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=256 -w $LINK

#erase
flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=256 -erase_all
