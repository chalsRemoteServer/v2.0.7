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
#include "ui_almacena_productos.h"
#include <stdio.h>
#include <string.h>

static UiLeafState s_leaf;
static char s_val_nombre[21];
static char s_val_numero[12];
static char s_val_sensibilidad[16];
static char s_val_angulo[16];
static char s_val_frecuencia[16];
static char s_val_velocidad[16];

/* Filas piloto: puedes cambiar estos textos después por los reales del equipo. */
static UiLeafRow s_rows[] =
{
      { UI_VID_PRODUCTO_DATOSP_ROW0, "Nombre del producto",       UI_LEAF_FIELD_TEXT, 0, 1, s_val_nombre,       20, "" },
	  { UI_VID_PRODUCTO_DATOSP_ROW1, "Numero del producto",       UI_LEAF_FIELD_TEXT, 0, 1, s_val_numero,       10, "" },
      { UI_VID_PRODUCTO_DATOSP_ROW2, "Sensibilidad",              UI_LEAF_FIELD_TEXT, 0, 1, s_val_sensibilidad, 15, "" },
	  { UI_VID_PRODUCTO_DATOSP_ROW3, "Angulo",                    UI_LEAF_FIELD_TEXT, 0, 1, s_val_angulo,       15, "" },
	  { UI_VID_PRODUCTO_DATOSP_ROW4, "Frecuencia",                UI_LEAF_FIELD_TEXT, 0, 1, s_val_frecuencia,   15, "" },
	  { UI_VID_PRODUCTO_DATOSP_ROW5, "Velocidad de transporte",   UI_LEAF_FIELD_TEXT, 0, 1, s_val_velocidad,    15, "" },

};

static void producto_datosp_refresh_values(void)
{
    const UiProductRecord* p = ui_product_store_get_active();

    if (p && p->used && p->name[0] != '\0')
    {
        strncpy(s_val_nombre, p->name, sizeof(s_val_nombre) - 1u);
        s_val_nombre[sizeof(s_val_nombre) - 1u] = '\0';

        snprintf(s_val_numero, sizeof(s_val_numero), "%u", p->id);
        strcpy(s_val_sensibilidad, "100 %");
        strcpy(s_val_angulo, "138");
        strcpy(s_val_frecuencia, "600 Khz");
        strcpy(s_val_velocidad, "0.33 m/s");
    }
    else
    {
        strcpy(s_val_nombre, "sin productos");
        strcpy(s_val_numero, "-");
        strcpy(s_val_sensibilidad, "-");
        strcpy(s_val_angulo, "-");
        strcpy(s_val_frecuencia, "-");
        strcpy(s_val_velocidad, "-");
    }

}

static const uint8_t s_rowsN = (uint8_t)(sizeof(s_rows) / sizeof(s_rows[0]));

/* - Ciclo de vida de la view ---------------------- */

static void opciones_producto_datosp_on_enter(void)
{
    ui_leaf_state_reset(&s_leaf, 4);
    producto_datosp_refresh_values();

}

static void opciones_producto_datosp_on_exit(void)
{
    /* Por ahora nada */
}

static void opciones_producto_datosp_update(uint32_t now_ms)
{
    (void)now_ms;    /* Por ahora nada */
}



static void opciones_producto_datosp_draw(void)
{
    UiLeafLayout lay;
    ui_leaf_layout_default(&lay, &UI_LAYOUT_MAIN);

    /* Ajustes visuales específicos para "Datos del producto" */
    lay.text_box_w = 250;                           /* ancho fijo */
    lay.text_box_h = (uint16_t)(lay.row_h - 4);    /* 4 px menos que la fila */
    lay.text_box_margin_right = 12;                /* pequeño margen al borde derecho */
    lay.text_value_pad_x = 10;
    lay.text_value_pad_y = 14;                     /* ajustar si quieres el texto un poco más arriba/abajo */

    ui_leaf_draw_form("Datos del producto", s_rows, s_rowsN, &s_leaf, &lay, UI_VID_RAJREG_BACK, UI_VID_RAJREG_ACCEPT, UI_VID_RAJREG_UP, UI_VID_RAJREG_DOWN);
}

static void opciones_producto_datosp_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_RAJREG_BACK:
            ui_view_pop();
            break;

        case UI_VID_RAJREG_ACCEPT:
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

//static void opciones_producto_datosp_on_released(uint16_t id)
//{
//    /* 1) Si se tocó una fila, toggle directo */
//    if (id >= UI_VID_PRODUCTO_DATOSP_ROW0 && id < (uint16_t)(UI_VID_PRODUCTO_DATOSP_ROW0 + s_rowsN))
//    {
//        uint8_t idx = (uint8_t)(id - UI_VID_PRODUCTO_DATOSP_ROW0);
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

const UiView VIEW_OPCIONES_PRODUCTO_DATOSP =
{
    .id = 12,
	.on_enter = opciones_producto_datosp_on_enter,
	.on_exit = opciones_producto_datosp_on_exit,
	.update = opciones_producto_datosp_update,
    .draw = opciones_producto_datosp_draw,
    .on_released = opciones_producto_datosp_on_released
};
