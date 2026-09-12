/*
 * view_cfg_producto_admin.c
 *
 *  Created on: 24 feb. 2026
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

static void opciones_producto_adm_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    // 1) Items primarios (6)
    const UiSubmPrimary items[] =
    {
        { UI_VID_OPT_PROD_ADM_CAMBIO,      "Cambio producto",   MEM_ICON_CAMBPRODUCTO},
        { UI_VID_OPT_PROD_ADM_CREAR,  	   "Crear producto" ,   MEM_ICON_CREAPRODUCTO},
        { UI_VID_OPT_PROD_ADM_ELIMINAR,    "Eliminar producto", MEM_ICON_ELIMPRODUCTO},
    };

    const uint8_t nItems = (uint8_t)(sizeof(items)/sizeof(items[0]));

    // 2) Título + área de grid (misma estrategia que registro_opts)
    const uint16_t title_x   = 20;
    const uint16_t title_y   = (uint16_t)(L->content_y0 + 10);
    const uint16_t title_h   = 34;
    const uint16_t title_gap = 20;

    const uint16_t BTN = 104;

    const uint16_t grid_x0 = title_x;
    const uint16_t grid_x1 = (uint16_t)(L->content_x1 + 50);

    const uint16_t grid_y0 = (uint16_t)(title_y + title_h + title_gap);
    const uint16_t grid_y1 = (uint16_t)(L->content_y1);

    UiRect16 cards[6];

    const uint8_t drawCount = ui_subm_grid2x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, cards);

    ui_subm_draw_primary_grid2x3("Administracion de productos", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, USER_FONT_SIZE, 8);

    // 3) Botón secundario (barra derecha): X
    const UiSubmSecondary side[] =
    {
        { UI_VID_OPT_PROD_ADM_BACK, "X" },
    };

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);
    const uint16_t BTN2 = 94;

    ui_subm_draw_side_buttons(xR0, yR0, BTN2, BTN2, 10, side, 1);
}

static void opciones_producto_admin_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_OPT_PROD_ADM_BACK:
            ui_view_pop();  // regresa a la vista anterior (normalmente main)
            break;

        case UI_VID_OPT_PROD_ADM_CAMBIO:
        	ui_view_push(&VIEW_CFG_PROD_ADMIN_CAMBIO);
        	break;

        case UI_VID_OPT_PROD_ADM_CREAR:
        	ui_view_push(&VIEW_CFG_PROD_ADMIN_CREAR);
        	break;

        case UI_VID_OPT_PROD_ADM_ELIMINAR:
        	ui_view_push(&VIEW_CFG_PROD_ADMIN_ELIMINAR);

            break;

        default:
            break;
    }
}


const UiView VIEW_OPCIONES_PRODUCTO_ADMIN =
{
    .id = 8,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_producto_adm_draw,
    .on_released = opciones_producto_admin_on_released
};
