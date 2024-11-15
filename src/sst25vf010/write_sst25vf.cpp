#include <spi/spi.hpp>
#include <st25vf010/st25vf010.hpp>
#include <iostream>

namespace ST25VF010{

	//St25vf010_t::St25vf010_t{
	//	init();
	//}

	void St25vf010_t::init(){
		spi.erase_sst25_all();
		spi.init_sst25vf();
		spi.read_status();
	}

	void St25vf010_t::write_rand() {	
	init();
		uint8_t buffer[4]={0xa1,0xbb,0xcc,0xdd};
		for(uint32_t address=0x000000;address < ADDRESS_END;){
				spi.write(address, buffer ,1);
				buffer[0]=rand()%256;
				address += 1;
			}
			spi.writeDisable();		
		}
	
	void St25vf010_t::write() {	
		uint8_t buffer[4]={0xa1,0xbb,0xcc,0xdd};
	init();
		for(uint32_t address=0x000000;address < ADDRESS_END;){
				spi.write(address, buffer ,1);
				//buffer[0]=rand()%256;
				buffer[0]=0xaa;
				address += 1;
			}

			spi.writeDisable();
			//return 0;
		}
	
	void St25vf010_t::write(const uint8_t data) {	
		uint8_t buffer[4]={0xa1,0xbb,0xcc,0xdd};
		init();
		for(uint32_t address=0x000000;address < ADDRESS_END;){
				spi.write(address, buffer ,1);				
				buffer[0]=data;
				address += 1;
			}
			spi.writeDisable();
		}

}