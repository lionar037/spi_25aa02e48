#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <tyme/tyme.hpp>
#include <iostream>

namespace ST25VF010{

	St25vf010_t::St25vf010_t()
	: spi { std::make_unique<SPI::Spi_t>() }
	{
		
	}

	void 
	St25vf010_t::init(){

	}

	void 
	St25vf010_t::write_rand() {	
		std::vector <uint8_t>vect_data(1,0xae);
		init();		
		for( uint32_t address=0x00 ; address < SPIConstants::ADDRESS_END ; ) {				
				spi->write(address, vect_data);
				vect_data[0]=rand()%256;
				address += vect_data.size();				
			}
		spi->writeDisable();		
	}//end St25vf010_t::write_rand()

	void 
	St25vf010_t::write(const uint8_t data) {	
		std::vector <uint8_t>vect_data(1,data);
		init();		
		for( uint32_t address=0x00 ; address < SPIConstants::ADDRESS_END ; ) 
			{				
				spi->write(address, vect_data);
				address += vect_data.size();				
			}
		
		spi->writeDisable();
	}
	void 
	St25vf010_t::write() {	
		write_aai();
	}
	void 
	St25vf010_t::write_aai() {	
		std::vector <uint8_t>vect_data(1,0xAE);
		init();
		uint32_t address=0x00;
		for( ; address < SPIConstants::ADDRESS_END ; ) {				
				spi->write(address, vect_data);
				address += vect_data.size();				
			}			
		spi->writeDisable();
		[[maybe_unused]] auto status = spi->cmd_byte_spi_duo(SPIConstants::RDSR);//Read Status Register (RDSR)				
		std::cout << "status : "<< std::to_string(status)<<"\n";
	}
	
}