/*
 * viev_cfg_sistema_admusers_eliminar.c
 *
 *  Created on: 25 feb. 2026
 *      Author: oscar
 */

/*
 * view_cfg_sistema_admusers_eliminar.c
 */

#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "ui_almacena_usuarios.h"
#include "ui_rects.h"
#include "ui_views.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "assets_mem.h"

#define USER_ELIM_MAX_VISIBLE   4

static uint8_t s_scroll = 0u;              /* índice visible inicial */
static int8_t  s_selected_visible = -1;    /* 0..3 relativo a la ventana visible */
static uint8_t s_selected_user_id = 0u;    /* ID real del usuario seleccionado */
static char    s_status_msg[64] = "";

static uint8_t user_elim_total_count(void)
{
    return ui_user_store_count();
}

static uint8_t user_elim_visible_count(void)
{
    uint8_t total = user_elim_total_count();

    if (total <= s_scroll)
        return 0u;

    total = (uint8_t)(total - s_scroll);

    if (total > USER_ELIM_MAX_VISIBLE)
        total = USER_ELIM_MAX_VISIBLE;

    return total;
}

static void user_elim_clamp_scroll(void)
{
    uint8_t total = user_elim_total_count();

    if (total == 0u)
    {
        s_scroll = 0u;
        return;
    }

    if (s_scroll >= total)
    {
        s_scroll = (uint8_t)(total - 1u);
    }

    if (total > USER_ELIM_MAX_VISIBLE)
    {
        uint8_t max_scroll = (uint8_t)(total - USER_ELIM_MAX_VISIBLE);
        if (s_scroll > max_scroll)
            s_scroll = max_scroll;
    }
    else
    {
        s_scroll = 0u;
    }
}

static void user_elim_clear_selection(void)
{
    s_selected_visible = -1;
    s_selected_user_id = 0u;
}

static void user_elim_restore_selection_after_delete(void)
{
    uint8_t total = user_elim_total_count();
    uint8_t visible_count;
    const UiUserRecord* rec;

    if (total == 0u)
    {
        user_elim_clear_selection();
        return;
    }

    user_elim_clamp_scroll();
    visible_count = user_elim_visible_count();

    if (visible_count == 0u)
    {
        user_elim_clear_selection();
        return;
    }

    if (s_selected_visible < 0)
    {
        s_selected_visible = 0;
    }

    if ((uint8_t)s_selected_visible >= visible_count)
    {
        s_selected_visible = (int8_t)(visible_count - 1u);
    }

    rec = ui_user_store_get_by_index((uint8_t)(s_scroll + (uint8_t)s_selected_visible));
    if (rec)
    {
        s_selected_user_id = rec->id;
    }
    else
    {
        user_elim_clear_selection();
    }
}

static void user_elim_select_visible(uint8_t visible_idx)
{
    const UiUserRecord* rec = ui_user_store_get_by_index((uint8_t)(s_scroll + visible_idx));
    if (!rec) return;

    s_selected_visible = (int8_t)visible_idx;
    s_selected_user_id = rec->id;
}

static void user_elim_draw_row(uint16_t tag_id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const UiUserRecord* rec, uint8_t is_selected)
{
    char line_buf[64];

    EVE_tag((uint8_t)tag_id);
    ui_rect_register(tag_id, x, y, w, h);

    if (is_selected)
        EVE_color_rgb(LIGHT_RED);
    else
        EVE_color_rgb(MEDIUM_GRAY);

    EVE_begin(EVE_RECTS);
    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);
    EVE_end();

    EVE_color_rgb(BLACK);

    snprintf(line_buf, sizeof(line_buf), "%u - %s", rec->id, rec->username);

    EVE_cmd_text((int16_t)(x + 14), (int16_t)(y + 10), USER_FONT_SIZE, EVE_OPT_FLAT, line_buf);
    EVE_tag(0);
}

static void opciones_sistema_admusers_eliminar_on_enter(void)
{
    const UiUserRecord* rec;

    s_scroll = 0u;
    s_status_msg[0] = '\0';

    if (ui_user_store_is_empty())
    {
        user_elim_clear_selection();
        strcpy(s_status_msg, "No hay usuarios");
        return;
    }

    rec = ui_user_store_get_by_index(0u);
    if (rec)
    {
        s_selected_visible = 0;
        s_selected_user_id = rec->id;
    }
    else
    {
        user_elim_clear_selection();
    }
}

static void opciones_sistema_admusers_eliminar_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    uint16_t x0 = 3;								uint16_t y0 = (uint16_t)(L->content_y0 + 60u);
    const uint16_t title_x = 20;					const uint16_t title_y = 140;


    uint16_t row_w = 700;
    uint16_t row_h = 58;
    uint16_t gap = 5;
    uint8_t visible_count;
    uint8_t i;

    /* Fondo / marco base del submenú */
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Eliminar usuario");

    visible_count = user_elim_visible_count();

    if (visible_count == 0u)
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text(320, 220, 28, EVE_OPT_CENTER, "No hay usuarios");
    }

    else
    {
        for (i = 0; i < visible_count; i++)
        {
            const UiUserRecord* rec = ui_user_store_get_by_index((uint8_t)(s_scroll + i));
            uint16_t y = (uint16_t)(y0 + i * (row_h + gap));
            uint8_t is_selected = ((s_selected_visible >= 0) && ((uint8_t)s_selected_visible == i)) ? 1u : 0u;

            if (!rec) continue;

            user_elim_draw_row((uint16_t)(UI_VID_SISTEMA_ELIM_ROW0 + i), x0, y, row_w, row_h, rec, is_selected);
        }
    }

    if (s_status_msg[0] != '\0')
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text(330, 430, 25, EVE_OPT_CENTER, s_status_msg);
    }

     const uint16_t BTN = 84;
     const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
     const uint16_t yR0 = (uint16_t)(L->content_y0);

     const UiSubmSecondary side[] =
     {
           { UI_VID_SISTEMA_ELIM_BACK  ,   	"X" 				   },
           { UI_VID_SISTEMA_ELIM_ACCEPT, 	""	, MEM_ICON_ACEPTAR },
           { UI_VID_SISTEMA_ELIM_UP    ,    ""	, MEM_ICON_FARRIBA },
           { UI_VID_SISTEMA_ELIM_DOWN  ,   	""	, MEM_ICON_FABAJO  },
     };

     ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 3, side, 4);

}

static void opciones_sistema_admusers_eliminar_on_released(uint16_t id)
{
    uint8_t total = user_elim_total_count();

    switch (id)
    {
        case UI_VID_SISTEMA_ELIM_BACK:
            ui_view_pop();
            return;

        case UI_VID_SISTEMA_ELIM_UP:
            if (s_scroll > 0u)
            {
                s_scroll--;
                strcpy(s_status_msg, "");
            }
            return;

        case UI_VID_SISTEMA_ELIM_DOWN:
            if (total > USER_ELIM_MAX_VISIBLE)
            {
                uint8_t max_scroll = (uint8_t)(total - USER_ELIM_MAX_VISIBLE);
                if (s_scroll < max_scroll)
                {
                    s_scroll++;
                    strcpy(s_status_msg, "");
                }
            }
            return;

        case UI_VID_SISTEMA_ELIM_ACCEPT:
            if (s_selected_user_id == 0u)
            {
                strcpy(s_status_msg, "Seleccione un usuario");
                return;
            }

            if (ui_user_store_delete_by_id(s_selected_user_id))
            {
                strcpy(s_status_msg, "Usuario eliminado");
                user_elim_restore_selection_after_delete();
            }
            else
            {
                strcpy(s_status_msg, "Error al eliminar");
            }
            return;

        case UI_VID_SISTEMA_ELIM_ROW0:
        case UI_VID_SISTEMA_ELIM_ROW1:
        case UI_VID_SISTEMA_ELIM_ROW2:
        case UI_VID_SISTEMA_ELIM_ROW3:
        {
            uint8_t visible_idx = (uint8_t)(id - UI_VID_SISTEMA_ELIM_ROW0);
            if (visible_idx < user_elim_visible_count())
            {
                user_elim_select_visible(visible_idx);
                strcpy(s_status_msg, "");
            }
            return;
        }

        default:
            return;
    }
}

const UiView VIEW_OPCIONES_SISTEMA_ADMINUSERS_ELIMINAR =
{
    .id = 28,
    .on_enter = opciones_sistema_admusers_eliminar_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_sistema_admusers_eliminar_draw,
    .on_released = opciones_sistema_admusers_eliminar_on_released
};
