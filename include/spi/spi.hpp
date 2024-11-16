//codigo spi.hpp
#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

//#define DBG_SPI

#define     SPI_DEVICE "/dev/spidev0.1"
    
#define     SPEED               10000
    
#define     STATUS_BUSY         0x1 
#define     WRSR                0x1
    
#define     CMD_WRITE_DATA      0x2
#define     WRITE               0x2
#define     CMD_WRITE           0x2
    
#define     CMD_READ            0x3
#define     CMD_READ_DATA       0x3 
#define     READ                0x3
    
#define     WRDI                0x4 //Write-Disable
    
#define     CMD_READ_STATUS     0x5
#define     RDSR                0x5 // Read Status Register
#define     CMD_WRITE_ENABLE    0x6
#define     WREN                0x6  // Comando para habilitar escritura
    
#define     PAGE_SIZE           256 
#define     EWSR                0x50
#define     CMD_CE              0x60 //chip erase
#define     CHIP_ERASE_ALL      CMD_CE //chip erase all
    
#define     SECTOR_SIZE_4       4
#define     SECTOR_SIZE_8       8
#define     LARGE_SECTOR_SIZE   256 // Tamaño de un sector grande (puede ser modificado)
#define     AAI_CMD             0xAF //Auto Address Increment 
#define     ADDRESS_END         0x1FFFF

namespace SPI{

union statusREGISTER {
    struct statusBits{
        unsigned int BUSY : 1;
        unsigned int WEL  : 1;
        unsigned int BP0  : 1;
        unsigned int BP1  : 1;
        unsigned int RES  : 2;
        unsigned int AAI  : 1;
        unsigned int BPL  : 1;
    }status;
    uint8_t byte;  // Permite el acceso directo al byte completo
};


    class Spi_t {
    public:
        Spi_t                                 ();
        ~Spi_t                                ();
       void                 init_sst25vf            ();
        void                write                   (const uint32_t , uint8_t*, const size_t);
        const bool          read                    (const uint32_t , uint8_t*,const uint32_t);        
        const bool          read                    (const uint32_t , std::vector<uint8_t>& buffer);
        void                erase_sst25_all         ();
        bool                is_open                 ();
        void                spi_close               ();
        const uint32_t      get_spi_speed     ();
        const uint8_t       get_fs                  ();
        const uint8_t       read_status             ();
        const uint8_t       cmd_byte_spi_duo        (const uint8_t);
        void                writeDisable            ();

    protected:                                                                
        void                cmd_byte_spi            (const uint8_t);
        void                writeEnable             () ;        
        const bool          read_write              (const uint8_t, const uint32_t, uint8_t* ,const uint32_t);
        const bool          read_write(const uint8_t cmd, const uint32_t address, std::vector<uint8_t>& vect) ;
        const bool          handle_spi_transfer(const struct spi_ioc_transfer* transfer, size_t length) ;        
    private:
        void                init                    ();
        void                settings_spi            ();
        uint8_t             tx_buffer           [LARGE_SECTOR_SIZE];
        uint8_t             rx_buffer           [LARGE_SECTOR_SIZE];
        const uint32_t      spi_speed           ;
        int                 fs                  ;
        struct              spi_ioc_transfer    spi;        
    };

}
