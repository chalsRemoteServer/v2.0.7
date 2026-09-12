/*
 * RTOS.h
 *
 *  Created on: Nov 26, 2025
 *      Author: desarrollo_1
 */

#ifndef ASD_APP_INCLUDE_RTOS_H_
#define ASD_APP_INCLUDE_RTOS_H_

#include <stdint.h>

//prioridades de las tareas----------------------------
#define PR_TFT_EVE tskIDLE_PRIORITY+1 //tarea que constrola y enciende el TFT EVE

//RTOS STACK SIZE TASK
#define ST_TFT_EVE ( ( unsigned short ) 256 )  //tarea de control de TFT EVE


void vDisplayTFT_EVE_Task1(void *pvParam);
unsigned char findComando(unsigned char c,unsigned char *cmd);
unsigned char findLen(unsigned char c,unsigned char cmd);
unsigned char comandoDDS(unsigned char *param);
unsigned char comandoBarras(unsigned char *param);
unsigned char comandoAlive(void);
void Task7_Serial_Receptor_Processor(void);
void vtaskDebugA_print_Rx_uart1(void);
void vTask9_Monitor_de_Errores(void);
unsigned char calculaCRC8(const unsigned char *data, unsigned char len);
unsigned char crc(uint8_t cmd,uint8_t len,uint8_t *p,uint8_t crc);

#endif /* ASD_APP_INCLUDE_RTOS_H_ */
