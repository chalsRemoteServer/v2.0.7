/*
 * view_cfg_producto.c
 *
 *  Created on: 24 feb. 2026
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

static int8_t s_grid_scroll = 0;   // 0: filas 1-2 visibles, 1: filas 2-3 visibles

static void opciones_producto_on_enter(void)
{
    s_grid_scroll = 0;
}

static void opciones_producto_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    // 1) Items primarios (6)
    const UiSubmPrimary items[] =
    {
        { UI_VID_OPT_PROD_ADM, 			"Adm. productos"     		, MEM_ICON_ADMPROD  },
        { UI_VID_OPT_PROD_DATOS,  		"Datos del producto"     	, MEM_ICON_DATPROD  },
        { UI_VID_OPT_PROD_APRENDER,    	"Aprender" 			 		, MEM_ICON_APRENDER },
        { UI_VID_OPT_PROD_AUDITORIA, 	"Auditoria"          		, MEM_ICON_AUDITORIA},
        { UI_VID_OPT_PROD_SALIDAS,   	"Salidas"            		, MEM_ICON_SALIDAS  },
        { UI_VID_OPT_PROD_AJUSTES,      "Ajustes"            		, MEM_ICON_AJUSTES  },
		{ UI_VID_OPT_PROD_MSPEED,        "Metal Speed"		 		, MEM_ICON_MSPEED   },
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

    UiRect16 cards[9];
    uint16_t stepPx = 0;

    // layout 3x3 (scrollRow: 0 o 1)
    const uint8_t drawCount = ui_subm_grid3x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, s_grid_scroll, cards, &stepPx);

    EVE_color_a(255);
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y + 5, PRODUCT_FONT_SIZE, 0, "Producto");

    EVE_scissor_xy(grid_x0, grid_y0);    // SCISSOR para que la fila extra quede "oculta" hasta que hagas scroll
    EVE_scissor_size((uint16_t)(grid_x1 - grid_x0), (uint16_t)(grid_y1 - grid_y0));

    ui_subm_draw_primary_grid3x3("", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, USER_FONT_SIZE, 8);

    // Restaurar scissor full-screen (ajusta si tu proyecto tiene constantes)
    EVE_scissor_xy(0, 0);
    EVE_scissor_size(800, 480);


    // 3) Botón secundario (barra derecha): X
    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    if (nItems > 6)
       {
        const UiSubmSecondary side[] =        							// X + UP + DOWN (3 botones)
        {
              { UI_VID_OPT_PROD_BACK,	"X"							},
              { UI_VID_OPT_PROD_UP,   	"",  		MEM_ICON_FARRIBA},
              { UI_VID_OPT_PROD_DOWN,	"",  		MEM_ICON_FABAJO },
        };
        ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 3);
       }
       else
       {
         const UiSubmSecondary side[] =		// Solo X (1 botón)
         {
               { UI_VID_OPT_PROD_BACK, "X"},
         };
         ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 1);
        }
}

static void opciones_producto_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_OPT_PROD_BACK:
             ui_view_pop();  // regresa a la vista anterior (normalmente main)
             break;

        case UI_VID_OPT_PROD_DOWN:        // Scroll grid (solo aplica si nItems > 6, pero no pasa nada si lo presionas)
        	 s_grid_scroll = 1;
        	 break;

        case UI_VID_OPT_PROD_UP:
        	 s_grid_scroll = 0;
        	 break;

        case UI_VID_OPT_PROD_ADM:
        	 ui_view_push(&VIEW_OPCIONES_PRODUCTO_ADMIN);
        	 break;

        case UI_VID_OPT_PROD_DATOS:
        	 ui_view_push(&VIEW_OPCIONES_PRODUCTO_DATOSP);
        	 break;

        case UI_VID_OPT_PROD_APRENDER:
        	 ui_view_push(&VIEW_OPCIONES_PRODUCTO_APRENDER);
        	 break;

        case UI_VID_OPT_PROD_AUDITORIA:
        	 ui_view_push(&VIEW_OPCIONES_PRODUCTO_AUDITORIA);
        	 break;

        case UI_VID_OPT_PROD_SALIDAS:
        	 ui_view_push(&VIEW_OPCIONES_PRODUCTO_SALIDAS);
        	 break;

        case UI_VID_OPT_PROD_AJUSTES:
       	     ui_view_push(&VIEW_OPCIONES_PRODUCTO_AJUSTES);
             break;

        case UI_VID_OPT_PROD_MSPEED:
             ui_view_push(&VIEW_OPCIONES_PRODUCTO_MSPEED);
             break;

        default:
            break;
    }
}

const UiView VIEW_OPCIONES_PRODUCTO =
{
    .id = 7,
    .on_enter = opciones_producto_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_producto_draw,
    .on_released = opciones_producto_on_released
};

