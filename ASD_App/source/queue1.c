/*
 * queue.c
 *
 *  Created on: Apr. 22, 2026
 *      Author: desarrollo_1
 */
#include "queue1.h"
#include "system.h"

volatile struct _DISPLAY_VFD_ comm;

unsigned char buffer50[SIZE_BUFFER50];//fifo de recepcion serial desde procesadora
unsigned char buffer51[SIZE_BUFFER51];//fifo de TX serial HACIA procesadora
struct _Recursos recurso;
unsigned char debug2;//eval var, fifo rx full
static unsigned char *shift_left(unsigned char *arr,unsigned char size);


void init_queues(void){

	init_FIFO_General_1byte_v2(&comm.RxProc,&buffer50[0],SIZE_BUFFER50);
	init_FIFO_General_1byte_v2(&comm.TxProc,&buffer51[0],SIZE_BUFFER51);
    comm.RxProc.mode=1;//modo debug
    comm.TxProc.mode=1;//modo debug

}//fin init queue-------------------------------------------


// Versión v2: funciones para _FIFO_1byte_v2_ (appendByte con un solo parámetro)
void init_FIFO_General_1byte_v2(struct _FIFO_1byte_v2 *s, unsigned char *buff,unsigned char size){
    s->head = buff;
    s->tail = buff+(size-1);//ultimo elemento
    s->pop = s->push= buff;
    s->ncount = 0;
    s->dequeue = FIFO_general_1byte_pop_v2;
    s->append = FIFO_general_1byte_push_v2;
    s->peek =FIFO_general_1byte_peek_v2;
    s->search=FIFO_general_1byte_search_v2;
    s->size = size;
    s->resetFIFO = reset_FIFO_general_UChar_v2;
}//fini init general-------------------------------------------

//sacar un dato de la queue fifo
unsigned char FIFO_general_1byte_pop_v2(unsigned char *dato, struct _FIFO_1byte_v2 *s){
    if(s->ncount == 0)
        return FALSE;
    if(s->mode==0){//normal
		if(s->ncount == 1){
			*dato = *(s->pop);
			*(s->pop) = 0;
			s->pop = s->push = s->tail;
			s->ncount = 0;}
		else{
			*dato = *(s->pop);
			*(s->pop) = 0;
			if(s->ncount > 0) s->ncount--;
			if(s->pop == s->tail)
				s->pop = s->head;
			else
				s->pop++;}
		return TRUE;}
    else{//modo debug
    	*dato=*(s->head);
    	s->push=shift_left(s->head,s->ncount);
        s->ncount--;
    	return TRUE;}
return FALSE;
}//-----------------------------------------------------------------

/* mueve todos elementos ala izq 1,2,3 ->queda-> 2,3 y regresa puntero al 3
 * ncount es la cantidad de elementos que hay en la fifo incluyendo el que se va a sacar */
static unsigned char *shift_left(unsigned char *arr,unsigned char size){
unsigned char i;
      switch(size){
    	  case 0:return &arr[0];
      	  case 1:arr[0]=0;return &arr[0];
      	  case 2:arr[0]=arr[1];arr[1]=0;return &arr[0];
      	  default:
      	  	  for(i=0;i< size-1;i++){
	               arr[i] = arr[i + 1];}
	          arr[size - 1] = 0;
	          return &arr[size-2];}
}//fin shift left------------------------------------------------


//regresa FALSE si no se pudo insertar en la cola
unsigned char FIFO_general_1byte_push_v2(unsigned char dato, struct _FIFO_1byte_v2 *s){
unsigned char ret=FALSE;
  if(s->ncount < s->size){
    if(s->mode==0){//normal
			*(s->push) = dato;
			if(s->push==s->tail) s->push=s->head;
			else{s->push++;}
			s->ncount++;
			ret=TRUE;}
    else{//modo debug, en esta version no es circular
    	*(s->push) = dato;
    	s->push++;
    	s->ncount++;
        ret=TRUE;}}
  else{if(s->push>s->tail){
	     debug2=TRUE;}//error  de software
  	   else{debug2=FALSE;}
       ret=FALSE;}   /* detiene la ejecución aquí */
return ret;
}//fin de push en fifo------------------------------------------------

/* muestra el primer elemento que se va  a sacar en fifo */
unsigned char FIFO_general_1byte_peek_v2(struct _FIFO_1byte_v2 *s){
	if(s->mode==0){//modo normal
		return *(s->pop);}
	else{ return *(s->head);}
}//fin de peek the first data pointed to be fifo poped-------------

//regresa 1 si lo encuentra
unsigned char FIFO_general_1byte_search_v2(unsigned char dato,struct _FIFO_1byte_v2 *s){
unsigned char *ptr;
	if(s->mode==0){//modo normal
	  ptr=s->pop;
	  switch(s->ncount){
		  case 0:return FALSE;
		  case 1:return(dato==*ptr)?TRUE:FALSE;
		  default:do{if(*ptr==dato)return TRUE;
                	 ptr++;
                	 if(ptr>=s->tail)ptr=s->head;}
		   	         while(ptr!=s->push);
		  	  	  	 return FALSE;}}
	else{return(busqueda2(s->head,s->ncount,dato))?TRUE:FALSE;}
return FALSE;
}//fin de search v2------------------------------------------------------------

unsigned char busqueda2(unsigned char *arr,unsigned char size,unsigned char dato){
	if(size==0)return 0;
	if(*arr==dato)return 1;
	return busqueda2(arr+1,size-1,dato);
}//fin algoritmo de busqueda----------------------------------------



void reset_FIFO_general_UChar_v2(struct _FIFO_1byte_v2 *s, unsigned char size){
unsigned char *i;
	s->pop = s->push =s->head;
    s->ncount = 0;
    for(i=s->head;i<(s->head+size);i++){//cleanArray(s->head, size, 0);
             *i=0;}
}//-----------------------------------------------



// Función para solicitar RECURSO: transmision serial  hacia tarjeta display IO
// TRUE solamente si se encola el id que no esta en cola y esta libre el rec
/*  1.- si esta ocupado return FALSE
 *        1.1.-se revisa si ya esta en el turno sino se mete al turno
 *  2.- Si esta libre return TRUE y se asigna
 *  3.- si
 * */
unsigned char solicitarRecurso(unsigned char id) {
//const unsigned char MAX_TASK=2;//numero de tareas a manejar
static unsigned char Recurso;
unsigned char dato;

    //r=&Recurso;    //isREC   0x07
    if(id==ID_vTask_isREC){return Recurso;}//comando para leer recurso
    if(id==ID_vTask_LIBERAR){Recurso=0;return 0;}
    if(!((id>=ID_vTask_2)&&(id<=ID_vTask_3))) return FALSE;//verificacion de seguiridad
    if(Recurso>0){//recurso ocupado
    	if(recurso.COMM_IO.ncount<recurso.COMM_IO.size){
    		if(recurso.COMM_IO.ncount==0){
    			 recurso.COMM_IO.append(id,&recurso.COMM_IO);return FALSE;}
    	    if(recurso.COMM_IO.search(id,&recurso.COMM_IO)){
    	    	 return FALSE;}//ya esta en la cola
    	    else{recurso.COMM_IO.append(id,&recurso.COMM_IO);return FALSE;}}
    	else{return FALSE;}}
    else{if(recurso.COMM_IO.ncount==0){ //recurso=0, libre,, fifo vacia
     	 	  Recurso=id;return TRUE;}
    	 else{if(recurso.COMM_IO.dequeue(&dato,&recurso.COMM_IO)){
    		       Recurso=dato;return TRUE;}
    	 	  else{return FALSE;}}}//no hay fifo,
return FALSE;
}//--------------------------------------------------------------------------------------




// Función para liberar recurso
/* verifica si el recurso esta vacio  si lo esta sale return true recurso=0
 * si no esta vacio, verifica, si esta en fifo si lo esta lo quita sino no
 * busca si esta el recurso en fifo si lo esta lo quita.  */
unsigned char liberarRecurso(unsigned char id){
	if(solicitarRecurso(ID_vTask_isREC)==id){
	    solicitarRecurso(ID_vTask_LIBERAR);
		return TRUE;}
return FALSE;  // Recurso liberado, sin esperas
}//----------------------------------------------------------------


