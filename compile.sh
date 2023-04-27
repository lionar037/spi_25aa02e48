#!/bin/bash

#echo ' Adivina el valor numerico de la variable'
#read A


if [[ -a app ]]
then
        rm app
	ls
else
        echo "no existe la app spi"
fi

g++ -o app main.cc spi.cc

if [[ -a app ]]
then
        sudo ./app
else
        echo "no se pudo iniciar la app por que no existe app master spi"
fi

