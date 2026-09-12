/*
 * view_aprender.c
 *
 *  Created on: 23 feb. 2026
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

static void aprender_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    // 1) Items primarios (2)
    const UiSubmPrimary items[] =
    {
        { UI_VID_APR_FAST,    "Aprendizaje rapido", MEM_ICON_APRENDER},
        { UI_VID_APR_RESULTS, "Resultados"    , MEM_ICON_RAPREND},
    };
    const uint8_t nItems = (uint8_t)(sizeof(items)/sizeof(items[0]));

    // 2) Geometría base (mismo criterio que usas en registro_opts)
    const uint16_t title_x = 20;
    const uint16_t title_y = (uint16_t)(L->content_y0 + 10);
    const uint16_t title_h = 34;
    const uint16_t title_gap = 20;

    const uint16_t BTN = 94;

    const uint16_t grid_x0 = title_x;
    const uint16_t grid_x1 = (uint16_t)(L->content_x1 + 50);

    const uint16_t grid_y0 = (uint16_t)(title_y + title_h + title_gap);
    const uint16_t grid_y1 = (uint16_t)(L->content_y1);

    UiRect16 cards[6];

    // 3) Layout dinámico (te centra 1..6 items en 2x3)
    const uint8_t drawCount = ui_subm_grid2x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, cards);

    // 4) Dibujo grid + título
    ui_subm_draw_primary_grid2x3("Aprender", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, USER_FONT_SIZE, 8);

    // 5) Botón secundario (X) en barra derecha
    const UiSubmSecondary side[] =
    {
        { UI_VID_APR_BACK, "X" },
    };

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 1);
}


static void aprender_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_APR_BACK:
            ui_view_pop();   // regresa a MAIN
            break;

        case UI_VID_APR_FAST:
            ui_view_push(&VIEW_APRENDER_FAST);
            break;

        case UI_VID_APR_RESULTS:
            ui_view_push(&VIEW_APRENDER_RESULTS);
            break;

        default:
            break;
    }
}

const UiView VIEW_APRENDER =
{
    .id = 3,              // ID interno del view (no tag). Evita chocar con 1=REG, 2=ROPT
    .on_enter = 0,
    .on_exit  = 0,
    .update   = 0,
    .draw     = aprender_draw,
    .on_released = aprender_on_released
};
