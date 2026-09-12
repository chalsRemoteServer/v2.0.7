/*
 * view_registro_opts.c
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */

#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "colores.h"
#include "EVE.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"

#define UI_VID_ROPT_BTN0	10
#define UI_VID_ROPT_BTN1	11
#define UI_VID_ROPT_BTN2	12
#define UI_VID_ROPT_BTN3	13
#define UI_VID_ROPT_BACK	14

static void ropt_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const UiSubmPrimary items[] =
    {
        { UI_VID_ROPT_BTN0, "Ajustes de registro",  MEM_ICON_AJREGISTRO  },
        { UI_VID_ROPT_BTN1, "Filtrar registro",     MEM_ICON_FREGISTRO  },
        { UI_VID_ROPT_BTN2, "Borrar registro",      MEM_ICON_BREGISTRO   },
        { UI_VID_ROPT_BTN3, "Exportar registro",    MEM_ICON_EXPREGISTRO },
    };

    const uint16_t title_x = 20;
    const uint16_t title_y = (uint16_t)(L->content_y0);
    const uint16_t title_h = 34;     // aproximación práctica
    const uint16_t title_gap = 20;   // tu requerimiento

    const uint16_t BTN = 94;
    //const uint16_t right_gap = 10;  // separación entre grid y barra derecha

    const uint16_t grid_x0 = (uint16_t)(title_x); // o + algún margen si quieres
    const uint16_t grid_x1 = (uint16_t)(L->content_x1 + 50);

    const uint16_t grid_y0 = (uint16_t)(title_y + title_h + title_gap);
    const uint16_t grid_y1 = (uint16_t)(L->content_y1);

    const uint8_t nItems = (uint8_t)(sizeof(items)/sizeof(items[0]));

    // --- 2) Calcular layout 2x3 centrado ---
    UiRect16 cards[6];

    // “zona de trabajo” dentro del content (ajusta márgenes si quieres)

    const uint8_t drawCount = ui_subm_grid2x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, cards);

    ui_subm_draw_primary_grid2x3("Opciones de registro", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, USER_FONT_SIZE, 8);

    // --- 4) Botones secundarios (barra derecha): por ahora solo la X ---
    const UiSubmSecondary side[] =
    {
        { UI_VID_ROPT_BACK, "X" },
    };

    const uint16_t xR0  = (uint16_t)(L->content_x1);
    const uint16_t yR0  = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, /*gapY*/10, side, 1);
}

static void ropt_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_ROPT_BACK:
             ui_view_pop(); // regresa a REGISTRO
             break;

        case UI_VID_ROPT_BTN0:
             ui_view_push(&VIEW_REGISTRO_AJREG);
             break;

        case UI_VID_ROPT_BTN1:
             ui_view_push(&VIEW_REGISTRO_FREG);
             break;

        case UI_VID_ROPT_BTN2:
             ui_view_push(&VIEW_REGISTRO_BREG);
             break;

        case UI_VID_ROPT_BTN3:
             ui_view_push(&VIEW_REGISTRO_EXPREG);
             break;

        default:
            break;
    }
}

const UiView VIEW_REGISTRO_OPTS =
{
    .id = 2,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = ropt_draw,
    .on_released = ropt_on_released
};

