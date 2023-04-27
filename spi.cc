#include "spi.hpp"
//#define DBG
#define PRINT_BUFFER

void SPI::init(){
fs = open(SPI_DEVICE, O_RDWR);
    if(fs < 0) {
        printf("Could not open the SPI device...\r\n");
        exit(EXIT_FAILURE);
    }

    ret = ioctl(fs, SPI_IOC_RD_MODE, &scratch32);
    if(ret != 0) {
        printf("Could not read SPI mode...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }
    scratch32 |= SPI_MODE_0;
    ret = ioctl(fs, SPI_IOC_WR_MODE, &scratch32);//SPI_IOC_WR_MODE32
    if(ret != 0) {
        printf("Could not write SPI mode...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }
#ifdef DBG
	printf("before this->spi_speed %d\n",scratch32);
#endif
	ret = ioctl(fs, SPI_IOC_RD_MAX_SPEED_HZ, &scratch32);
#ifdef DBG
	printf("after this->spi_speed %d\n",scratch32);
#endif
    if(ret != 0) {
        printf("Could not read the SPI max speed...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }

    scratch32 = 1000000;

    ret = ioctl(fs, SPI_IOC_WR_MAX_SPEED_HZ, &scratch32);
#ifdef DBG
	printf("before this->spi_speed %d\n",scratch32);
printf("value SPI_IOC_WR_MAX_SPEED_HZ :  %d\n",SPI_IOC_WR_MAX_SPEED_HZ);
#endif
    if(ret != 0) {
        printf("Could not write the SPI max speed...\r\n");
        close(fs);
        exit(EXIT_FAILURE);
    }
#ifdef DBG
	printf("this->spi_speed %d\n",scratch32);
#endif
}


void SPI::read_write(){

    for(looper=0; looper<4; ++looper) {
        tx_buffer[looper] = CMD_READ;//looper;
        rx_buffer[looper] = 0x00;
    }
#ifdef DBG
	std::cout << " spi.tx_buf  : "<< spi.tx_buf<<"\n";
	std::cout << " spi.rx_buf  : "<< spi.rx_buf<<"\n";
	std::cout << " spi.len  : "<< spi.len<<"\n";
	std::cout << " spi.delay_usecs  : "<< spi.delay_usecs<<"\n";
	std::cout << " spi.speed_hz  : "<< spi.speed_hz<<"\n";
	printf("spi.bits_per_word :%d \n",spi.bits_per_word);
	printf("spi.cs_change :%d \n",spi.cs_change);
	std::cout << " spi.bits_per_word  : "<< std::hex<<spi.bits_per_word<<"\n";
	std::cout << " spi.cs_change : "<< std::hex<<spi.cs_change<<"\n";
#endif
ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
#ifdef DBG
	printf("fs : status : %d\n" , fs);
#endif
    if(ret != 0) {
#ifdef DBG
	printf("SPI transfer returned %d...\r\n", ret);
#endif
   }
#ifdef DBG
	printf("Received SPI buffer...\r\n");
#endif
for(looper=0; looper<32; ++looper) {
#ifdef DBG
  printf(" %02x",rx_buffer[looper]);
#endif
printf("\n");
 }
#ifdef DBG
    printf("\r\n");
#endif
    close(fs);
    exit(EXIT_SUCCESS);
}

void SPI::read_write(size_t size ,uint32_t cmd){
#ifdef DBG
	std::cout<<"size_t : "<< size <<"\n";
#endif
spi.len = size;
	tx_buffer[0]= cmd & 0xFF;
 	tx_buffer[1]=(cmd>>8)&0xff;
  	tx_buffer[2]=(cmd>>16)&0xff;
    for(looper=0; looper<size+1; ++looper) {
        rx_buffer[looper] = 0x0;
    }

ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
#ifdef DBG
	printf("fs : status : %d\n" , fs);
#endif
    if(ret != 0) {
#ifdef DBG
  printf("SPI transfer returned %d...\r\n", ret);
#endif
 }
#ifdef PRINT_BUFFER
#ifdef DBG
    printf("Received SPI buffer...\r\n");
#endif
 for(looper=0; looper<4; ++looper) {
if(looper==2)
        printf(" 0x%02x",rx_buffer[looper]);
    }
#endif
 printf("\n");
}

void SPI::spi_close(){
    close(fs);
    exit(EXIT_SUCCESS);
}

void SPI::settings_spi(){
spi.tx_buf = (unsigned long)tx_buffer;
spi.rx_buf = (unsigned long)rx_buffer;
spi.bits_per_word = 0;
spi.speed_hz = spi_speed;
spi.delay_usecs = 0;
spi.len = 3;//despues se modifica
   for(looper=0; looper<spi.len+1; ++looper) {
        tx_buffer[looper] = 0x00;
        rx_buffer[looper] = 0xFF;
    }
}

SPI::SPI(){
#ifdef DBG
std::cout<<"constructor spi\n";
#endif
	init();
	settings_spi();
}

SPI::~SPI(){
#ifdef DBG
std::cout<<"destructor spi\n";
#endif
	spi_close();
if(fs)
	close(fs);
}



uint8_t SPI::read(uint16_t address){
spi.len = 3;
tx_buffer[0]= CMD_READ;
tx_buffer[1]=address&0xff;
tx_buffer[2]=(address>>8)&0xff;
for(looper=0; looper<spi.len +1; ++looper) {
	rx_buffer[looper] = 0x0;
    }
ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
if(ret != 0){return 0x00 ;}
#ifdef DBG
 for(looper=0; looper<4; ++looper) {
	if(looper==2) printf(" 0x%02x",rx_buffer[looper]);
    }
 printf("\n");
#endif
return rx_buffer[2];
}


void SPI::write(uint16_t cmd){
spi.len = 3;
tx_buffer[0]= CMD_WRITE;
tx_buffer[1]=cmd&0xff;
tx_buffer[2]=(cmd>>8)&0xff;
for(looper=0; looper<spi.len+1; ++looper) {
	rx_buffer[looper] = 0x0;
    }
ret = ioctl(fs, SPI_IOC_MESSAGE(1), &spi);
if(ret != 0){
	return;
	}
#ifdef DBG
 for(looper=0; looper<4; ++looper) {
	if(looper==2) printf(" 0x%02x",rx_buffer[looper]);
    }
 printf("\n");
#endif
return;
}
