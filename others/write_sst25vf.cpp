#include "spi.h"
#include <iostream>
//#include <iomanip>


int main() {
SPACE_SPI::SPI spi("/dev/spidev0.1",100000);
//spi.writeEnable();

uint8_t buffer[]={0xDC,0xA0,0xA1,0xA2};
//uint32_t address=0x00;

for(uint32_t address=0x0000 ; address < 0x1ffff ; ++address){

//spi.write(address, buffer ,4);
//	buffer[3]=rand()%256;
 //	buffer[2]=rand()%256;
// 	buffer[1]=rand()%256;
// 	buffer[0]=rand()%256;
	spi.write(address, buffer ,1);
	//address += 1;
	}
    return 0;
}
