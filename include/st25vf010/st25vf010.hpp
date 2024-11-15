#pragma once
#include <spi/spi.hpp>
#include <cstdint>

#define SECTOR_ERASE_CMD 0x20 // Comando para borrar un sector
#define SECTOR_SIZE 4096      // Tamaño de cada sector (en bytes)

#define ADDRESS_END         0x1FFFF

#define MEMORY_SIZE ADDRESS_END
//#define MEMORY_SIZE (32 * 1024) // Tama      o de la memoria en bytes (32 KB)
//#define MEMORY_SIZE 131072    // Tamaño total de la memoria (en bytes)

namespace SPI{
    struct Spi_t;
}

namespace ST25VF010{

struct St25vf010_t{
    public:
        St25vf010_t()=default;
        ~St25vf010_t()=default;
        int read();
        int write();
        int erase();
    protected:
        void erase_sector(SPI::Spi_t &spi, uint32_t address)    ;
        void erase_entire_memory(SPI::Spi_t &spi) ;
    private:
        SPI::Spi_t spi{};
    };

}// end namespace ST25VF010