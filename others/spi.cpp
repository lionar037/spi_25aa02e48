//archivo spi.cpp"
#include "spi.hpp"
#include <iostream>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <cerrno>

namespace SPACE_SPI {

SPI::SPI(const std::string& device, uint32_t speed) : spi_speed(speed) {
    fs = open(device.c_str(), O_RDWR);
    if (fs < 0) {
        std::cerr << "Error al abrir el dispositivo SPI: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    init();
}

SPI::~SPI() {
    if (fs >= 0) {
        close(fs);
    }
}

void SPI::init() {
    // Configurar el bus SPI
    uint8_t mode = SPI_MODE_0;  // Modo SPI
    uint8_t bits = 8;           // Bits por palabra
    if (ioctl(fs, SPI_IOC_WR_MODE, &mode) < 0) {
        std::cerr << "Error al configurar el modo SPI: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (ioctl(fs, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        std::cerr << "Error al configurar bits por palabra SPI: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }

    if (ioctl(fs, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed) < 0) {
        std::cerr << "Error al configurar la velocidad SPI: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

void SPI::writeEnable() {
    //uint8_t cmd = WREN;  // Comando de habilitación de escritura
    std::memset(tx_buffer, 0xff, sizeof(tx_buffer));  // Limpiar buffer de TX
    std::memset(rx_buffer, 0xff, sizeof(rx_buffer));  // Limpiar buffer de RX

    tx_buffer[0] = WREN;  // Comando de habilitación de escritura
    spi_ioc_transfer spi_transfer{};
    spi_transfer.tx_buf = reinterpret_cast<unsigned long>(tx_buffer);
    spi_transfer.rx_buf = reinterpret_cast<unsigned long>(rx_buffer);
    spi_transfer.bits_per_word = 8;
    spi_transfer.speed_hz = spi_speed;
    spi_transfer.len = 1;  // Solo el comando

    if (ioctl(fs, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        std::cerr << "writeEnable() - Error al habilitar escritura: " << strerror(errno) << std::endl;
    }
}

void SPI::write(uint32_t address, uint8_t* data, size_t length) {
    if (length == 0 || data == nullptr) {
        std::cerr << "write() - Datos vacíos o tamaño de datos incorrecto." << std::endl;
        return;
    }

    std::memset(tx_buffer, 0xff, sizeof(tx_buffer));  // Limpiar buffer de TX
    std::memset(rx_buffer, 0xff, sizeof(rx_buffer));  // Limpiar buffer de RX

    writeEnable();  // Habilitar escritura

    uint8_t cmd = 0x02;  // Comando de escritura
    tx_buffer[0] = cmd;
    tx_buffer[1] = (address >> 16) & 0xFF;  // Dirección alta
    tx_buffer[2] = (address >> 8) & 0xFF;   // Dirección media
    tx_buffer[3] = address & 0xFF;          // Dirección baja
    std::memcpy(tx_buffer + 4, data, length);  // Copiar datos a tx_buffer

    spi_ioc_transfer spi_transfer{};
    spi_transfer.tx_buf = reinterpret_cast<unsigned long>(tx_buffer);
    spi_transfer.rx_buf = reinterpret_cast<unsigned long>(rx_buffer);
    spi_transfer.bits_per_word = 8;
    spi_transfer.speed_hz = spi_speed;
    spi_transfer.len = length + 4;  // Comando + Dirección + Datos

    if (ioctl(fs, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        std::cerr << "write() - Error al escribir en la memoria SPI: " << strerror(errno) << std::endl;
    }
}

uint8_t SPI::read(uint32_t address) {
    std::memset(tx_buffer, 0xff, sizeof(tx_buffer));  // Limpiar buffer de TX
    std::memset(rx_buffer, 0xff, sizeof(rx_buffer));  // Limpiar buffer de RX

    uint8_t cmd = 0x03;  // Comando de lectura
    tx_buffer[0] = cmd;
    tx_buffer[1] = (address >> 16) & 0xFF;  // Dirección alta
    tx_buffer[2] = (address >> 8) & 0xFF;   // Dirección media
    tx_buffer[3] = address & 0xFF;          // Dirección baja

    spi_ioc_transfer spi_transfer{};
    spi_transfer.tx_buf = reinterpret_cast<unsigned long>(tx_buffer);
    spi_transfer.rx_buf = reinterpret_cast<unsigned long>(rx_buffer);
    spi_transfer.bits_per_word = 8;
    spi_transfer.speed_hz = spi_speed;
    spi_transfer.len = 4;  // Comando + Dirección

    if (ioctl(fs, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        std::cerr << "read() - Error al leer desde la memoria SPI: " << strerror(errno) << std::endl;
        return 0x00;  // Error en lectura
    }

    return rx_buffer[4];  // Dato recibido
}

uint8_t SPI::read(uint32_t address, uint8_t* buffer, size_t size_buffer) {
    if (size_buffer == 0 || buffer == nullptr) {
        std::cerr << "read() - Tamaño del buffer inválido o puntero nulo." << std::endl;
        return 0x00;  // Error en lectura
    }

    std::memset(tx_buffer, 0xff, sizeof(tx_buffer));  // Limpiar buffer de TX
    std::memset(rx_buffer, 0xff, sizeof(rx_buffer));  // Limpiar buffer de RX

    uint8_t cmd = 0x03;  // Comando de lectura
    tx_buffer[0] = cmd;
    tx_buffer[1] = (address >> 16) & 0xFF;  // Dirección alta
    tx_buffer[2] = (address >> 8) & 0xFF;   // Dirección media
    tx_buffer[3] = address & 0xFF;          // Dirección baja

    spi_ioc_transfer spi_transfer{};
    spi_transfer.tx_buf = reinterpret_cast<unsigned long>(tx_buffer);
    spi_transfer.rx_buf = reinterpret_cast<unsigned long>(rx_buffer);
    spi_transfer.bits_per_word = 8;
    spi_transfer.speed_hz = spi_speed;
    spi_transfer.len = 4 + size_buffer;  // Comando + Dirección + Datos

    if (ioctl(fs, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        std::cerr << "read() - Error al leer desde la memoria SPI: " << strerror(errno) << std::endl;
        return 0x00;  // Error en lectura
    }

    std::memcpy(buffer, rx_buffer + 4, size_buffer);  // Copiar los datos leídos a buffer
    return 0x00;  // Lectura exitosa
}

}  // namespace SPACE_SPI
