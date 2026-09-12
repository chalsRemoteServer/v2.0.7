/*
 * view_cfg_producto_admin_cambio.c
 *
 *  Created on: 24 feb. 2026
 *      Author: oscar

 */

#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "ui_views.h"
#include "ui_submenu_widgets.h"
#include "ui_almacena_productos.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define UI_VID_PROD_CAMBIO_BACK       			10
#define	UI_VID_PROD_CAMBIO_ACCEPT     			11
#define	UI_VID_PROD_CAMBIO_UP         			12
#define	UI_VID_PROD_CAMBIO_DOWN 				13

#define UI_VID_PROD_CAMBIO_ROW0					14
#define UI_VID_PROD_CAMBIO_ROW1					15
#define UI_VID_PROD_CAMBIO_ROW2					16
#define UI_VID_PROD_CAMBIO_ROW3					17

static int16_t s_scroll = 0;
static int16_t s_selected = -1;
static char s_status_msg[50] = "";

#define PROD_CAMBIO_VISIBLE_ROWS   4

static uint8_t prod_cambio_count(void)
{
    return ui_product_store_count();
}

static void prod_cambio_clamp_scroll(void)
{
    int16_t maxScroll = (int16_t)prod_cambio_count() - (int16_t)PROD_CAMBIO_VISIBLE_ROWS;

    if (maxScroll < 0) maxScroll = 0;
    if (s_scroll < 0) s_scroll = 0;
    if (s_scroll > maxScroll) s_scroll = maxScroll;
}

static uint16_t prod_cambio_row_id_from_visible(uint8_t vis)
{
    switch (vis)
    {
        case 0: return UI_VID_PROD_CAMBIO_ROW0;
        case 1: return UI_VID_PROD_CAMBIO_ROW1;
        case 2: return UI_VID_PROD_CAMBIO_ROW2;
        case 3: return UI_VID_PROD_CAMBIO_ROW3;

        default: return 0;
    }
}

static int8_t prod_cambio_visible_from_row_id(uint16_t id)
{
    switch (id)
    {
        case UI_VID_PROD_CAMBIO_ROW0: return 0;
        case UI_VID_PROD_CAMBIO_ROW1: return 1;
        case UI_VID_PROD_CAMBIO_ROW2: return 2;
        case UI_VID_PROD_CAMBIO_ROW3: return 3;

        default: return -1;
    }
}

static void opciones_producto_admin_cambio_on_enter(void)
{
    int8_t idx_active = ui_product_store_find_visible_index_by_id(ui_product_store_get_active_id());

    s_status_msg[0] = '\0';

    if (idx_active >= 0)
    {
        s_selected = idx_active;
    }
    else if (ui_product_store_count() > 0)
    {
        s_selected = 0;
    }
    else
    {
        s_selected = -1;
    }

    if (s_selected < 0)
    {
        s_scroll = 0;
    }
    else
    {
        s_scroll = s_selected;
    }

    prod_cambio_clamp_scroll();
}

static void opciones_producto_admin_cambio_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t title_x = 15;
    const uint16_t title_y = 140;

    const uint16_t table_x = 3;
    const uint16_t table_y = 185;
    const uint16_t side_x  = 710;
    const uint16_t table_w = (uint16_t)(side_x - table_x - 5);

    const uint16_t row_h = 68;
    const uint16_t row_gap = 1;
    //const uint16_t pad_x = 18;
    const uint16_t text_y = 18;

    uint8_t i;

    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Cambio de producto");

    if (ui_product_store_count() == 0u)
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text(330, 300, PRODUCT_FONT_SIZE, EVE_OPT_CENTER, "NO HAY PRODUCTOS CREADOS");
    }
    else
    {
        prod_cambio_clamp_scroll();

        for (i = 0; i < PROD_CAMBIO_VISIBLE_ROWS; i++)
        {
            uint8_t idx = (uint8_t)(s_scroll + i);
            const UiProductRecord* p;
            uint16_t y;
            char line1[40];

            uint8_t is_selected = 0u;
            uint8_t is_active = 0u;

            if (idx >= ui_product_store_count()) break;

            p = ui_product_store_get_by_index(idx);
            if (!p) continue;

            y = (uint16_t)(table_y + i * (row_h + row_gap));

            if (ui_product_store_get_active_id() == p->id)
            	is_active = 1u;

            if (s_selected == (int16_t)idx)
            	is_selected = 1u;

            if (is_selected)
            {
                EVE_color_rgb(BLUE);   /* azul claro */
            }
            else
            {
                EVE_color_rgb(MEDIUM_GRAY); /* o 0xD0D0D0 si prefieres */
            }

            EVE_begin(EVE_RECTS);
            EVE_tag(prod_cambio_row_id_from_visible(i));
            EVE_vertex2f(table_x * PRESICION, y * PRESICION);
            EVE_vertex2f((table_x + table_w) * PRESICION, (y + row_h - 6) * PRESICION);
            EVE_end();
            EVE_tag(0);

            if (is_active)
            {
                snprintf(line1, sizeof(line1), "%u: %s - producto activo", p->id, p->name);
            }
            else
            {
                snprintf(line1, sizeof(line1), "%u: %s", p->id, p->name);
            }
            EVE_color_rgb(BLACK);
            EVE_cmd_text((int16_t)(table_x), (int16_t)(y + text_y), USER_FONT_SIZE, EVE_OPT_FLAT, line1);

        }
    }

    if (s_status_msg[0] != '\0')
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text(220, 240, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, s_status_msg);
    }

    {
        const uint16_t BTN = 84;
        const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
        const uint16_t yR0 = (uint16_t)(L->content_y0);

        const UiSubmSecondary side[] =
        {
            { UI_VID_PROD_CAMBIO_BACK,   "X" },
            { UI_VID_PROD_CAMBIO_ACCEPT, "", MEM_ICON_ACEPTAR },
            { UI_VID_PROD_CAMBIO_UP,     "", MEM_ICON_FARRIBA },
            { UI_VID_PROD_CAMBIO_DOWN,   "", MEM_ICON_FABAJO  },
        };

        ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 3, side, 4);
    }
}

static void opciones_producto_admin_cambio_on_released(uint16_t id)
{
    int8_t vis;
    uint8_t idx;
    const UiProductRecord* p;

    switch (id)
    {
        case UI_VID_PROD_CAMBIO_BACK:
            ui_view_pop();
            return;

        case UI_VID_PROD_CAMBIO_UP:
            if (s_scroll > 0) s_scroll--;
            return;

        case UI_VID_PROD_CAMBIO_DOWN:
        {
            int16_t maxScroll = (int16_t)ui_product_store_count() - (int16_t)PROD_CAMBIO_VISIBLE_ROWS;
            if (maxScroll < 0) maxScroll = 0;
            if (s_scroll < maxScroll) s_scroll++;
            return;
        }

        case UI_VID_PROD_CAMBIO_ACCEPT:
        {
            if (s_selected < 0 || s_selected >= (int16_t)ui_product_store_count())
            {
                strcpy(s_status_msg, "No hay producto seleccionado");
                return;
            }

            p = ui_product_store_get_by_index((uint8_t)s_selected);
            if (!p)
            {
                strcpy(s_status_msg, "Error de seleccion");
                return;
            }

            if (ui_product_store_set_active_by_id(p->id))
            {
                snprintf(s_status_msg, sizeof(s_status_msg), "Cambio de producto - : %u", p->id);
                return;
            }

            strcpy(s_status_msg, "No se pudo activar");
            return;
        }

        default:
            break;
    }

    vis = prod_cambio_visible_from_row_id(id);
    if (vis >= 0)
    {
        idx = (uint8_t)(s_scroll + vis);

        if (idx < ui_product_store_count())
        {
            s_selected = idx;
            s_status_msg[0] = '\0';
        }
        return;
    }
}

const UiView VIEW_CFG_PROD_ADMIN_CAMBIO =
{
    .id = 9,
    .on_enter = opciones_producto_admin_cambio_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_producto_admin_cambio_draw,
    .on_released = opciones_producto_admin_cambio_on_released
};

