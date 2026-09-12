/*
 * ComSer.h
 *
 *  Created on: May 7, 2026
 *      Author: desarrollo_1
 */

#ifndef ASD_APP_INCLUDE_COMSER_H_
#define ASD_APP_INCLUDE_COMSER_H_


#define CMD_DDS 0xF1 //comando de transmision datos de deteccion raw X&Y
#define CMD_DDS_ANS 0x1F //CONtestacion
#define CMD_IM_ALIVE 0xF2 //comando de transmision datos de
#define CMD_IM_ALIVE_ANS 0x2F//CONTESTACION
#define CMD_BARR 0xF4
#define CMD_BARR_ANS 0x4F //contestacion del comando barras
#define SIZE_TRAMA_GRAPH_ANS 2 //tamño de trama de 4F y 1F

#define ID_vTask_isREC   0x07 //LEER EL recurso actual quien lo tiene
#define ID_vTask_OCUPADO 0x08
#define ID_vTask_LIBERAR 0x09//liberar reecurso
#define ID_vTask_2  0x0A //tarea send x&y
#define ID_vTask_3  0x0B//tarea im alive

#define ERR_SEVERO_DAQ 0x30 //sin adquisicion de datos
#define ERR_SEVERO_AN_CLK 0x31 //SIN reloj ANALOGO
#define ERR_SEVERO_AD9833 0x32 //SIN programacion en AD9833 no se detecta bandera de Transmision
#define ERR_SEVERO_AD9833_CLEAR 0x33 //limpiar este error

#define SIZE_TRAMA 4 //comando IM alive



void vTask8_ImAlive_Transm_al_CPU_TFT(void);
void enableIntSerial(void);
void vTask11_Tranmision_Serial_ala_Procesadora(void);
void disable_Int_Serial(void);



#endif /* ASD_APP_INCLUDE_COMSER_H_ */
