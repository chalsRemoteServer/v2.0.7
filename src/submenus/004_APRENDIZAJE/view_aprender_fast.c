/*
 * view_aprender_fast.c
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

#define UI_VID_APREND_APRENDFAST_CANCEL    10
#define UI_VID_APREND_APRENDFAST_ACCEPT	   11
#define UI_VID_APREND_APRENDFAST_BACK      12

static void aprender_fast_draw(void)
{

	const UiMainLayout* L = &UI_LAYOUT_MAIN;

	const uint16_t BTN_W = 94;
	const uint16_t BTN_H = 94;
	const uint16_t BTN_GAP = 5;

	const uint16_t box_w = 695;
	const uint16_t box_h = 270;
	const uint16_t box_x = (uint16_t)(L->footer_x0);

	const uint16_t box_y = (uint16_t)(L->content_y0 + 50);

	const uint16_t xR0 = (uint16_t)(L->content_x1);
	const uint16_t yR0 = (uint16_t)(L->content_y0);

	const UiSubmSecondary side[] =
	{
			{ UI_VID_APREND_APRENDFAST_CANCEL, "X"                    },
			{ UI_VID_APREND_APRENDFAST_ACCEPT, "", MEM_ICON_INIMANUAL },
	};

	EVE_color_rgb(BLACK);
	EVE_cmd_text_bold(20, (int16_t)(L->content_y0 + 10), PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Aprendizaje rapido");

	EVE_color_rgb(BLUE);
	EVE_begin(EVE_RECTS);
	EVE_vertex2f(box_x * PRESICION, box_y * PRESICION);
	EVE_vertex2f((box_x + box_w) * PRESICION, (box_y + box_h) * PRESICION);
	EVE_end();

	EVE_color_rgb(WHITE);
	EVE_cmd_text(20, 240, 25, EVE_OPT_FLAT, "Preparar productos");
	EVE_cmd_text(20, 290, 25, EVE_OPT_FLAT, "e iniciar aprendizaje");

	ui_subm_draw_side_buttons(xR0, yR0, BTN_W, BTN_H, BTN_GAP, side, 2);

}

static void aprender_fast_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_APREND_APRENDFAST_CANCEL:
             ui_view_pop(); // regresa a VIEW_APRENDER
             break;

        default:
            break;
    }
}

const UiView VIEW_APRENDER_FAST =
{
    .id = 4,
    .on_enter = 0,
    .on_exit  = 0,
    .update   = 0,
    .draw     = aprender_fast_draw,
    .on_released = aprender_fast_on_released
};
