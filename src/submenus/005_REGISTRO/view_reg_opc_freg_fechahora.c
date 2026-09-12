/*
 * view_reg_opc_freg_fechahora.c
 *
 *  Created on: 10 mar. 2026
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

static UiLeafRow s_rows[] =
{
    //{ UI_VID_FECHAHORA_ROW0, 		"Intervalo"			,	 UI_LEAF_FIELD_BOOL, 0, 1 },
    { UI_VID_FECHAHORA_ROW0, 		"Intervalo"			,	 UI_LEAF_FIELD_BOOL, 0, 1, 0, 0, 0 },
};

static const uint8_t s_rowsN = (uint8_t)(sizeof(s_rows) / sizeof(s_rows[0]));

/* - Ciclo de vida de la view ---------------------- */

static void registro_fechahora_on_enter(void)
{
    /* 4 filas visibles como definimos para el esqueleto */
    ui_leaf_state_reset(&s_leaf, 4);
}

static void registro_fechahora_on_exit(void)
{
    /* Por ahora nada */
}

static void registro_fechahora_update(uint32_t now_ms)
{
    (void)now_ms;
    /* Por ahora nada */
}

static void registro_fechahora_draw(void)
{
    UiLeafLayout lay;
    ui_leaf_layout_default(&lay, &UI_LAYOUT_MAIN);

    lay.check_w = 180;
    lay.check_h = 60;

    lay.row_h = 60;

    ui_leaf_draw_bool_form("Fecha y hora", s_rows, s_rowsN, &s_leaf, &lay, UI_VID_RAJREG_BACK, UI_VID_RAJREG_ACCEPT, UI_VID_RAJREG_UP, UI_VID_RAJREG_DOWN);
}


static void registro_fechahora_on_released(uint16_t id)
{
    /* 1) Si se tocó una fila, toggle directo */
    if (id >= UI_VID_FECHAHORA_ROW0 && id < (uint16_t)(UI_VID_FECHAHORA_ROW0 + s_rowsN))
    {
        uint8_t idx = (uint8_t)(id - UI_VID_FECHAHORA_ROW0);

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

const UiView VIEW_REGISTRO_FECHAHORA =
{
    .id = 94,
	.on_enter = registro_fechahora_on_enter,
	.on_exit = registro_fechahora_on_exit,
	.update = registro_fechahora_update,
    .draw = registro_fechahora_draw,
    .on_released = registro_fechahora_on_released
};

