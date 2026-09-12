/*
 * queue.h
 *
 *  Created on: Apr. 22, 2026
 *      Author: desarrollo_1
 */
#include "system.h"
#include <stdint.h>

#ifndef ASD_APP_INCLUDE_QUEUE_H_
#define ASD_APP_INCLUDE_QUEUE_H_

/*
 * queue.h
 *
 *  Created on: Oct 3, 2017
 *      Author: chals
 */




#ifndef SIZE_FIFO_OFFSET //tamaño del array del filtro digital del offset
#define SIZE_FIFO_OFFSET 32
#endif

#ifndef bool
typedef unsigned char bool;
#endif

#if(SIZE_FIFO_OFFSET==32)
#define  DIV_OFFSET 5 //DIVISOR PARA offset  2^5=32
#else
#error "No coincide el tamaño matematico del filtro con el divisor"
#endif



#define SIZE_BUF_TFT 5
#define SIZE_IO_TX_BUFFER 85 //ARRAY donde se guarda la direccion de la cabeza del paquete a enviar
#define SIZE_B 60//ARRAY donde se guarda los paquetes que se van a enviar

#define SIZE_FIFO_BOX 10
#define SIZE_FIFO SIZE_FIFO_BOX//30  //array size where the ADC's values are saved

#define SIZE_BARR_NUM 22//size fifo to displat numeros and position where menus has barra detection
#define SIZE_FIFO_CMD_DDS 30 //tamaño de la FIFO para transmitir comando DDS al touch

#define SIZE_BUFFER_TX 90 //BUFER DE TRANSMISION SERIAL ALA O
#define SIZE_BUFFER_DDS_FIFO 50
#define SIZE_TEMP_PIXEL  6//tamaño del buffer para guardar memoria de pixels del display DD
#define SIZE_DDS_PIXEL   40//para pintar el pixel en tiempo real
#define SIZE_DDS_ZOOM   2000
#define SIZE_X  24//array zoom repaint char pantalla 0
#define SIZE_Y  129//array zoom repaint char pantalla0

#define SIZE_SER_KEYPAD 10 //TAMAÑO de la fifo del serial del keypad de entraada
#define SIZE_KEYPAD_IN 10 //buffer de entrada de chars de serial de keypad
#define SIZE_BUFFER1  10U //buffers de rx y tx de serial a IOUP
#define SIZE_BUFFER2  5U//tamaño de rx y tx de i2c fifo memorias
#define SIZE_BUFFER5  20U//kEYPAD FLIP FLOP save keysrokes tamaño de buffer char que entran keypad serial
#define SIZE_BUFFER6  10//1954U//FIFO de graficos box,char,rayas,puntos,pos,con SO
#define SIZE_BUFFER9  50 //fifo GUARDA LOS pixeles que estan fuera de la pantalla DDS porque se salieton del zoom,estan mas alla del valor desplegable actual al zoom actual
#define SIZE_BUFFER13 10 //FIFO para gestionar recurso VFD de los hilos
#define SIZE_BUFFER14 20//20=No modif streaming-block size transmision a la IOUP
#define SIZE_BUFFER15 20 //20 no modificar FIFO guarda deteccion, para transmitirla por serial IOUP
#define SIZE_BUFFER16 5  //buffer de recursos de vfd de gestion de
#define SIZE_BUFFER21 10//FIFO DE transmision serial a display tft
#define SIZE_BUFFER22 5//FIFO DE control de recursos de transmision

#define SIZE_BUFFER50 30//FIFO de recepcion RX serial desde uProcesadora
#define SIZE_BUFFER51 15//fifo de Transmision TX serial hacia la Procesadora

#define SIZE_BOXES    20
#define SIZE_SYS_MONITOR 30
#define SIZE_TBC 15
#define SIZE_CHAR 5
#define SIZE_FEW  3

#if(SIZE_BUFFER6>0xFFF1)
#error 'tamaño del buffer excede de 0xFFF1';
#endif



#define CMD_DDS 0xF1 //comando de transmision datos de deteccion raw X&Y
#define CMD_IM_ALIVE 0xF2 //comando de transmision datos de
#define CMD_IM_ALIVE_ANS 0x2F//CONTESTACION


#define ID_vTask_isREC   0x07 //LEER EL recurso actual quien lo tiene
#define ID_vTask_OCUPADO 0x08
#define ID_vTask_LIBERAR 0x09//liberar reecurso
#define ID_vTask_2  0x0A //tarea send x&y
#define ID_vTask_3  0x0B//tarea im alive
#define ID_vTask_10 0x0C//tarea de monitor de DDS|Barras comandos de envio serial

#define ERR_SEVERO_DAQ 0x30 //sin adquisicion de datos
#define ERR_SEVERO_AN_CLK 0x31 //SIN reloj ANALOGO
#define ERR_SEVERO_AD9833 0x32 //SIN programacion en AD9833 no se detecta bandera de Transmision
#define ERR_SEVERO_AD9833_CLEAR 0x33 //limpiar este error






struct NodoBox{
	struct NodoBox *next;
//	struct NodoBox *prev;
	unsigned char box;
};





struct _SysMon{
   unsigned short int t[SIZE_SYS_MONITOR];
   unsigned char i;//index
   unsigned char size; //=SIZE_SYS_MONITOR;
   void(*reset)(void);//funcion de reseNVITACIONt de los regs
};


struct Serial_Bandera{
    union _Byte1_{
		unsigned char ByteF;
		struct{
			unsigned char FreeBuffTX:1;//bandera de que ya se envio el buffer completo
			unsigned char flipflopBuff:1;//seleccionar el buffer que llenamos en tiempo t
			unsigned char resetDDS:1;//indica si borramos registro de datos repetidos de DDS
			unsigned char DDS_ACTIVA:1;
			unsigned char x4:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
			unsigned char EnableComunicationsTX:1;
			unsigned char x6:1;
			unsigned char x7:1;//timercomms
		}bitF;
	  }Flags;
   struct _VAR_{
		   unsigned char ncountNopeAns;
		   unsigned char timercomms;
	  }var;
};



struct DDS_GEN{
   union _Byte91_{
	  unsigned char status;
	  struct{
		unsigned char overflow:1;//flag de over flow
		unsigned char IRQcapture:1;//INTERRUPCION
		unsigned char contrlVar:1;//para ver cual capture vamos a usar
		unsigned char display_Cntrl_mens:1;//control de despliegue de mensaje de error
		unsigned char x3:1;
		unsigned char x2:1;
		unsigned char x1:1;
		unsigned char x0:1;
	  }Bits;
    }bits;
  struct _VAR145_ {
    unsigned long int capture;//conteo de pulsos en 10mseg
    unsigned char estado;//estado de control de frecq
    unsigned short int delay;//delay de este algoritmo
    unsigned short int ndelay;//variable a comparar para el delay
    unsigned short int t3A; //valor de timestamp timer 3
    unsigned short int t3B;//valor de timerstamp timer 3
    unsigned short int countEvent;//cuenta el numero de veces que hay errores de frecuencia
    unsigned short int countRelojAnalogo;//monitor del reloj analogo muerto,
    unsigned short int countTimeMensajeError; //cuenta el tiempo que paso desde la ultima alerta de error si ya no aparece la alerta se quita el mensaje de errror
  }var;
};



/* version 110422-1119 */
struct IO_Bandera{
    union _Byte2_{
		unsigned char Outs2;
		struct{
			unsigned char Rechazo:1;//bandera de que ya se envio el buffer completo
			unsigned char Warnning:1;//seleccionar el buffer que llenamos en tiempo t
			unsigned char Fail:1;//indica si borramos registro de datos repetidos de DDS
			unsigned char Lock:1;
			unsigned char Aux_out:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
			unsigned char PFVE:1;
			unsigned char Gotis:1;
			unsigned char x7:1;//timercomms
		}bitA;
	  }Salidas;
	union _Byte3_{
	  		unsigned char IN2;
	  		struct{
	  			unsigned char PACK_CHK:1;//bandera de que ya se envio el buffer completo
	  			unsigned char GATING:1;//seleccionar el buffer que llenamos en tiempo t
	  			unsigned char AUX1:1;//indica si borramos registro de datos repetidos de DDS
	  			unsigned char AUX2:1;
	  			unsigned char SYS_CHK:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
	  			unsigned char LockSW:1;
	  			unsigned char ResetSW:1;
	  			unsigned char x7:1;//timercomms
	  		}bitB;
	  	  }Ent;
   unsigned char TACHO;
};






/*version 310322-1641 add reset genaral */
struct _FIFO_1byte_{//FIFO PARA UNA VARIABLE para un byte
    unsigned char *pop;//primer de la FIFO para salir
    unsigned char *head;//indice 0 del buffer
    unsigned char *tail;//ultimo indice del buffer
    unsigned char *push;//pointer buffer 2
	unsigned char (*popf)(unsigned char *n,struct _FIFO_1byte_ *s);//quitar el prmer elemento de la fifo DDS
	unsigned char (*appendByte)(unsigned char  n,struct _FIFO_1byte_ *s);//agregar byte al buffer de transmision
    void (*vaciarbuff)(unsigned char *p1,unsigned char *p2,unsigned short int size);
    void (*resetFIFO)(struct _FIFO_1byte_ *s,unsigned char *arr,unsigned short int size);
    unsigned short int ncount;//cuenta los nodos llenos en la fifo
    unsigned short int size; //size de la fifo
    unsigned char recurso;//recurso ocupado  o libre

};

// Nueva estructura para appendByte con un solo parámetro
struct _FIFO_1byte_v2{
    unsigned char *pop; //apunta al siguiente a sacar
    unsigned char *head; //elemento 0
    unsigned char *tail;  //ultimo elemento
    unsigned char *push; //apunta al siguiente location libre
    unsigned char mode; //modo debug=1 o modo normal=0 {en debug el pop siempre es  elemento=0}
	unsigned char (*dequeue)(unsigned char *n,volatile struct _FIFO_1byte_v2 *s);
	unsigned char (*append)(unsigned char n,struct _FIFO_1byte_v2 *s);  // SOLO UN PARÁMETRO
    unsigned char (*peek)(struct _FIFO_1byte_v2 *s);
    unsigned char (*search)(unsigned char n,struct _FIFO_1byte_v2 *s);
	void (*vaciarbuff)(unsigned char *p1,unsigned char *p2,unsigned short int size);
    void (*resetFIFO)(struct _FIFO_1byte_v2 *s,unsigned char size);
    unsigned char ncount;
    unsigned char size;
};


/* version 110422-1119 */
struct  _SERIAL_{
	struct _FIFO_1byte_ rx;
	struct _FIFO_1byte_ tx;
	struct Serial_Bandera vars;
	struct IO_Bandera IO;
};//serial-------------------------------------------------------------



struct _FIFO_func_{
	  unsigned char (*append)(unsigned char x,unsigned char y, unsigned char p);
	  unsigned char (*pop)(unsigned char *x,unsigned char *y, unsigned char *p);
	  unsigned char (*resetFIFOS)(void);//resetear todas las FIFOs Y arrays y registros
};//fin _FIFO_func_----------------------------------------

struct _FIFO_func2_{
	  unsigned char (*append)(signed short int x,signed short int  y);
	  unsigned char (*pop)(signed short int *x,signed short int  *y);
	  unsigned char (*resetFIFOS)(void);//resetear todas las FIFOs Y arrays y registros
};//fin _FIFO_func_----------------------------------------


struct _FIFO_func3_{
	  unsigned char (*append)(unsigned char x);
	  unsigned char (*pop)(unsigned char *x);
	  unsigned char (*resetFIFOS)(void);//resetear todas las FIFOs Y arrays y registros
};//fin _FIFO_func_----------------------------------------


struct _Recursos{
	 //struct _FIFO_func_  f;//funciones de la fifo de VFDprocesos
	 struct _FIFO_1byte_ VFDprocesos;//procesos que quieren el recurso
	 struct _FIFO_1byte_ VFD;//fifo de VFD
	 struct _FIFO_1byte_ IO;//fifo de IO
	 struct _FIFO_1byte_v2 COMM_IO;//commnicacion con IO
	 unsigned char (*insertVFDProcess)(unsigned char proceso);
	 unsigned char (*solicitar)(unsigned char recurso,unsigned char PID);
	 	 	  void (*devolver)(unsigned char,unsigned char);
   	 unsigned char (*get_inst)(void);//obtener instancia
   	          void (*dev_inst)(unsigned char inst);//devolver instancia
	 struct{
		unsigned short int control;//VFD numero de registros,que guarda quien esta en la FIFO
		unsigned short int control_IO;//IO numero de procesos/hilos a manejar
		unsigned char VFD_pid;//pid que ocupa el VFD
		unsigned char control_uchar;
		unsigned char char1[8];
		unsigned char asignado;//a quien es asigando el recurso de VFD
		unsigned char asignadoIO;//a quien es asiganado el recurso IO
	 }v;//variable

};//fin recursos ------------------------------------------


struct COMUNICACIONES_{//-----------------------------------------------
	struct IOUP_{
	unsigned char estado;//estado actual de maquina de estado de comando
   unsigned char buffer[SIZE_BUF_TFT];
//    unsigned char commandoActual;//comando procesando actualmente
//    unsigned char contextoActual; //contexto,Tab,Pantalla Actual controlando
//    void (*fTabActual)(unsigned char c);//funcion actual de la tabactual
   void (*fAck)(unsigned char c);//mandar acknowledge
   unsigned char aux1;
   struct _FIFO_1byte_ tx;
   unsigned char txBusy;//ocupado el Tx fifo

	    }IOUP;
	struct GOTIS_{
		unsigned char DDS_activado;//si entonces se manda stream de deteccion al DDS de Desktop
   	    struct DDS_{
   	    	struct _Func_{
  	    	  unsigned char (*fpush)(signed short int x,signed short int);
   	    	  unsigned char (*fpop)(signed short int *x,signed short int *y);
   	    	   }f;
   	    	struct _FIFOS_{
   	    	  struct _FIFO_1byte_ xl;
   	    	  struct _FIFO_1byte_ xh;
   	    	  struct _FIFO_1byte_ yl;
   	    	  struct _FIFO_1byte_ yh;}fifos;}dds;
	    }gotis;
};//-------------------------------------------------------------



struct _DISPLAY_VFD_{
	struct _FIFO_1byte_v2  RxProc;//fifo serial recepcion de procesadora
	struct _FIFO_1byte_v2  TxProc;//fifo de recursos de tx serial
    uint8_t status;//status serial de configuracion general.

	struct _FIFO_1byte_ x;//parametro 1
	struct _FIFO_1byte_ y;//parametro 2
	struct _FIFO_1byte_ p;//parametro 3
	struct _FIFO_func_  f1;//funciones para guardar lo que se grafica
	struct _FIFO_func3_  fifo;//funciones para guardar lo que se grafica

	struct _box_control{
		 unsigned char boxs[SIZE_BOXES];
		 unsigned char box0;
		 unsigned char box;
		 unsigned short int timer;//se activa  por timer y resetea el array
	   }box;
	union _Byte5_{
	   	  		unsigned char bytes1;
	   	  		struct{
	   	  			unsigned char FIFOonReset:1;//Las FIFOS estan reseteadas?? osea que esan en ceros y desbilitadas, esto para cambiar de contexto
	   	  			unsigned char DDSon:1;//indica si borramos registro de datos repetidos de DDS
	   	  			unsigned char TxBuffOFF:1;//buffer de TX vacio, para saber que ya se transmitio todo
	   	  			unsigned char Req_DDS:1;//se requiere el comando F1 de DDS
	   	  			unsigned char CMD_dds_Recibido:1;//tenemos el comnado F1 recii¿bido
	   	  			unsigned char IRQ_Serial_G:1;//IRQ Global serial
	   	  			unsigned char CMD_Barras_Recepcion:1;//Comando F4 recibido,
	   	  		    unsigned char Req_Barras:1;//se requiere comando Barras
	   	  		}b;
	   	  	  }bits;


};//fin display VFD----------------------------------------------




struct _CONTROL_IIC_{
    union _Byte11_{
		unsigned char ByteF;
		struct{
			unsigned char x0:1;//bandera de que ya se envio el buffer completo
			unsigned char x1:1;//seleccionar el buffer que llenamos en tiempo t
			unsigned char x2:1;//indica si borramos registro de datos repetidos de DDS
			unsigned char x3:1;
			unsigned char x4:1;//recargar DDS por cambio de de zoom u otra cosa, se recarga los puntos de deteccion guardados
			unsigned char x5:1;
			unsigned char x6:1;
			unsigned char x7:1;//timercomms
		}bitG;
	  }Flags;
   struct _VAR19_{
		   unsigned char y0;
		   unsigned char y1;
	  }var;
};


struct  _IIC_{
	struct _FIFO_1byte_ rx;
	struct _FIFO_1byte_ tx;
	struct  _CONTROL_IIC_ cntrl;
};//serial-------------------------------------------------------------

/* estructura de datos del manejo de los datos de entrada del
 * teclado-------------------------------------------------------------*/
struct _KEYPAD_ {
	struct _FIFO_1byte_ F;//fifo managment
 	struct __vars_{
		  unsigned char s;//estado dela maquina de estados de procesamiento;
		  unsigned char a0[SIZE_BUFFER5];
		  unsigned char a1[SIZE_BUFFER5];
 		  unsigned char ss;//estado dela maquina que controla que array se vacia ala fifo
 		  unsigned char delay;//cuando se activa  el teclado char desechados antes de empezar a capturar
 		  unsigned char i;//indice delos arrays que guardan los chars rapidos del keypad por int
 		  unsigned char c0;//keypad.v.c0 es el char anterior aceptado que se apreto
 		  unsigned char ncount;//es el numero de veces que vemos el char anterior en este actual ocacion
 		  unsigned char c;//guarda el ultimo char que representa la tecla presionada en el actual preocesamiento

 		  //unsigned short int timerkeypad;//running time after last rx byte
 		}v;//variables
 	struct{
		  unsigned char enable:1;//keyboard enable IRQ
		  unsigned char swap:1;//eleccion para escoger el arraya a llenar
		  unsigned char x2:1;//indica que uno de los dos array todavia no se vacia a la fifo y la interrupcion ya quiere cambiar de array, al que estamos usando, interrupcion mas rapida que la FIFO
		  unsigned char x3:1;
		  unsigned char x4:1;
		  unsigned char x5:1;
		  unsigned char x6:1;
		  unsigned char x7:1;
 		}b;//bits
};//fin --------------------------------------------------------------


/*para manejo de las fifos que guardan los pix fuera
 *  de rango para el zoom actual seleccionado en DDS menu*/
struct _PIX_{
	union{
	  signed short int usint;
	  struct _XH_{
		  unsigned char H;
		  unsigned char L;
	  }b;
	}u;

	struct _FIFO_1byte_ xL;//parametro 1
	struct _FIFO_1byte_ xH;//parametro 1
	struct _FIFO_1byte_ yL;//parametro 1
	struct _FIFO_1byte_ yH;//parametro 1
	struct _FIFO_func2_ f;
    unsigned char pix[24][128];//192/8=24, x=24 registros hor, 128 lineas  o rows
};



struct _Comando_DDS{//comando DDS para graficar en touch
	struct _PIX_ pix;
	union Bytex{
	   unsigned char Bnderas;
	   struct{
		   unsigned char DDS_Reload:1;//cuando se recarga el DDS por un zoom CON LOS valores anteriores guardadso de las detecciones
		   unsigned char Apagando:1;//le dice que nos vamos a salir del menu DDS, para que no mande datos del DDS en otro menu
	       unsigned char EventDDS:1;//nos dice que tenemos autorizacion para ejecutar graficacion en DDS
		   unsigned char DDS1_BORRAR:1;//Borrando de el DDS
		   unsigned char DDS1_TIMER:1;
		   unsigned char clean_Buffers:1;//para decirle cuando limpiar los buffers que guardan los datos de los puntos pintados en el DDS para repintrlos
		   unsigned char Aspect_Zoom:1;
		   unsigned char debug:1;
		   unsigned char Gain_AB:1;
	   }bit;
	}Bnderas;

	union Bytex2{
		   unsigned char Registro1;
		   struct{
			   unsigned char rePaintLines:1;//repintar lineas de ganacia
			   unsigned char isGraficable0:1;//la grafica anterior se grafico
		       unsigned char x2:1;//nos dice que tenemos autorizacion para ejecutar graficacion en DDS
			   unsigned char x3:1;//Borrando de el DDS
			   unsigned char x4:1;
			   unsigned char x5:1;//para decirle cuando limpiar los buffers que guardan los datos de los puntos pintados en el DDS para repintrlos
			   unsigned char x6:1;
			   unsigned char x7:1;
		   }b;//bit
		}b;



	struct _Display_{
		unsigned char flag_TX_Pixel; //timer para desplegar en DDS
		unsigned short int delay;//para manejo de delay por comando y timer de display VFD
	}display;

	struct _Cursores_Contrl{
		unsigned char status;
	}cursor;

	struct _TOUCHSCREEN_{
		signed short int bufferX[SIZE_BUFFER_DDS_FIFO];//buffer DDS fifo TRansmision Serial
		signed short int bufferY[SIZE_BUFFER_DDS_FIFO];//buffer y DDS FIFO Transmision Serial
		signed short int *headx; //head de buffer x
		signed short int *tailx; //head de buffer y
		signed short int *popx;  //pointer to pop from fifo
		signed short int *pushx; //
		signed short int *heady; //head de buffer x
		signed short int *taily; //head de buffer y
		signed short int *popy;  //
		signed short int *pushy;
	    unsigned char ncount;//numero de nodos ocupados en la fifo
	}touch;

	struct _SAVE_PIXELS_{
		signed short int xPixel[SIZE_TEMP_PIXEL];
		signed short int yPixel[SIZE_TEMP_PIXEL];
		signed short int *p[8];
#if(SIZE_TEMP_PIXEL<254)
		unsigned char ncount;
#else
	    unsigned short int ncount;
#endif
	}SaveTempPix;

//	unsigned char (*pop)(signed short int *x,signed short int *y);//pop function
//    unsigned char (*push)(signed short int x,signed short int y);
    unsigned char (*remove)(signed short int *x,signed short int *y);//quita el nodo primer en indicce 0
    unsigned char (*append)(signed short int x,signed short int y);//pone un nodo al final dela cola

    struct _DispayPixel_{//para pintar el pixel
    	unsigned char *pop,*popx,*popy;
    	unsigned char *push,*pushx,*pushy;
    	unsigned char *tail,*tailx,*taily;
    	unsigned char *head,*headx,*heady;
    	unsigned char buffX[SIZE_DDS_PIXEL];
    	unsigned char buffY[SIZE_DDS_PIXEL];
    	unsigned char pen[SIZE_DDS_PIXEL];
#if(SIZE_DDS_PIXEL<254)
    	unsigned char ncount;
#else
    	unsigned short int ncount;
#endif
    }pixel;

    struct _Repaint_ZOOM{
    	//signed short int xx[SIZE_DDS_ZOOM];
        //signed short int yy[SIZE_DDS_ZOOM];
    	unsigned char  xy[SIZE_Y][SIZE_X];
    	unsigned char xy0[SIZE_Y][SIZE_X];
    	unsigned short int ii;
    	unsigned char j,k,uy;
    	unsigned char ix,iy,b;
    	unsigned char x1[100],y1[100];//depurar
    	unsigned short int debug5;
    	unsigned short int debug6;
    }rePaint;
}; //FIN ESTRUCTURA COMANDO DDS--------------------------------------


// Estructura de una FIFO (cola circular)
typedef struct {
    uint8_t *buffer;        // Buffer de datos
    uint16_t head;          // Índice de escritura
    uint16_t tail;          // Índice de lectura
    uint16_t size;          // Tamaño del buffer
    uint16_t count;         // Cantidad de elementos en la cola
    bool overflow;          // Bandera de desbordamiento
} fifo_t;

typedef struct {
    fifo_t *VFDserial;      // FIFO para datos del puerto serial VFD
    // Puedes agregar más FIFOs aquí:
    // fifo_t *UART2;
    // fifo_t *SPI;
} fifo_manager_t;

// Variable global para acceder a las FIFOs
extern fifo_manager_t fifo;










/*  FIN DDS ESTRUCURA ********************************************++++*/
/*  FIN DDS ESTRUCURA ********************************************++++*/
/*  FIN DDS ESTRUCURA ********************************************++++*/





unsigned char FIFO_DDS_pop(unsigned char *dato);
void init_FIFO_DDS(void);
unsigned char isFIFO_DDS_empty(void);
unsigned char isFIFO_DDS_pixel_llena(void);
unsigned char isFIFO_DDS_pixel_empty(void);
unsigned char FIFO_DDS_pixel_pop(unsigned char *x,unsigned char *y,unsigned char *pen);
void display_DDS_transmitter_Controller(void);
void init_FIFO_DDS_Display_Pixel2(void);
void init_FIFO_DDS_Display_Pixel2_v2(void);
unsigned char isFIFO_DDS_Display_Pixel_llena2(void);
unsigned char isFIFO_DDS_Display_Pixel_empty2(void);
unsigned char FIFO_DDS_Display_Pixel_push(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char FIFO_DDS_Display_Pixel_pop2(unsigned char *x,unsigned char *y,unsigned char *p);
unsigned char isFIFO_DDS_Display_Pixel_empty2_v2(void);
unsigned char *FIFO_DDS_next_X(unsigned char *p);
unsigned char isFIFO_DDS_Display_Pixel_empty2(void);
void init_FIFO_DDS_pixel(void);
void FIFO_DDS_pixels_push(unsigned char x,unsigned char y,unsigned char pen);
unsigned char FIFO_DDS_Display_Pixel_push(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char isFIFO_DDS_Display_Pixel_llena2_v2(void);
unsigned char isFIFO_DDS_pixel_llena(void);
unsigned char isFIFO_DDS_Display_Pixel_empty_v2(void);
unsigned char isFIFO_DDS_pixel_empty(void);
unsigned char FIFO_DDS_Display_Pixel_pop2(unsigned char *x,unsigned char *y,unsigned char *p);
unsigned char FIFO_DDS_pixel_pop(unsigned char *x,unsigned char *y,unsigned char *pen);
void init_queues(void);
unsigned char FIFO_Display_DDS_Pixel_pop2(unsigned char *x,unsigned char *y,unsigned char *p);
//unsigned char FIFO_Display_DDS_Pixel_push_v2(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char FIFO_Display_DDS_Pixel_pop2_v2(unsigned char *x,unsigned char *y,unsigned char *p);
unsigned char FIFO_Display_DDS_Pixel_push(unsigned char datox,unsigned char datoy,unsigned char pen);
unsigned char *FIFO_DDS_next_X(unsigned char *p);
unsigned char isFIFO_DDS_Display_Char_empty2(void);
void init_FIFO_DDS_Display_Char2(void);
unsigned char FIFO_Display_DDS_Char_pop2(unsigned char *x,unsigned char *y);
unsigned char FIFO_Display_DDS_Char_push(unsigned char datox,unsigned char datoy);
unsigned char *gotonext(unsigned char *last,unsigned char *first,unsigned char *tail);
unsigned char FIFO_displayBox_pop(void);
void BarraDet_displayUINT_var(unsigned char posx,unsigned char posy,unsigned short int *usint);
unsigned char FIFO_displayBox_push(unsigned char box);
unsigned char FIFO_displayBox_isEmpty(void);
void FIFO_Display_DDS_Char_clean(void);
void FIFO_Display_DDS_Pixel_clean(void);
unsigned char FIFObox_nextLast(void);
void malloc_display_Box(void);
unsigned char FIFOboxChars_nextLast(void);
unsigned char FIFO_displayBoxChars_pop(unsigned char *x,unsigned char *y);
unsigned char FIFO_displayBoxChar_push(unsigned char x,unsigned char y);
void malloc_display_Box_Chars(void);
signed short int popPushFIFO_OFFSET(signed short int *ry,signed short int xn,signed short int yn);
void init_FIFO_OFFSET(void);
void init_FIFOs_TX_IOUP(void);
unsigned char getComand_FIFO_B(unsigned char n);
unsigned char pushFIFO_TX(unsigned char cmd,unsigned char *dato);
signed short int **next2(signed short int *inicio,signed short int *final,signed short int **pos);
//unsigned char pushFIFO_IO_TX(unsigned char *data,unsigned char size);
unsigned char isLoad_next(unsigned char **p);
void reset_pointer(void);
void pushFIFO_average_Offset(signed short int **pos,signed short int **pop,signed short int *tail,signed short int *ini,signed short int dato);
signed short int popFIFO_average_Offset(signed short int **pos,signed short int **pop,signed short int *tail,signed short int *ini);
unsigned char *next(unsigned char *inicio,unsigned char *final,unsigned char *pos,unsigned char *pop);
void init_FIFO_BARRA_NUMERO(void);
unsigned char FIFO_barraNum_push(unsigned char c);
void FIFO_barraNum_VFDposicion(unsigned char posx,unsigned char posy);
unsigned char  FIFO_barraNum_pop(void);
unsigned char isFIFO_num_Not_Empty(void);
void FIFO_barraNum_VFDserial_SendChar(unsigned char c);
void init_FIFO_CMD_DDS(void);
void init_FIFO_TX_serial_General(void);
void vaciarBuffer(unsigned char *p,unsigned char *p2,unsigned short int size);
void push_FIFO_TRANSMISION_serial_IO(unsigned char byte);
unsigned char xpop_FIFO_CMD_DDS(signed short int *x,signed short int *y);
unsigned char push_FIFO_CMD_DDS(signed short int x,signed short int y);
unsigned char pop_FIFO_TRANSMISION_serial_IO(unsigned char *status);
void reset_FIFO_serial_TX(void);
unsigned char search_NO_Repetido(signed short int x,signed short int y);
void init_FIFO_SaveTemp_pixel_DDS(void);
unsigned char pop_FIFO_save_Temp_pixel_DDS(signed short int *x,signed short int *y);
unsigned char push_FIFO__save_Temp_pixel_DDS(signed short int x,signed short int y);
unsigned char is_FULL_FIFO_save_Temp_pixel_DDS(void);
unsigned char is_Empty_FIFO_save_Temp_pixel_DDS(void);
void clean_repaint_Pixels_DDS(void);
unsigned char IIC_FIFO_push_RX(unsigned char);
unsigned char IIC_FIFO_push_TX(unsigned char);
unsigned char IIC_FIFO_pop_RX(void);
unsigned char IIC_FIFO_pop_TX(void);
unsigned char get_Paquete(unsigned char *p,unsigned char (*pop)(void));
unsigned char is_device(unsigned char (*pop)(void),unsigned char *n);
unsigned char is_Lenght(unsigned char(*pop)(void),unsigned char *n);
unsigned char is_App(unsigned char(*pop)(void),unsigned char *n);
unsigned char is_CMD(unsigned char(*pop)(void),unsigned char *n,unsigned char app);
void get_parametros(unsigned char (*pop)(void),unsigned char *p);
void IIC_stop(void);
void init_FIFO_IIC(void);
void init_FIFO_RX_serial_Keypad(struct _FIFO_1byte_ *s);
unsigned char FIFO_general_1byte_push(unsigned char dato,struct _FIFO_1byte_ *s);
//unsigned char get_case_FIFO_general(struct _FIFO_1byte_ *s);
unsigned char FIFO_general_1byte_pop(unsigned char *dato,struct _FIFO_1byte_ *s);
void reset_FIFO_general_UChar(struct _FIFO_1byte_ *s,unsigned char *arr,unsigned short int size);
void Testing_SO_Debug(void);
unsigned char vfd_FIFO_push(unsigned char x,unsigned char y,unsigned char p);
unsigned char vfd_FIFO_pop(unsigned char *x,unsigned char *y,unsigned char *p);
unsigned char vfd_FIFOs_RESET(void);
void init_FIFO_General_1byte(struct _FIFO_1byte_ *s,unsigned char *h,unsigned char *t,unsigned short int size);
unsigned char dds_pix_pop(signed short int *x,signed short int *y);
unsigned char dds_pix_append(signed short int x,signed short int y);
unsigned char dds_pix_reset(void);
void init_System_Monitor(void);
void reset_sys_mon(void);
void reset_sys_mon2(void);
unsigned char test_FIFOS_VFD(void);
//unsigned char Recursos_Solicitud(unsigned char recurso,unsigned char pid);
//void Recursos_Devolver(unsigned char recurso1);
void insertVFDProcess(unsigned char process);
unsigned char insertVFDProcess_(unsigned char process);
unsigned char Solicitar_Recurso(unsigned char Recurso,unsigned char pid);
unsigned char ya_esta_en_la_FIFO_VFD(unsigned char pid);
void get_parameters_to_process_PID(unsigned char *pid1,unsigned char *index,unsigned char pid);
unsigned char sacar_de_la_FIFO_vfd(unsigned char pid);
unsigned char insertarlo_en_Fifo_VFD(unsigned char pid);
//void Devolver_Recurso_VFD(unsigned char pid);
unsigned char sacar_de_la_FIFO_IO(unsigned char pid);
unsigned char insertarlo_en_Fifo_IO(unsigned char pid);
unsigned char ya_esta_en_la_FIFO_IO(unsigned char pid);
void Devolver_Recurso_VFD(unsigned char recurso,unsigned char pid);
// Nuevas funciones para _FIFO_1byte_v2_
void init_FIFO_General_1byte_v2(struct _FIFO_1byte_v2 *s, unsigned char *buff,unsigned char size);
unsigned char FIFO_general_1byte_push_v2(unsigned char dato, struct _FIFO_1byte_v2 *s);
unsigned char FIFO_general_1byte_pop_v2(unsigned char *dato, struct _FIFO_1byte_v2 *s);
void reset_FIFO_general_UChar_v2(struct _FIFO_1byte_v2 *s, unsigned char size);
unsigned char FIFO_push_1byte_v2(unsigned char x);
unsigned char colaVacia(struct _FIFO_1byte_ *f);
unsigned char idEnCola(struct _FIFO_1byte_ *f,unsigned char id);
unsigned char desencolar(struct _FIFO_1byte_ *f,unsigned char id);
unsigned char solicitarRecurso(unsigned char id);
unsigned char liberarRecurso( unsigned char id);
unsigned char encolar(struct _FIFO_1byte_ *f,unsigned char id);
unsigned char FIFO_general_1byte_peek_v2(struct _FIFO_1byte_v2 *s);
unsigned char busqueda2(unsigned char *arr,unsigned char size,unsigned char dato);
unsigned char FIFO_general_1byte_search_v2(unsigned char dato,struct _FIFO_1byte_v2 *s);
//void inicializarSistemaRecursos(void);
// Funciones para manejar FIFOs
void fifo_init(fifo_t *f, uint8_t *buffer, uint16_t size);
bool fifo_append(fifo_t *f, uint8_t data);
bool fifo_pop(fifo_t *f, uint8_t *data);
bool fifo_is_empty(fifo_t *f);
bool fifo_is_full(fifo_t *f);
uint16_t fifo_get_count(fifo_t *f);
void fifo_clear(fifo_t *f);
bool fifo_has_overflow(fifo_t *f);
void fifo_clear_overflow(fifo_t *f);
void fifo_system_init(void);    // Reservar memoria para las estructuras FIFO
unsigned char manejarTurno(unsigned char *task, unsigned char *turno,
                           unsigned char task_id, unsigned char next_turno,
                           unsigned char *recurso);

// Macros para sintaxis tipo C#: fifo.algo->Append(x)
// Estas macros permiten la sintaxis que deseas
#define FIFO_APPEND(fifo_ptr, data) fifo_append(fifo_ptr, data)
#define FIFO_POP(fifo_ptr, data_ptr) fifo_pop(fifo_ptr, data_ptr)
unsigned char solicitarRecurso(unsigned char id);
unsigned char liberarRecurso(unsigned char id);










#endif /* ASD_APP_INCLUDE_QUEUE_H_ */
