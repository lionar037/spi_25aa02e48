#include <spi/spi.hpp>
#include <iomanip>

namespace SPI {

Spi::Spi() : fd(-1) {
    initDevice();
    configureSPI();
}

Spi::~Spi() {
    if (fd >= 0) {
        close(fd);
    }
}

void Spi::initDevice() {
    fd = open(SPIConstants::SPI_DEVICE, O_RDWR);
    if (fd < 0) {
        throw std::runtime_error("Error al abrir el dispositivo SPI: " + std::string(strerror(errno)));
    }

    uint32_t mode = SPI_MODE_0;
    if (ioctl(fd, SPI_IOC_WR_MODE, &mode) < 0) {
        throw std::runtime_error("Error al configurar el modo SPI: " + std::string(strerror(errno)));
    }

    uint32_t speed = SPIConstants::SPEED;
    if (ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        throw std::runtime_error("Error al configurar la velocidad SPI: " + std::string(strerror(errno)));
    }

    uint8_t bits = 8;
    if (ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
        throw std::runtime_error("Error al configurar bits por palabra SPI: " + std::string(strerror(errno)));
    }
}

void Spi::configureSPI() {
    memset(txBuffer, 0xFF, sizeof(txBuffer));
    memset(rxBuffer, 0xFF, sizeof(rxBuffer));
}

void Spi::initMemory() {
    sendCommand(SPIConstants::WREN);
}

uint8_t Spi::sendCommand(uint8_t command, const uint8_t* txBuf, uint8_t* rxBuf, size_t len) {
    spi_ioc_transfer transfer = {};
    transfer.tx_buf = reinterpret_cast<uintptr_t>(&command);
    transfer.rx_buf = reinterpret_cast<uintptr_t>(rxBuf);
    transfer.len = len;
    transfer.speed_hz = SPIConstants::SPEED;
    transfer.bits_per_word = 8;

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        throw std::runtime_error("Error en la transferencia SPI: " + std::string(strerror(errno)));
    }
    return rxBuf ? rxBuf[0] : 0;
}

void Spi::writeEnable() {
    sendCommand(SPIConstants::WREN);
}

void Spi::writeDisable() {
    sendCommand(SPIConstants::WRDI);
}

uint8_t Spi::readStatus() {
    uint8_t status = 0;
    sendCommand(SPIConstants::CMD_READ, nullptr, &status);
    return status;
}

void Spi::eraseAll() {
    writeEnable();
    sendCommand(SPIConstants::CHIP_ERASE_ALL);
    writeDisable();
}

void Spi::write(uint32_t address, const std::vector<uint8_t>& data) {
    if (data.size() > SPIConstants::LARGE_SECTOR_SIZE) {
        throw std::invalid_argument("El tamaño de datos excede el tamaño del sector.");
    }

    writeEnable();
    // Configurar comando y dirección
    txBuffer[0] = SPIConstants::BYTE_PROGRAM;
    txBuffer[1] = (address >> 16) & 0xFF;
    txBuffer[2] = (address >> 8) & 0xFF;
    txBuffer[3] = address & 0xFF;

    // Copiar los datos al buffer de transmisión
    std::copy(data.begin(), data.end(), txBuffer + 4);

    spi_ioc_transfer transfer = {};
    transfer.tx_buf = reinterpret_cast<uintptr_t>(txBuffer);
    transfer.len = data.size() + 4;
    transfer.speed_hz = SPIConstants::SPEED;
    transfer.bits_per_word = 8;

    if (ioctl(fd, SPI_IOC_MESSAGE(1), &transfer) < 0) {
        throw std::runtime_error("Error al escribir datos en la memoria SPI.");
    }
    writeDisable();
}

bool Spi::read(uint32_t address, std::vector<uint8_t>& data) {
    if (data.empty()) {
        throw std::invalid_argument("El buffer de lectura está vacío.");
    }

    txBuffer[0] = SPIConstants::CMD_READ;
    txBuffer[1] = (address >> 16) & 0xFF;
    txBuffer[2] = (address >> 8) & 0xFF;
    txBuffer[3] = address & 0xFF;

    spi_ioc_transfer transfer[2] = {};
    transfer[0].tx_buf = reinterpret_cast<uintptr_t>(txBuffer);
    transfer[0].len = 4;
    transfer[1].rx_buf = reinterpret_cast<uintptr_t>(data.data());
    transfer[1].len = data.size();

    if (ioctl(fd, SPI_IOC_MESSAGE(2), transfer) < 0) {
        return false;
    }
    return true;
}

bool Spi::isOpen() const {
    return fd >= 0;
}

} // namespace SPI
