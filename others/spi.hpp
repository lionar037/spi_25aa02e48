#ifndef SPI_H
#define SPI_H

#include <iostream>
#include <cstring>
#include <cerrno>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

namespace SPACE_SPI {

struct SPI {
public:
    SPI(const std::string& device, uint32_t speed);
    ~SPI();

    void writeEnable();
    void write(uint32_t address, uint8_t* data, size_t length);
    uint8_t read(uint32_t address);
    uint8_t read(uint32_t address, uint8_t* buffer, size_t size_buffer);

private:
    int fs;  // Descriptor de archivo SPI
    uint32_t spi_speed;  // Velocidad SPI
    uint8_t tx_buffer[256];  // Buffer de transmisión
    uint8_t rx_buffer[256];  // Buffer de recepción

    void init();
};

}  // namespace SPACE_SPI

#endif  // SPI_H
