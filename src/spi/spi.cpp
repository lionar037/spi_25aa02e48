//codigo spi.cpp
#include <spi/spi.hpp>
#include <iomanip> 
#include <cstring>
//#include <vector>
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
        std::memset(&spi, 0, sizeof(spi)); // Inicializar estructura
        //std::memset(rx_buffer, 0x00, LARGE_SECTOR_SIZE);
        //std::memset(tx_buffer, 0x00, LARGE_SECTOR_SIZE);

        spi.len = 1;
        tx_buffer[0] = cmd;
        spi.tx_buf = (unsigned long)tx_buffer;
        spi.rx_buf = (unsigned long)rx_buffer;
        spi.speed_hz = get_spi_speed();
        spi.bits_per_word = 8;
        spi.cs_change = 0;

        int ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
        if (ret < 0) {
            std::cerr << "Error en EWSR: " << strerror(errno) << std::endl;
        }
    }//end cmd_byte_spi


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


    const bool Spi_t::read(const uint32_t address, uint8_t* buffer_received,const uint32_t length) {
        auto t = read_write(CMD_READ_DATA ,address,buffer_received,length);
        return t;
    }// end read_data

    const bool Spi_t::read(const uint32_t address, std::vector<uint8_t>& buffer) {
        if (buffer.empty()) {
            throw std::invalid_argument("El buffer proporcionado está vacío.");
            return true;
        }

        // Llamar a read_write con el comando de lectura y la dirección
        auto t = read_write(CMD_READ_DATA, address, buffer.data(), buffer.size());
        return t;
    }


    const bool Spi_t::handle_spi_transfer(const struct spi_ioc_transfer* transfer, size_t length) {
        if (ioctl(fs, SPI_IOC_MESSAGE(2), transfer) < 0) {
            perror("Error al realizar la transferencia SPI");
            throw std::runtime_error("Error en la transferencia SPI");
            return true;
        }
        return false;
    }

    const bool Spi_t::read_write(const uint8_t cmd, const uint32_t address, uint8_t* buffer_received, const uint32_t length) {
        // Inicializar los buffers
        std::vector<uint8_t> cmd_buffer_tx(4, 0);
        std::vector<uint8_t> cmd_buffer_rx(4, 0);
        std::vector<uint8_t> tx_buffer_tmp(length, 0xFF);
        std::vector<uint8_t> rx_buffer_tmp(length, 0xFF);

        // Llenar el buffer de transmisión con el comando de lectura y la dirección
        cmd_buffer_tx[0] = cmd;
        cmd_buffer_tx[1] = (address >> 16) & 0xFF;  // Dirección alta
        cmd_buffer_tx[2] = (address >> 8) & 0xFF;   // Dirección media
        cmd_buffer_tx[3] = address & 0xFF;          // Dirección baja

        // Configurar las transferencias SPI
        struct spi_ioc_transfer spi_transfer[2] = {};

        // Transferencia para enviar el comando de lectura y la dirección
        spi_transfer[0].tx_buf = reinterpret_cast<unsigned long>(cmd_buffer_tx.data());
        spi_transfer[0].rx_buf = reinterpret_cast<unsigned long>(cmd_buffer_rx.data());
        spi_transfer[0].len = 4;
        spi_transfer[0].speed_hz = get_spi_speed();
        spi_transfer[0].bits_per_word = 8;
        spi_transfer[0].cs_change = 0;

        // Transferencia para enviar/recibir los datos
        spi_transfer[1].tx_buf = reinterpret_cast<unsigned long>(tx_buffer_tmp.data());
        spi_transfer[1].rx_buf = reinterpret_cast<unsigned long>(rx_buffer_tmp.data());
        spi_transfer[1].len = length;
        spi_transfer[1].speed_hz = get_spi_speed();
        spi_transfer[1].bits_per_word = 8;
        spi_transfer[1].cs_change = 1;

        // Copiar los datos al buffer de transmisión para escribir
        if (cmd == CMD_WRITE_DATA) {
            std::memcpy(tx_buffer_tmp.data(), buffer_received, length);
        }

        // Ejecutar ambas transferencias SPI
        auto t = handle_spi_transfer(spi_transfer, 2);

        // Copiar los datos recibidos al buffer de lectura
        if (cmd == CMD_READ_DATA) {
            std::memcpy(buffer_received, rx_buffer_tmp.data(), length);
        }
        
        return t;
    }

    void Spi_t::write(uint32_t address, uint8_t* data, const size_t length) {
            writeEnable(); // Habilitar escritura
            tx_buffer[0] = CMD_WRITE;
            tx_buffer[1] = (address >> 16) & 0xFF; // Dirección alta
            tx_buffer[2] = (address >> 8) & 0xFF;  // Dirección media
            tx_buffer[3] = address & 0xFF;         // Dirección baja
            spi.len = length + 4; // Comando + Dirección (3 bytes) + Datos (length bytes)
            //spi.speed_hz = 100000;
            spi.tx_buf = (unsigned long)tx_buffer;
            spi.rx_buf = (unsigned long)rx_buffer;
            spi.speed_hz = get_spi_speed();
            spi.bits_per_word = 8;
            spi.cs_change = 0;
            std::memcpy(tx_buffer + 4, data, length); // Copiar datos a tx_buffer
            std::memset(rx_buffer,0x00,length + 4);
        if ((ioctl(fs, SPI_IOC_MESSAGE(1), &spi)) < 0) {
            std::cerr << "write -> Error al escribir en la memoria: " << strerror(errno) << std::endl;
        }
    }


    const bool Spi_t::read_write(const uint8_t cmd, const uint32_t address, std::vector<uint8_t>& vect) {
        // Inicializar los buffers para el comando
        std::vector<uint8_t> cmd_buffer_tx(4, 0);
        std::vector<uint8_t> cmd_buffer_rx(4, 0);

        // Llenar el buffer de transmisión con el comando y la dirección
        cmd_buffer_tx[0] = cmd;
        cmd_buffer_tx[1] = (address >> 16) & 0xFF;  // Dirección alta
        cmd_buffer_tx[2] = (address >> 8) & 0xFF;   // Dirección media
        cmd_buffer_tx[3] = address & 0xFF;          // Dirección baja

        // Configurar las transferencias SPI
        struct spi_ioc_transfer spi_transfer[2] = {};

        // Transferencia para enviar el comando y la dirección
        spi_transfer[0].tx_buf = reinterpret_cast<unsigned long>(cmd_buffer_tx.data());
        spi_transfer[0].rx_buf = reinterpret_cast<unsigned long>(cmd_buffer_rx.data());
        spi_transfer[0].len = cmd_buffer_tx.size();
        spi_transfer[0].speed_hz = get_spi_speed();
        spi_transfer[0].bits_per_word = 8;
        spi_transfer[0].cs_change = 0;

        // Transferencia para enviar/recibir datos usando el vector
        spi_transfer[1].tx_buf = reinterpret_cast<unsigned long>(cmd == CMD_WRITE_DATA ? vect.data() : nullptr);
        spi_transfer[1].rx_buf = reinterpret_cast<unsigned long>(cmd == CMD_READ_DATA ? vect.data() : nullptr);
        spi_transfer[1].len = vect.size();
        spi_transfer[1].speed_hz = get_spi_speed();
        spi_transfer[1].bits_per_word = 8;
        spi_transfer[1].cs_change = 1;

        // Ejecutar ambas transferencias SPI
        return handle_spi_transfer(spi_transfer, 2);

        // Nota: Para lectura, los datos se escribirán directamente en el vector `vect`.
        // Para escritura, el vector `vect` se usará como buffer de transmisión.
    }
void Spi_t::write(const uint32_t address, const uint8_t data) {
    // Asegúrate de habilitar la escritura en el dispositivo SPI
    writeEnable(); 

    // Llenar el buffer de transmisión con el comando de escritura y los datos
    tx_buffer[0] = CMD_WRITE;  
    tx_buffer[1] = (address >> 16) & 0xFF; // Dirección alta
    tx_buffer[2] = (address >> 8) & 0xFF;  // Dirección media
    tx_buffer[3] = address & 0xFF;         // Dirección baja
    tx_buffer[4] = data;                   // El dato que se va a escribir

    // Configurar la estructura para la transferencia SPI
    spi_ioc_transfer spi_transfer = {};
    spi_transfer.tx_buf = reinterpret_cast<unsigned long>(tx_buffer); // Dirección de tx_buffer
    spi_transfer.rx_buf = 0; // No es necesario recibir datos en esta operación
    spi_transfer.len = 5; // Comando + Dirección (3 bytes) + Dato (1 byte)
    spi_transfer.speed_hz = get_spi_speed();
    spi_transfer.bits_per_word = 8;
    spi_transfer.cs_change = 0; // No cambiar el CS tras la operación

    // Enviar el comando y los datos a través de SPI
    if (ioctl(fs, SPI_IOC_MESSAGE(1), &spi_transfer) < 0) {
        std::cerr << "write -> Error al escribir en la memoria: " << strerror(errno) << std::endl;
    }
}

}//end namespace spi
