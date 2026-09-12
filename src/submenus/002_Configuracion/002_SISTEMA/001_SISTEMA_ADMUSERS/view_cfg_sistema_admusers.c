#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "colores.h"
#include "EVE.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"

static void opciones_sistema_admusers_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    // 1) Items primarios (6)
    const UiSubmPrimary items[] =
    {
        { UI_VID_OPT_SIS_ADMUSERS_EDITAR, 			"Editar"     		, MEM_ICON_EDITAR       },
        { UI_VID_OPT_SIS_ADMUSERS_CREAR,  			"Crear"      		, MEM_ICON_CREAR        },
        { UI_VID_OPT_SIS_ADMUSERS_ELIMINAR,    		"Eliminar" 			, MEM_ICON_ELIMINAR     },
        { UI_VID_OPT_SIS_ADMUSERS_COPIAR,   		"Copiar"            , MEM_ICON_COPIAR       },
        { UI_VID_OPT_SIS_ADMUSERS_AJUSTES,          "Ajustes"         	, MEM_ICON_AJUSTES      },
		{ UI_VID_OPT_SIS_ADMUSERS_LOGIN2GO, 		"Login2Go"          , MEM_ICON_LOGIN2GO     },
    };

    const uint8_t nItems = (uint8_t)(sizeof(items)/sizeof(items[0]));

    // 2) Título + área de grid (misma estrategia que registro_opts)
    const uint16_t title_x   = 20;
    const uint16_t title_y   = (uint16_t)(L->content_y0);
    const uint16_t title_h   = 34;
    const uint16_t title_gap = 20;
    // 2) Título + área de grid (misma estrategia que registro_opts)
    const uint16_t BTN = 94;

    const uint16_t grid_x0 = title_x;
    const uint16_t grid_x1 = (uint16_t)(L->content_x1 + 50);

    const uint16_t grid_y0 = (uint16_t)(title_y + title_h + title_gap);
    const uint16_t grid_y1 = (uint16_t)(L->content_y1);

    UiRect16 cards[6];

    const uint8_t drawCount = ui_subm_grid2x3_layout_dynamic(grid_x0, grid_y0, grid_x1, grid_y1, nItems, BTN, 10, 16, 8, 24, cards);


    ui_subm_draw_primary_grid2x3("Administracion de usuarios", title_x, title_y, PRODUCT_FONT_SIZE, items, nItems, cards, drawCount, 26, USER_FONT_SIZE, 8);

    // 3) Botón secundario (barra derecha): X
    const UiSubmSecondary side[] =
    {
        { UI_VID_OPT_PROD_BACK, "X" },
    };

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 10, side, 1);
}


static void opciones_sistema_admusers_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_OPT_PROD_BACK:
             ui_view_pop();  // regresa a la vista anterior (normalmente main)
             break;

        case UI_VID_OPT_SIS_ADMUSERS_EDITAR:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_ADMINUSERS_EDITAR);
        	 break;

        case UI_VID_OPT_SIS_ADMUSERS_CREAR:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_ADMINUSERS_CREAR);
        	 break;

        case UI_VID_OPT_SIS_ADMUSERS_ELIMINAR:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_ADMINUSERS_ELIMINAR);
        	 break;

        case UI_VID_OPT_SIS_ADMUSERS_COPIAR:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_ADMINUSERS_COPIAR);
        	 break;

        case UI_VID_OPT_SIS_ADMUSERS_AJUSTES:
        	 ui_view_push(&VIEW_OPCIONES_SISTEMA_ADMINUSERS_AJUSTES);
        	 break;

        case UI_VID_OPT_SIS_ADMUSERS_LOGIN2GO:
       	     ui_view_push(&VIEW_OPCIONES_SISTEMA_ADMINUSERS_LOGIN2GO);
             break;

        default:
            break;
    }
}


const UiView VIEW_OPCIONES_SISTEMA_ADMUSERS =
{
    .id = 25,
    .on_enter = 0,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_sistema_admusers_draw,
    .on_released = opciones_sistema_admusers_on_released
};

