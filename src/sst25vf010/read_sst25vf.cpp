//read_sst25vf.cpp

#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <fstream>
#include <iostream>
#include <filesystem> // C++17


namespace ST25VF010 {

    void St25vf010_t::read() {
        // Verifica si el dispositivo SPI se ha abierto correctamente
        if (!spi->is_open()) {
            std::cerr << "Failed to open SPI device." << std::endl;
            return;
        }

        const uint32_t block_size = 256;    // Tamaño del bloque en bytes
        //const uint32_t num_blocks = 128 * 4; // Número de bloques para leer
        const uint32_t num_blocks = (MEMORY_SIZE + block_size - 1) / block_size; // Ajustar según tamaño real

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

        //s[[maybe_unused]]uint8_t buffer_rd[block_size] = {0};

        std::vector<uint8_t>vect_buffer_rd(block_size,0);
        std::cout <<"\n";
        // Leer datos de la memoria SPI y guardarlos en el archivo
        for (uint32_t block = 0; block < num_blocks; ++block) {
            uint32_t address = block * block_size; // Dirección de inicio para cada bloque

            // Leer el bloque de datos desde la memoria SPI
            //spi->read(address, buffer_rd, block_size);            
            spi->read(address, vect_buffer_rd);

            //if (vect_buffer_rd[0] != 0xae){
            //    std::cout <<std::to_string(address)<<"\n";
            //}

            auto it = std::find_if(vect_buffer_rd.begin(), vect_buffer_rd.end(), [](uint8_t byte) {
                return byte != 0xAE;
            });

            //// Si se encontró un elemento diferente, imprimir la dirección
            if (it != vect_buffer_rd.end()) {
                std::cout << "Diferente de 0xAE en dirección: " << std::to_string(address) << "\n";
            }

            // Escribir el bloque leído en el archivo
            //outputFile.write(reinterpret_cast<char *>(buffer_rd), block_size);
            outputFile.write(reinterpret_cast<const char *>(vect_buffer_rd.data()), vect_buffer_rd.size());
        }

        std::cout <<"\n";
        outputFile.close();
        std::cout << "Lectura completa y guardada en " << file_path << std::endl;
    }

} // namespace ST25VF010


/*
namespace ST25VF010 {

    void St25vf010_t::read() {
        if (!spi->is_open()) {
            std::cerr << "Failed to open SPI device." << std::endl;
            return;
        }

        const uint32_t block_size = 256;
        const uint32_t num_blocks = (MEMORY_SIZE + block_size - 1) / block_size; // Ajustar según tamaño real

        std::string folder = "files";
        std::string file_path = folder + "/25vf010a.bin";

        if (!std::filesystem::exists(folder)) {
            if (!std::filesystem::create_directory(folder)) {
                std::cerr << "Error al crear la carpeta: " << folder << std::endl;
                return;
            }
        }

        std::ofstream outputFile(file_path, std::ios::binary);
        if (!outputFile) {
            std::cerr << "Error al abrir el archivo para escritura: " << file_path << std::endl;
            return;
        }

        uint8_t buffer_rd[block_size] = {0};

        for (uint32_t block = 0; block < num_blocks; ++block) {
            uint32_t address = block * block_size;

            if (address >= MEMORY_SIZE) {
                std::cerr << "Dirección fuera de rango: " << address << std::endl;
                break; // Detener lectura
            }

            std::cout << "Leyendo bloque " << block 
                      << " en dirección: " << address << std::endl;

            if (!spi->read(address, buffer_rd, block_size)) {
                std::cerr << "Error al leer el bloque en la dirección: " << address << std::endl;
                continue;
            }

            outputFile.write(reinterpret_cast<char *>(buffer_rd), block_size);
        }

        outputFile.close();
        std::cout << "Lectura completa y guardada en " << file_path << std::endl;
    }

} // namespace ST25VF010


*/