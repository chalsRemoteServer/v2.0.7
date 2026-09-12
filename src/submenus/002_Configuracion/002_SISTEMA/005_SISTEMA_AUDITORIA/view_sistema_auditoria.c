/*
 * view_sistema_auditoria.c
 *
 *  Created on: 25 feb. 2026
 *      Author: oscar
 */

#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"

static void opciones_sistema_auditoria_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t title_x   = 20;
    const uint16_t title_y   = (uint16_t)(L->content_y0 + 10);

    // ---- Título ----
    EVE_color_a(255);
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Ajustes auditoria");

    // ---- Texto placeholder ----
    EVE_color_rgb(BLACK);
    EVE_cmd_text(title_x + 150, title_y + 150, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "En construccion...");

    // ---- Botón X (barra derecha) ----
    const UiSubmSecondary side[] =
    {
        { UI_VID_OPT_PROD_LEAF_BACK, "X" },
    };

    const uint16_t BTN = 94;
    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 1);
}

static void opciones_sistema_auditoria_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_OPT_PROD_LEAF_BACK:
            ui_view_pop(); // regresa a VIEW_APRENDER
            break;

        default:
            break;
    }
}


const UiView VIEW_OPCIONES_SISTEMA_AUDITORIA =
{
    .id = 42,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_sistema_auditoria_draw,
    .on_released = opciones_sistema_auditoria_on_released
};
