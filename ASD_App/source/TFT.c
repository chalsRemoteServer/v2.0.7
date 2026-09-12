#include <platform_time.h>
#include <ui_layout.h>
#include <stdio.h>
#include <ui_reloj.h>
#include "system.h"
#include "TFT.h"
#include "eve_app.h"
#include "../eve/include/EVE_commands.h"
#include "tft_data.h"
#include "../ASD_App/include/colores.h"
#include "../ASD_App/include/Posicion_Objetos.h"
#include "icons.h"
#include "assets_mem.h"
#include "ft900.h"
#include "ui_bargraph.h"
#include "ui_main_buttons.h"
#include "ui_footer.h"
#include "ui_pagers.h"
#include "../eve/include/EVE.h"
#include "../eve/include/HAL.h"
#include "ui_pantallas.h"
#include "ui_counters.h"
#include "ui_ids.h"
#include "ui_submenus.h"
#include "ui_view_mgr.h"
#include "ui_view_ids.h"
#include "ui_views.h"
#include "ui_rects.h"
#include <stdlib.h>
#include "ui_persist_productos.h"
#include "diag_dlog.h"

UiBarGraph g_bar;
UiFooter g_footer;
UiReloj g_reloj;         // si lo necesitas afuera

//const char *g_user = "Servicio";
const char *g_user = 0;

uint32_t num_dl_static = 0; /* amount of bytes in the static part of our display-list */
uint8_t tft_active = 0;
uint16_t toggle_state = 0;
uint16_t horas, minutos, segundos, mseg, display_list_size = 0;
uint16_t signal_Detection;//señal del detector
#define LAYOUT_Y1 66
static int8_t g_prod_load_rc = 0;

typedef struct {
    uint8_t  down;
    uint8_t  tag_now;
    uint8_t  pressed_id;   // id que se resalta
    uint8_t  released_id;  // evento: 1 frame
    uint32_t pressed_until_ms;

    uint16_t last_x;
    uint16_t last_y;
    uint16_t release_x;
    uint16_t release_y;
} UiTouchState;

static uint32_t now_ms(void){
    return platform_millis();}  // AJUSTA al nombre real en tu plataforma


static UiTouchState g_touch = {0};

uint16_t ui_touch_release_x(void){
    return g_touch.release_x;
}

uint16_t ui_touch_release_y(void){
    return g_touch.release_y;
}

#define TOUCH_LINGER_MS 150u
//void touch_calibrate(void);
void initStaticBackground(void);


//void touch_calibrate(void)
//{
// EVE_calibrate_write(0xffff09c0, 0x00000192, 0x03705215, 0x00000000, 0xffff43ae, 0x02700000); // COEFICIENTES PRUEBA 2
 //EVE_calibrate_write(0x0000fd55, 0xfffffc64, 0x0006f496, 0xfffffef1, 0x000106f4, 0xfff794c4);
//}

static void touch_apply_transform(void){
    EVE_memWrite32(REG_TOUCH_TRANSFORM_A, 0xffff09c0);    // tus coeficientes
    EVE_memWrite32(REG_TOUCH_TRANSFORM_B, 0x00000192);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_C, 0x03705215);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_D, 0x00000000);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_E, 0xffff43ae);
    EVE_memWrite32(REG_TOUCH_TRANSFORM_F, 0x02700000);

    // habilita touch
    EVE_memWrite8(REG_TOUCH_MODE, 3);       // continuous
    EVE_memWrite16(REG_TOUCH_RZTHRESH, 15000); // umbral razonable (ajustable)
}//fin  touch_apply_transform------------------------


static void ui_touch_update(UiTouchState *t){
    // Lee regs (fuera de burst)
    uint32_t xy  = EVE_memRead32(REG_TOUCH_SCREEN_XY);
    uint16_t rz  = EVE_memRead16(REG_TOUCH_RZ);
    uint32_t raw = EVE_memRead32(REG_TOUCH_RAW_XY);
    uint8_t  tag = EVE_memRead8(REG_TOUCH_TAG);
    uint8_t touching = (rz != 0x7FFF) && (xy != 0x80008000) && (raw != 0xFFFFFFFF);

    t->released_id = 0;
    uint32_t tnow = now_ms();
    if (touching){
        t->down = 1;
        t->tag_now = tag;
        t->last_x = (uint16_t)(xy >> 16);
        t->last_y = (uint16_t)(xy & 0xFFFF);
        if (tag != 0){
            t->pressed_id = tag;            // “hold”: mientras mantienes, pressed_id = tag actual
            t->pressed_until_ms = tnow + TOUCH_LINGER_MS;
        }}
    else{if (t->down){
            t->released_id = t->pressed_id;            // transición DOWN -> UP: evento click
            t->release_x = t->last_x;
            t->release_y = t->last_y;
            t->pressed_until_ms = tnow + TOUCH_LINGER_MS;}            // “linger”: conserva el pressed un ratito después de soltar
        t->down = 0;
        t->tag_now = 0;}        // no borres pressed_id aquí: lo borra el timeout
    if (t->pressed_id != 0 && (int32_t)(tnow - t->pressed_until_ms) > 0){    // Si ya pasó el linger, borra highlight
        t->pressed_id = 0;}
}//fin ui_touch_update --------------------------------------------------

static void ui_draw_pressed_overlay(uint8_t id)
{
    uint16_t x,y,w,h;
    if (!ui_get_rect_by_id(id, &x, &y, &w, &h)) return;

    EVE_tag_mask(0);
    EVE_tag(0);
    // Overlay (elige estilo similar al Arduino: borde o fill con alpha)
    EVE_blend_func(EVE_SRC_ALPHA, EVE_ONE_MINUS_SRC_ALPHA);
    EVE_color_a(80);                 // transparencia
    EVE_color_rgb(WHITE);      			// o usa un color tuyo (por ejemplo DARK_ORANGE con alpha)

    EVE_begin(EVE_RECTS);
    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x+w) * PRESICION, (y+h) * PRESICION);
    EVE_end();

    EVE_color_a(255);    // Regresa alpha
}

static void simulador_de_signal_detector(uint16_t *s)
{
    static int estado = 0;
    static int duracion = 0;
    static uint16_t base_pico = 0;

    if (!s) return;

    if (estado == 0)
    {
        if ((rand() % 25) == 0)
        {
            estado = 1;
            duracion = 2 + (rand() % 3);
            base_pico = 40 + (rand() % 12);
            *s = base_pico;
        }
        else
        {
            *s = 20 + (rand() % 21);
        }
    }
    else
    {
        *s = base_pico - (rand() % 20);

        duracion--;
        if (duracion <= 0)
        {
            estado = 0;
        }
    }
}

//void vStart_Pantalla_TFT(void)
//{
//	eve_setup();
//
//}//FIN DE iniciar pantalla TFT-------------

void TFT_init(void)
{
     if(E_OK == EVE_init())
     {
    	int rc_diag;

    	rc_diag = diag_dlog_init_only();
    	printf("diag_dlog_init_only rc = %d\r\n", rc_diag);

    	diag_read_flash_raw_3f000();

        tft_active = 1;horas=12,minutos=0;segundos=0;mseg=0;
        EVE_memWrite32(REG_PWM_DUTY, 0x30);  /* setup backlight, range is from 0 = off to 0x80 = max */
        EVE_memWrite8(REG_TOUCH_MODE, 3);
        //touch_calibrate();
        touch_apply_transform(); // <-- aquí, al final
        EVE_cmd_inflate(MEM_LOGO, Logo160x41, Logo160x41_len);

        EVE_cmd_inflate(MEM_ICON_HOME,                  	Home_001,                       	Home_001_len);
        EVE_cmd_inflate(MEM_ICON_USUARIO,               	Usuario_002,                    	Usuario_002_len);
        EVE_cmd_inflate(MEM_ICON_CONFIGURACION,         	Configuracion_003,              	Configuracion_003_len);

        EVE_cmd_inflate(MEM_ICON_FARRIBA,               	FlechaArriba,                   	FlechaArriba_len);
        EVE_cmd_inflate(MEM_ICON_FABAJO,                	FlechaAbajo,                    	FlechaAbajo_len);
        EVE_cmd_inflate(MEM_ICON_ACEPTAR,                	Aceptar,                    		Aceptar_len);

        EVE_cmd_inflate(MEM_ICON_RAPREND,                	ResultAprend_002_V2,                ResultAprend_002_V2_len);

        EVE_cmd_inflate(MEM_ICON_FDERECHA,               	FlechaDer,                   		FlechaDer_len);
        EVE_cmd_inflate(MEM_ICON_FIZQUIERDA,                FlechaIzq,                    		FlechaIzq_len);

        EVE_cmd_inflate(MEM_ICON_AJREGISTRO,                AjRegistro_001,                     AjRegistro_001_len);
        EVE_cmd_inflate(MEM_ICON_FREGISTRO,                 FiltRegistro_002,                   FiltRegistro_002_len);
        EVE_cmd_inflate(MEM_ICON_BREGISTRO,         		BorrarReg_003_V2,                   BorrarReg_003_V2_len);
        EVE_cmd_inflate(MEM_ICON_EXPREGISTRO,         		ExpReg_004_V2,                    	ExpReg_004_V2_len);

        EVE_cmd_inflate(MEM_ICON_FECHAHORA,         		FechaHora_005,                    	FechaHora_005_len);

        EVE_cmd_inflate(MEM_ICON_PRODUCTO,              	Producto_001_V2,                	Producto_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_SISTEMA,               	Sistema_002_V2,                 	Sistema_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_PERIFERICOS,           	Perifericos_003,                	Perifericos_003_len);

        EVE_cmd_inflate(MEM_ICON_ENTRADAS,              	Entradas_004,                		Entradas_004_len);
        EVE_cmd_inflate(MEM_ICON_BOBINA,                	Bobina_005_V2,                  	Bobina_005_V2_len);
        EVE_cmd_inflate(MEM_ICON_USB,           			USB_006,                			USB_006_len);

        EVE_cmd_inflate(MEM_ICON_ADMPROD,              		AdmProd_001_V2,                 	AdmProd_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_DATPROD,               	DatosProd_002_V2,                	DatosProd_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_APRENDER,              	Aprender_003_V2,                 	Aprender_003_V2_len);
        EVE_cmd_inflate(MEM_ICON_AUDITORIA,             	Auditcheck_004,                  	Auditcheck_004_len);
        EVE_cmd_inflate(MEM_ICON_SALIDAS,             		Salidas_005_V2,                  	Salidas_005_V2_len);
        EVE_cmd_inflate(MEM_ICON_AJUSTES,               	Ajustes_001_V2,                  	Ajustes_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_MSPEED,               		MSpeed_007,                  		MSpeed_007_len);

        EVE_cmd_inflate(MEM_ICON_CAMBPRODUCTO,              CambProducto_001_V2,               	CambProducto_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_CREAPRODUCTO,              CreaProducto_002_V2,               	CreaProducto_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_ELIMPRODUCTO,              ElimProducto_003_V2,               	ElimProducto_003_V2_len);

        EVE_cmd_inflate(MEM_ICON_PROBETAS,              	CuerpTest_002_V2,               	CuerpTest_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_TODOSPROD,              	TodosProd_003_V2,               	TodosProd_003_V2_len);
        EVE_cmd_inflate(MEM_ICON_INIMANUAL,              	IniManual_004_V2,               	IniManual_004_V2_len);

        EVE_cmd_inflate(MEM_ICON_MOMR,                      MOMR_003_V2,                       	MOMR_003_V2_len);

        EVE_cmd_inflate(MEM_ICON_ADMUSERS,              	AdmUsers_001_V2,               	 	AdmUsers_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_SHARDWARE,             	SuperHardware_002_V2,           	SuperHardware_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_ESTADISTICAS,          	Estadisticas_003_V2,            	Estadisticas_003_V2_len);
        EVE_cmd_inflate(MEM_ICON_INTERFACES,          		Interfaces_004_V2,            		Interfaces_004_V2_len);
        EVE_cmd_inflate(MEM_ICON_CONFABRICA,             	ConfFabrica_007,               		ConfFabrica_007_len);
        EVE_cmd_inflate(MEM_ICON_LICENCIA,              	Licencia_008_V2,                	Licencia_008_V2_len);

        EVE_cmd_inflate(MEM_ICON_EDITAR,                    Editar_001_V2,                      Editar_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_CREAR,                     Crear_002_V2,                       Crear_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_ELIMINAR,                  Eliminar_003_V2,                    Eliminar_003_V2_len);
        EVE_cmd_inflate(MEM_ICON_COPIAR,                    Copiar_004_V2,                      Copiar_004_V2_len);
        EVE_cmd_inflate(MEM_ICON_LOGIN2GO,                  Login2Go_006_V2,                    Login2Go_006_V2_len);

        EVE_cmd_inflate(MEM_ICON_ENTRADASYS,                EntradasS_001_V2,                   EntradasS_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_UC,                    	UnidadControl_002,                  UnidadControl_002_len);
        EVE_cmd_inflate(MEM_ICON_VERSIONES,                 Versiones_004_V2,                   Versiones_004_V2_len);
        EVE_cmd_inflate(MEM_ICON_DEBUG,                 	Debug_005,                    		Debug_005_len);

        EVE_cmd_inflate(MEM_ICON_CONTADORES,            	Contadores_001,                 	Contadores_001_len);
        EVE_cmd_inflate(MEM_ICON_METALSIGNAL,           	MetalSignal_002,                	MetalSignal_002_len);
        EVE_cmd_inflate(MEM_ICON_DISPLAYS,              	Displays_003,                   	Displays_003_len);

        EVE_cmd_inflate(MEM_ICON_GUARDCONFIG,           	GuardarConfig_001_V2,           	GuardarConfig_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_RESTCONFIG,            	RestConfig_002,                 	RestConfig_002_len);

        EVE_cmd_inflate(MEM_ICON_CARACTERISTICAS,       	Caracteristicas_001_V2,         	Caracteristicas_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_IMPORTAR,              	Importar_002_V2,                	Importar_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_EXPORTAR,              	Exportar_003_V2,                	Exportar_003_V2_len);

        EVE_cmd_inflate(MEM_ICON_SUPERVISION,           	Supervision_002,                	Supervision_002_len);
        EVE_cmd_inflate(MEM_ICON_ADVERTENCIA,           	Advertencia_003,                	Advertencia_003_len);
        EVE_cmd_inflate(MEM_ICON_PRUEBAAUTO,            	AutomatTest_004,                	AutomatTest_004_len);
        EVE_cmd_inflate(MEM_ICON_OPCEXP,            		Expulsion_001_V2,               	Expulsion_001_V2_len);

        EVE_cmd_inflate(MEM_ICON_RECHAZO,               	Rechazo_001,                   	 	Rechazo_001_len);
        EVE_cmd_inflate(MEM_ICON_SENSORAIRE,            	SensorAire_002,                 	SensorAire_002_len);
        EVE_cmd_inflate(MEM_ICON_CONTENEDOR,            	Contenedor_003,                 	Contenedor_003_len);

        EVE_cmd_inflate(MEM_ICON_EXTERNO,               	Externo_001,                    	Externo_001_len);
        EVE_cmd_inflate(MEM_ICON_DIGITAL,               	Digital_002_V2,                 	Digital_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_CONFIGURABLE,          	Configurable_003,               	Configurable_003_len);

        EVE_cmd_inflate(MEM_ICON_DESVFREC,              	DesvFrec_002_V2,                	DesvFrec_002_V2_len);

        EVE_cmd_inflate(MEM_ICON_EXPORTAR,              	Exportar_003_V2,                	Exportar_003_V2_len);

        EVE_cmd_inflate(MEM_ICON_INFO,              		Info_008,                			Info_008_len);
        EVE_cmd_inflate(MEM_ICON_CALCULA,              		Calcula_003,                	    Calcula_003_len);

        EVE_cmd_inflate(MEM_ICON_RED,                   	Red_001,                        	Red_001_len);
        EVE_cmd_inflate(MEM_ICON_CONEXLAN,              	ConexLAN_002,                   	ConexLAN_002_len);
        EVE_cmd_inflate(MEM_ICON_CONEXIONES,            	Conexiones_003_V2,              	Conexiones_003_V2_len);
        EVE_cmd_inflate(MEM_ICON_ESSERIE,              		EsSerie_004,                    	EsSerie_004_len);
        EVE_cmd_inflate(MEM_ICON_SERVIDOR,            		Servidor_005_V2,              		Servidor_005_V2_len);

        EVE_cmd_inflate(MEM_ICON_IDENTIFICACION,        	Identificacion_001_V2,          	Identificacion_001_V2_len);
        EVE_cmd_inflate(MEM_ICON_TIPOMAQUINA,           	TipoMaquina_002_V2,             	TipoMaquina_002_V2_len);
        EVE_cmd_inflate(MEM_ICON_TOUCH,           			Touch_005,             				Touch_005_len);

        EVE_memWrite8(REG_DITHER, 0x00);
        EVE_execute_cmd();

        initStaticBackground();
        //platform_time_init();//la init del timer se saco del TFT
        ui_reloj_init(&g_reloj, 0, 0, 0);

        //ui_product_store_init();
        //ui_persist_productos_load();

        ui_product_store_init();
        g_prod_load_rc = ui_persist_productos_load();

        ui_user_store_init();
        ui_bargraph_init(&g_bar, /*threshold*/100, /*tol_pct*/10);
        signal_Detection = 90;
        //ui_footer_init(&g_footer, FOOTER_ESTADO_CONEXION_CU, 2000);
        ui_footer_init_v2(&g_footer, FOOTER_ESTADO_CONEXION_CU);
        ui_screens_init();
        ui_counters_init();
        ui_view_init();

    }//------------------------------------------------------------------
}//-----------------------------------------------------------------------------------------------------


void initStaticBackground(void)
{
     EVE_cmd_dlstart(); /* Start the display list */
     EVE_tag(0); /* tag = 0 - do not use the following objects for touch-detection */
     EVE_cmd_bgcolor(WHITE); /* light grey */
     EVE_vertex_format(_FRAC_PRESICION); /* set to 0 - reduce precision for VERTEX2F to 1 pixel instead of 1/16 pixel default */

     /* display the logo */
     EVE_color_mask(1, 1, 1, 1);
     EVE_color_a(128);
     EVE_color_rgb(WHITE);

     EVE_begin(EVE_BITMAPS);  // DESCOMENTAR PARA VOLVER A LOGO "ORIGINAL"
     EVE_cmd_setbitmap(MEM_LOGO, EVE_RGB565, 160U, 41U);
     EVE_vertex2f(X_LOGO*PRESICION,Y_LOGO*PRESICION); //EVE_HSIZE - 78, 5);
     EVE_end();

     EVE_color_a(255);
     EVE_color_mask(1,1,1,1);
     EVE_scissor_xy(0,0);
     EVE_scissor_size(EVE_HSIZE, EVE_VSIZE);

     EVE_color_rgb(BLACK);
	 EVE_cmd_text(338,18,24,EVE_OPT_CENTERX,VERSION);// VERSION sprintf


     EVE_execute_cmd();
     num_dl_static = EVE_memRead16(REG_CMD_DL);
     EVE_cmd_memcpy(MEM_DL_STATIC, EVE_RAM_DL, num_dl_static);
     EVE_execute_cmd();
}//--------------------------------------------------------------

/*we are running on 8-bit without DMA, optimize some more by using the special EVE_cmd_xxx_burst() functions*/

void TFT_display(void){
static unsigned char estado91;
static uint32_t now;
static uint8_t grp;
	if (tft_active == 0U) return;
   switch(estado91){
   case 1:ui_touch_update(&g_touch);estado91++;break;    // 0) Touch (sin debug printing por frame)
   case 2:// 1) ACCIONES POR TOUCH (solo en released)
    switch (g_touch.released_id){
        case UI_ID_TOP_BTN0:ui_view_clear();ui_screens_set(0);break; // BOTÓN HOME
        case UI_ID_ARROW_RIGHT:  // FLECHA DERECHA - NAVEGA PANTALLAS
             if (!ui_view_is_active()) ui_screens_next();break;
        case UI_ID_ARROW_LEFT:   // FLECHA IZQUIERDA - NAVEGA PANTALLAS
             if (!ui_view_is_active()) ui_screens_prev();break;
        case UI_ID_TOP_BTN1: // BOTÓN USUARIO (TOP BAR)
             ui_view_push(&VIEW_USUARIO);break;
        case UI_ID_TOP_BTN2: // BOTÓN CONFIGURACIÓN (TOP BAR)
             ui_view_push(&VIEW_OPCIONES);break;
        case UI_ID_SIDE_BTN0: // BOTON 'LATERAL', PRODUCTO
             ui_view_push(&VIEW_REGISTRO);break;
        case UI_ID_SIDE_BTN1: // BOTÓN 'LATERAL', APRENDIZAJE
             ui_view_push(&VIEW_CFG_PROD_ADMIN_CAMBIO);break;
        case UI_ID_SIDE_BTN2: // BOTÓN 'LATERAL', REGISTRO/LOGBOOK
             ui_view_push(&VIEW_APRENDER);break;
        default:if (ui_view_is_active() && (g_touch.released_id != 0)){
        	              ui_view_handle_released(g_touch.released_id);}
                break;}//fin switch
        now = now_ms();    // 2) Tiempo actual (si lo ocupas)
        estado91++;break;
	case 3:// 3) UPDATES GLOBALES (SIEMPRE, aunque cambies pantalla)
		ui_reloj_update(&g_reloj);
		//ui_footer_update(&g_footer); //update parametros barra de estado simulacion
		estado91++;break;
	case 4:if(signal_Detection<100)
		   simulador_de_signal_detector(&signal_Detection);
    	   ui_bargraph_update(&g_bar,signal_Detection);
    	   signal_Detection=0;
    	   estado91++;break;
	case 5:	ui_screens_update(now);
    		ui_view_update(now);
    		estado91++;break;
	case 6:	EVE_cmd_dlstart();
    		EVE_scissor_xy(0, 0);
    		EVE_scissor_size(EVE_HSIZE, EVE_VSIZE);

			EVE_color_mask(1,1,1,1);
			EVE_color_a(255);
			EVE_blend_func(EVE_SRC_ALPHA, EVE_ONE_MINUS_SRC_ALPHA);
			EVE_stencil_op(EVE_KEEP, EVE_KEEP);
			EVE_stencil_func(EVE_ALWAYS, 0, 255);
			EVE_alpha_func(EVE_ALWAYS, 0);

			EVE_clear_color_rgb(WHITE);
			EVE_clear(1, 1, 1);
			EVE_tag(0);

			EVE_cmd_append(MEM_DL_STATIC, num_dl_static);
			ui_rects_begin_frame();
			estado91++;break;
	case 7: grp = UI_GRP_TOPBAR;
    		if (!ui_view_is_active()) {grp |= UI_GRP_ARROWS;}/* dibuja arrows */

			ui_main_buttons_draw_groups(&UI_LAYOUT_MAIN, grp);
			//ui_reloj_draw(&g_reloj, &UI_LAYOUT_MAIN, g_user);    // reloj + footer siempre visibles
			ui_reloj_draw(&g_reloj, &UI_LAYOUT_MAIN, ui_user_get_active_name());
			ui_footer_draw(&g_footer, &UI_LAYOUT_MAIN);//dibujar barra de estado
			estado91++;break;
	case 8:// 7) CONTENIDO de pantalla activa
			if (ui_view_is_active()){ui_view_draw();}
			else{ui_screens_draw();}
			if (!ui_view_is_active()) {
				ui_pagers_draw(&UI_LAYOUT_MAIN, ui_screens_get(), ui_screens_count());}/* dibuja pagers */
			estado91++;break;
	case 9:// 9) Overlay pressed
			if (g_touch.pressed_id != 0) {
					ui_draw_pressed_overlay(g_touch.pressed_id);}
			EVE_display();
			EVE_cmd_swap();
			EVE_execute_cmd();
			estado91++;break;
	case 10:now=now_ms();estado91++;break;
	case 11:if(now_ms()>(now+5)){estado91=1;}break;
	default:estado91=1;break;}//finswitch

}//findisplay---------------------------------------------------------------------

/* Regresa el comando que se va enviar si hay uno, requerido
 retunr False si no hay comunicacion a enviar */
uint8_t ui_monitor_graficadores(void){
	if(!ui_barGraph_status(&g_bar,is_GRAPH_OK)){
	   return(ui_barGraph_status(&g_bar,FIND_ERROR));}//algo falta de corregir
return 0;
}//fin de monitor de variable monitor de estado de menu graficacion

