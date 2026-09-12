/*
 * view_sistema_config.c
 *
 *  Created on: 5 mar. 2026
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

static void opciones_sistema_config_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    // 1) Items primarios (6)
    const UiSubmPrimary items[] =
    {
        { UI_VID_OPT_SISTEMA_CONFIG_ID, 			"Identificacion"    	, MEM_ICON_IDENTIFICACION	},
        { UI_VID_OPT_SISTEMA_CONFIG_TMACHINE,  		"Tipo de maquina"     	, MEM_ICON_TIPOMAQUINA  	},
        { UI_VID_OPT_SISTEMA_CONFIG_TIME,    		"Fecha y hora" 			, MEM_ICON_FECHAHORA 		},
        { UI_VID_OPT_SISTEMA_CONFIG_AJUSTES, 		"Ajustes"    			, MEM_ICON_AJUSTES			},
        { UI_VID_OPT_SISTEMA_CONFIG_CALTOUCH,   	"Calibrar touch"      	, MEM_ICON_TOUCH			},
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
    //const uint16_t grid_x1 = EVE_HSIZE;


    const uint16_t grid_y0 = (uint16_t)(title_y + title_h + title_gap - 10);
    const uint16_t grid_y1 = (uint16_t)(L->content_y1);

    UiRect16 cards[6];

    const uint8_t drawCount = ui_subm_grid2x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, cards);

    ui_subm_draw_primary_grid2x3("Configuracion", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, 28, 8);

    // 3) Botón secundario (barra derecha): X
    const UiSubmSecondary side[] =
    {
        { UI_VID_OPT_BACK, "X" },
    };

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 1);
}


static void opciones_sistema_config_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_OPT_BACK:
             ui_view_pop();  // regresa a la vista anterior (normalmente main)
             break;

        case UI_VID_OPT_SISTEMA_CONFIG_ID:
             ui_view_push(&VIEW_OPCIONES_PRODUCTO);
             break;

        case UI_VID_OPT_SISTEMA_CONFIG_TMACHINE:
             ui_view_push(&VIEW_OPCIONES_SISTEMA);
             break;

        case UI_VID_OPT_SISTEMA_CONFIG_TIME:
        	 ui_view_push(&VIEW_OPCIONES_PERIFERICOS);
        	 break;

        case UI_VID_OPT_SISTEMA_CONFIG_AJUSTES:
        	 ui_view_push(&VIEW_OPCIONES_ENTRADAS);
        	 break;

        case UI_VID_OPT_SISTEMA_CONFIG_CALTOUCH:
        	 ui_view_push(&VIEW_OPCIONES_BOBINA);
        	 break;

        default:
            break;
    }
}


const UiView VIEW_OPCIONES_SISTEMA_CONFIG =
{
    .id = 92,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_sistema_config_draw,
    .on_released = opciones_sistema_config_on_released
};

