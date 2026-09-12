/*
 * g_screen_3.c
 *
 *  Created on: 17 feb. 2026
 *      Author: oscar
 */
#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"
#include "ui_pantallas.h"
#include "EVE.h"
#include "EVE_commands.h"
#include "EVE_arduino.h"
#include "ui_rects.h"
#include <stdint.h>


static void scr3_on_enter(void) { }
static void scr3_update(uint32_t now_ms)
{
	(void)now_ms;
}

static void scr3_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const UiSubmPrimary items[] =
    {
          { UI_VID_SC3_AUDITORIA, "Auditoria"    , MEM_ICON_INIMANUAL},
    };

    const uint8_t nItems = (uint8_t)(sizeof(items)/sizeof(items[0]));

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

    ui_subm_draw_primary_grid2x3("Inicio prueba Auditcheck externo", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, 28, 8);
}

const UiScreen g_screen_3 =
{
	  .on_enter = scr3_on_enter,
	  .on_exit  = 0,
	  .update   = scr3_update,
	  .draw     = scr3_draw
};
