#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "spi.h"

#define MEMORY_SIZE (32 * 1024) // Tama      o de la memoria en bytes (32 KB)


int main() {
    SPACE_SPI::SPI spi;

    // Verifica si el dispositivo SPI se ha abierto correctamente
    if (!spi.is_open()) {
        std::cerr << "Failed to open SPI device." << std::endl;
        return 1;
    }
 
//    const uint32_t block_size = 256; // Tamaño del bloque en bytes
 //   const uint32_t num_blocks = 128; // Número de bloques para leer

//const uint32_t block_size = 4; 
    std::ofstream outputFile("25vf010a.bin", std::ios::binary);
    if (!outputFile) {
        std::cerr << "Error al abrir el archivo para escritura." << std::endl;
        return 1;
    }

   

  uint8_t buffer[256]; // Buffer para leer bloques de datos

    for (uint32_t address = 0; address < MEMORY_SIZE; address += sizeof(buffer)) {
        spi.read_data(address, buffer, sizeof(buffer));

        outputFile.write(reinterpret_cast<char*>(buffer), sizeof(buffer));
    }

    outputFile.close();
    std::cout << "Lectura completa y guardada en 25vf010a.bin" << std::endl;

    return 0;
}
