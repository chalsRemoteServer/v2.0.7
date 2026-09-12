/**
  @file
  @brief
  Free RTOS Example 3

  An example with FreeRTOS illustrating the use of time-slicing, preemption and mutex based synchronization.
*/
/*
 * ============================================================================
 * History
 * =======
 * 29 Dec 2015 : Created
 *
 * Copyright (C) Bridgetek Pte Ltd
 * ============================================================================
 *
 * This source code ("the Software") is provided by Bridgetek Pte Ltd 
 * ("Bridgetek") subject to the licence terms set out
 * http://brtchip.com/BRTSourceCodeLicenseAgreement/ ("the Licence Terms").
 * You must read the Licence Terms before downloading or using the Software.
 * By installing or using the Software you agree to the Licence Terms. If you
 * do not agree to the Licence Terms then do not download or use the Software.
 *
 * Without prejudice to the Licence Terms, here is a summary of some of the key
 * terms of the Licence Terms (and in the event of any conflict between this
 * summary and the Licence Terms then the text of the Licence Terms will
 * prevail).
 *
 * The Software is provided "as is".
 * There are no warranties (or similar) in relation to the quality of the
 * Software. You use it at your own risk.
 * The Software should not be used in, or for, any medical device, system or
 * appliance. There are exclusions of Bridgetek liability for certain types of loss
 * such as: special loss or damage; incidental loss or damage; indirect or
 * consequential loss or damage; loss of income; loss of business; loss of
 * profits; loss of revenue; loss of contracts; business interruption; loss of
 * the use of money or anticipated savings; loss of information; loss of
 * opportunity; loss of goodwill or reputation; and/or loss of, damage to or
 * corruption of data.
 * There is a monetary cap on Bridgetek's liability.
 * The Software may have subsequently been amended by another user and then
 * distributed by that other user ("Adapted Software").  If so that user may
 * have additional licence terms that apply to those amendments. However, Bridgetek
 * has no liability in relation to those amendments.
 * ============================================================================
 */

#include <stdint.h>
#include <stdio.h>
#include "ft900.h"

#include "TFT.h"
#include "system.h"

#include "ff.h"
#include "diskio.h"
#include "assert.h"
#include "fileSys.h"
#include "queue1.h"
#include "RTOS.h"
#include "ComSer.h"
#include "platform_time.h"

/* Three Demo Tasks are implemented here to illustrate the usage of FreeRTOS.
 *
 * DEMO 1 - Time-slicing example
 * ------------------------------
 * 3 Tasks with equal priority are created. FreeRTOS will try give all 3 of them equal execution time on average.
 *
 * DEMO 2 - Testing task priorities
 * ------------------------------
 * 3 Tasks of different priorities (1,2,3) are created. The task with priority 2 runs constantly, never yielding.
 * The task with priority 3 (highest priority) prints it's name and yields every 500mS. The net result is that Task_2
 * runs constantly, while being interrupted by Task_3 every 500mS. Task_1 never gets to run.
 *
 * DEMO 3 - Using a mutex to synchronize access
 * ------------------------------
 * 4 Tasks are created with different priorities. Each of the tasks print lines onto UART0. Since the tasks have
 * different priorities, preemption will occur and a mutex is used to synchronize access to UART0, keeping the printed
 * strings uninterrupted.
 *
 */

void UART_1_ISR(void);
extern volatile struct _DISPLAY_VFD_ comm;



int main(void)
{
	sys_reset_all(); /****** Peripheral reset ******/
	sys_enable(sys_device_uart0);/* enable uart */
	sys_enable(sys_device_uart1);/* enable uart */

	sys_enable(sys_device_timer_wdt);
    gpio_function(48, pad_uart0_txd); /* UART0 TXD */ /* Set UART0 GPIO functions to UART0_TXD and UART0_RXD... */
    gpio_function(49, pad_uart0_rxd); /* UART0 RXD */
    gpio_function(52, pad_uart1_txd); /* UART1 TXD */ /* Set UART0 GPIO functions to UART0_TXD and UART0_RXD... */
  	gpio_function(53, pad_uart1_rxd); /* UART1 RXD */
    gpio_function(54, pad_gpio54);//pin entrada de pruebas de
    gpio_dir(54,pad_dir_input);
    gpio_function(12,pad_gpio12);
    gpio_dir(12,pad_dir_output);
    uart_mode(UART1, uart_mode_16550);
    uart_set_trigger_level(UART1, uart_mode_16550,
                           uart_fifo_trigger_level_14,  // RX trigger en 8 bytes
                           uart_fifo_trigger_level_0,  // TX trigger (no usado)
                           0, 0);
	uart_open(UART0, 1,UART_DIVIDER_19200_BAUD, uart_data_bits_8, uart_parity_none,uart_stop_bits_1);
	uart_open(UART1, 1,UART_DIVIDER_19200_BAUD, uart_data_bits_8, uart_parity_odd,uart_stop_bits_2);

	uart_puts(UART0,/* Print out a welcome message... */
			"\x1B[2J" /* ANSI/VT100 - Clear the Screen */
			"\x1B[H" /* ANSI/VT100 - Move Cursor to Home */
			"\x1B[32m"//verde
			"Copyright (c) Gracida Sistemas \r\n"
			"--------------------------------------------------------------------- \r\n"
			"\x1B[36m"//cyan
			"Welcome to Free RTOS Test ASD + v0.2.14 \r\n"
			"\r\n"
				"\x1B[33m"      // Amarillo
			"Communication tasks which will send and receive data over UART1 using FreeRTOS Queues\r\n"
			"Please ensure that UART1 RX and TX lines are connected to each other.\r\n"
			"--------------------------------------------------------------------- \r\n"
			"\x1B[0m" //reset
			 );//fin mensaje de inicio del uart0
	uart_puts(UART1,"Uart1 iniciado");/* Print out a welcome message... */

	//vStart_Pantalla_TFT();//_Encender Pantalla TFT
    interrupt_attach(interrupt_uart1,(uint8_t)interrupt_uart1,UART_1_ISR);/* Attach the interrupt so it can be called... */
    //uart_enable_interrupt(UART1, uart_interrupt_rx);/* Enable the UART to fire interrupts when receiving data... */
	//uart_enable_interrupts_globally(UART1);/* Enable interrupts to be fired... */
	//interrupt_enable_globally();

	gpio_functions_FileSystem();
    sys_enable(sys_device_sd_card);/* Start up the SD Card */
    sdhost_init();
    wait_for_inserted_card();
    montar_memoria_SD();
    init_queues();
    platform_time_init();//init timer
    TFT_init();
    disable_Int_Serial();
    while(1){
    	TFT_display();//delayms(5);
    	demo_SDCard();
        Task7_Serial_Receptor_Processor();//2.0.4
        vtaskDebugA_print_Rx_uart1();
        vTask9_Monitor_de_Errores();
        vTask11_Tranmision_Serial_ala_Procesadora();
        /*if(gpio_read(54)){gpio_write(12,1);signal_Detection=222;}
        else{gpio_write(12,0);signal_Detection=0;}*/
    }//fin while
    //vTaskStartScheduler();/*RTOS start Now all the tasks have been started - start the scheduler. */
}//fin main +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


void UART_1_ISR(){
uint8_t c;
uint8_t status=UART1->LSR_ICR_XON2;//leer estado
	if(status& (MASK_UART_LSR_OE | MASK_UART_LSR_PE_RXDATA | MASK_UART_LSR_FE | MASK_UART_LSR_BI)){
		uart_flush_rx_fifo(UART1);//limpiar FIFO y reiniciar
		disable_Int_Serial();}
    if(uart_is_interrupted(UART1,uart_interrupt_rx)){//llegaron 8 minimo
        while (uart_rx_has_data(UART1)) {
    	 uart_read(UART1,&c);
		 if(!comm.RxProc.append(c,&comm.RxProc)){
		    asm_noop();}}}
}//ifin interrupt---------------------------------------------------------




