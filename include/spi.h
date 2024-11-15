#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>


#define     SPI_DEVICE "/dev/spidev0.1"
    
#define     SPEED               100000
    
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

namespace SPACE_SPI
{

union statusREGISTER {
    struct {
        unsigned int BUSY : 1;
        unsigned int WEL  : 1;
        unsigned int BP0  : 1;
        unsigned int BP1  : 1;
        unsigned int RES  : 2;
        unsigned int AAI  : 1;
        unsigned int BPL  : 1;
    };
    uint8_t byte;  // Permite el acceso directo al byte completo
};


    class SPI {
    public:
        SPI                                 ();
        ~SPI                                ();
       
        void        spi_close               ();
        void        write                   (const uint32_t, uint8_t*, size_t);
        void        read                    (const uint32_t , uint8_t*,const uint32_t);
        uint32_t    get_spi_speed           ();
        uint8_t     get_fs                  ();
        uint8_t     cmd_byte_spi_duo        (const uint8_t);
        bool        is_open                 ();
        void        init_sst25vf            ();
        void        cmd_byte_spi            (const uint8_t);
        void        writeEnable             () ;
        void        writeDisable            ();
        void        read_write              (const uint8_t, const uint32_t, uint8_t* ,const uint32_t);
        void        erase_sst25_all         ();
        uint8_t     read_status             ();
    private:
        void        init                    ();
        void        settings_spi            ();
        uint8_t         tx_buffer           [LARGE_SECTOR_SIZE];
        uint8_t         rx_buffer           [LARGE_SECTOR_SIZE];
        uint32_t        spi_speed           ;
        int             fs                  ;
        struct          spi_ioc_transfer    spi;
        //struct          spi_ioc_transfer    spi_transfer[2] = {};
    };

}
