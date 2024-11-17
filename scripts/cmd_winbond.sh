#!/bin/bash

# Archivo para leer/escribir/guardar datos en la memoria
LINK="files/st25vf010a_flashrom.bin"

# Comando recibido como argumento
CMD="$1"

# Verifica que se proporcionó un comando válido
if [[ -z $CMD ]]; then
    echo "Uso: $0 {read|write|erase}"
    exit 1
fi

# Asegúrate de que el usuario tenga permisos sobre SPI (configuración previa)
if ! groups $(whoami) | grep -q '\bspi\b'; then
    echo "El usuario no pertenece al grupo 'spi'. Añádelo manualmente con:"
    echo "sudo usermod -aG spi $(whoami)"
    exit 1
fi

# Ejecuta el comando correspondiente
case $CMD in
    read)
        echo "Leyendo desde la memoria..."
        flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -r "$LINK"
        #hexdump -C $LINK 
        ;;
    write)
        echo "Escribiendo en la memoria..."
        flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -w "$LINK"
        ;;
    erase)
        echo "Borrando toda la memoria..."
        flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -E
        #flashrom -p linux_spi:dev=/dev/spidev0.1,spispeed=512 -r "$LINK"
        #hexdump -C $LINK
        ;;
    *)
        echo "Comando inválido. Uso: $0 {read|write|erase}"
        exit 1
        ;;
esac
