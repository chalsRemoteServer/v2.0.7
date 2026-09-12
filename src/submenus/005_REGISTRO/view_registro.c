/*
 * view_registro.c
 *
 *  Created on: 18 feb. 2026
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

static int16_t s_scroll = 0;/* Estado local del view (ej: scroll) */

#define  	UI_VID_REG_BACK      				 10 	// CRUZ ROJA (pop)
#define   	UI_VID_REG_CONFIG    			     11 	// CONFIG (push opciones)
#define   	UI_VID_REG_UP        				 12 	// Flecha arriba (scroll -)
#define  	UI_VID_REG_DOWN      			     13 	// Flecha aba


typedef struct
{
  const char *id;
  const char *fecha;
  const char *hora;
  const char *usuario;
  const char *detalle;
} UiLogRow;

static const UiLogRow s_rows[] =
{
  { "#12", "03.10.2025", "12:44:37", "Service",        "Sesion de usuario iniciada: Service" },
  { "#11", "03.10.2025", "12:42:44", "Ningun usuario", "IO Reboot -> 0101  0"               },
  { "#10", "03.10.2025", "12:40:29", "Ningun usuario", "Red ACT  101"                       },
  { "#09", "03.10.2025", "12:38:15", "Ningun usuario", "Evento extra para probar scroll"    },
};
static const uint8_t s_rowsN = (uint8_t)(sizeof(s_rows)/sizeof(s_rows[0]));

static void reg_on_enter(void) { /* opcional */ }
static void reg_on_exit(void)  { /* opcional */ }
static void reg_update(uint32_t now_ms) { (void)now_ms; /* opcional */ }

/* Dibuja barra derecha + título (por ahora) */
static void reg_draw(void)
{
	const UiMainLayout* L = &UI_LAYOUT_MAIN;

	const uint16_t title_y = (uint16_t)(L->content_y0 + 50);
	const uint16_t table_x = 3;

	const uint16_t table_y = (uint16_t)(title_y + 15);
	const uint16_t table_w = (uint16_t)(L->content_x1 - table_x + 5); // ancho real desde table_x

	const uint16_t row_h   = 85;
	const uint16_t pad_x   = 12;
	const uint16_t pad_y   = 10;

	const uint16_t font_header = USER_FONT_SIZE; // o tu fuente real
	const uint16_t font_detail = 24;             // ajusta si usas otra

	// Título
	EVE_color_rgb(BLACK);
	EVE_cmd_text_bold(20, L->content_y0, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Registro");

	/* 3) Columnas (4) */
	const uint16_t col0 = (uint16_t)(table_x + pad_x + 0);
	const uint16_t col1 = (uint16_t)(table_x + pad_x + 90);
	const uint16_t col2 = (uint16_t)(table_x + pad_x + 250);
	const uint16_t col3 = (uint16_t)(table_x + pad_x + 410);

	/* 4) Visible rows según alto disponible */
	const uint8_t visibleRows = 3; // empieza con 3 como en tu ejemplo Arduino

	/* 5) Clamp scroll */
	int16_t maxScroll = (int16_t)s_rowsN - (int16_t)visibleRows;
	if (maxScroll < 0) maxScroll = 0;
	if (s_scroll < 0) s_scroll = 0;
	if (s_scroll > maxScroll) s_scroll = maxScroll;

	/* 6) Dibuja filas visibles */
	for (uint8_t i = 0; i < visibleRows; i++)
	{
		uint8_t idx = (uint8_t)(s_scroll + i);
		if (idx >= s_rowsN) break;

		const uint16_t y = (uint16_t)(table_y + i * row_h);

		// Fondo alternado
		if ((i & 1u) == 0u) EVE_color_rgb(BLUE);
		else                EVE_color_rgb(BLUE);

		EVE_begin(EVE_RECTS);
		EVE_vertex2f(table_x * PRESICION, y * PRESICION);
		EVE_vertex2f((table_x + table_w) * PRESICION, (y + row_h - 5) * PRESICION);
		EVE_end();

		// Línea 1 (4 columnas)
		EVE_color_rgb(WHITE);
		EVE_cmd_text_bold(col0, (uint16_t)(y + pad_y), font_header, EVE_OPT_FLAT, s_rows[idx].id);
		EVE_cmd_text_bold(col1, (uint16_t)(y + pad_y), font_header, EVE_OPT_FLAT, s_rows[idx].fecha);
		EVE_cmd_text_bold(col2, (uint16_t)(y + pad_y), font_header, EVE_OPT_FLAT, s_rows[idx].hora);
		EVE_cmd_text_bold(col3, (uint16_t)(y + pad_y), font_header, EVE_OPT_FLAT, s_rows[idx].usuario);

		// Línea 2 (detalle)
		EVE_color_rgb(WHITE);
		EVE_cmd_text((int16_t)col0, (int16_t)(y + pad_y + 34), font_detail, EVE_OPT_FLAT, s_rows[idx].detalle);
	}

    const uint16_t w = 84;
    const uint16_t h = 84;

    const UiSubmSecondary side[] =
    {
       { UI_VID_REG_BACK, 		"X", 						},
	   { UI_VID_REG_CONFIG, 	"" , 	MEM_ICON_AJUSTES},
	   { UI_VID_REG_UP, 		"" ,	MEM_ICON_FARRIBA},
	   { UI_VID_REG_DOWN, 		"" , 	MEM_ICON_FABAJO},
    };

    const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    ui_subm_draw_side_buttons(xR0, yR0, w, h, 5, side, 4);

}

/* Handle touch (released) */
extern const UiView VIEW_REGISTRO;
extern const UiView VIEW_REGISTRO_OPTS;

static void reg_on_released(uint16_t id)
{
    switch (id)
    {
        case UI_VID_REG_BACK:
             ui_view_pop(); // regresa (p.ej. a MAIN)
             break;

        case UI_VID_REG_CONFIG:
             ui_view_push(&VIEW_REGISTRO_OPTS);
             break;

        case UI_VID_REG_UP:
             if (s_scroll > 0) s_scroll--;
             break;

        case UI_VID_REG_DOWN:
        {
             const uint8_t visibleRows = 3; // AJUSTA según tu layout
             int16_t maxScroll = (int16_t)s_rowsN - (int16_t)visibleRows;
             if (maxScroll < 0) maxScroll = 0;
             if (s_scroll < maxScroll) s_scroll++;
        }    break;

        default:
             break;
    }
}

const UiView VIEW_REGISTRO =
{
    .id = 1,
    .on_enter = reg_on_enter,
    .on_exit = reg_on_exit,
    .update = reg_update,
    .draw = reg_draw,
    .on_released = reg_on_released
};

