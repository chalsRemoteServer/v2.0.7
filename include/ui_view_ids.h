/*
 * ui_view_ids.h
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

typedef enum
{
	// CONFIGURACION - OPCIONES - PRODUCTO - ADM PRODUCTOS - ELIMINAR PRODUCTO
//	UI_VID_PROD_ELIM_BACK      	 			= 10,
//	UI_VID_PROD_ELIM_ACCEPT     			= 11,
//	UI_VID_PROD_ELIM_UP         			= 12,
//	UI_VID_PROD_ELIM_DOWN       			= 13,

//	UI_VID_PROD_ELIM_ROW0       			= 14,
//	UI_VID_PROD_ELIM_ROW1       			= 15,
//	UI_VID_PROD_ELIM_ROW2       			= 16,
//	UI_VID_PROD_ELIM_ROW3       			= 17,

	// CONFIGURACION - OPCIONES - PRODUCTO - ADM PRODUCTOS - CAMBIO PRODUCTO
//	UI_VID_PROD_CAMBIO_BACK       			= 20,
//	UI_VID_PROD_CAMBIO_ACCEPT     			= 21,
//	UI_VID_PROD_CAMBIO_UP         			= 22,
//	UI_VID_PROD_CAMBIO_DOWN       			= 23,

//	UI_VID_PROD_CAMBIO_ROW0       			= 24,
//	UI_VID_PROD_CAMBIO_ROW1       			= 25,
//	UI_VID_PROD_CAMBIO_ROW2       			= 26,
//	UI_VID_PROD_CAMBIO_ROW3       			= 27,

    // ---- OPCIONES DE REGISTRO (200..299) ----
//    UI_VID_ROPT_BACK     					= 30, // CRUZ ROJA (pop)
//    UI_VID_ROPT_BTN0     					= 31, // botones de opciones (ejemplos)
//    UI_VID_ROPT_BTN1     					= 32,
//    UI_VID_ROPT_BTN2     					= 33,
//    UI_VID_ROPT_BTN3     					= 34,
//
//	UI_VID_ROPT_FREG_BTN0					= 35,
//	UI_VID_ROPT_FREG_BTN1					= 36,

	// --- APRENDER submenu (View) ---
	UI_VID_APR_FAST     	 				= 40,  // Aprend. rapido
	UI_VID_APR_RESULTS   					= 41,  // Resultados
	UI_VID_APR_BACK     	 				= 42,  // X (cerrar)
	// --- APRENDER views secundarios ---
	UI_VID_APRF_BACK     					= 43,   // X en "Aprend. rapido"
	UI_VID_APRR_BACK     					= 44,   // X en "Resultados"

    // ---- REGISTRO (100..199) ----
//    UI_VID_REG_BACK      					= 100, // CRUZ ROJA (pop)
//    UI_VID_REG_CONFIG    					= 101, // CONFIG (push opciones)
//    UI_VID_REG_UP        					= 102, // Flecha arriba (scroll -)
//    UI_VID_REG_DOWN      					= 103, // Flecha abajo (scroll +)

	 // ---- OPCIONES (CONFIG) (300..399) ----
	 UI_VID_OPT_BACK       					= 60, // CRUZ ROJA (pop)
	 UI_VID_OPT_PRODUCTO   					= 61,
	 UI_VID_OPT_SISTEMA    					= 62,
	 UI_VID_OPT_PERIF      					= 63,
	 UI_VID_OPT_ENTRADAS   					= 64,
	 UI_VID_OPT_BOBINA     					= 65,
	 UI_VID_OPT_USB        					= 66,

	 // CONFIGURACION - OPCIONES - PRODUCTO
	 UI_VID_OPT_PROD_BACK       			= 67, // CRUZ ROJA (pop)
     UI_VID_OPT_PROD_ADM   					= 68,
	 UI_VID_OPT_PROD_DATOS    				= 69,
	 UI_VID_OPT_PROD_APRENDER      			= 70,
	 UI_VID_OPT_PROD_AUDITORIA   			= 71,
	 UI_VID_OPT_PROD_SALIDAS     			= 72,
	 UI_VID_OPT_PROD_AJUSTES        		= 73,
	 UI_VID_OPT_PROD_UP   					= 138,
	 UI_VID_OPT_PROD_DOWN 					= 139,
	 UI_VID_OPT_PROD_MSPEED        			= 140,

	 // CONFIGURACION - OPCIONES - PRODUCTO - ADM PRODUCTOS
	 UI_VID_OPT_PROD_ADM_BACK       		= 74, // CRUZ ROJA (pop)
	 UI_VID_OPT_PROD_ADM_CAMBIO   			= 75,
	 UI_VID_OPT_PROD_ADM_CREAR    			= 76,
	 UI_VID_OPT_PROD_ADM_ELIMINAR      		= 77,

	 // CONFIGURACION - OPCIONES - PRODUCTO - ADM PRODUCTOS - HOJAS
	 UI_VID_OPT_PROD_LEAF_BACK 				= 78, // X (pop) para hojas

	 // CONFIGURACION - OPCIONES - PRODUCTO - APRENDER - HOJAS
	 UI_VID_OPT_PROD_APRENDER_APRAPIDO 		= 79, // X (pop) para hojas
	 UI_VID_OPT_PROD_APRENDER_APRESULTADOS 	= 80,

	 // CONFIGURACION - OPCIONES - PRODUCTO - AUDITORIA
	 UI_VID_OPT_PROD_AUDITORIA_AJUSTES 		= 81, // X (pop) para hojas
	 UI_VID_OPT_PROD_AUDITORIA_PATRONES 	= 82,
	 UI_VID_OPT_PROD_AUDITORIA_TPRODUCTOS 	= 83, // X (pop) para hojas
	 UI_VID_OPT_PROD_AUDITORIA_INIMANUAL 	= 84,

	 // CONFIGURACION - OPCIONES - PRODUCTO - SALIDAS
     UI_VID_OPT_PROD_SALIDAS_AJUSTES 		= 85, // X (pop) para hojas
	 UI_VID_OPT_PROD_SALIDS_TPRODUCTOS 		= 86,
	 UI_VID_OPT_PROD_SALIDAS_MOMR 			= 87, // X (pop) para hojas

	 UI_VID_OPT_SIS_ADMUSERS				= 88,
	 UI_VID_OPT_SIS_SHARDW                  = 89,
	 UI_VID_OPT_SIS_ESTADIST				= 90,
	 UI_VID_OPT_SIS_INTERFACES				= 91,
	 UI_VID_OPT_SIS_AUDITORIA				= 92,
	 UI_VID_OPT_SIS_CONFFABRICA				= 93,
	 UI_VID_OPT_SIS_LICENCIA				= 94,

	 UI_VID_OPT_SIS_ADMUSERS_EDITAR         = 95,
	 UI_VID_OPT_SIS_ADMUSERS_CREAR          = 96,
	 UI_VID_OPT_SIS_ADMUSERS_ELIMINAR       = 97,
	 UI_VID_OPT_SIS_ADMUSERS_COPIAR         = 98,
	 UI_VID_OPT_SIS_ADMUSERS_AJUSTES        = 99,
	 UI_VID_OPT_SIS_ADMUSERS_LOGIN2GO       = 100,

	 UI_VID_OPT_SIS_SHARDWARE_ES            = 101,
	 UI_VID_OPT_SIS_SHARDWARE_UC            = 102,
	 UI_VID_OPT_SIS_SHARDWARE_VERSIONES     = 103,
	 UI_VID_OPT_SIS_SHARDWARE_DEPURAR       = 104,

	 UI_VID_OPT_SIS_ESTADISTICAS_CONT       = 105,
	 UI_VID_OPT_SIS_ESTADISTICAS_SMETAL     = 106,
	 UI_VID_OPT_SIS_ESTADISTICAS_GRAFICAS   = 107,
	 UI_VID_OPT_SIS_ESTADISTICAS_AJUSTES    = 108,

	 UI_VID_OPT_SIS_CONFABRICA_GUARDARC     = 109,
	 UI_VID_OPT_SIS_CONFABRICA_RESTAURARC   = 110,

	 UI_VID_OPT_SIS_LICENCIA_CARAC          = 111,
	 UI_VID_OPT_SIS_LICENCIA_EXPORTARINF    = 112,
	 UI_VID_OPT_SIS_LICENCIA_IMPORTAR       = 113,
	 UI_VID_OPT_SIS_LICENCIA_REESTABLECER   = 114,

	 UI_VID_OPT_PER_OPCEXP					= 115,
	 UI_VID_OPT_PER_SUPERV					= 116,
	 UI_VID_OPT_PER_ADVERT					= 117,
	 UI_VID_OPT_PER_PRAUTO					= 118,

	 UI_VID_OPT_SUP_RECHAZO					= 119,
	 UI_VID_OPT_SUP_SENSAIRE				= 120,
	 UI_VID_OPT_SUP_CONTLLENO				= 121,

	 UI_VID_OPT_ENTRADAS_EXT				= 122,
	 UI_VID_OPT_ENTRADAS_DIGITAL			= 123,
	 UI_VID_OPT_ENTRADAS_CONFIG				= 124,

	 UI_VID_OPT_ENTRADAS_CONFIG_AJUST		= 125,
	 UI_VID_OPT_ENTRADAS_CONFIG_ENT1		= 126,
	 UI_VID_OPT_ENTRADAS_CONFIG_ENT2		= 127,
	 UI_VID_OPT_ENTRADAS_CONFIG_ENT3		= 128,

	 UI_VID_OPT_BOBINA_AJUSTES				= 129,
	 UI_VID_OPT_BOBINA_DFREC				= 130,

	 UI_VID_SC3_AUDITORIA					= 131,

	 UI_VID_SC4_EXPMANUAL					= 132,
	 UI_VID_SC4_RECHAZO						= 133,

	 UI_VID_OPT_USB_EXPREG					= 134,
	 UI_VID_OPT_USB_EXP						= 135,
	 UI_VID_OPT_USB_IMPORT					= 136,
	 UI_VID_OPT_USB_LOGIN2GO				= 137,

	 UI_VID_OPT_PROD_MSPEED_INFO 			= 141,
	 UI_VID_OPT_PROD_MSPEED_MEDIR 			= 142,
	 UI_VID_OPT_PROD_MSPEED_CALC 			= 143,
	 UI_VID_OPT_PROD_MSPEED_AJUSTES 		= 144,

	 UI_VID_OPT_SIS_CONFIG					= 145,

	 UI_VID_OPT_SIS_INTERFACES_RED      	= 146,
	 UI_VID_OPT_SIS_INTERFACES_CLAN	  		= 147,
	 UI_VID_OPT_SIS_INTERFACES_CONEX		= 148,
	 UI_VID_OPT_SIS_INTERFACES_ESSERIE		= 149,
	 UI_VID_OPT_SIS_INTERFACES_SERVER		= 150,

	 UI_VID_OPT_SISTEMA_CONFIG_ID			= 151,
	 UI_VID_OPT_SISTEMA_CONFIG_TMACHINE		= 152,
	 UI_VID_OPT_SISTEMA_CONFIG_TIME			= 153,
	 UI_VID_OPT_SISTEMA_CONFIG_AJUSTES		= 154,
	 UI_VID_OPT_SISTEMA_CONFIG_CALTOUCH		= 155,

	 // ---- REGISTRO - AJUSTES DE REGISTRO (138..149) ----
	 UI_VID_RAJREG_BACK     				= 156,
	 UI_VID_RAJREG_ACCEPT  					= 157,
	 UI_VID_RAJREG_UP      					= 158,
	 UI_VID_RAJREG_DOWN    					= 159,

	 UI_VID_RAJREG_ROW0    					= 160,
	 UI_VID_RAJREG_ROW1    					= 161,
	 UI_VID_RAJREG_ROW2    					= 162,
	 UI_VID_RAJREG_ROW3    					= 163,
	 UI_VID_RAJREG_ROW4    					= 164,
	 UI_VID_RAJREG_ROW5    					= 165,
	 UI_VID_RAJREG_ROW6    					= 166,
	 UI_VID_RAJREG_ROW7    					= 167,

	 UI_VID_FILTREG_ROW0 					= 168,
	 UI_VID_FILTREG_ROW1					= 169,
	 UI_VID_FILTREG_ROW2					= 170,
	 UI_VID_FILTREG_ROW3					= 171,
	 UI_VID_FILTREG_ROW4					= 172,

	 UI_VID_FECHAHORA_ROW0					= 173,

	 UI_VID_PRODUCTO_DATOSP_ROW0			= 174,
	 UI_VID_PRODUCTO_DATOSP_ROW1			= 175,
	 UI_VID_PRODUCTO_DATOSP_ROW2			= 176,
	 UI_VID_PRODUCTO_DATOSP_ROW3			= 177,
	 UI_VID_PRODUCTO_DATOSP_ROW4			= 178,
	 UI_VID_PRODUCTO_DATOSP_ROW5			= 179,

     /* ---- TECLADO TOUCH (180..219) ---- */
     UI_VID_KB_Q                         	= 180,
     UI_VID_KB_W                         	= 181,
     UI_VID_KB_E                         	= 182,
     UI_VID_KB_R                         	= 183,
     UI_VID_KB_T                         	= 184,
     UI_VID_KB_Y                         	= 185,
     UI_VID_KB_U                         	= 186,
     UI_VID_KB_I                         	= 187,
     UI_VID_KB_O                         	= 188,
     UI_VID_KB_P                         	= 189,

     UI_VID_KB_A                         	= 190,
     UI_VID_KB_S                         	= 191,
     UI_VID_KB_D                         	= 192,
     UI_VID_KB_F                         	= 193,
     UI_VID_KB_G                         	= 194,
     UI_VID_KB_H                         	= 195,
     UI_VID_KB_J                         	= 196,
     UI_VID_KB_K                         	= 197,
     UI_VID_KB_L                         	= 198,

     UI_VID_KB_Z                         	= 199,
     UI_VID_KB_X                         	= 200,
     UI_VID_KB_C                         	= 201,
     UI_VID_KB_V                         	= 202,
     UI_VID_KB_B                         	= 203,
     UI_VID_KB_N                         	= 204,
     UI_VID_KB_M                         	= 205,

     UI_VID_KB_SHIFT                     	= 206,
     UI_VID_KB_MODE                      	= 207,   /* 123 / ABC */
     UI_VID_KB_SPACE                     	= 208,
     UI_VID_KB_BKSP                      	= 209,
     UI_VID_KB_OK                        	= 210,
     UI_VID_KB_CANCEL                    	= 211,

     UI_VID_KB_1                         	= 212,
     UI_VID_KB_2                         	= 213,
     UI_VID_KB_3                         	= 214,
     UI_VID_KB_4                         	= 215,
     UI_VID_KB_5                         	= 216,
     UI_VID_KB_6                         	= 217,
     UI_VID_KB_7                         	= 218,
     UI_VID_KB_8                         	= 219,
     UI_VID_KB_9                         	= 220,
     UI_VID_KB_0                         	= 221,

	 /* CONFIGURACION - OPCIONES - PRODUCTO - ADM PRODUCTOS - CREAR */
	 UI_VID_OPT_PROD_LEAF_ACCEPT          	= 222,

	 UI_VID_PROD_CREAR_ROW0               	= 223,   /* Nombre del producto */
	 UI_VID_PROD_CREAR_ROW1               	= 224,   /* Numero del producto */

	 UI_VID_SISTEMA_CREAR_ROW0				= 225,
	 UI_VID_SISTEMA_CREAR_ROW1				= 226,
	 UI_VID_SISTEMA_CREAR_ROW2				= 227,
	 UI_VID_SISTEMA_CREAR_ROW3				= 228,

	 UI_VID_SISTEMA_CREAR_BACK	            = 229,
	 UI_VID_SISTEMA_CREAR_ACCEPT	        = 230,
	 UI_VID_SISTEMA_POP_OPT0              	= 231,
	 UI_VID_SISTEMA_POP_OPT1              	= 232,
	 UI_VID_SISTEMA_POP_OPT2              	= 233,
	 UI_VID_SISTEMA_POP_OPT3              	= 234,

	 UI_VID_SISTEMA_ELIM_BACK              	= 235,
	 UI_VID_SISTEMA_ELIM_ACCEPT            	= 236,
	 UI_VID_SISTEMA_ELIM_UP                	= 237,
	 UI_VID_SISTEMA_ELIM_DOWN              	= 238,

	 UI_VID_SISTEMA_ELIM_ROW0              	= 239,
	 UI_VID_SISTEMA_ELIM_ROW1              	= 240,
	 UI_VID_SISTEMA_ELIM_ROW2              	= 241,
	 UI_VID_SISTEMA_ELIM_ROW3              	= 242,

	 UI_VID_KB_ROW1 						= 243,
	 UI_VID_KB_ROW2 						= 244,
	 UI_VID_KB_ROW3 						= 245,
	 UI_VID_KB_ROW4 						= 246,

//	 UI_VID_PROD_CREAR_POP_CANCEL  			= 247,
//	 UI_VID_PROD_CREAR_POP_ACCEPT  			= 248,
//	 UI_VID_PROD_CREAR_FAST_CANCEL 			= 249,
//	 UI_VID_PROD_CREAR_FAST_ACCEPT 			= 250,

	 /* PRODUCTO - AUDITORIA - AJUSTES */
//	 UI_VID_PROD_AUD_AJ_BACK            	= 267,
//	 UI_VID_PROD_AUD_AJ_ACCEPT          	= 256,

//	 UI_VID_PROD_AUD_AJ_ROW_MODO        	= 257,
//	 UI_VID_PROD_AUD_AJ_ROW_TPREP       	= 258,

//	 UI_VID_PROD_AUD_AJ_POP_BACK        	= 259,
//	 UI_VID_PROD_AUD_AJ_POP_ACCEPT      	= 260,
//	 UI_VID_PROD_AUD_AJ_POP_UP          	= 261,
//	 UI_VID_PROD_AUD_AJ_POP_DOWN        	= 262,

//	 UI_VID_PROD_AUD_AJ_POP_ROW0        	= 263,
//	 UI_VID_PROD_AUD_AJ_POP_ROW1        	= 264,
//	 UI_VID_PROD_AUD_AJ_POP_ROW2        	= 265,
//	 UI_VID_PROD_AUD_AJ_POP_ROW3        	= 266,

} UiViewId;
