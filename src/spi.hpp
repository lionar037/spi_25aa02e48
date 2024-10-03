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
#include <sys/stat.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#define SPI_DEVICE          "/dev/spidev0.0"

#define CMD_WRITE 0x2
#define CMD_READ 0x3

#define READ 0b011 //Read data from memory array beginning at selected address 
#define WRITE 0b010 //Write data to memory array beginning at selected address 
#define WRDI 0b100//Reset the write enable latch (disable write operations) 
#define WREN 0b110//Set the write enable latch (enable write operations) 
#define RDSR 0b101//Read STATUS 
#define WRSR 0b001//Write STATUS register

class SPI{
public:
SPI();
~SPI();
void read_write();
void init();
void read_write(size_t,uint32_t);
void settings_spi();
void spi_close();
uint8_t read(uint16_t);
void write(uint16_t cmd);
private:

uint8_t tx_buffer[255];
uint8_t rx_buffer[255];
uint32_t len_data = 32;
uint32_t spi_speed = 1000000;
int fs;
int ret;
struct spi_ioc_transfer spi;
uint8_t looper;
uint32_t scratch32;
};
