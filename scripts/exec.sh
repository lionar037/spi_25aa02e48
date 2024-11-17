#!/bin/bash

DIR=files

CMD="| head -n 0x1ff"
# Detectar el sistema operativo
os_name=$(uname)

# Detectar la arquitectura (32 o 64 bits)
if [ "$(uname -m)" == "x86_64" ]; then
    arch="64 bits"
elif [[ "$(uname -m)" == aarch64 ]]; then
    arch="ARM de 64 bits"    
elif [[ "$(uname -m)" == arm* || "$(uname -m)" == aarch32 ]]; then
    arch="ARM de 32 bits"
else
    arch="32 bits"
fi

# Identificar el sistema operativo
case "$os_name" in
    "Linux")
        echo "El sistema operativo es Linux y es de $arch."
        ;;
    "Darwin")
        echo "El sistema operativo es macOS y es de $arch."
        ;;
    "CYGWIN"* | "MINGW"* | "MSYS"*)
        echo "El sistema operativo es Windows y es de $arch."
        ;;
    *)
        echo "Sistema operativo desconocido ($os_name) y es de $arch."
        ;;
esac

FILES_GEN=$DIR/25vf010a.bin




# Verificar si el directorio contiene archivos
if [ -d "$DIR" ] && [ "$(ls -A "$DIR")" ]; then
    echo "El directorio $DIR contiene los siguientes archivos:"
    ls -l "$DIR"
    rm $DIR/*
else
    echo "El directorio $DIR está vacío o no contiene archivos."
fi

arch=$(uname -m)

if [ "$arch" == "aarch64" ]; then
    ./bin/st25vf010_app_$arch
elif [ "$arch" == "armv7l" ]; then
    ./bin/st25vf010_app_$arch
elif [[ "$arch" == arm* ]]; then
    ./bin/st25vf010_app_$arch
fi

#verificar si el archivo existe pero no está vacío
if [[ -s $FILES_GEN ]];then 
    hexdump -C $FILES_GEN | head -n 10
fi

./scripts/cmd_winbond.sh  read

FILES_GEN=$DIR/st25vf010a_flashrom.bin

hexdump -C $FILES_GEN $CMD

#python3 scripts/python_compare/compare.py 

