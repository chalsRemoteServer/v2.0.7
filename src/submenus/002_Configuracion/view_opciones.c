/*
 * view_opciones.c
 *
 *  Created on: 23 feb. 2026
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

static void opciones_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const UiSubmPrimary items[] =
    {
        { UI_VID_OPT_PRODUCTO, "Producto"    , MEM_ICON_PRODUCTO},
        { UI_VID_OPT_SISTEMA,  "Sistema"     , MEM_ICON_SISTEMA  },
        { UI_VID_OPT_PERIF,    "Perifericos" , MEM_ICON_PERIFERICOS },
        { UI_VID_OPT_ENTRADAS, "Entradas"    , MEM_ICON_ENTRADAS},
        { UI_VID_OPT_BOBINA,   "Bobina"      , MEM_ICON_BOBINA},
        { UI_VID_OPT_USB,      "USB"         , MEM_ICON_USB},
    };

    const uint8_t nItems = (uint8_t)(sizeof(items)/sizeof(items[0]));

    // 2) Título + área de grid (misma estrategia que registro_opts)
    const uint16_t title_x   = 20;
    const uint16_t title_y   = (uint16_t)(L->content_y0);
    const uint16_t title_h   = 34;
    const uint16_t title_gap = 20;

    const uint16_t BTN = 94;

    const uint16_t grid_x0 = 5;
    const uint16_t grid_x1 = (uint16_t)(L->content_x1);

    const uint16_t grid_y0 = (uint16_t)(title_y + title_h + title_gap - 10);
    const uint16_t grid_y1 = (uint16_t)(L->content_y1);

    UiRect16 cards[6];

    const uint8_t drawCount = ui_subm_grid2x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, cards);

    ui_subm_draw_primary_grid2x3("Opciones", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, USER_FONT_SIZE, 8);

    const UiSubmSecondary side[] =
    {
        { UI_VID_OPT_BACK, "X" },
    };

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 1);
}


static void opciones_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_OPT_BACK:
             ui_view_pop();  // regresa a la vista anterior (normalmente main)
             break;

        case UI_VID_OPT_PRODUCTO:
             ui_view_push(&VIEW_OPCIONES_PRODUCTO);
             break;

        case UI_VID_OPT_SISTEMA:
             ui_view_push(&VIEW_OPCIONES_SISTEMA);
             break;

        case UI_VID_OPT_PERIF:
        	 ui_view_push(&VIEW_OPCIONES_PERIFERICOS);
        	 break;

        case UI_VID_OPT_ENTRADAS:
        	 ui_view_push(&VIEW_OPCIONES_ENTRADAS);
        	 break;

        case UI_VID_OPT_BOBINA:
        	 ui_view_push(&VIEW_OPCIONES_BOBINA);
        	 break;
        case UI_VID_OPT_USB:
       	     ui_view_push(&VIEW_OPCIONES_USB);
             break;

        default:
            break;
    }
}


const UiView VIEW_OPCIONES =
{
    .id = 6,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_draw,
    .on_released = opciones_on_released
};
