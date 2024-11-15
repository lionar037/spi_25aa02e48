#include <iomanip> 
#include <cstring>
#include "spi.h"


namespace SPACE_SPI {

    SPI::SPI(): spi_speed(SPEED) { 
        init();
        settings_spi();
        init_sst25vf();
    }

    SPI::~SPI() {
        spi_close();
    }

    void SPI::init() {
        fs = open(SPI_DEVICE, O_RDWR);
        if (fs < 0) {
            perror("Could not open the SPI device");
            exit(EXIT_FAILURE);
        }

        uint32_t mode = SPI_MODE_0;
        if (ioctl(fs, SPI_IOC_WR_MODE, &mode) < 0) {
            perror("Could not set SPI mode");
            close(fs);
            exit(EXIT_FAILURE);
        }

        uint32_t speed = spi_speed;
        if (ioctl(fs, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
            perror("Could not set SPI max speed");
            close(fs);
            exit(EXIT_FAILURE);
        }


        uint8_t bits = 8;
        if (ioctl(fs, SPI_IOC_WR_BITS_PER_WORD, &bits) < 0) {
            perror("Could not set SPI bits per word");
            close(fs);
            exit(EXIT_FAILURE);
        }

    }//end init


    void SPI::settings_spi() {
        spi.len = LARGE_SECTOR_SIZE;
        spi.tx_buf = (unsigned long)tx_buffer;
        spi.rx_buf = (unsigned long)rx_buffer;
        spi.bits_per_word = 8;
        spi.speed_hz = get_spi_speed();
        spi.delay_usecs = 0;        
        memset(tx_buffer, 0xff, sizeof(tx_buffer));
        memset(rx_buffer, 0xff, sizeof(rx_buffer));
    }//end settings_spi

void SPI::init_sst25vf() {
    cmd_byte_spi(EWSR);
    int status= cmd_byte_spi_duo(WRSR);

}


    void SPI::spi_close() {
        if (fs >= 0) {
            close(fs);
        }
    }//end spi_close


   uint32_t SPI::get_spi_speed(){
        return spi_speed;
    }//end get_spi_speed


void SPI::writeEnable() {
    cmd_byte_spi(WREN);
}


    void SPI::writeDisable() {
        cmd_byte_spi(WRDI); //Write-Disable
    }

    void SPI::cmd_byte_spi(const uint8_t cmd) {
        std::memset(&spi, 0, sizeof(spi)); // Inicializar estructura
        std::memset(rx_buffer, 0xff, LARGE_SECTOR_SIZE);
        std::memset(tx_buffer, 0xff, LARGE_SECTOR_SIZE);

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


    uint8_t SPI::cmd_byte_spi_duo(const uint8_t cmd){        
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


    uint8_t SPI::read_status(){
        uint8_t stat= cmd_byte_spi_duo(RDSR);

        statusREGISTER statReg;
        statReg.byte = stat;
        std::cout<< " BP0 : "<<statReg.BP0 << " BP1 : "<< statReg.BP1 <<std::endl;
        return stat;
    }

    void SPI::erase_sst25_all(){
        writeEnable();
        init_sst25vf();
        cmd_byte_spi(CHIP_ERASE_ALL);
        writeDisable();
    }


    bool SPI::is_open(){

    return true;

    }// end is_open

    uint8_t SPI::get_fs(){

    return 0x00;
    }// end get_fs


    void SPI::read(const uint32_t address, uint8_t* buffer_received,const uint32_t length) {
        read_write(CMD_READ_DATA ,address,buffer_received,length);
    }// end read_data
/*
    void SPI::write(const uint32_t address, uint8_t* data,const uint32_t length) {
        //read_write(CMD_WRITE_DATA ,address,data,length);
            //writeEnable(); // Habilitar escritura
        	uint8_t cmd = CMD_WRITE; // Comando WRITE
        	spi.len = length + 4; // Comando + Dirección (3 bytes) + Datos (length bytes)
        	tx_buffer[0] = cmd;
        	tx_buffer[1] = (address >> 16) & 0xFF; // Dirección alta
        	tx_buffer[2] = (address >> 8) & 0xFF;  // Dirección media
        	tx_buffer[3] = address & 0xFF;         // Dirección baja
    	    spi.speed_hz = get_spi_speed();
            spi.tx_buf = (unsigned long)tx_buffer;
            spi.rx_buf = (unsigned long)rx_buffer;
            spi.bits_per_word = 8;
            spi.cs_change = 0;
        	std::memcpy(tx_buffer + 4, data, length ); // Copiar datos a tx_buffer
    	    std::memset(rx_buffer,0x00,length + 4);
            if ((ioctl(fs, SPI_IOC_MESSAGE(1), &spi)) < 0) {
            std::cerr << "write -> Error al escribir en la memoria: " << strerror(errno) << std::endl;
        }
    }// end read_data
*/


void SPI::read_write(const uint8_t cmd , const uint32_t address, uint8_t* buffer,const uint32_t length) {
    std::memset(rx_buffer, 0xff, LARGE_SECTOR_SIZE);
    std::memset(tx_buffer, 0xff, LARGE_SECTOR_SIZE);

    uint8_t cmd_buffer_tx_tmp[4]={0};
    uint8_t cmd_buffer_rx_tmp[4]={0};

    // Llenar el buffer de transmisión con el comando de lectura y la dirección
    cmd_buffer_tx_tmp[0] = cmd;
    cmd_buffer_tx_tmp[1] = (address >> 16) & 0xFF; // Dirección alta
    cmd_buffer_tx_tmp[2] = (address >> 8) & 0xFF;  // Dirección media
    cmd_buffer_tx_tmp[3] = address & 0xFF;         // Dirección baja

    struct spi_ioc_transfer spi_transfer[2] = {};

    // Transferencia para enviar el comando de lectura y la dirección
    spi_transfer[0].tx_buf = (unsigned long)cmd_buffer_tx_tmp;
    spi_transfer[0].rx_buf = (unsigned long)cmd_buffer_rx_tmp;  // No necesitamos recibir datos en este paso
    spi_transfer[0].len = 4;
    spi_transfer[0].speed_hz = get_spi_speed();
    spi_transfer[0].bits_per_word = 8;
    spi_transfer[0].cs_change = 0; // Mantener la selección de chip activa después de esta transferencia
    uint8_t tx_buffer_tmp[length];
    uint8_t rx_buffer_tmp[length];
    // Transferencia para enviar/recibir los datos
    spi_transfer[1].tx_buf = (unsigned long)tx_buffer_tmp;   
    spi_transfer[1].rx_buf = (unsigned long)rx_buffer_tmp;    
    spi_transfer[1].len = length;
    spi_transfer[1].speed_hz = get_spi_speed();
    spi_transfer[1].bits_per_word = 8;
    spi_transfer[1].cs_change = 1; // Desactivar la selección de chip después de esta transferencia
    if(cmd==CMD_WRITE_DATA) {
        std::memcpy(tx_buffer_tmp,buffer,length);
    }

    // Ejecutar ambas transferencias SPI
    if (ioctl(fs, SPI_IOC_MESSAGE(2), spi_transfer) < 0) {
        perror("Error al realizar la transferencia SPI");
        std::cerr << "Comando: " << std::hex << (int)cmd << ", Dirección: " << std::hex << address << std::endl;
        exit(EXIT_FAILURE);
    }
    if(cmd==CMD_READ_DATA) std::memcpy(buffer,rx_buffer_tmp,length);
    
    return;
}


void SPI::write(uint32_t address, uint8_t* data, size_t length) {
        writeEnable(); // Habilitar escritura

    	//uint8_t cmd = CMD_WRITE; // Comando WRITE
    	tx_buffer[0] = CMD_WRITE;
    	tx_buffer[1] = (address >> 16) & 0xFF; // Dirección alta
    	tx_buffer[2] = (address >> 8) & 0xFF;  // Dirección media
    	tx_buffer[3] = address & 0xFF;         // Dirección baja
        spi.len = length + 4; // Comando + Dirección (3 bytes) + Datos (length bytes)
	    spi.speed_hz = 100000;
        spi.tx_buf = (unsigned long)tx_buffer;
        spi.rx_buf = (unsigned long)rx_buffer;
        //spi.speed_hz = get_spi_speed();
        spi.bits_per_word = 8;
        spi.cs_change = 0;
    	std::memcpy(tx_buffer + 4, data, length); // Copiar datos a tx_buffer
	    std::memset(rx_buffer,0x00,length + 4);
    if ((ioctl(fs, SPI_IOC_MESSAGE(1), &spi)) < 0) {
        std::cerr << "write -> Error al escribir en la memoria: " << strerror(errno) << std::endl;
    }
}


}//end namespace spi
