#pragma once
#include <cstdint>

#define SECTOR_ERASE_CMD 0x20 // Comando para borrar un sector
#define MEMORY_SIZE 131072    // Tamaño total de la memoria (en bytes)
#define SECTOR_SIZE 4096      // Tamaño de cada sector (en bytes)

namespace SPI{
    struct Spi_t ;
}

namespace ST25VF010{

struct St25vf010_t{
    public:
        int read();
        int write();
        int erase();
    protected:
        void erase_sector(SPI::Spi_t &spi, uint32_t address)    ;
        void erase_entire_memory(SPI::Spi_t &spi) ;
    };

}// end namespace ST25VF010