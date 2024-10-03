#include <iostream>
#include <vector>
#include <cstdint>
#include <string>
#include "spi.hpp"

#define READ 0b011 //Read data from memory array beginning at selected address
#define WRITE 0b010 //Write data to memory array beginning at selected address
#define WRDI 0b100//Reset the write enable latch (disable write operations)
#define WREN 0b110//Set the write enable latch (enable write operations)
#define RDSR 0b101//Read STATUS register
#define WRSR 0b001//Write STATUS register

#define CMD READ

int main(void) {
SPI *spi = new (SPI);
uint8_t diference = 0x6;
uint16_t first_address = 0xff-diference;
uint32_t address = 0x00 | READ | (first_address<<8);
//first_address= first_address >>8;
//std::cout <<"first adress : "<< std::hex<< first_address<<std::endl;
//std::cout <<" adress : "<< std::hex<< address<<std::endl;
for (int i=0;i<diference;++i){
std::cout <<" adress : "<< std::hex<< address<<std::endl;
	address = address + 0x100;
	spi->read_write(3,address);
	}
std::cout<<".........................................."<<std::endl;
/*
address=0x000000 | READ;
for (int i=0;i<255-7;++i)
        {
        spi->read_write(3,address);
	address = address + 0x100;        
	}
std::cout<<".........................................."<<std::endl;


address = 0xFF0000 | WRITE;
for (int i=0;i<255-7;++i){
        spi->read_write(3,address);
	address = address + 0x100;
        }

std::cout<<".........................................."<<std::endl;

address = 0x000000 | READ;
for (int i=0;i<255 ;++i) {
	//address = address + 0x100;
        spi->read_write(3,address);
	address = address + 0x100;  
      }

std::cout<<".........................................."<<std::endl;
*/
/*
spi->read_write(3,0xffff00|CMD);
spi->read_write(3,0xb7fe00|CMD);
spi->read_write(3,0x84fd00|CMD);
spi->read_write(3,0x3efc00|CMD);
spi->read_write(3,0x28fb00|CMD);
spi->read_write(3,0x34fa00|CMD);
spi->read_write(3,0x80f900|CMD);
*/

/*
spi->read_write(3,0xff00|CMD);
spi->read_write(3,0xfe00|CMD);
spi->read_write(3,0xfd00|CMD);
spi->read_write(3,0xfc00|CMD);
spi->read_write(3,0xfb00|CMD);
spi->read_write(3,0xfa00|CMD);
spi->read_write(3,0xf900|CMD);
*/
return 0;
}
