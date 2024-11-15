#include <spi/spi.hpp>

namespace ST25VF010{

	int St25VF010_t::write() {
	SPI::Spi_t spi;
	//spi.writeEnable();

	uint8_t buffer[4]={0xa1,0xbb,0xcc,0xdd};
	uint32_t address=0x00;
	//spi.init_sst25vf();
	spi.erase_sst25_all();
	spi.init_sst25vf();

	spi.read_status();

	for(uint32_t address=0x000000;address < ADDRESS_END;){

	spi.write(address, buffer ,1);

		buffer[0]=rand()%256;
		address += 1;
		}

		spi.writeDisable();
		return 0;
	}

}