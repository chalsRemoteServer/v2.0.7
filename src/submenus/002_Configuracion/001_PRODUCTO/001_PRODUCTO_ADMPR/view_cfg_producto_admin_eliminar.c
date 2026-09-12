/*
 * view_cfg_producto_admin_eliminar.c
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

static int16_t s_scroll = 0;
static int16_t s_selected = -1;
static char s_status_msg[50] = "";

#define  UI_VID_PROD_CREAR_POP_CANCEL  			 247
#define	 UI_VID_PROD_CREAR_POP_ACCEPT  			 248
#define	 UI_VID_PROD_CREAR_FAST_CANCEL 			 249
#define	 UI_VID_PROD_CREAR_FAST_ACCEPT 			 250

#define UI_VID_PROD_ELIM_BACK      	 			 10
#define UI_VID_PROD_ELIM_ACCEPT     			 11
#define UI_VID_PROD_ELIM_UP         			 12
#define UI_VID_PROD_ELIM_DOWN       			 13

#define UI_VID_PROD_ELIM_ROW0					 14
#define UI_VID_PROD_ELIM_ROW1					 15
#define UI_VID_PROD_ELIM_ROW2					 16
#define UI_VID_PROD_ELIM_ROW3					 17

typedef enum
{
    PROD_ELIM_MODE_LIST = 0,
    PROD_ELIM_MODE_CONFIRM
} ProdElimMode;

static ProdElimMode s_mode = PROD_ELIM_MODE_LIST;

#define PROD_ELIM_VISIBLE_ROWS   4

static uint8_t prod_elim_count(void)
{
    return ui_product_store_count();
}

static void prod_elim_clamp_scroll(void)
{
    int16_t maxScroll = (int16_t)prod_elim_count() - (int16_t)PROD_ELIM_VISIBLE_ROWS;

    if (maxScroll < 0) maxScroll = 0;
    if (s_scroll < 0) s_scroll = 0;
    if (s_scroll > maxScroll) s_scroll = maxScroll;
}

static uint16_t prod_elim_row_id_from_visible(uint8_t vis)
{
    switch (vis)
    {
        case 0: return UI_VID_PROD_ELIM_ROW0;
        case 1: return UI_VID_PROD_ELIM_ROW1;
        case 2: return UI_VID_PROD_ELIM_ROW2;
        case 3: return UI_VID_PROD_ELIM_ROW3;

        default: return 0;
    }
}

static int8_t prod_elim_visible_from_row_id(uint16_t id)
{
    switch (id)
    {
        case UI_VID_PROD_ELIM_ROW0: return 0;
        case UI_VID_PROD_ELIM_ROW1: return 1;
        case UI_VID_PROD_ELIM_ROW2: return 2;
        case UI_VID_PROD_ELIM_ROW3: return 3;

        default: return -1;
    }
}

static void prod_elim_fix_selection_after_delete(void)
{
    uint8_t count = ui_product_store_count();

    if (count == 0u)
    {
        s_selected = -1;
        s_scroll = 0;
        return;
    }

    if (s_selected >= (int16_t)count)
    {
        s_selected = (int16_t)(count - 1u);
    }

    if (s_selected < 0)
    {
        s_selected = 0;
    }

    if (s_selected < s_scroll)
    {
        s_scroll = s_selected;
    }

    prod_elim_clamp_scroll();
}

static void opciones_producto_admin_eliminar_on_enter(void)
{
    s_status_msg[0] = '\0';

    if (ui_product_store_count() > 0u)
    {
        s_selected = 0;
    }
    else
    {
        s_selected = -1;
    }

    s_scroll = 0;
    prod_elim_clamp_scroll();
    s_mode = PROD_ELIM_MODE_LIST;
}

static void prod_elim_draw_confirm_popup(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t box_w = 695;
    const uint16_t box_h = 270;
    const uint16_t box_x = (uint16_t)(L->footer_x0);
    const uint16_t box_y = (uint16_t)(L->content_y0 + 50);

    const uint16_t BTN = 94;
    const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    const UiSubmSecondary side[] =
    {
        { UI_VID_PROD_CREAR_FAST_CANCEL, "X"                 },
        { UI_VID_PROD_CREAR_FAST_ACCEPT, "", MEM_ICON_ACEPTAR },

    };

    EVE_color_rgb(BLUE);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(box_x * PRESICION, box_y * PRESICION);
    EVE_vertex2f((box_x + box_w) * PRESICION, (box_y + box_h) * PRESICION);
    EVE_end();

    EVE_color_rgb(WHITE);
    EVE_cmd_text((int16_t)(box_x + box_w / 2u) - 150, (int16_t)(box_y + 65u) + 50, 25, EVE_OPT_FLAT, " Quieres eliminar el producto ?");

    ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 3, side, 2);
}


static void opciones_producto_admin_eliminar_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t title_x = 20;
    const uint16_t title_y = 140;

    const uint16_t table_x = 3;
    const uint16_t table_y = 185;
    const uint16_t side_x  = 710;
    const uint16_t table_w = (uint16_t)(side_x - table_x - 5u);

    const uint16_t row_h   = 68;
    const uint16_t row_gap = 1;
    const uint16_t pad_x   = 18;
    const uint16_t text_y  = 12;

    uint8_t i;

    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Eliminar producto");

    if (s_mode == PROD_ELIM_MODE_LIST)
    {
        if (ui_product_store_count() == 0u)
        {
            EVE_color_rgb(BLACK);
            EVE_cmd_text(330, 300, PRODUCT_FONT_SIZE, EVE_OPT_CENTER, "NO HAY PRODUCTOS CREADOS");
        }
        else
        {
            prod_elim_clamp_scroll();

            for (i = 0; i < PROD_ELIM_VISIBLE_ROWS; i++)
            {
                uint8_t idx = (uint8_t)(s_scroll + i);
                const UiProductRecord* p;
                uint16_t y;
                char line1[80];

                uint8_t is_selected = 0u;
                uint8_t is_active = 0u;

                if (idx >= ui_product_store_count()) break;

                p = ui_product_store_get_by_index(idx);
                if (!p) continue;

                y = (uint16_t)(table_y + i * (row_h + row_gap));

                if (s_selected == (int16_t)idx) is_selected = 1u;
                if (ui_product_store_get_active_id() == p->id) is_active = 1u;

                /* Nuevo criterio visual:
                   - Seleccionado = rojo
                   - No seleccionado = gris
                   - El producto activo ya NO va en verde
                */
                if (is_selected)
                {
                    EVE_color_rgb(LIGHT_RED);
                }
                else
                {
                    EVE_color_rgb(MEDIUM_GRAY);
                }

                EVE_begin(EVE_RECTS);
                EVE_tag(prod_elim_row_id_from_visible(i));
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
                EVE_cmd_text((int16_t)(table_x + pad_x), (int16_t)(y + text_y), USER_FONT_SIZE, EVE_OPT_FLAT, line1);
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
                { UI_VID_PROD_ELIM_BACK,   "X"                  },
                { UI_VID_PROD_ELIM_ACCEPT, "", MEM_ICON_ACEPTAR },
                { UI_VID_PROD_ELIM_UP,     "", MEM_ICON_FARRIBA },
                { UI_VID_PROD_ELIM_DOWN,   "", MEM_ICON_FABAJO  },
            };

            ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 3, side, 4);
        }
    }
    else if (s_mode == PROD_ELIM_MODE_CONFIRM)
    {
        prod_elim_draw_confirm_popup();
    }
}

static void opciones_producto_admin_eliminar_on_released(uint16_t id)
{
    int8_t vis;
    uint8_t idx;
    const UiProductRecord* p;
    uint8_t del_id;

    /* --- Modo popup de confirmacion --- */
    if (s_mode == PROD_ELIM_MODE_CONFIRM)
    {
        switch (id)
        {
            case UI_VID_PROD_CREAR_FAST_CANCEL:
                s_mode = PROD_ELIM_MODE_LIST;
                return;

            case UI_VID_PROD_CREAR_FAST_ACCEPT:
            {
                if (s_selected < 0 || s_selected >= (int16_t)ui_product_store_count())
                {
                    strcpy(s_status_msg, "No hay producto seleccionado");
                    s_mode = PROD_ELIM_MODE_LIST;
                    return;
                }

                p = ui_product_store_get_by_index((uint8_t)s_selected);
                if (!p)
                {
                    strcpy(s_status_msg, "Error de seleccion");
                    s_mode = PROD_ELIM_MODE_LIST;
                    return;
                }

                del_id = p->id;

                if (ui_product_store_delete_by_id(del_id))
                {
                    snprintf(s_status_msg, sizeof(s_status_msg), "Producto %u eliminado", del_id);
                    prod_elim_fix_selection_after_delete();
                    s_mode = PROD_ELIM_MODE_LIST;
                    return;
                }

                strcpy(s_status_msg, "No se pudo eliminar");
                s_mode = PROD_ELIM_MODE_LIST;
                return;
            }

            default:
                return;
        }
    }

    /* --- Modo normal: lista de productos --- */
    switch (id)
    {
        case UI_VID_PROD_ELIM_BACK:
            ui_view_pop();
            return;

        case UI_VID_PROD_ELIM_UP:
            if (s_scroll > 0) s_scroll--;
            return;

        case UI_VID_PROD_ELIM_DOWN:
        {
            int16_t maxScroll = (int16_t)ui_product_store_count() - (int16_t)PROD_ELIM_VISIBLE_ROWS;
            if (maxScroll < 0) maxScroll = 0;
            if (s_scroll < maxScroll) s_scroll++;
            return;
        }

        case UI_VID_PROD_ELIM_ACCEPT:
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

            s_status_msg[0] = '\0';
            s_mode = PROD_ELIM_MODE_CONFIRM;
            return;
        }

        default:
            break;
    }

    vis = prod_elim_visible_from_row_id(id);
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


const UiView VIEW_CFG_PROD_ADMIN_ELIMINAR =
{
    .id = 11,
    .on_enter = opciones_producto_admin_eliminar_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_producto_admin_eliminar_draw,
    .on_released = opciones_producto_admin_eliminar_on_released
};

