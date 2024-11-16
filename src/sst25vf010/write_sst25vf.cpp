#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <iostream>

namespace ST25VF010{

	St25vf010_t::St25vf010_t()
	: spi { std::make_unique<SPI::Spi_t>() }
	{
		
	}

	void St25vf010_t::init(){
		spi->erase_sst25_all();
		spi->init_sst25vf();
		spi->read_status();
	}

	void St25vf010_t::write_rand() {	
	init();
		uint8_t buffer[4]={0xa1,0xbb,0xcc,0xdd};
		for(uint32_t address=0x000000;address < ADDRESS_END;){
				spi->write(address, buffer ,1);
				buffer[0]=rand()%256;
				address += 1;
			}
			spi->writeDisable();		
		}
	

	void St25vf010_t::write(const uint8_t data) {	
		uint8_t buffer[4]={0xa1,0xbb,0xcc,0xdd};
		init();
		for(uint32_t address=0x000000;address < ADDRESS_END;){
				spi->write(address, buffer ,1);				
				buffer[0]=data;
				address += 1;
			}
			spi->writeDisable();
		}

	void St25vf010_t::write() {	
		std::vector <uint8_t>vect_data(1,0xAE);
		init();
		
		for(uint32_t address=0x00 ; address < ADDRESS_END ; ) {
				spi->write(address, vect_data);
				address += vect_data.size();
			}

			spi->writeDisable();			
		}

		void St25vf010_t::write_aai() {	
		std::vector <uint8_t>vect_data(256,0xAE);
		init();

		uint32_t address=0x00;
		spi->write(address, vect_data);
		address++;
		for( ; address < ADDRESS_END ; ) {
				for(const auto& it:vect_data)spi->write_aai(address, it);
				address += vect_data.size();
			}

		//spi->cmd_byte_spi_duo(WRDI);// Write Disable (WRDI) , Instruction to terminate , 
		cmd_byte_spi(WRDI);//1 byte de comando // Write Disable (WRDI) , Instruction to terminate , 
		[[maybe_unused]] auto status = spi->cmd_byte_spi_duo(RDSR);//Read Status Register (RDSR)				

		spi->writeDisable();			

		}
	
}