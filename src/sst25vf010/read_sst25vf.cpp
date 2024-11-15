#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

//#define MEMORY_SIZE (32 * 1024) // Tama      o de la memoria en bytes (32 KB)

#define MEMORY_SIZE ADDRESS_END

namespace ST25VF010{

    int St25vf010_t::read() {
        SPI::Spi_t spi;
    int print=0;
        // Verifica si el dispositivo SPI se ha abierto correctamente
        if (!spi.is_open()) {
            std::cerr << "Failed to open SPI device." << std::endl;
            return 1;
        }


        const uint32_t block_size =  256; // Tamaño del bloque en bytes
        const uint32_t num_blocks = 128*4; // * 64; // Número de bloques para leer

        std::ofstream outputFile("25vf010a.bin", std::ios::binary);
        if (!outputFile) {
            std::cerr << "Error al abrir el archivo para escritura." << std::endl;
            return 1;
        }
        uint8_t buffer_rd[block_size] = {0};

        // Leer datos de la memoria SPI y guardarlos en el archivo
        for (uint32_t block = 0; block < num_blocks; ++block) {
            uint32_t address = block * block_size; // Dirección de inicio para cada bloque

            // Lee el bloque de datos desde la memoria SPI
            spi.read(address, buffer_rd, block_size);


        if(print==1){
            // Verifica el contenido del buffer antes de escribir
            std::cout << "Bloque " << block << ":" << std::endl;
            for (size_t i = 0; i < block_size; ++i) {
                std::cout << " " << std::hex << static_cast<int>(buffer_rd[i]);
            if((i%32) == 0){std::cout  << std::endl;}
            }
            std::cout << std::dec << std::endl;
        }
            // Escribe el bloque leído en el archivo
            outputFile.write(reinterpret_cast<char*>(buffer_rd), block_size);
        }

        outputFile.close();
        std::cout << "Lectura completa y guardada en 25vf010a.bin" << std::endl;

        return 0;
    }

}