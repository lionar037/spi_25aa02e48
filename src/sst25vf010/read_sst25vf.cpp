/*
#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace ST25VF010{

    void St25vf010_t::read() {
        
        int print=0;
        // Verifica si el dispositivo SPI se ha abierto correctamente
        if (!spi.is_open()) {
            std::cerr << "Failed to open SPI device." << std::endl;
            //return 1;
        }

        const uint32_t block_size =  256; // Tamaño del bloque en bytes
        const uint32_t num_blocks = 128*4; // * 64; // Número de bloques para leer

        std::ofstream outputFile("25vf010a.bin", std::ios::binary);
        if (!outputFile) {
            std::cerr << "Error al abrir el archivo para escritura." << std::endl;
            //return 1;
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

        //return 0;
    }

}

*/

#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <fstream>
#include <iostream>
#include <filesystem> // C++17

namespace ST25VF010 {

    void St25vf010_t::read() {
        // Verifica si el dispositivo SPI se ha abierto correctamente
        if (!spi.is_open()) {
            std::cerr << "Failed to open SPI device." << std::endl;
            return;
        }

        const uint32_t block_size = 256;    // Tamaño del bloque en bytes
        const uint32_t num_blocks = 128 * 4; // Número de bloques para leer

        // Define la carpeta y archivo de salida
        std::string folder = "files";
        std::string file_path = folder + "/25vf010a.bin";

        // Crear carpeta si no existe
        if (!std::filesystem::exists(folder)) {
            if (!std::filesystem::create_directory(folder)) {
                std::cerr << "Error al crear la carpeta: " << folder << std::endl;
                return;
            }
        }

        // Abrir archivo para guardar los datos
        std::ofstream outputFile(file_path, std::ios::binary);
        if (!outputFile) {
            std::cerr << "Error al abrir el archivo para escritura: " << file_path << std::endl;
            return;
        }

        uint8_t buffer_rd[block_size] = {0};

        // Leer datos de la memoria SPI y guardarlos en el archivo
        for (uint32_t block = 0; block < num_blocks; ++block) {
            uint32_t address = block * block_size; // Dirección de inicio para cada bloque

            // Leer el bloque de datos desde la memoria SPI
            spi.read(address, buffer_rd, block_size);

            // Escribir el bloque leído en el archivo
            outputFile.write(reinterpret_cast<char *>(buffer_rd), block_size);
        }

        outputFile.close();
        std::cout << "Lectura completa y guardada en " << file_path << std::endl;
    }

} // namespace ST25VF010
