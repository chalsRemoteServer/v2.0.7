/*
 * view_sistema_interfaces.c
 *
 *  Created on: 5 mar. 2026
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

static void opciones_sistema_interfaces_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    // 1) Items primarios (6)
    const UiSubmPrimary items[] =
    {
        { UI_VID_OPT_SIS_INTERFACES_RED       ,       "Red"			 ,   MEM_ICON_RED},
        { UI_VID_OPT_SIS_INTERFACES_CLAN	  ,  	  "Conexion LAN" ,   MEM_ICON_CONEXLAN},
        { UI_VID_OPT_SIS_INTERFACES_CONEX	  ,       "Conexiones"   ,   MEM_ICON_CONEXIONES},
        { UI_VID_OPT_SIS_INTERFACES_ESSERIE   ,       "ES en serie"  ,   MEM_ICON_ESSERIE},
        { UI_VID_OPT_SIS_INTERFACES_SERVER    ,       "Servidor"     ,   MEM_ICON_SERVIDOR},
    };

    const uint8_t nItems = (uint8_t)(sizeof(items)/sizeof(items[0]));

    // 2) Título + área de grid (misma estrategia que registro_opts)
    const uint16_t title_x   = 20;
    const uint16_t title_y   = (uint16_t)(L->content_y0);
    const uint16_t title_h   = 34;
    const uint16_t title_gap = 20;

    const uint16_t BTN = 94;

    const uint16_t grid_x0 = title_x;
    const uint16_t grid_x1 = (uint16_t)(L->content_x1);

    const uint16_t grid_y0 = (uint16_t)(title_y + title_h + title_gap - 10);
    const uint16_t grid_y1 = (uint16_t)(L->content_y1);

    UiRect16 cards[6];

    const uint8_t drawCount = ui_subm_grid2x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, cards);

    ui_subm_draw_primary_grid2x3("Interfaces", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, 28, 8);

    const UiSubmSecondary side[] =    // 3) Botón secundario (barra derecha): X
    {
        { UI_VID_OPT_PROD_ADM_BACK, "X" },
    };

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 1);
}

static void opciones_sistema_interfaces_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_OPT_PROD_ADM_BACK:
             ui_view_pop();  // regresa a la vista anterior (normalmente main)
             break;

        case UI_VID_OPT_SIS_INTERFACES_RED:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_INTERFACES_RED);
        	 break;

        case UI_VID_OPT_SIS_INTERFACES_CLAN:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_INTERFACES_CLAN);
        	 break;

        case UI_VID_OPT_SIS_INTERFACES_CONEX:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_INTERFACES_CONEX);
             break;

        case UI_VID_OPT_SIS_INTERFACES_ESSERIE:
             ui_view_push(&VIEW_OPCIONES_SISTEMA_INTERFACES_ESSERIE);
             break;

        case UI_VID_OPT_SIS_INTERFACES_SERVER:
             ui_view_push(&VIEW_OPCIONES_SISTEMA_INTERFACES_SERVIDOR);
             break;

        default:
            break;
    }
}

const UiView VIEW_OPCIONES_SISTEMA_INTERFACES =
{
    .id = 86,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_sistema_interfaces_draw,
    .on_released = opciones_sistema_interfaces_on_released
};
