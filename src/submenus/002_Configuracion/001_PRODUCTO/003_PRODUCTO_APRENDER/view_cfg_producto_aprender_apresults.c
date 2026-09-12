
#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"

#define UI_VID_PROD_RAPREND_CANCEL		10
#define UI_VID_PROD_RAPREND_ACEPTA		11
#define UI_VID_PROD_RAPREND_FUP			12
#define UI_VID_PROD_RAPREND_FDOWN		13

static void opciones_producto_aprender_apresultados_draw(void)
{
	const UiMainLayout* L = &UI_LAYOUT_MAIN;

		const uint16_t BTN_W = 84;
		const uint16_t BTN_H = 84;
		const uint16_t BTN_GAP = 3;

		const uint16_t box_w = 695;
		const uint16_t box_h = 270;
		const uint16_t box_x = (uint16_t)(L->footer_x0);

		const uint16_t box_y = (uint16_t)(L->content_y0 + 50);

	    const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
	    const uint16_t yR0 = (uint16_t)(L->content_y0);

		const UiSubmSecondary side[] =
		{
				{ UI_VID_PROD_RAPREND_CANCEL, "X"                    	},
				{ UI_VID_PROD_RAPREND_ACEPTA, ""	, MEM_ICON_ACEPTAR 	},
				{ UI_VID_PROD_RAPREND_FUP, ""		, MEM_ICON_FARRIBA 	},
				{ UI_VID_PROD_RAPREND_FDOWN, ""		, MEM_ICON_FABAJO 	},
		};

		EVE_color_rgb(BLACK);
		EVE_cmd_text_bold(20, (int16_t)(L->content_y0 + 10), PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Resultados aprendizaje");

		EVE_color_rgb(BLUE);
		EVE_begin(EVE_RECTS);
		EVE_vertex2f(box_x * PRESICION, box_y * PRESICION);
		EVE_vertex2f((box_x + box_w) * PRESICION, (box_y + box_h) * PRESICION);
		EVE_end();

		EVE_color_rgb(WHITE);
		EVE_cmd_text(20,  220, 24, EVE_OPT_FLAT, "Frecuencia");
		EVE_cmd_text(180, 220, 24, EVE_OPT_FLAT, "Sensibilidad");
		EVE_cmd_text(360, 220, 24, EVE_OPT_FLAT, "Angulo");

		ui_subm_draw_side_buttons(xR0, yR0, BTN_W, BTN_H, BTN_GAP, side, 4);
}

static void opciones_producto_aprender_apresultados_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_PROD_RAPREND_CANCEL:
            ui_view_pop(); // regresa a VIEW_APRENDER
            break;

        default:
            break;
    }
}


const UiView VIEW_CFG_PROD_APRENDER_APRESULTADOS =
{
    .id = 15,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_producto_aprender_apresultados_draw,
    .on_released = opciones_producto_aprender_apresultados_on_released
};
