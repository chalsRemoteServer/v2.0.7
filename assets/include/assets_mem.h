/*
 * assets_mem.h
 *
 *  Created on: 11 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

#define MEM_FONT      							0x000F7E00UL
#define MEM_LOGO 								0x000F8000UL
#define MEM_PIC1 								0x000FC000UL   // >= 0xFB340, alineado “bonito”
#define MEM_DL_STATIC 							(EVE_RAM_G_SIZE - 4096) /* parte estática del DL */

#define MEM_ICON_HOME                        	0x000A000UL
#define MEM_ICON_USUARIO                     	0x000C000UL
#define MEM_ICON_CONFIGURACION               	0x000E000UL

#define MEM_ICON_FECHAHORA                 		0x0014000UL
#define MEM_ICON_AJREGISTRO                	 	0x0020000UL
#define MEM_ICON_FREGISTRO                   	0x0022000UL
#define MEM_ICON_BREGISTRO                	 	0x0024000UL
#define MEM_ICON_EXPREGISTRO                 	0x0026000UL

#define MEM_ICON_PRODUCTO                	 	0x0028000UL
#define MEM_ICON_SISTEMA                     	0x0030000UL
#define MEM_ICON_PERIFERICOS                 	0x0032000UL
#define MEM_ICON_ENTRADAS                	 	0x0034000UL
#define MEM_ICON_BOBINA                      	0x0036000UL
#define MEM_ICON_USB                 			0x0038000UL

#define MEM_ICON_ADMPROD                 	 	0x0040000UL
#define MEM_ICON_DATPROD                 	 	0x0042000UL
#define MEM_ICON_APRENDER                	 	0x0044000UL
#define MEM_ICON_AUDITORIA						0x0046000UL
#define MEM_ICON_SALIDAS						0x0048000UL
#define MEM_ICON_AJUSTES                	 	0x0050000UL
#define MEM_ICON_MSPEED                 		0x0012000UL

#define MEM_ICON_INFO   						0x00C2000UL

#define MEM_ICON_CAMBPRODUCTO                	0x0052000UL
#define MEM_ICON_CREAPRODUCTO                	0x0054000UL
#define MEM_ICON_ELIMPRODUCTO                	0x0056000UL

#define MEM_ICON_PROBETAS                		0x0058000UL
#define MEM_ICON_TODOSPROD                		0x0060000UL
#define MEM_ICON_INIMANUAL                	    0x0062000UL

#define MEM_ICON_MOMR                	    	0x0064000UL

#define MEM_ICON_ADMUSERS                		0x0066000UL
#define MEM_ICON_SHARDWARE                		0x0068000UL
#define MEM_ICON_ESTADISTICAS                	0x0070000UL
#define MEM_ICON_CONFABRICA                		0x0072000UL
#define MEM_ICON_LICENCIA                       0x0074000UL

#define MEM_ICON_EDITAR                			0x0076000UL
#define MEM_ICON_CREAR                			0x0078000UL
#define MEM_ICON_ELIMINAR                		0x0080000UL
#define MEM_ICON_COPIAR                			0x0082000UL
#define MEM_ICON_LOGIN2GO                		0x0084000UL

#define MEM_ICON_ENTRADASYS                		0x0086000UL
#define MEM_ICON_UC								0x0088000UL
#define MEM_ICON_VERSIONES                		0x0090000UL
#define MEM_ICON_DEBUG                			0x0092000UL

#define MEM_ICON_CONTADORES                		0x0094000UL
#define MEM_ICON_METALSIGNAL                    0x0096000UL
#define MEM_ICON_DISPLAYS                		0x0098000UL

#define MEM_ICON_GUARDCONFIG                    0x009A000UL
#define MEM_ICON_RESTCONFIG                		0x009C000UL

#define MEM_ICON_CARACTERISTICAS   			    0x009E000UL
#define MEM_ICON_IMPORTAR   				    0x00A0000UL
#define MEM_ICON_EXPORTAR   				    0x00A2000UL

#define MEM_ICON_SUPERVISION   					0x00A4000UL
#define MEM_ICON_ADVERTENCIA   					0x00A6000UL
#define MEM_ICON_PRUEBAAUTO   					0x00A8000UL
#define MEM_ICON_OPCEXP   						0x00AA000UL

#define MEM_ICON_RECHAZO                     	0x00AC000UL
#define MEM_ICON_SENSORAIRE                  	0x00AE000UL
#define MEM_ICON_CONTENEDOR                  	0x00B0000UL

#define MEM_ICON_EXTERNO                     	0x00B2000UL
#define MEM_ICON_DIGITAL                     	0x00B4000UL
#define MEM_ICON_CONFIGURABLE                	0x00B6000UL

#define MEM_ICON_DESVFREC                    	0x00B8000UL

#define MEM_ICON_RAPREND                    	0x00BA000UL
#define MEM_ICON_EXPORTAR2                    	0x00BC000UL

#define MEM_ICON_FARRIBA                     	0x00BE000UL
#define MEM_ICON_FABAJO                      	0x00C0000UL

#define MEM_ICON_FDERECHA                     	0x0016000UL
#define MEM_ICON_FIZQUIERDA                     0x0018000UL

#define MEM_ICON_CALCULA   						0x00C4000UL

#define MEM_ICON_RED							0x00C6000UL
#define MEM_ICON_CONEXLAN						0x00C8000UL
#define MEM_ICON_CONEXIONES						0x00CA000UL
#define MEM_ICON_ESSERIE						0x00CC000UL
#define MEM_ICON_SERVIDOR						0x00CE000UL

#define MEM_ICON_INTERFACES						0x00D0000UL

#define MEM_ICON_IDENTIFICACION					0x00D2000UL
#define MEM_ICON_TIPOMAQUINA					0x00D4000UL
#define MEM_ICON_TOUCH							0x00D6000UL

#define MEM_ICON_ACEPTAR						0x00D8000UL
