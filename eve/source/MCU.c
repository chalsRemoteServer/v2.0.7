/* MCU.c
 *
 *  Created on: Nov 24, 2025
 *      Author: desarrollo_1
 */

#include <stdint.h>
//#include "FT8xx.h"
#include <ft900.h>
#include "../include/MCU.h"
#include "../include/EVE_arduino.h"
#include <stddef.h>

// salida de las pines--------------------------
#define GPIO_SPIM_CLK    27 //--SPI MASTER CLK  GPIO27
#define GPIO_SPIM_MISO   30 //--SPI MASTER MISO GPIO30
#define GPIO_SPIM_MOSI   29 //--SPI MASTER MOSI GPIO29
#define GPIO_EVE_PD      58 //PD del EVE, GPIO58,{MM900EV[pin12]}
#define SPI_CHANNEL (3)
#if SPI_CHANNEL == 3
#define SPI_SS_GPIO (35)
#define SPI_SS_FUNC (pad_gpio35)
#endif

void MCU_Init(void){
    sys_enable(sys_device_spi_master);//init SPI para FT812
    gpio_function(GPIO_SPIM_CLK, pad_spim_sck);
    gpio_function(SPI_SS_GPIO, SPI_SS_FUNC);
    gpio_dir(SPI_SS_GPIO, pad_dir_output);
    gpio_function(GPIO_SPIM_MOSI, pad_spim_mosi);
    gpio_function(GPIO_SPIM_MISO, pad_spim_miso);
    spi_init(SPIM, spi_dir_master, spi_mode_0, 128); // 0.78 MHz
    spi_option(SPIM, spi_option_fifo, 1);            // Enable 16-byte FIFO
    gpio_dir(GPIO_EVE_PD, pad_dir_output);
}//fin MCU_Init-----------------------------------------

/**
 @brief MCU specific setup
 @details Called after the EVE has been power cycled and started. Contains
 	 any MCU-specific configuration options for the EVE. */
void MCU_Setup(void){
#if defined (EVE_HAS_CRYSTAL)
    EVE_cmdWrite(EVE_CLKEXT, 0U); /* setup EVE for external clock */
#else
    EVE_cmdWrite(EVE_CLKINT, 0U); /* setup EVE for internal clock */
#endif


}//--fin MCU SETUP---------------------------------------------------------------



void MCU_CSlow(void){ gpio_write(SPI_SS_GPIO, 0); }
void MCU_CShigh(void){ gpio_write(SPI_SS_GPIO, 1); }
void MCU_PDlow(void){ gpio_write(GPIO_EVE_PD,0); }
void MCU_PDhigh(void){ gpio_write(GPIO_EVE_PD,1); }
void EVE_pdn_Reset(void){MCU_PDlow();}
void EVE_pdn_Normal(void){MCU_PDhigh();}

void EVE_cs_set(void){MCU_CSlow();}
void EVE_cs_clear(void){MCU_CShigh();}

void MCU_SPIWrite(const uint8_t *DataToWrite, uint32_t length){
    if (length == 0) return;
    spi_writen(SPIM, (uint8_t *)DataToWrite, length);
}//---------------------------------------------------------------


uint8_t MCU_SPIRead8(void){
    uint8_t rx;
    uint8_t tx = 0x00;  // ← variable obligatoria
    spi_xchange(SPIM, &rx, tx);
return rx;
}//---------------------------------------------------------------

uint8_t spi_receive(uint8_t var){
uint8_t rx;
    spi_xchange(SPIM, &rx, var);
return rx;
}//-------------------------------------


void spi_transmit(uint8_t var){
	MCU_SPIWrite8(var);
}//-----------------------------------------

uint16_t MCU_SPIRead16(void){
uint8_t tx[2] = {0x00, 0x00};
uint8_t rx[2];
    spi_xchangen(SPIM, rx, tx, 2);
return ((uint16_t)rx[0] << 8) | rx[1];
}//-*---------------------------------------

void MCU_SPIwriten(const uint8_t *data,uint8_t size){
	spi_writen(SPIM,data,size);
}//------------------------------------------------------


void MCU_SPIWrite16(uint16_t DataToWrite){//revisado
    uint8_t buf[2];
    buf[0] = (DataToWrite >> 8) & 0xFF;  // MSB
    buf[1] = DataToWrite & 0xFF;         // LSB
    spi_writen(SPIM, buf, 2);  //send  MSB LSB
}//-*---------------------------------------




uint16_t MCU_SPImemRead16(uint32_t const ft_address){
uint8_t cmd[3],low, high,dummy = 0x00;

	cmd[0] = (uint8_t)((ft_address >> 16U) & 0x7F);   // bit 7 = 0 para lectura
    cmd[1] = (uint8_t)(ft_address >> 8U);
    cmd[2] = (uint8_t)(ft_address & 0xFF);
    EVE_cs_set();
    spi_writen(SPIM,cmd,3);
    spi_xchange(SPIM,&low,dummy);
    spi_xchange(SPIM,&high,dummy);
    EVE_cs_clear();
return((uint16_t)high<<8)|low;
}//------------------------------------------------------------------------------



uint32_t MCU_SPImemRead32(uint32_t ft_address){//revisada
uint32_t data=0;
uint8_t addrbuff[3],b0,b1,b2,b3,dummy=0x00;
   addrbuff[2] = (uint8_t)((ft_address >> 16U) & 0x7F);   // READ = bit7=0
   addrbuff[1] = (uint8_t)(ft_address >> 8U);
   addrbuff[0] = (uint8_t)(ft_address & 0xFF);
   EVE_cs_set();
   spi_writen(SPIM,addrbuff,3);
   spi_xchange(SPIM, &b0, dummy);   // Low byte
   spi_xchange(SPIM, &b1, dummy);
   spi_xchange(SPIM, &b2, dummy);
   spi_xchange(SPIM, &b3, dummy);   // High byte
   EVE_cs_clear();
   data  =  (uint32_t)b0;// Reconstrucción little-endian (correcta)
   data |= ((uint32_t)b1 << 8);
   data |= ((uint32_t)b2 << 16);
   data |= ((uint32_t)b3 << 24);
return data;
}//-----------------------------------------------------------------------

void MCU_SPImemWrite_flash_buffer(uint8_t  *address,size_t len,const uint8_t * const p_data){
     spi_writen(SPIM, address, len);
     for(uint32_t i = 0; i < len; i++){
             uint8_t dummy = p_data[i];  // byte a enviar
             uint8_t trash;              // recepción no usada
             spi_xchange(SPIM, &trash, dummy);}
}//--------------------------------------------------------------------------

uint8_t fetch_flash_byte(const uint8_t *p_data){
/*#if defined (RAMPZ)
    return (pgm_read_byte_far(p_data));
#else
    return (pgm_read_byte_near(p_data));
#endif*/
    //esto cambiarlo por una memoria externa EEPROM spi o I2C
    //usando sistema operativo
return 0;
}//---------------------------------------------------------------------



//funcion revisada
void MCU_SPI_eve_begin_cmd(const uint32_t command){
uint8_t buff[3];
uint8_t trash;
uint8_t tx_byte;  // Variable temporal para enviar

  	buff[2]=0xB0;//0x30 | 0x80; //dir: REG_CMDB_write 0x302578+MEM_WRITE(bit=1)
  	buff[1]=0x25;
  	buff[0]=0x78;
    //EVE_cs_set();
    spi_writen(SPIM,buff,3);//send first: 78h,25h,B0h =LSB,--,MSB
    tx_byte = (uint8_t)(command & 0xFF);
	spi_xchange(SPIM, &trash, tx_byte);
	tx_byte = (uint8_t)((command >> 8) & 0xFF);
	spi_xchange(SPIM, &trash, tx_byte);
	tx_byte = (uint8_t)((command >> 16) & 0xFF);
	spi_xchange(SPIM, &trash, tx_byte);
	tx_byte = (uint8_t)((command >> 24) & 0xFF);
	spi_xchange(SPIM, &trash, tx_byte);
}//fin---------------------------------------------------------------------------------

/* Escribe un bloque de datos al FT812 y agrega el padding necesario (múltiplos de 4) */
void MCU_private_block_write(const uint8_t * const p_data, const uint16_t len){
uint8_t padding;
uint8_t zero = 0x00;

	    if(p_data == NULL) return;/* Calcular padding */
	    padding = len & 3;      // residuo modulo 4 (0,1,2,3)
	    padding = 4 - padding;  // cuanto falta para llegar a 4
	    padding &= 3;           // si era múltiplo de 4 → padding = 0
	    EVE_cs_set();/* Enviar los datos reales */
	    spi_writen(SPIM, (uint8_t*)p_data, len);
	    while (padding--){/* Enviar relleno (0x00) para completar múltiplos de 4 */
	        spi_xchange(SPIM, NULL, zero);}
	    EVE_cs_clear();
}//---------------------------------------------------------------------------------------

uint32_t MCU_SPIRead24(void){//corregida
uint8_t tx[3] = {0, 0, 0};
uint8_t rx[3];
    spi_xchangen(SPIM, rx, tx, 3);
return  ((uint32_t)rx[2] << 16) |
		((uint32_t)rx[1] << 8)  |
		(uint32_t)rx[0];
}//-*---------------------------------------------------------------

void MCU_SPIWrite8(uint8_t DataToWrite){
uint8_t dummy;
    spi_xchange(SPIM, &dummy, DataToWrite);
    //spi_xchangen(SPIM,&dummy,DataToWrite,1);
}//-*----------------------------------------------------------

uint32_t MCU_SPIRead32(void){
uint8_t tx[4] = {0,0,0,0};
uint8_t rx[4];

		spi_xchangen(SPIM, rx, tx, 4);
return ((uint32_t)rx[0] << 24) |
	   ((uint32_t)rx[1] << 16) |
	   ((uint32_t)rx[2] << 8 ) |
		(uint32_t)rx[3];
}//---------------------------------------------------------------


void MCU_SPIWrite24(uint32_t DataToWrite){//corregida
uint8_t dummy;
uint8_t a,b,c;
	a=(uint8_t)(DataToWrite & 0x000000ffUL);
	b= (uint8_t)(DataToWrite >> 8U);
	c=(uint8_t)(DataToWrite >> 16U);
	spi_xchange(SPIM, &dummy,a);//Byte 0:
	spi_xchange(SPIM, &dummy,b);//byte 1:
	spi_xchange(SPIM, &dummy,c);//byte 2
}//-*-----------------------------------------------------------------------


void spi_transmit_burst(uint32_t var){MCU_SPIWrite32(var);}//----------------
void spi_transmit_32(uint32_t var){MCU_SPIWrite32(var);}
void MCU_SPIWrite32(uint32_t DataToWrite){
uint8_t dummy;
uint8_t a,b,c,d;
	a=(uint8_t)(DataToWrite & 0x000000ffUL);
	b= (uint8_t)(DataToWrite >> 8U);
	c=(uint8_t)(DataToWrite >> 16U);
	d=(uint8_t)(DataToWrite >> 24U);
 spi_xchange(SPIM, &dummy, a);//Byte 0:
 spi_xchange(SPIM, &dummy, b);//byte 1:
 spi_xchange(SPIM, &dummy, c);//byte 2
 spi_xchange(SPIM, &dummy, d);//byte 3
}//---------------------------------------------------------------


void MCU_Delay_20ms(void){ delayms(20); }
void MCU_Delay_500ms(void){ delayms(500); }
uint16_t MCU_htobe16(uint16_t h){ return (h >> 8) | (h << 8); }
uint32_t MCU_htobe32(uint32_t h){
return  ((h >> 24) & 0xFF)       |
		((h >> 8)  & 0xFF00)     |
		((h & 0xFF00) << 8)      |
		((h & 0xFF)   << 24);
}//---------------------------------------------------------------------------

uint16_t MCU_htole16(uint16_t h){ return h; }
uint32_t MCU_htole32(uint32_t h){ return h; }
uint16_t MCU_be16toh(uint16_t h){ return MCU_htobe16(h); }
uint32_t MCU_be32toh(uint32_t h){ return MCU_htobe32(h); }
uint16_t MCU_le16toh(uint16_t h){ return h; }
uint32_t MCU_le32toh(uint32_t h){ return h; }
