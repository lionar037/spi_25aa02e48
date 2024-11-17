#!/bin/bash
#software para leer y escribir memorias WINBOND 25Q32 etc...

sudo apt install -y libpci-dev
sudo apt install -y libusb-1.0
sudo apt install -y libusb-dev
sudo apt-get install build-essential -y
sudo apt-get install -y libusb-1.0-0-dev

sudo apt isntall -y meson

cd ~/src
git clone https://github.com/flashrom/flashrom.git
cd flashrom
make
sudo make install

find . -name "chip.c" -exec sed -i 's/printf("Progress almost complete for stage %d, current %ld, total %ld\\n",/printf("Progress almost complete for stage %d, current %zu, total %zu\\n",/g' {} +


meson setup builddir
meson compile -C builddir
meson test -C builddir
meson install -C builddir


#flashrom -p linux_spi:dev=/dev/spidev0.0,spispeed=512  


# Ruta al archivo .bashrc
BASHRC_FILE="$HOME/.bashrc"

# Línea que queremos agregar
LINE="export PATH=\$PATH:/usr/sbin"

# Verificar si la línea ya existe en el archivo .bashrc
if ! grep -Fxq "$LINE" "$BASHRC_FILE"; then
    # Si no existe, agregarla al final del archivo
    echo "$LINE" >> "$BASHRC_FILE"
    echo "Línea agregada a .bashrc"
else
    echo "La línea ya existe en .bashrc"
fi

# Recargar .bashrc para que los cambios tomen efecto
source "$BASHRC_FILE"

# Verificar si flashrom funciona
flashrom --version

