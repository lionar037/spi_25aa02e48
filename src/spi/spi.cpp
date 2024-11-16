//codigo spi.cpp
#include <spi/spi.hpp>
#include <iomanip> 
#include <cstring>
#include <stdexcept>

namespace SPI {

    Spi_t::Spi_t(): spi_speed(SPEED) { 
        init();
        settings_spi();
        init_sst25vf();
    }

    Spi_t::~Spi_t() {
        spi_close();
    }

    void Spi_t::init() {
        fs = open(SPI_DEVICE, O_RDWR);
        if (fs < 0) {
            perror("Could not open the SPI device");
            exit(EXIT_FAILURE);
        }

        const uint32_t mode = SPI_MODE_0;
        if (ioctl(fs, SPI_IOC_WR_MODE, &mode) < 0) {
            perror("Could not set SPI mode");
            close(fs);
            exit(EXIT_FAILURE);
        }

        const uint32_t speed = spi_speed;
        if (ioctl(fs, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
            perror("Could not set SPI max speed");
            close(fs);
            exit(EXIT_FAILURE);
        }

        const uint8_t bits = 8;
        if (ioctl(fs, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
            perror("Could not set SPI bits per word");
            close(fs);
            exit(EXIT_FAILURE);
        }

    }//end init

    void Spi_t::settings_spi() {
        spi.len = LARGE_SECTOR_SIZE;
        spi.tx_buf = (unsigned long)tx_buffer;
        spi.rx_buf = (unsigned long)rx_buffer;
        spi.bits_per_word = 8;
        spi.speed_hz = get_spi_speed();
        spi.delay_usecs = 0;        
        memset(tx_buffer, 0xff, sizeof(tx_buffer));
        memset(rx_buffer, 0xff, sizeof(rx_buffer));
    }//end settings_spi

    void Spi_t::init_sst25vf() {
        cmd_byte_spi(EWSR);
        [[maybe_unused]]int status= cmd_byte_spi_duo(WRSR);
        #ifdef DBG_SPI
        std::cout << std::to_string(status)<<"\n";
        #endif
    }

    void Spi_t::spi_close() {
        if (fs >= 0) {
            close(fs);
        }
    }//end spi_close

   const uint32_t Spi_t::get_spi_speed(){
        return spi_speed;
    }//end get_spi_speed


    void Spi_t::writeEnable() {
        cmd_byte_spi(WREN);
    }


    void Spi_t::writeDisable() {
        cmd_byte_spi(WRDI); //Write-Disable
    }

    void Spi_t::cmd_byte_spi(const uint8_t cmd) {
        // Inicializar la estructura spi_ioc_transfer
        spi_ioc_transfer spi_transfer = {};        

        // Llenar el buffer de transmisión con el comando
        tx_buffer[0] = cmd;

        // Configurar la estructura de transferencia SPI
        spi_transfer.len = 1;                               // Solo un byte
        spi_transfer.tx_buf = reinterpret_cast<unsigned long>(tx_buffer);  // Dirección del buffer de transmisión
        spi_transfer.rx_buf = reinterpret_cast<unsigned long>(rx_buffer);  // Dirección del buffer de recepción
        spi_transfer.speed_hz = get_spi_speed();            // Configuración de velocidad SPI
        spi_transfer.bits_per_word = 8;                     // Configuración de bits por palabra
        spi_transfer.cs_change = 0;                         // No cambiar el chip select

        // Ejecutar la transferencia SPI
        int ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi_transfer);
        if (ret < 0) {
            std::cerr << "Error en EWSR: " << strerror(errno) << " (Código de error: " << ret << ")" << std::endl;
        }
    }

    const uint8_t Spi_t::cmd_byte_spi_duo(const uint8_t cmd){        
        std::memset(&spi, 0, sizeof(spi)); // Reinicializar estructura
        spi.len = 2;
        tx_buffer[0] = cmd;
        tx_buffer[1] = 0x00;
        spi.tx_buf = (unsigned long)tx_buffer;
        spi.rx_buf = (unsigned long)rx_buffer;
        spi.speed_hz = get_spi_speed();
        spi.bits_per_word = 8;
        spi.cs_change = 0;
        int ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
        if (ret < 0) {
            std::cerr << "Error en WRSR: " << strerror(errno) << std::endl;
        }
        return rx_buffer[1];
    }// end cmd_byte_spi_duo


    const uint8_t Spi_t::read_status(){
        const uint8_t stat = cmd_byte_spi_duo(RDSR);
    
        [[maybe_unused]]statusREGISTER statReg;
        statReg.byte = stat;    
    #ifdef DBG_SPI
        // Accede a los campos dentro de la estructura 'status'
        std::cout << " BP0 : " << statReg.status.BP0 << " BP1 : " << statReg.status.BP1 << std::endl;
    #endif
        return stat;
    }

    void Spi_t::erase_sst25_all(){
        writeEnable();
        init_sst25vf();
        cmd_byte_spi(CHIP_ERASE_ALL);
        writeDisable();
    }


    bool Spi_t::is_open(){
    return true;
    }// end is_open

    const uint8_t Spi_t::get_fs(){
    return 0x00;
    }// end get_fs



    const bool Spi_t::handle_spi_transfer(const struct spi_ioc_transfer* transfer, size_t length) {
        if (ioctl(fs, SPI_IOC_MESSAGE(2), transfer) < 0) {
            perror("Error al realizar la transferencia SPI");
            throw std::runtime_error("Error en la transferencia SPI");
        return true;
        }
    return false;
    }

    const bool Spi_t::read(const uint32_t address, std::vector<uint8_t>& vect) {
        return read_write(CMD_READ_DATA, address, vect);
    }

    const bool Spi_t::read(const uint32_t address, uint8_t* buffer, const uint32_t length) {
        std::vector<uint8_t> tmp_buffer(buffer, buffer + length);
        return read_write(CMD_READ_DATA, address, tmp_buffer);
    }


    void Spi_t::write(const uint32_t address, uint8_t* buffer, const size_t length) {
        writeEnable();
        std::vector<uint8_t> tmp_buffer(buffer, buffer + length);
        read_write(CMD_WRITE_DATA, address, tmp_buffer);
    }

        void Spi_t::write(const uint32_t address, std::vector<uint8_t>& vect_buffer) {
        writeEnable();
        read_write(AAI_CMD, address, vect_buffer);
    }

        void Spi_t::write_aai(const uint32_t address, std::vector<uint8_t>& vect_buffer) {
        //writeEnable();
        read_write(AAI_CMD, address, vect_buffer);
    }

}//end namespace spi
