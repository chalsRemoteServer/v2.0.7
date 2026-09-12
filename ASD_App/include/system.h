/*
 * system.h
 *
 *  Created on: Nov 24, 2025
 *      Author: desarrollo_1
 */

#ifndef ASD_APP_INCLUDE_SYSTEM_H_
#define ASD_APP_INCLUDE_SYSTEM_H_

#include <stdio.h>   // para printf

#define VERSION "v2.0.7.1"
#define DEBUG_SYSTEM

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
/* =============================== */
/*         CÓDIGOS ANSI COLORES    */
/* =============================== */

/* Colores básicos */
#define COLOR_RESET   "\033[0m"
#define COLOR_BLACK   "\033[30m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN    "\033[36m"
#define COLOR_WHITE   "\033[37m"

/* Colores brillantes */
#define COLOR_BRIGHT_BLACK   "\033[90m"
#define COLOR_BRIGHT_RED     "\033[91m"
#define COLOR_BRIGHT_GREEN   "\033[92m"
#define COLOR_BRIGHT_YELLOW  "\033[93m"
#define COLOR_BRIGHT_BLUE    "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN    "\033[96m"
#define COLOR_BRIGHT_WHITE   "\033[97m"

/* Fondos de colores */
#define BG_BLACK   "\033[40m"
#define BG_RED     "\033[41m"
#define BG_GREEN   "\033[42m"
#define BG_YELLOW  "\033[43m"
#define BG_BLUE    "\033[44m"
#define BG_MAGENTA "\033[45m"
#define BG_CYAN    "\033[46m"
#define BG_WHITE   "\033[47m"

/* Fondos brillantes */
#define BG_BRIGHT_BLACK   "\033[100m"
#define BG_BRIGHT_RED     "\033[101m"
#define BG_BRIGHT_GREEN   "\033[102m"
#define BG_BRIGHT_YELLOW  "\033[103m"
#define BG_BRIGHT_BLUE    "\033[104m"
#define BG_BRIGHT_MAGENTA "\033[105m"
#define BG_BRIGHT_CYAN    "\033[106m"
#define BG_BRIGHT_WHITE   "\033[107m"

/* Estilos de texto */
#define STYLE_BOLD       "\033[1m"
#define STYLE_DIM        "\033[2m"
#define STYLE_ITALIC     "\033[3m"
#define STYLE_UNDERLINE  "\033[4m"
#define STYLE_BLINK      "\033[5m"
#define STYLE_REVERSE    "\033[7m"
#define STYLE_HIDDEN     "\033[8m"
#define STYLE_STRIKE     "\033[9m"


#define CMD_DDS 0xF1 //comando de transmision datos de deteccion raw X&Y
#define CMD_IM_ALIVE 0xF2 //comando de transmision datos de
#define CMD_IM_ALIVE_ANS 0x2F//CONTESTACION
#define CMD_DDS_CTL  0x1F //ON|OFF  envio de comandos numeros del DDS
#define CMD_BARR_CTL 0x4F //ON|OFF envio desde uP la desccion en barra


#define STX  0x03
#define ETX  0x02


/* =============================== */
/*       EJEMPLOS DE USO           */
/* =============================== */
/*
uart_puts(UART0, COLOR_RED "Error crítico!\n" COLOR_RESET);
uart_puts(UART0, COLOR_GREEN "Operación exitosa\n" COLOR_RESET);
uart_puts(UART0, COLOR_YELLOW "Advertencia del sistema\n" COLOR_RESET);
uart_puts(UART0, COLOR_BLUE "Información importante\n" COLOR_RESET);
uart_puts(UART0, STYLE_BOLD COLOR_CYAN "Texto en negrita y cian\n" COLOR_RESET);
uart_puts(UART0, COLOR_WHITE BG_RED "Texto blanco sobre rojo\n" COLOR_RESET);
*/
#endif /* ASD_APP_INCLUDE_SYSTEM_H_ */
