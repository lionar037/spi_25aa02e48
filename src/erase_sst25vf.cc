#include "spi.h"
#include <iostream>

#define SECTOR_ERASE_CMD 0x20 // Comando para borrar un sector
#define MEMORY_SIZE 131072    // Tamaño total de la memoria (en bytes)
#define SECTOR_SIZE 4096      // Tamaño de cada sector (en bytes)

void erase_sector(SPACE_SPI::SPI &spi, uint32_t address) {
    uint8_t wren_cmd = WREN;
    uint8_t erase_cmd[4] = {SECTOR_ERASE_CMD, (uint8_t)(address >> 16), (uint8_t)(address >> 8), (uint8_t)(address & 0xFF)};

    // Habilitar escritura
    struct spi_ioc_transfer wr_spi = {};
    wr_spi.tx_buf = reinterpret_cast<unsigned long>(&wren_cmd);
    wr_spi.len = 1;
    wr_spi.speed_hz = spi.get_spi_speed(); // Utiliza el método get_spi_speed()
    wr_spi.bits_per_word = 8;

    if (ioctl(spi.get_fs(), SPI_IOC_MESSAGE(1), &wr_spi) < 0) { // Utiliza el método get_fs()
        std::cerr << "Error enabling write: " << strerror(errno) << std::endl;
        return;
    }

    // Leer el registro de estado para verificar que la escritura está habilitada
    uint8_t status =spi.cmd_byte_spi_duo(RDSR);// spi.read_status();

    if (!(status & 0x02)) { // Verificar el bit WEL
        std::cerr << "Write enable latch (WEL) not set." << std::endl;
        return;
    }

    // Borrar el sector
    wr_spi.tx_buf = reinterpret_cast<unsigned long>(erase_cmd);
    wr_spi.len = 4;

    if (ioctl(spi.get_fs(), SPI_IOC_MESSAGE(1), &wr_spi) < 0) { // Utiliza el método get_fs()
        std::cerr << "Error erasing sector: " << strerror(errno) << std::endl;
    }
}

void erase_entire_memory(SPACE_SPI::SPI &spi) {
    const uint32_t num_sectors = MEMORY_SIZE / SECTOR_SIZE;

    for (uint32_t i = 0; i < num_sectors; ++i) {
        uint32_t address = i * SECTOR_SIZE;
        erase_sector(spi, address);
        std::cout << "Sector " << i << " erased." << std::endl;
    }

    std::cout << "All sectors erased." << std::endl;
}

int main() {
    SPACE_SPI::SPI spi;

    // Verifica si el dispositivo SPI se ha abierto correctamente
    if (!spi.is_open()) {
        std::cerr << "Failed to open SPI device." << std::endl;
        return 1;
    }

spi.erase_sst25_all();
spi.spi_close();
    // Borrar toda la memoria flash
//    erase_entire_memory(spi);

    return 0;
}
