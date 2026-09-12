/*
 * RTOS.c
 *
 *  Created on: Nov 26, 2025
 *      Author: desarrollo_1

 aqui se deben PONER TODAS LAS TAREAS QUE SE CREAN SOLAMENTE
 */
//#include "FreeRTOS.h"  // DEBE ir primero
//#include "task.h"
#include "RTOS.h"
#include "queue1.h"
#include <stdio.h>
#include "system.h"
#include "ComSer.h"
#include "errCon.h"
#include "platform_time.h"
#include "TFT.h"
#include "ui_footer.h"
#include "ui_bargraph.h"
#include <stdarg.h>  // si no está incluido



extern volatile struct _DISPLAY_VFD_ comm;
extern unsigned char debug2;//eval var, fifo rx full
extern status_bar_s StatusBar;
extern UiFooter g_footer;
extern UiBarGraph g_bar;
extern uint8_t estado7;

static void print2(const char *format, ...) {
#ifdef DEBUG_SYSTEM
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
#endif
}

void vDisplayTFT_EVE_Task1(void *pvParam){

  for(;;){
	printf(COLOR_YELLOW"%s"COLOR_RESET, "Estoy en tarea EVE!\n");
     }

}//fin display TFT EVE TASK1-------------------------------

//debug A ver rx uart1 ver si se lleno la fifo Rx
void vtaskDebugA_print_Rx_uart1(void){
static unsigned long int countA;
    if(countA++>8000){countA=0;
      if(debug2){
    	print2("\nFIFO RX UART1 full"); }}
}//fin debug A--------------------------------------------




void Task7_Serial_Receptor_Processor(void){
static unsigned char c,q,delay=0;
enum{SIZE_MEMO=2,S_PARAM=11,RESET_CMD=300};
static unsigned char param[S_PARAM],i;
static unsigned char cmd,len,len0;
static uint32_t t0;
const uint8_t LEN[]={0,1,1,1,0,1,0,0,1,1};
                 //  0 1 2 3 4 5 6 7 8 9   <--Len (lenght) del comando
    switch(estado7){
    case 1:if(delay++>0xFE){enableIntSerial();estado7++;q=0;delay=0;}break;
L71:case 2:if(comm.RxProc.dequeue(&c,&comm.RxProc)){
    	      switch(q){
              case 0:if(c==STX){q++;}break;
              case 1:if((c>0xF0)&&(c<0xFA)){
            	  	  	  if((c==0xF2)||(c==0xF9)){cmd=c;q=5;}
            	  	  	  else{cmd=c;q++;}}
                     else{q=0;}break;
              case 2:if(c>S_PARAM){q=0;}
              	  	 else{if(LEN[c]){q=3;len0=len=c;i=0;}else{q=0;}}
              	  	 break;
              case 3:if(len==2){param[i]=c;q=4;}
                     else{len--;param[i++]=c;}break;//
              case 4:if(crc(cmd,len0,&param[0],c)){
            	       switch(cmd){
            	       case 0xF1:comandoDDS(&param[0]);break;
            	       case 0xF4:comandoBarras(&param[0]); break;
            	       case 0xF5:/* En CONSTRUCCUION*/ break;
            	       case 0xF6:/* En CONSTRUCCUION*/ break;
            	       case 0xF7:/* En CONSTRUCCUION*/ break;
            	       case 0xF8:/* En CONSTRUCCUION*/ break;
            	       case 0xF9:/* En CONSTRUCCUION*/ break;
            	       default:break;}}
              	  	 q=0;break;
              case 5:if(c==0xFF){
                      switch(cmd){
                          case 0xF2:comandoAlive();break;
                          case 0xF9:/*en cosntruccion  */ break;
                          default:q=0;break;}}
                      q=0;break;
			  default:q=0;break;}
             goto L71;}//que se vacie la FIFO
		     if(!comm.bits.b.IRQ_Serial_G)
		    	 estado7=1;
             break;
	  default:estado7=1;break;}
    uint32_t now=millis();
    if((now-t0)>RESET_CMD){//reseteae variables de control
          comm.bits.b.CMD_Barras_Recepcion=0;
          comm.bits.b.Req_Barras=0;//se rrequiere barras
          comm.bits.b.CMD_dds_Recibido=0;//tenemos comando DDS recepcionado
          comm.bits.b.Req_DDS=0;//se requiere DDS
    	  t0=now;}


}//fin de serial receptcion del procesador-------------------------------------


/* esta funcion encuantra el comando cmd que se ha dado de alta
 **/
unsigned char findComando(unsigned char c,unsigned char *cmd){
	switch(c){
	case CMD_DDS:
	case CMD_IM_ALIVE:*cmd=c;return TRUE;break;
	default:break;}
return FALSE;
}//find comando-----------------------------------

/* encuentra la longirtud que corresponde a este comando
 *     DDS   STX,F1,04,*/
unsigned char findLen(unsigned char c,unsigned char cmd){
	switch(cmd){
	case CMD_DDS:if(c==5){return TRUE;}break;
	case CMD_IM_ALIVE:if((c==0)||(c==0xFF)){return TRUE;}break;
	default:break;}//fin switch
return FALSE;
}//fin find lenght----------------------------------


//esta funcion se usa para cuando el comando incluye el crc en el conteo
//junto con los parametros e.g.  cmd,5,p0,p1,p2,p3,crc,ETX
unsigned char crc(uint8_t cmd,uint8_t len,uint8_t *p,uint8_t crc){
enum{SIZECRC=9};
uint8_t arr[SIZECRC];
	   if(SIZECRC>len){
		   arr[0]=cmd;arr[1]=len;
		   for(uint8_t i=0;i<(len-1);i++){
			   arr[i+2]=*(p+i);}
		   if(calculaCRC8(&arr[0],6)==crc)
		       return TRUE;}
return FALSE;
}//fin crc --------------------------

//duracion 6 useg para 9 bytes
unsigned char calculaCRC8(const unsigned char *data, unsigned char len) {
unsigned char crc = 0x00;                                       // valor inicial del CRC
unsigned char i, j;                                             // contadores de bytes y bits

for (i = 0; i < len; i++) { crc ^= data[i];                    // XOR con el byte actual
        for (j = 0; j < 8; j++) {                               // procesar cada bit
            if (crc & 0x80) { crc = (crc << 1) ^ 0x07; }        // bit MSB=1, desplazar y XOR
            else { crc <<= 1; }}}                                 // bit MSB=0, solo desplazar
return crc;                                                     // devolver CRC calculado
} // fin calculaCRC8 ------------------------------------------ posicion 66

/*aux es la memoria del proceso
 *  03,F1,05,XH,XL,YH,YL,CRC,02
 *   0, 1, 2, 3, 4, 5, 6,  7, 8   */
unsigned char comandoDDS(unsigned char *param){
unsigned short int x,y;
	x=(unsigned short int)((param[0]<<8)|param[1]);
	y=(unsigned short int)((param[2]<<8)|param[3]);
	StatusBar.Alive=TRUE;
	comm.bits.b.CMD_dds_Recibido=1;//comando DDS recibido
	g_bar.rawX=x;g_bar.rawY=y;
	print2(" %u,%u",x,y);
return TRUE;
}//fin comando DDS----------------------------------------


/*aux es la memoria del proceso
 *  03,F4,03,XH,XL,CRC,02
 *   0, 1, 2, 3, 4,  5, 6   */
unsigned char comandoBarras(unsigned char *param){
unsigned short int x;
    comm.bits.b.CMD_Barras_Recepcion=1;//tenemos RX
	x=(unsigned short int)((param[0]<<8)|param[1]);
	StatusBar.Alive=TRUE;
	g_bar.rawX=x;
	print2("barra: %u",x);
return TRUE;
}//fin comando DDS-------------------------------------------



//return TRUE cuando ya se insertaron todas las tramas en la FIFO tx
/*manda el comando de contestacion alive */
unsigned char comandoAlive(void){
//unsigned char estado88,i,ret=0;
		StatusBar.Alive=TRUE;
return StatusBar.Alive;
}//fin comando alive-------------------

void vTask9_Monitor_de_Errores(void){
enum { NORMAL_MS=3000, WARN_MS=6000, ERR_MS=11000,
      DURACION_ESTADO_MS=8000}; // 8 segundos minimo en pantalla
enum {NORMAL=0, WARN=1, ERR=2};
static uint8_t first_run=1, estado_actual=NORMAL;
static uint32_t t0=0, t0_cambio_estado=0;
static uint32_t now;
static uint32_t elapsed;
static uint8_t estado9=0;
static uint8_t nuevo_estado;
   switch(estado9){
   case 1:if (first_run){
    	    t0=millis(); first_run=0; // init
            StatusBar.Alive=0; StatusBar.code=0; estado_actual=ERR;
            t0_cambio_estado=t0;}estado9++;
          break;
   case 2:now=millis()+2;estado9++;break;//plus 2 miliseg del loop
   case 3:if (StatusBar.Alive) { // Alive=1 -> poner en 0 y guardar timestamp
               StatusBar.Alive=0; t0=now+2; }estado9++;break;
   case 4:elapsed=now-t0; // tiempo sin alive
          elapsed &= ~0x800000;  // LIMPIAR BIT 23 DE DESBORDAMIENTO
          estado9++;break;
   case 5:if (elapsed < WARN_MS) { nuevo_estado=NORMAL; } // <3s -> OK
    	  else{ if (elapsed < ERR_MS) { nuevo_estado=WARN; } // 3-4s -> WARN
                else { nuevo_estado=ERR; }} // >4s -> ERROR (cambie a 4s)
         estado9++;break;
   case 6:if(nuevo_estado != estado_actual) { // cambio de estado
            if ((now - t0_cambio_estado) >= DURACION_ESTADO_MS){ // ya paso 8s?
					estado_actual=nuevo_estado; t0_cambio_estado=now;
					switch(estado_actual){
					case NORMAL: // OK
						g_footer.state=FOOTER_ESTADO_FUNCIONAMIENTO;
						StatusBar.code=0; break;
					case WARN: // WARNING
						g_footer.state=FOOTER_ESTADO_ADVERTENCIAS;
						StatusBar.code=STATUS_WARN_COMM; break;
					case ERR: // ERROR
						g_footer.state=FOOTER_ESTADO_CONEXION_CU;
						StatusBar.code=STATUS_ERR_COMMS; break;}}}
    	   else{if(t0_cambio_estado==0){t0_cambio_estado=now;}}
   	   	   estado9=2;break;
   default:estado9=1;break;}
}//fin de vTask9_Monitor_de_Errores ----------------------------------------
