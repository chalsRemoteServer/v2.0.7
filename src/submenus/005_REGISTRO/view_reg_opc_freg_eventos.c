/*
 * view_reg_opc_freg_eventos.c
 *
 *  Created on: 9 mar. 2026
 *      Author: oscar
 */

/*
 * view_reg_opc_freg_eventos.c
 *
 *  Created on: 9 mar. 2026
 *      Author: oscar
 */

#include "ui_leaf_form.h"
#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"
#include <stdint.h>


static UiLeafState s_leaf;

/* Filas piloto:
 * puedes cambiar estos textos después por los reales del equipo.
 */
static UiLeafRow s_rows[] =
{
//    { UI_VID_FILTREG_ROW0, 		"Error"			,	 UI_LEAF_FIELD_BOOL, 0, 1 },
//    { UI_VID_FILTREG_ROW1, 		"Advertencia"	,    UI_LEAF_FIELD_BOOL, 1, 1 },
//    { UI_VID_FILTREG_ROW2, 		"Informacion"	,    UI_LEAF_FIELD_BOOL, 0, 1 },
//    { UI_VID_FILTREG_ROW3, 		"Metal"			,    UI_LEAF_FIELD_BOOL, 0, 1 },
//    { UI_VID_FILTREG_ROW4, 		"Auditoria"		,    UI_LEAF_FIELD_BOOL, 0, 1 },

	  { UI_VID_FILTREG_ROW0, 		"Error"			,	 UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
	  { UI_VID_FILTREG_ROW1, 		"Advertencia"	,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
	  { UI_VID_FILTREG_ROW2, 		"Informacion"	,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
	  { UI_VID_FILTREG_ROW3, 		"Metal"			,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
      { UI_VID_FILTREG_ROW4, 		"Auditoria"		,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
};

static const uint8_t s_rowsN = (uint8_t)(sizeof(s_rows) / sizeof(s_rows[0]));

/* - Ciclo de vida de la view ---------------------- */

static void registro_filtreg_on_enter(void)
{
    /* 4 filas visibles como definimos para el esqueleto */
    ui_leaf_state_reset(&s_leaf, 4);
}

static void registro_filtreg_on_exit(void)
{
    /* Por ahora nada */
}

static void registro_filtreg_update(uint32_t now_ms)
{
    (void)now_ms;
    /* Por ahora nada */
}

static void registro_filtreg_draw(void)
{
    UiLeafLayout lay;
    ui_leaf_layout_default(&lay, &UI_LAYOUT_MAIN);

    ui_leaf_draw_bool_form("Eventos", s_rows, s_rowsN, &s_leaf, &lay,
                           UI_VID_RAJREG_BACK, UI_VID_RAJREG_ACCEPT, UI_VID_RAJREG_UP, UI_VID_RAJREG_DOWN);
}

//ui_leaf_draw_bool_form("Ajustes de registro", s_rows, s_rowsN, &s_leaf, &lay,
//                       UI_VID_RAJREG_BACK, UI_VID_RAJREG_ACCEPT, UI_VID_RAJREG_UP, UI_VID_RAJREG_DOWN);

static void registro_filtreg_on_released(uint16_t id)
{
    /* 1) Si se tocó una fila, toggle directo */
    if (id >= UI_VID_FILTREG_ROW0 && id < (uint16_t)(UI_VID_FILTREG_ROW0 + s_rowsN))
    {
        uint8_t idx = (uint8_t)(id - UI_VID_FILTREG_ROW0);

        if (idx < s_rowsN && s_rows[idx].enabled)
        {
            s_rows[idx].checked ^= 1u;
        }
        return;
    }

    /* 2) Botones laterales */
    switch (id)
    {
        case UI_VID_RAJREG_BACK:
            ui_view_pop();
            break;

        case UI_VID_RAJREG_ACCEPT:
            /* Por ahora comportamiento piloto:
             * aceptar = regresar a la vista anterior.
             *
             * Más adelante aquí conectas:
             * - guardado real
             * - copia a estructura global
             * - persistencia en flash / config / etc.
             */
            ui_view_pop();
            break;

        case UI_VID_RAJREG_UP:
            ui_leaf_scroll_up(&s_leaf, s_rowsN);
            break;

        case UI_VID_RAJREG_DOWN:
            ui_leaf_scroll_down(&s_leaf, s_rowsN);
            break;

        default:
            break;
    }
}

const UiView VIEW_REGISTRO_FILTREG =
{
    .id = 93,
	.on_enter = registro_filtreg_on_enter,
	.on_exit = registro_filtreg_on_exit,
	.update = registro_filtreg_update,
    .draw = registro_filtreg_draw,
    .on_released = registro_filtreg_on_released
};



//#include "ui_leaf_form.h"
//#include "ui_submenu_widgets.h"
//#include "ui_layout.h"
//#include "ui_view_ids.h"
//#include "ui_view_mgr.h"
//#include "EVE.h"
//#include "colores.h"
//#include "Posicion_Objetos.h"
//#include "assets_mem.h"
//#include "ui_views.h"
//#include <stdint.h>
//
//
//static UiLeafState s_leaf;
//
//static UiLeafRow s_rows[] =
//{
//    { UI_VID_RAJREG_ROW0, "Entrada de metal"				,	 UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0  },
//    { UI_VID_RAJREG_ROW1, "Entrada cambio de producto"		,    UI_LEAF_FIELD_BOOL, 1, 1, 0, 0, 0  },
//    { UI_VID_RAJREG_ROW2, "Entrada de bypass"				,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0  },
//    { UI_VID_RAJREG_ROW3, "Metal durante bypass"			,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0  },
//    { UI_VID_RAJREG_ROW4, "USB logging"						,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0  },
//    { UI_VID_RAJREG_ROW5, "Advertencia Registro lleno"		,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0  },
//    { UI_VID_RAJREG_ROW6, "Mas cambios"						,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0  },
//    { UI_VID_RAJREG_ROW7, "Mas cambios 2"					,  	 UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0  },
//};
//
//static const uint8_t s_rowsN = (uint8_t)(sizeof(s_rows) / sizeof(s_rows[0]));
//
///* - Ciclo de vida de la view ---------------------- */
//
//static void registro_ajreg_on_enter(void)
//{
//    /* 4 filas visibles como definimos para el esqueleto */
//    ui_leaf_state_reset(&s_leaf, 4);
//}
//
//static void registro_ajreg_on_exit(void)
//{
//    /* Por ahora nada */
//}
//
//static void registro_ajreg_update(uint32_t now_ms)
//{
//    (void)now_ms;
//    /* Por ahora nada */
//}
//
//static void registro_ajreg_draw(void)
//{
//
//
//    UiLeafLayout lay;
//    ui_leaf_layout_default(&lay, &UI_LAYOUT_MAIN);
//
//    EVE_scissor_xy(0, 0);
//    EVE_scissor_size(800, 480);
//
//    EVE_color_rgb(BLACK);
//	EVE_cmd_text_bold(50, 140, 28, 0, "ENTRE A EVENTOS");
//
//    ui_leaf_draw_bool_form("Ajustes de registro", s_rows, s_rowsN, &s_leaf, &lay,
//                           UI_VID_RAJREG_BACK, UI_VID_RAJREG_ACCEPT, UI_VID_RAJREG_UP, UI_VID_RAJREG_DOWN);
//}
//
//
//static void registro_ajreg_on_released(uint16_t id)
//{
//    /* 1) Si se tocó una fila, toggle directo */
//    if (id >= UI_VID_RAJREG_ROW0 && id < (uint16_t)(UI_VID_RAJREG_ROW0 + s_rowsN))
//    {
//        uint8_t idx = (uint8_t)(id - UI_VID_RAJREG_ROW0);
//
//        if (idx < s_rowsN && s_rows[idx].enabled)
//        {
//            s_rows[idx].checked ^= 1u;
//        }
//        return;
//    }
//
//    /* 2) Botones laterales */
//    switch (id)
//    {
//        case UI_VID_RAJREG_BACK:
//            ui_view_pop();
//            break;
//
//        case UI_VID_RAJREG_ACCEPT:
//            /* Por ahora comportamiento piloto:
//             * aceptar = regresar a la vista anterior.
//             *
//             * Más adelante aquí conectas:
//             * - guardado real
//             * - copia a estructura global
//             * - persistencia en flash / config / etc.
//             */
//            ui_view_pop();
//            break;
//
//        case UI_VID_RAJREG_UP:
//            ui_leaf_scroll_up(&s_leaf, s_rowsN);
//            break;
//
//        case UI_VID_RAJREG_DOWN:
//            ui_leaf_scroll_down(&s_leaf, s_rowsN);
//            break;
//
//        default:
//            break;
//    }
//}
//
//const UiView VIEW_REGISTRO_FILTREG =
//{
//    .id = 95,
//	.on_enter = registro_ajreg_on_enter,
//	.on_exit = registro_ajreg_on_exit,
//	.update = registro_ajreg_update,
//    .draw = registro_ajreg_draw,
//    .on_released = registro_ajreg_on_released
//};


//#include "ui_leaf_form.h"
//#include "ui_submenu_widgets.h"
//#include "ui_layout.h"
//#include "ui_view_ids.h"
//#include "ui_view_mgr.h"
//#include "EVE.h"
//#include "colores.h"
//#include "Posicion_Objetos.h"
//#include "assets_mem.h"
//#include "ui_views.h"
//#include <stdint.h>
//
//
//static UiLeafState s_leaf;
//
//static UiLeafRow s_rows[] =
//{
//	  { UI_VID_FILTREG_ROW0, 		"Error"			,	 UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
//	  { UI_VID_FILTREG_ROW1, 		"Advertencia"	,    UI_LEAF_FIELD_BOOL, 1, 1, 0, 0, 0 },
//	  { UI_VID_FILTREG_ROW2, 		"Informacion"	,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
//	  { UI_VID_FILTREG_ROW3, 		"Metal"			,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
//      { UI_VID_FILTREG_ROW4, 		"Auditoria"		,    UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
//};
//
//static const uint8_t s_rowsN = (uint8_t)(sizeof(s_rows) / sizeof(s_rows[0]));
//
///* - Ciclo de vida de la view ---------------------- */
//
//static void registro_filtreg_on_enter(void)
//{
//    /* 4 filas visibles como definimos para el esqueleto */
//    ui_leaf_state_reset(&s_leaf, 4);
//}
//
//static void registro_filtreg_on_exit(void)
//{
//    /* Por ahora nada */
//}
//
//static void registro_filtreg_update(uint32_t now_ms)
//{
//    (void)now_ms;
//    /* Por ahora nada */
//}
//
//static void registro_filtreg_draw(void)
//{
//    UiLeafLayout lay;
//    ui_leaf_layout_default(&lay, &UI_LAYOUT_MAIN);
//    ui_leaf_draw_bool_form("Eventos", s_rows, s_rowsN, &s_leaf, &lay, UI_VID_RAJREG_BACK, UI_VID_RAJREG_ACCEPT, UI_VID_RAJREG_UP, UI_VID_RAJREG_DOWN);
//}
//
//static void registro_filtreg_on_released(uint16_t id)
//{
//    /* 1) Si se tocó una fila, toggle directo */
//    if (id >= UI_VID_FILTREG_ROW0 && id < (uint16_t)(UI_VID_FILTREG_ROW0 + s_rowsN))
//    {
//        uint8_t idx = (uint8_t)(id - UI_VID_FILTREG_ROW0);
//
//        if (idx < s_rowsN && s_rows[idx].enabled)
//        {
//            s_rows[idx].checked ^= 1u;
//        }
//        return;
//    }
//
//    /* 2) Botones laterales */
//    switch (id)
//    {
//        case UI_VID_RAJREG_BACK:
//            ui_view_pop();
//            break;
//
//        case UI_VID_RAJREG_ACCEPT:
//            ui_view_pop();
//            break;
//
//        case UI_VID_RAJREG_UP:
//            ui_leaf_scroll_up(&s_leaf, s_rowsN);
//            break;
//
//        case UI_VID_RAJREG_DOWN:
//            ui_leaf_scroll_down(&s_leaf, s_rowsN);
//            break;
//
//        default:
//            break;
//    }
//}
//
//const UiView VIEW_REGISTRO_FILTREG =
//{
//    .id = 93,
//	.on_enter = registro_filtreg_on_enter,
//	.on_exit = registro_filtreg_on_exit,
//	.update = registro_filtreg_update,
//    .draw = registro_filtreg_draw,
//    .on_released = registro_filtreg_on_released
//};
