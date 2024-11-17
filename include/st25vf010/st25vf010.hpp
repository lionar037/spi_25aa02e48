//codigo st25vf010.hpp

#pragma once

#include <cstdint>
#include <memory>

namespace SPI{
    struct Spi_t;
}

namespace ST25VF010{

struct St25vf010_t{
    public:
        St25vf010_t();
        ~St25vf010_t()=default;

        void read();
        void write();
        void write_aai();
        void write(const uint8_t);
        void write_rand();
        void erase();

    protected:
        void init();
        void erase_sector(SPI::Spi_t &spi, uint32_t address)    ;
        void erase_entire_memory(SPI::Spi_t &spi) ;
    private:
        std::unique_ptr<SPI::Spi_t>spi;
    };

}// end namespace ST25VF010