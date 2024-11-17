#pragma once


#include <vector>
#include <cstdint>
//#include <cstring>
//#include <stdexcept>
//#include <fcntl.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <linux/spi/spidev.h>
//#include <iostream>

namespace SPIConstants {
    constexpr const char* SPI_DEVICE = "/dev/spidev0.1";
    constexpr uint32_t SPEED = 1000000;
    constexpr uint8_t BYTE_PROGRAM = 0x2;
    constexpr uint8_t CMD_READ = 0x3;
    constexpr uint8_t WREN = 0x6;
    constexpr uint8_t WRDI = 0x4;
    constexpr uint8_t RDSR = 0x05;
    constexpr uint8_t CHIP_ERASE_ALL = 0x60;
    constexpr size_t LARGE_SECTOR_SIZE = 256;
    constexpr uint8_t SECTOR_ERASE_CMD = 0x20;
    constexpr uint16_t SECTOR_SIZE = 4096 ;
    constexpr uint32_t ADDRESS_END = 0x20000;
    constexpr uint32_t MEMORY_SIZE = 0x20000;
    
} // namespace SPIConstants

namespace SPI {

union StatusRegister {
    struct {
        uint8_t BUSY : 1;
        uint8_t WEL : 1;
        uint8_t BP0 : 1;
        uint8_t BP1 : 1;
        uint8_t RES : 2;
        uint8_t AAI : 1;
        uint8_t BPL : 1;
    } bits;
    uint8_t byte;
};

    struct Spi_t {
    public:
        Spi_t();
        ~Spi_t();

        void initMemory();
        void write(uint32_t address, const std::vector<uint8_t>& data);
        bool read(uint32_t address, std::vector<uint8_t>& data);
        void eraseAll();
        uint8_t readStatus();
        bool isOpen() const;
        const uint32_t get_spi_speed(){return SPIConstants::SPEED ;}
        uint8_t cmd_byte_spi_duo(const uint8_t);
        void cmd_byte_spi(const uint8_t cmd){            
            sendCommand(cmd);
        }
        void writeEnable();
        void writeDisable();
        const int get_fd(){return fd;}
    private:
        void initDevice();
        void configureSPI();
        
        
        uint8_t sendCommand(uint8_t command, const uint8_t* txBuf = nullptr, uint8_t* rxBuf = nullptr, size_t len = 1);

        int fd;
        uint8_t txBuffer[SPIConstants::LARGE_SECTOR_SIZE]{};
        uint8_t rxBuffer[SPIConstants::LARGE_SECTOR_SIZE]{};
    };
} // namespace SPI
