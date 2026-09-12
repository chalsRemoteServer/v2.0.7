/*
 * view_opciones_entradas_conf_ent3.c
 *
 *  Created on: 2 mar. 2026
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

static void opciones_entradas_config_ent3_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t title_x   = 20;
    const uint16_t title_y   = (uint16_t)(L->content_y0 + 10);

    // ---- Título ----
    EVE_color_a(255);
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Entrada 3");

    // ---- Texto placeholder ----
    EVE_color_rgb(BLACK);
    EVE_cmd_text(title_x + 150, title_y + 150, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "En proceso ...");

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

static void opciones_entradas_config_ent3_on_released(uint16_t id)
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


const UiView VIEW_OPCIONES_ENTRADAS_CONFIG_ENT3 =
{
    .id = 66,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_entradas_config_ent3_draw,
    .on_released = opciones_entradas_config_ent3_on_released
};
