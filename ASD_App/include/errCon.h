/*
 * errCon.h
 *
 *  Created on: Jul 22, 2026
 *      Author: chals
 */

#ifndef ASD_APP_INCLUDE_ERRCON_H_
#define ASD_APP_INCLUDE_ERRCON_H_

#include <stdint.h>

typedef enum {
	// Estados de operación normal (OK) VERDE
	STATUS_OK = 0x00,           // Todo funcionando correctamente
	STATUS_IDLE = 0x01,         // Sistema en reposo

	// Estados de booteo GRIS
    STATUS_BOOTING = 0x05,      // Iniciando sistema
    STATUS_BOOT_OK = 0x06,      // Booteo exitoso
    STATUS_BOOT_FAIL = 0x07,    // Fallo en booteo
    STATUS_DETECTION_DISABLE =0x08,

    //NARANJE
    STATUS_METAL = 0x12,      // Sistema en DETECTO METAL
    STATUS_METAL_MAX = 0x13,      // dtecto mucho metal

    // Estados de advertencia (Warning - no crítico)
    STATUS_WARNING       = 0x20,      // Advertencia genérica
    STATUS_WARN_BATTERY  = 0x21,  // Batería baja
    STATUS_WARN_TEMP_HIGH = 0x22,    // Temperatura elevada
    STATUS_WARN_COMM     = 0x23,    // Problema de comunicación menor
    STATUS_WARN_LOGBOOK_FULL = 0x24,     // CRC incorrecto (pero recuperable)
    STATUS_WARN_SD_NODET = 0x25,   // Parámetro fuera de rango
	STATUS_WARN_EXT_FAULT = 0x26, //external fault
	STATUS_WARN_REJECT_BUN_FULL=0x27, //
	STATUS_WARN_REJECT_MON=0x28,  //Adapt the setting for the pusher reaction time.
	STATUS_WARN_SHORT_MO=0x29, //Switching outputs MO1 ? MO4, short-circuit or overload.
	STATUS_WARN_CONN_MO=0x2A,  //Switching outputs MO1 – MO4, connection faulty.
	STATUS_WARN_REJECT_CHECK=0x2B, //Faulty sensor (light barrier) for reject check.
	STATUS_WARN_BIN_LOCK=0x2C, //The door of the reject bin is open (lock).
	STATUS_WARN_MOTOR=0x2D,  //Conveyor belt drive motor overloaded.


	// Estados de error (Error - crítico)
    STATUS_ERROR = 0x30,        // Error genérico
    STATUS_ERR_TIMEOUT = 0x31,  // Timeout
    STATUS_ERR_FLAP_POS = 0x32,      // Error de flap position
    STATUS_ERR_AIR_PRESS = 0x33,    // AIR PRESURE
    STATUS_ERR_CONTROL_TEMP = 0x34,   // Error de memoria
    STATUS_ERR_COMMS = 0x35,    // Error de comunicaciones
    STATUS_ERR_TRANSMITER_OPEN = 0x36,   // Error NO SE detecta transmisor
    STATUS_ERR_RECEIVER = 0x37,      // Error no se detecta receptor
    STATUS_ERR_RECEIVER_TOO_HIGH = 0x38,   // Error BALANCE MUY ALTO
    STATUS_ERR_TRANSMITTER_SHORT = 0x39, // Error de  TRANmistor se detecta corto
	STATUS_ERR_POWER_SUPP_CU =0x3A,
	STATUS_ERR_COMM_CPLD = 0x3B,
	STATUS_ERR_POWER_SUPP = 0x3C, //ERROR de voltaje 24,12,5,
	STATUS_ERR_FRAM_WRITE =0x3E, //error data memo SD, storage, faulty
	STATUS_ERR_FRAM_READ =0x3F, //remplace memo
	STATUS_ERR_HARDWARE =0x40, //ERROR INTERNO
	STATUS_ERR_OUT = 0x41,//ERROR interno rele, output, con j5

    STATUS_ERR_UNKNOWN = 0xFF,   // Error desconocido

	//Azul audicheck
	STATUS_AUDICHECK = 0x80
} status_code_e;


typedef struct{
	    status_code_e code;         // Código de estado (del enum)
	    uint8_t source;             // Origen (SOURCE_*)
	    uint8_t user;             // Submódulo (MODULE_*)
	    char message[80];           // Mensaje descriptivo (más largo)
	    uint32_t timestamp;         // Timestamp del estado
	    uint8_t severity;           // 0=OK, 1=WARN, 2=ERROR, 3=BOOT, 4=AUDIT
	    uint8_t retry_count;        // Contador de reintentos
	    uint8_t fault_count;        // Contador de fallos acumulados
	    volatile uint8_t Alive;   //la procesadora esta viva y contesta.
}status_bar_s;















#endif /* ASD_APP_INCLUDE_ERRCON_H_ */
