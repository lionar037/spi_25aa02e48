#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <iostream>
#include <cstring>
#include <linux/spi/spidev.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cerrno>

namespace ST25VF010 {

    void St25vf010_t::erase_sector(SPI::Spi_t &spi_s, uint32_t address) {
        uint8_t wren_cmd = SPIConstants::WREN;
        uint8_t erase_cmd[4] = {
            SPIConstants::SECTOR_ERASE_CMD,
            static_cast<uint8_t>(address >> 16),
            static_cast<uint8_t>(address >> 8),
            static_cast<uint8_t>(address & 0xFF)
        };

        // Habilitar escritura
        if (!spi_s.cmd_byte_spi_duo(wren_cmd)) { 
            std::cerr << "Error enabling write: " << strerror(errno) << std::endl;
            return;
        }

        // Leer el registro de estado para verificar que la escritura está habilitada
        uint8_t status = spi_s.cmd_byte_spi_duo(SPIConstants::RDSR);
        if (!(status & 0x02)) { // Verificar el bit WEL
            std::cerr << "Write enable latch (WEL) not set." << std::endl;
            return;
        }

        // Borrar el sector
        struct spi_ioc_transfer erase_spi = {};
        erase_spi.tx_buf = reinterpret_cast<unsigned long>(erase_cmd);
        erase_spi.len = 4;
        erase_spi.speed_hz = SPIConstants::SPEED;
        erase_spi.bits_per_word = 8;

        if (ioctl(spi_s.get_fs(), SPI_IOC_MESSAGE(1), &erase_spi) < 0) {
            std::cerr << "Error erasing sector: " << strerror(errno) << std::endl;
        }
    }

    void St25vf010_t::erase_entire_memory(SPI::Spi_t &spi_s) {
        const uint32_t num_sectors = MEMORY_SIZE / SECTOR_SIZE;

        for (uint32_t i = 0; i < num_sectors; ++i) {
            uint32_t address = i * SECTOR_SIZE;
            erase_sector(spi_s, address);
            std::cout << "Sector " << i << " erased." << std::endl;
        }
        std::cout << "All sectors erased." << std::endl;
    }

    void St25vf010_t::erase() {
        // Verifica si el dispositivo SPI se ha abierto correctamente
        if (!spi->is_open()) {
            std::cerr << "Failed to open SPI device." << std::endl;
            return;
        }

        // Borrar toda la memoria
        erase_entire_memory(*spi);

        // Cerrar el dispositivo SPI
        spi->spi_close();
    }

} // end namespace
