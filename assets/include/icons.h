#pragma once
#include <stdint.h>

#if defined(__AVR__)
  #include <avr/pgmspace.h>
#else
  #ifndef PROGMEM
    #define PROGMEM
  #endif
#endif

// LOGO GRACIDA COLOR 160*41 pixeles
extern const uint8_t Logo160x41[] PROGMEM;  // el nombre debe coincidir con el del .c
extern const uint32_t Logo160x41_len;
#define LOGO_W 160
#define LOGO_H 41

//// FLECHAS DE NAVEGACIÓN
extern const uint8_t FlechaArriba[] PROGMEM;
extern const uint32_t FlechaArriba_len;
#define FLECHAARRIBA_W 64U
#define FLECHAARRIBA_H 64U

extern const uint8_t FlechaAbajo[] PROGMEM;
extern const uint32_t FlechaAbajo_len;
#define FLECHAABAJO_W 64U
#define FLECHAABAJO_H 64U

extern const uint8_t FlechaDer[] PROGMEM;
extern const uint32_t FlechaDer_len;
#define FLECHADER_W 64U
#define FLECHADER_H 64U

extern const uint8_t FlechaIzq[] PROGMEM;
extern const uint32_t FlechaIzq_len;
#define FLECHAIZQ_W 64U
#define FLECHAIZQ_H 64U

extern const uint8_t Aceptar[] PROGMEM;
extern const uint32_t Aceptar_len;
#define ACEPTAR_W 64U
#define ACEPTAR_H 64U

extern const uint8_t ResultAprend_002_V2[] PROGMEM;
extern const uint32_t ResultAprend_002_V2_len;
#define RAPREND_W 64U
#define RAPREND_H 64U


//// PANTALLA PRINCIPAL
extern const uint8_t Home_001[] PROGMEM;
extern const uint32_t Home_001_len;
#define ICON_INICIO_W 64U
#define ICON_INICIO_H 64U
//
extern const uint8_t Usuario_002[] PROGMEM;
extern const uint32_t Usuario_002_len;
#define ICON_USUARIO_W 64U
#define ICON_USUARIO_H 64U
//
extern const uint8_t Configuracion_003[] PROGMEM;
extern const uint32_t Configuracion_003_len;
#define ICON_CONFIGURACION_W 64U
#define ICON_CONFIGURACION_H 64U

///////////////////////// OPCIONES DE REGISTRO ///////////////////
extern const uint8_t AjRegistro_001[] PROGMEM;
extern const uint32_t AjRegistro_001_len;
#define ICON_AJREGISTRO_W 64U
#define ICON_AJREGISTRO_H 64U
//
extern const uint8_t FiltRegistro_002[] PROGMEM;
extern const uint32_t FiltRegistro_002_len;
#define ICON_FREGISTRO_W 64U
#define ICON_FREGISTRO_H 64U
//
extern const uint8_t BorrarReg_003_V2[] PROGMEM;
extern const uint32_t BorrarReg_003_V2_len;
#define ICON_BREGISTRO_W 64U
#define ICON_BREGISTRO_H 64U

extern const uint8_t ExpReg_004_V2[] PROGMEM;
extern const uint32_t ExpReg_004_V2_len;
#define ICON_EXPREGISTRO_W 64U
#define ICON_EXPREGISTRO_H 64U

extern const uint8_t FechaHora_005[] PROGMEM;
extern const uint32_t FechaHora_005_len;
#define ICON_FECHAHORA_W 64U
#define ICON_FECHAHORA_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES///////////////////
extern const uint8_t Producto_001_V2[] PROGMEM;
extern const uint32_t Producto_001_V2_len;
#define ICON_PRODUCTO_W 64U
#define ICON_PRODUCTO_H 64U
//
extern const uint8_t Sistema_002_V2[] PROGMEM;
extern const uint32_t Sistema_002_V2_len;
#define ICON_SISTEMA_W 64U
#define ICON_SISTEMA_H 64U
//
extern const uint8_t Perifericos_003[] PROGMEM;
extern const uint32_t Perifericos_003_len;
#define ICON_PERIFERICOS_W 64U
#define ICON_PERIFERICOS_H 64U

extern const uint8_t Entradas_004[] PROGMEM;
extern const uint32_t Entradas_004_len;
#define ICON_ENTRADAS_W 64U
#define ICON_ENTRADAS_H 64U
//
extern const uint8_t Bobina_005_V2[] PROGMEM;
extern const uint32_t Bobina_005_V2_len;
#define ICON_BOBINA_W 64U
#define ICON_BOBINA_H 64U
//
extern const uint8_t USB_006[] PROGMEM;
extern const uint32_t USB_006_len;
#define ICON_USB_W 64U
#define ICON_USB_H 64U


///////////////////////// CONFIGURACIÓN OPCIONES - PRODUCTO///////////////////
extern const uint8_t AdmProd_001_V2[] PROGMEM;
extern const uint32_t AdmProd_001_V2_len;
#define ICON_ADMPRODUCTO_W 64U
#define ICON_ADMPRODUCTO_H 64U
//
extern const uint8_t DatosProd_002_V2[] PROGMEM;
extern const uint32_t DatosProd_002_V2_len;
#define ICON_DATOSPRODUCTO_W 64U
#define ICON_DATOSPRODUCTO_H 64U
//
extern const uint8_t Aprender_003_V2[] PROGMEM;
extern const uint32_t Aprender_003_V2_len;
#define ICON_APRENDER_W 64U
#define ICON_APRENDER_H 64U

extern const uint8_t Auditcheck_004[] PROGMEM;
extern const uint32_t Auditcheck_004_len;
#define ICON_AUDITORIA_W 64U
#define ICON_AUDITORIA_H 64U

extern const uint8_t Salidas_005_V2[] PROGMEM;
extern const uint32_t Salidas_005_V2_len;
#define ICON_SALIDAS_W 64U
#define ICON_SALIDAS_H 64U

extern const uint8_t Ajustes_001_V2[] PROGMEM;
extern const uint32_t Ajustes_001_V2_len;
#define ICON_AJUSTES_W 64U
#define ICON_AJUSTES_H 64U

extern const uint8_t MSpeed_007[] PROGMEM;
extern const uint32_t MSpeed_007_len;
#define ICON_MSPEED_W 64U
#define ICON_MSPEED_H 64U

/// CONFIGURACION OPCIONES - PRODUCTO - VELOCIDAD METAL
extern const uint8_t Info_008[] PROGMEM;
extern const uint32_t Info_008_len;
#define ICON_INFO_W 64U
#define ICON_INFO_H 64U

extern const uint8_t Calcula_003[] PROGMEM;
extern const uint32_t Calcula_003_len;
#define ICON_CALCULA_W 64U
#define ICON_CALCULA_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - PRODUCTO - ADMPRODUCTOS 		///////////////////
extern const uint8_t CambProducto_001_V2[] PROGMEM;
extern const uint32_t CambProducto_001_V2_len;
#define ICON_CAMBPRODUCTO_W 64U
#define ICON_CAMBPRODUCTO_H 64U
//
extern const uint8_t CreaProducto_002_V2[] PROGMEM;
extern const uint32_t CreaProducto_002_V2_len;
#define ICON_CREARPRODUCTO_W 64U
#define ICON_CREARPRODUCTO_H 64U
//
extern const uint8_t ElimProducto_003_V2[] PROGMEM;
extern const uint32_t ElimProducto_003_V2_len;
#define ICON_ELIMPRODUCTO_W 64U
#define ICON_ELIMPRODUCTO_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - PRODUCTO - AUDITORIA 		///////////////////
extern const uint8_t CuerpTest_002_V2[] PROGMEM;
extern const uint32_t CuerpTest_002_V2_len;
#define ICON_PATRONES_W 64U
#define ICON_PATRONES_H 64U
//
extern const uint8_t TodosProd_003_V2[] PROGMEM;
extern const uint32_t TodosProd_003_V2_len;
#define ICON_TPRODUCTOS_W 64U
#define ICON_TPRODUCTOS_H 64U

extern const uint8_t IniManual_004_V2[] PROGMEM;
extern const uint32_t IniManual_004_V2_len;
#define ICON_INIMANUAL_W 64U
#define ICON_INIMANUAL_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - PRODUCTO - SALIDAS		///////////////////
extern const uint8_t MOMR_003_V2[] PROGMEM;
extern const uint32_t MOMR_003_V2_len;
#define ICON_MOMR_W 64U
#define ICON_MOMR_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA		///////////////////
extern const uint8_t AdmUsers_001_V2[] PROGMEM;
extern const uint32_t AdmUsers_001_V2_len;
#define ICON_ADMUSERS_W 64U
#define ICON_ADMUSERS_H 64U
//
extern const uint8_t SuperHardware_002_V2[] PROGMEM;
extern const uint32_t SuperHardware_002_V2_len;
#define ICON_SHARDWARE_W 64U
#define ICON_SHARDWARE_H 64U

extern const uint8_t Estadisticas_003_V2[] PROGMEM;
extern const uint32_t Estadisticas_003_V2_len;
#define ICON_ESTADISTICAS_W 64U
#define ICON_ESTADISTICAS_H 64U

extern const uint8_t Interfaces_004_V2[] PROGMEM;
extern const uint32_t Interfaces_004_V2_len;
#define ICON_INTERFACES_W 64U
#define ICON_INTERFACES_H 64U

extern const uint8_t ConfFabrica_007[] PROGMEM;
extern const uint32_t ConfFabrica_007_len;
#define ICON_CONFABRICA_W 64U
#define ICON_CONFABRICA_H 64U

extern const uint8_t Licencia_008_V2[] PROGMEM;
extern const uint32_t Licencia_008_V2_len;
#define ICON_LICENCIA_W 64U
#define ICON_LICENCIA_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA	- ADM USUARIOS	///////////////////
extern const uint8_t Editar_001_V2[] PROGMEM;
extern const uint32_t Editar_001_V2_len;
#define ICON_EDITAR_W 64U
#define ICON_EDITAR_H 64U

extern const uint8_t Crear_002_V2[] PROGMEM;
extern const uint32_t Crear_002_V2_len;
#define ICON_CREAR_W 64U
#define ICON_CREAR_H 64U

extern const uint8_t Eliminar_003_V2[] PROGMEM;
extern const uint32_t Eliminar_003_V2_len;
#define ICON_ELIMINAR_W 64U
#define ICON_ELIMINAR_H 64U

extern const uint8_t Copiar_004_V2[] PROGMEM;
extern const uint32_t Copiar_004_V2_len;
#define ICON_COPIAR_W 64U
#define ICON_COPIAR_H 64U

extern const uint8_t Login2Go_006_V2[] PROGMEM;
extern const uint32_t Login2Go_006_V2_len;
#define ICON_LOGIN2GO_W 64U
#define ICON_LOGIN2GO_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA	- SUPERVISION	///////////////////
extern const uint8_t EntradasS_001_V2[] PROGMEM;
extern const uint32_t EntradasS_001_V2_len;
#define ICON_EYS_W 64U
#define	ICON_EYS_H 64U

extern const uint8_t UnidadControl_002[] PROGMEM;
extern const uint32_t UnidadControl_002_len;
#define  ICON_UC_W 64U
#define	 ICON_UC_H 64U

extern const uint8_t Versiones_004_V2[] PROGMEM;
extern const uint32_t Versiones_004_V2_len;
#define ICON_VERSIONES_W 64U
#define ICON_VERSIONES_H 64U

extern const uint8_t Debug_005[] PROGMEM;
extern const uint32_t Debug_005_len;
#define ICON_DEBUG_W 64U
#define ICON_DEBUG_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA	- ESTADISTICAS	///////////////////
extern const uint8_t Contadores_001[] PROGMEM;
extern const uint32_t Contadores_001_len;
#define ICON_CONTADORES_W 64U
#define ICON_CONTADORES_H 64U

extern const uint8_t MetalSignal_002[] PROGMEM;
extern const uint32_t MetalSignal_002_len;
#define ICON_METALSIGNAL_W 64U
#define ICON_METALSIGNAL_H 64U

extern const uint8_t Displays_003[] PROGMEM;
extern const uint32_t Displays_003_len;
#define ICON_GRAFICAS_W 64U
#define ICON_GRAFICAS_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA	- CONF. FABRICA	///////////////////
extern const uint8_t GuardarConfig_001_V2[] PROGMEM;
extern const uint32_t GuardarConfig_001_V2_len;
#define ICON_GUARDCONFIG_W 64U
#define ICON_GUARDCONFIG_H 64U

extern const uint8_t RestConfig_002[] PROGMEM;
extern const uint32_t RestConfig_002_len;
#define ICON_RESTCONFIG_W 64U
#define ICON_RESTCONFIG_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA	- LICENCIA	///////////////////
extern const uint8_t Caracteristicas_001_V2[] PROGMEM;
extern const uint32_t Caracteristicas_001_V2_len;
#define ICON_CARACTERISTICAS_W 64U
#define ICON_CARACTERISTICAS_H 64U

extern const uint8_t Importar_002_V2[] PROGMEM;
extern const uint32_t Importar_002_V2_len;
#define ICON_IMPORTAR_W 64U
#define ICON_IMPORTAR_H 64U

extern const uint8_t Exportar_003_V2[] PROGMEM;
extern const uint32_t Exportar_003_V2_len;
#define ICON_EXPORTAR_W 64U
#define ICON_EXPORTAR_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA	- INTERFACES	///////////////////
extern const uint8_t Red_001[] PROGMEM;
extern const uint32_t Red_001_len;
#define ICON_RED_W 64U
#define ICON_RED_H 64U

extern const uint8_t ConexLAN_002[] PROGMEM;
extern const uint32_t ConexLAN_002_len;
#define ICON_CONEXLAN_W 64U
#define ICON_CONEXLAN_H 64U

extern const uint8_t Conexiones_003_V2[] PROGMEM;
extern const uint32_t Conexiones_003_V2_len;
#define ICON_CONEXIONES_W 64U
#define ICON_CONEXIONES_H 64U

extern const uint8_t Conexiones_003_V2[] PROGMEM;
extern const uint32_t Conexiones_003_V2_len;
#define ICON_CONEXIONES_W 64U
#define ICON_CONEXIONES_H 64U

extern const uint8_t EsSerie_004[] PROGMEM;
extern const uint32_t EsSerie_004_len;
#define ICON_ESSERIE_W 64U
#define ICON_ESSERIE_H 64U

extern const uint8_t Servidor_005_V2[] PROGMEM;
extern const uint32_t Servidor_005_V2_len;
#define ICON_SERVIDOR_W 64U
#define ICON_SERVIDOR_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - SISTEMA	- CONFIGURACION	///////////////////
extern const uint8_t Identificacion_001_V2[] PROGMEM;
extern const uint32_t Identificacion_001_V2_len;
#define ICON_IDENTIFICACION_W 64U
#define ICON_IDENTIFICACION_H 64U

extern const uint8_t TipoMaquina_002_V2[] PROGMEM;
extern const uint32_t TipoMaquina_002_V2_len;
#define ICON_TMAQUINA_W 64U
#define ICON_TMAQUINA_H 64U

extern const uint8_t Touch_005[] PROGMEM;
extern const uint32_t Touch_005_len;
#define ICON_TOUCH_W 64U
#define ICON_TOUCH_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - PERIFERICOS	///////////////////
extern const uint8_t Expulsion_001_V2[] PROGMEM;
extern const uint32_t Expulsion_001_V2_len;
#define ICON_EXPULSION_W 64U
#define ICON_EXPULSION_H 64U

extern const uint8_t Supervision_002[] PROGMEM;
extern const uint32_t Supervision_002_len;
#define ICON_SUPERVISION_W 64U
#define ICON_SUPERVISION_H 64U

extern const uint8_t Advertencia_003[] PROGMEM;
extern const uint32_t Advertencia_003_len;
#define ICON_WARNING_W 64U
#define ICON_WARNING_H 64U

extern const uint8_t AutomatTest_004[] PROGMEM;
extern const uint32_t AutomatTest_004_len;
#define ICON_PRUEBAAUTO_W 64U
#define ICON_PRUEBAAUTO_H 64U


///////////////////////// CONFIGURACIÓN OPCIONES - PERIFERICOS - SUPERVISION	///////////////////

extern const uint8_t Rechazo_001[] PROGMEM;
extern const uint32_t Rechazo_001_len;
#define ICON_RECHAZO_W 64U
#define ICON_RECHAZO_H 64U

extern const uint8_t SensorAire_002[] PROGMEM;
extern const uint32_t SensorAire_002_len;
#define ICON_SENSORAIRE_W 64U
#define ICON_SENSORAIRE_H 64U

extern const uint8_t Contenedor_003[] PROGMEM;
extern const uint32_t Contenedor_003_len;
#define ICON_CONTENEDOR_W 64U
#define ICON_CONTENEDOR_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - ENTRADAS	///////////////////
extern const uint8_t Externo_001[] PROGMEM;
extern const uint32_t Externo_001_len;
#define ICON_EXTERNO_W 64U
#define ICON_EXTERNO_H 64U

extern const uint8_t Digital_002_V2[] PROGMEM;
extern const uint32_t Digital_002_V2_len;
#define ICON_DIGITAL_W 64U
#define ICON_DIGITAL_H 64U

extern const uint8_t Configurable_003[] PROGMEM;
extern const uint32_t Configurable_003_len;
#define ICON_CONFIGURABLE_W 64U
#define ICON_CONFIGURABLE_H 64U

///////////////////////// CONFIGURACIÓN OPCIONES - BOBINA	///////////////////
extern const uint8_t DesvFrec_002_V2[] PROGMEM;
extern const uint32_t DesvFrec_002_V2_len;
#define ICON_DESVFREC_W 64U
#define ICON_DESVFREC_H 64U
