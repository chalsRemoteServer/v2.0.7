/*
 * ComSer.c
 *
 *  Created on: May 7, 2026
 *      Author: desarrollo_1
 */
#include "ComSer.h"
#include "queue1.h"
#include "ft900.h"
#include "errCon.h"
#include "ui_bargraph.h"
#include "TFT.h"


extern volatile struct _DISPLAY_VFD_ comm;
uint8_t estado7;//estado de task7
static void Task6_Transmisor_Serial_a_Procesadora(void);
static void vTask10_Monitor_de_Comandos_graficacion_Deteccion(void);


void enableIntSerial(void){
    uart_enable_interrupt(UART1, uart_interrupt_rx);/* Enable the UART to fire interrupts when receiving data... */
	uart_enable_interrupts_globally(UART1);/* Enable interrupts to be fired... */
	interrupt_enable_globally();
    comm.bits.b.IRQ_Serial_G=TRUE;
}//enable interrupt serial-------------------

void disable_Int_Serial(void){
    uart_disable_interrupt(UART1, uart_interrupt_rx);
	comm.bits.b.IRQ_Serial_G=FALSE;
	estado7=0;//reset para que se inicie la int con delay
}//Disable  interrupcion del serial-------------------------


/*Driver principal de transmision ala procesadora  * por serial* */
static void Task6_Transmisor_Serial_a_Procesadora(void){
static unsigned char estado;
static uint8_t dato;
unsigned char *x;
	x=&dato;
    switch(estado){
    	case 1:if(comm.TxProc.ncount>0){estado++;}break;
    	case 2:if(comm.TxProc.dequeue(x,&comm.TxProc)){estado++;}break;
    	case 3:if(uart_get_tx_fifo_level(UART1)<16)estado++;break;//hay espacio en la fifo de hardware?
    	case 4:if(uart_write(UART1,*x)>=0){estado++;}break;
       	default:estado=1;break;}//fin de estado
}//fin de transmision del display tft-----------------------------------

void vTask4_Process_RX_VFD(void) {    // Reservar memoria para las estructuras FIFO
static uint8_t estado,estado2;
static uint8_t c;
//static uint8_t buffer_patron[4];

	switch(estado){
		case 1:estado2=1;estado++;break;
		case 2:if(fifo_pop(fifo.VFDserial, &c))estado++;break;
		case 3:switch(estado2){
			    case 1:if(c==STX)estado2++;else{estado2=1;}break;
			    case 2:if(c==CMD_IM_ALIVE_ANS)estado2++;else{estado2=1;}break;
			    case 3:if(c==0xFF)estado2++;else{estado2=1;}break;
			    case 4:if(c==ETX){//Monitor_System_status_LEDs(CMD_IM_ALIVE_ANS);
			                      }
			           else{estado2=1;}
			    	   break;
			    default:estado2=1;break;}
		       estado=2;break;
		default:estado=1;break;}//fin switch---------------------
}//task4---------------------------------------------------

//Transmisor serial a MCU-IO-TFT
void vTask8_ImAlive_Transm_al_CPU_TFT(void){//STX,CMD,LEN,ETX Manda comando, que dice que estamos conectados
static unsigned char estado72;
const unsigned char cmd=CMD_IM_ALIVE_ANS;
const unsigned char len=0xFF;
static unsigned char  datos[SIZE_TRAMA],i;
static unsigned int count;
//unsigned char n;

    switch(estado72){
	    	case 1:if(++count>8000)estado72++;break;
	    	case 2:if(solicitarRecurso(ID_vTask_3))estado72++;break;
	    	case 3:datos[0]=STX;datos[1]=cmd;datos[2]=len;estado72++;break;
	    	case 4:datos[3]=ETX;i=0;estado72++;break;
	    	case 5:if(comm.TxProc.append(datos[i],&comm.TxProc))estado72++;break;
	    	case 6:if(++i<SIZE_TRAMA)estado72--;
	    	       else{estado72++;}break;
			case 7://Comando_Alive_Respuesta();
			       estado72++;break;
			case 8:if(liberarRecurso(ID_vTask_3))estado72++;break;
			default:estado72=1;count=0;break;}//fin switch------------
}//fin de transmision del display tft---------------------------------



/*Transmisor serial, Monitorea si hay alguna
 bandera del comando barras o DDS   1F y 4F  */
#if defined(CMD_BARR_ANS) && (CMD_BARR_ANS == 0x4F) && defined(CMD_DDS_ANS) && (CMD_DDS_ANS == 0x1F)
static void vTask10_Monitor_de_Comandos_graficacion_Deteccion(void){//
static unsigned char estado10;
static unsigned char cmd;//=CMD_IM_ALIVE_ANS;
const unsigned char len=0xFF;
static unsigned char  datos[SIZE_TRAMA_GRAPH_ANS],i;
static unsigned int count10;

    switch(estado10){
	    	case 1:if(++count10>800){estado10++;}break;//monitorear cada cierto tiempo  en lugar de cada 2 milisegundos
	    	case 2:cmd=ui_monitor_graficadores();
	    	       if(!cmd)estado10=1;
	    	       else{estado10++;}break;
	    	case 3:switch(cmd){
	    			  case 0x1F:
	    	          case 0x4F:datos[0]=1;break;
	    	          case 0x10:
	    	          case 0x40:datos[0]=0xFF;cmd=cmd|0x0F;break;
	    	          default:estado10=1;break;}
	    	       estado10++;break;
	    	case 4:if(solicitarRecurso(ID_vTask_10))estado10++;break;
	    	case 5:datos[0]=STX;datos[1]=cmd;datos[2]=len;estado10++;break;
	    	case 6:datos[3]=ETX;i=0;estado10++;break;
	    	case 7:if(comm.TxProc.append(datos[i],&comm.TxProc))estado10++;break;
	    	case 8:if(++i<SIZE_TRAMA)estado10--;
	    	       else{estado10++;}break;
			case 9://Comando_Alive_Respuesta();
			       estado10++;break;
			case 10:if(liberarRecurso(ID_vTask_10))estado10++;break;
			default:estado10=1;count10=0;break;}//fin switch------------
}//fin de transmision del display tft---------------------------------
#endif

void vTask11_Tranmision_Serial_ala_Procesadora(void){
    Task6_Transmisor_Serial_a_Procesadora();//Transmisor fisico de las FIFOS
    vTask8_ImAlive_Transm_al_CPU_TFT();//2.0.4.3
    vTask10_Monitor_de_Comandos_graficacion_Deteccion();

}//fin transmision serial ala Procesadora----------------------


