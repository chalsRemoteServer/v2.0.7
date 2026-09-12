/*
 * view_usuario.c
 *
 *  Created on: 3 mar. 2026
 *      Author: oscar
 */

#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "ui_views.h"
#include "ui_leaf_form.h"
#include "ui_rects.h"
#include "ui_almacena_usuarios.h"

#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#define USR_VISIBLE_ROWS 4u

/* IDs locales SOLO para esta view */
#define UI_VID_USR_BACK          247u
#define UI_VID_USR_ACCEPT        248u
#define UI_VID_USR_UP            249u
#define UI_VID_USR_DOWN          250u

#define UI_VID_USR_ROW0          251u
#define UI_VID_USR_ROW1          252u
#define UI_VID_USR_ROW2          253u
#define UI_VID_USR_ROW3          254u

#define UI_VID_USUARIO_CREAR_ROW0 255u

typedef enum
{
    USR_MODE_LIST = 0,
    USR_MODE_PASSWORD
} UsuarioMode;

static UsuarioMode s_mode = USR_MODE_LIST;

/* Lista */
static int8_t  s_selected = 0;
static int8_t  s_scroll = 0;

/* Estado / mensajes */
static char    s_error_msg[48];
static uint8_t s_show_error = 0u;

/* Password */
static UiLeafState  s_leaf;
static UiLeafLayout s_lay;
static UiLeafRow    s_pwd_rows[1];
static char         s_pwd_buf[UI_USER_PASSWORD_MAX + 1];

/*  Helpers    */
static uint16_t usuario_row_id_from_visible(uint8_t vis_idx)
{
    switch (vis_idx)
    {
        case 0u: return UI_VID_USR_ROW0;
        case 1u: return UI_VID_USR_ROW1;
        case 2u: return UI_VID_USR_ROW2;
        case 3u: return UI_VID_USR_ROW3;
        default: return 0u;
    }
}

static int8_t usuario_find_pwd_row_by_id(uint16_t id)
{
    if (s_pwd_rows[0].id == id) return 0;
    return -1;
}

static void usuario_scroll_clamp(void)
{
    int16_t total = (int16_t)ui_user_store_count();
    int16_t max_scroll = total - (int16_t)USR_VISIBLE_ROWS;

    if (max_scroll < 0) max_scroll = 0;
    if (s_scroll < 0) s_scroll = 0;
    if (s_scroll > max_scroll) s_scroll = (int8_t)max_scroll;
}

static void usuario_selection_clamp(void)
{
    int16_t total = (int16_t)ui_user_store_count();

    if (total <= 0)
    {
        s_selected = -1;
        s_scroll = 0;
        return;
    }

    if (s_selected < 0) s_selected = 0;
    if (s_selected >= total) s_selected = (int8_t)(total - 1);

    usuario_scroll_clamp();

    if (s_selected < s_scroll)
        s_scroll = s_selected;

    if (s_selected >= (s_scroll + (int8_t)USR_VISIBLE_ROWS))
        s_scroll = (int8_t)(s_selected - (int8_t)USR_VISIBLE_ROWS + 1);

    usuario_scroll_clamp();
}

static void usuario_pwd_clear(void)
{
    s_pwd_buf[0] = '\0';
}

static uint8_t usuario_key_to_char(uint16_t id, UiLeafState* st, char* out_c)
{
    if (!st || !out_c) return 0u;

    switch (id)
    {
        case UI_VID_KB_Q: *out_c = st->kb_shift ? 'Q' : 'q'; return 1u;
        case UI_VID_KB_W: *out_c = st->kb_shift ? 'W' : 'w'; return 1u;
        case UI_VID_KB_E: *out_c = st->kb_shift ? 'E' : 'e'; return 1u;
        case UI_VID_KB_R: *out_c = st->kb_shift ? 'R' : 'r'; return 1u;
        case UI_VID_KB_T: *out_c = st->kb_shift ? 'T' : 't'; return 1u;
        case UI_VID_KB_Y: *out_c = st->kb_shift ? 'Y' : 'y'; return 1u;
        case UI_VID_KB_U: *out_c = st->kb_shift ? 'U' : 'u'; return 1u;
        case UI_VID_KB_I: *out_c = st->kb_shift ? 'I' : 'i'; return 1u;
        case UI_VID_KB_O: *out_c = st->kb_shift ? 'O' : 'o'; return 1u;
        case UI_VID_KB_P: *out_c = st->kb_shift ? 'P' : 'p'; return 1u;

        case UI_VID_KB_A: *out_c = st->kb_shift ? 'A' : 'a'; return 1u;
        case UI_VID_KB_S: *out_c = st->kb_shift ? 'S' : 's'; return 1u;
        case UI_VID_KB_D: *out_c = st->kb_shift ? 'D' : 'd'; return 1u;
        case UI_VID_KB_F: *out_c = st->kb_shift ? 'F' : 'f'; return 1u;
        case UI_VID_KB_G: *out_c = st->kb_shift ? 'G' : 'g'; return 1u;
        case UI_VID_KB_H: *out_c = st->kb_shift ? 'H' : 'h'; return 1u;
        case UI_VID_KB_J: *out_c = st->kb_shift ? 'J' : 'j'; return 1u;
        case UI_VID_KB_K: *out_c = st->kb_shift ? 'K' : 'k'; return 1u;
        case UI_VID_KB_L: *out_c = st->kb_shift ? 'L' : 'l'; return 1u;

        case UI_VID_KB_Z: *out_c = st->kb_shift ? 'Z' : 'z'; return 1u;
        case UI_VID_KB_X: *out_c = st->kb_shift ? 'X' : 'x'; return 1u;
        case UI_VID_KB_C: *out_c = st->kb_shift ? 'C' : 'c'; return 1u;
        case UI_VID_KB_V: *out_c = st->kb_shift ? 'V' : 'v'; return 1u;
        case UI_VID_KB_B: *out_c = st->kb_shift ? 'B' : 'b'; return 1u;
        case UI_VID_KB_N: *out_c = st->kb_shift ? 'N' : 'n'; return 1u;
        case UI_VID_KB_M: *out_c = st->kb_shift ? 'M' : 'm'; return 1u;

        case UI_VID_KB_1: *out_c = '1'; return 1u;
        case UI_VID_KB_2: *out_c = '2'; return 1u;
        case UI_VID_KB_3: *out_c = '3'; return 1u;
        case UI_VID_KB_4: *out_c = '4'; return 1u;
        case UI_VID_KB_5: *out_c = '5'; return 1u;
        case UI_VID_KB_6: *out_c = '6'; return 1u;
        case UI_VID_KB_7: *out_c = '7'; return 1u;
        case UI_VID_KB_8: *out_c = '8'; return 1u;
        case UI_VID_KB_9: *out_c = '9'; return 1u;
        case UI_VID_KB_0: *out_c = '0'; return 1u;

        default:
            return 0u;
    }
}

static void usuario_password_prepare(void)
{
    ui_leaf_layout_default(&s_lay, &UI_LAYOUT_MAIN);
    ui_leaf_state_reset(&s_leaf, 1u);

    s_pwd_rows[0].id           = UI_VID_USUARIO_CREAR_ROW0;
    s_pwd_rows[0].label        = "Contrasena";
    s_pwd_rows[0].type         = UI_LEAF_FIELD_TEXT;
    s_pwd_rows[0].checked      = 0u;
    s_pwd_rows[0].enabled      = 1u;
    s_pwd_rows[0].text_buf     = s_pwd_buf;
    s_pwd_rows[0].text_max_len = UI_USER_PASSWORD_MAX;
    s_pwd_rows[0].placeholder  = "";

    s_lay.text_box_w = 285u;
    s_lay.text_box_h = 46u;
}

static void usuario_password_cancel(void)
{
    ui_leaf_keyboard_close(&s_leaf);
    usuario_pwd_clear();
    s_show_error = 0u;
    s_error_msg[0] = '\0';
    s_mode = USR_MODE_LIST;
}

static void usuario_password_try_login(void)
{
    const UiUserRecord* u;
    UiUserResult r;

    if (s_selected < 0) return;

    u = ui_user_store_get_by_index((uint8_t)s_selected);
    if (!u) return;

    r = ui_user_login_by_id(u->id, s_pwd_buf);

    if (r == UI_USER_OK)
    {
        ui_leaf_keyboard_close(&s_leaf);
        usuario_pwd_clear();
        s_show_error = 0u;
        s_error_msg[0] = '\0';
        s_mode = USR_MODE_LIST;
        ui_view_pop();
    }
    else
    {
        s_show_error = 1u;
        snprintf(s_error_msg, sizeof(s_error_msg), "Password incorrecto");
    }
}

/* ------------------------------------------------------------------------- */
/* Draw lista                                                                */
/* ------------------------------------------------------------------------- */

static void usuario_draw_list(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;
    uint8_t total = ui_user_store_count();
    uint8_t i;
    uint16_t row_x, row_y, row_w, row_h;
    uint16_t title_x, title_y;
    char info[80];

    usuario_selection_clamp();

    title_x = 20u;
    title_y = (uint16_t)(L->content_y0 + 10u);

    EVE_color_a(255);
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Inicio de sesion");

//    snprintf(info, sizeof(info), "Usuario activo: %s | total: %u", ui_user_get_active_name(), (unsigned)ui_user_store_count());
//    EVE_cmd_text((int16_t)title_x, (int16_t)(title_y + 34u), 24, EVE_OPT_FLAT, info);

    row_x = 3u;
    row_y = (uint16_t)(L->content_y0 + 60u);
    row_h = 58u;

    if (L->content_x1 > row_x + 95u)
        row_w = (uint16_t)(L->content_x1 - row_x + 5u);
    else
        row_w = 600u;

    for (i = 0u; i < USR_VISIBLE_ROWS; i++)
    {
        uint8_t idx = (uint8_t)(s_scroll + i);
        const UiUserRecord* u;
        uint16_t id;
        uint8_t is_selected;
        uint8_t is_active;
        char line[96];

        if (idx >= total) break;

        u = ui_user_store_get_by_index(idx);
        if (!u) continue;

        id = usuario_row_id_from_visible(i);
        is_selected = ((int8_t)idx == s_selected) ? 1u : 0u;

        is_active = 0u;
        if (ui_user_has_active())
        {
            const UiUserRecord* ua = ui_user_get_active();
            if (ua && ua->id == u->id)
                is_active = 1u;
        }

        if (is_active)
            snprintf(line, sizeof(line), "%u: %s - activo", (unsigned)u->id, u->username);
        else
            snprintf(line, sizeof(line), "%u: %s", (unsigned)u->id, u->username);

        EVE_tag((uint8_t)id);
        ui_rect_register(id, row_x, row_y, row_w, row_h);

        if (is_selected) EVE_color_rgb(BLUE);
        else             EVE_color_rgb(MEDIUM_GRAY);

        EVE_begin(EVE_RECTS);
        EVE_vertex2f(row_x * PRESICION, row_y * PRESICION);
        EVE_vertex2f((row_x + row_w) * PRESICION, (row_y + row_h) * PRESICION);
        EVE_end();

        EVE_color_rgb(BLACK);
        EVE_cmd_text((int16_t)(row_x + 18u), (int16_t)(row_y + 22u), 25, EVE_OPT_FLAT, line);
        EVE_tag(0);

        row_y = (uint16_t)(row_y + row_h + 4u);
    }

    {
        uint8_t need_scroll = (total > USR_VISIBLE_ROWS) ? 1u : 0u;

        if (need_scroll)
        {
            const UiSubmSecondary side4[] =
            {
                { UI_VID_USR_BACK,   "X", 0u },
                { UI_VID_USR_ACCEPT, "",  MEM_ICON_ACEPTAR },
                { UI_VID_USR_UP,     "",  MEM_ICON_FARRIBA },
                { UI_VID_USR_DOWN,   "",  MEM_ICON_FABAJO  },
            };

            ui_subm_draw_side_buttons(710u, 130u, 84u, 84u, 2u, side4, 4u);
        }
        else
        {
            const UiSubmSecondary side2[] =
            {
                { UI_VID_USR_BACK,   "X", 0u },
                { UI_VID_USR_ACCEPT, "",  MEM_ICON_ACEPTAR },
            };

            ui_subm_draw_side_buttons(710u, 130u, 84u, 84u, 2u, side2, 2u);
        }
    }
}

/*  Draw password  */
static void usuario_draw_password(void)
{
    char line[80];
    const UiUserRecord* u = 0;

    if (s_selected >= 0)
        u = ui_user_store_get_by_index((uint8_t)s_selected);

    ui_leaf_draw_form("Password", s_pwd_rows, 1u, &s_leaf, &s_lay, UI_VID_USR_BACK, UI_VID_USR_ACCEPT, 0u, 0u);

    /* ESTA era la parte faltante */
    if (s_leaf.keyboard_open)
    {
        ui_leaf_draw_keyboard(&s_leaf, s_pwd_rows, 1u, &s_lay);
    }

    if (u)
    {
        snprintf(line, sizeof(line), "Usuario: %s", u->username);
        EVE_color_rgb(BLACK);
        EVE_cmd_text(20, 105, 24, EVE_OPT_FLAT, line);
    }

    if (s_show_error)
    {
        EVE_color_rgb(RED);
        EVE_cmd_text(20, 430, 24, EVE_OPT_FLAT, s_error_msg);
    }
}

static void usuario_main_draw(void)
{
    if (s_mode == USR_MODE_PASSWORD)
        usuario_draw_password();
    else
        usuario_draw_list();
}

/* ------------------------------------------------------------------------- */
/* Eventos lista                                                             */
/* ------------------------------------------------------------------------- */

static void usuario_handle_list_release(uint16_t id)
{
    switch (id)
    {
        case UI_VID_USR_BACK:
            ui_view_pop();
            return;

        case UI_VID_USR_ACCEPT:
            if (s_selected >= 0 && s_selected < (int8_t)ui_user_store_count())
            {
                usuario_pwd_clear();
                s_show_error = 0u;
                s_error_msg[0] = '\0';

                usuario_password_prepare();
                s_mode = USR_MODE_PASSWORD;

                /* abrir teclado automáticamente */
                ui_leaf_keyboard_open(&s_leaf, 0);
                s_leaf.kb_numeric = 0u;
            }
            return;

        case UI_VID_USR_UP:
            if (s_selected > 0) s_selected--;
            usuario_selection_clamp();
            return;

        case UI_VID_USR_DOWN:
            if (s_selected < ((int8_t)ui_user_store_count() - 1)) s_selected++;
            usuario_selection_clamp();
            return;

        case UI_VID_USR_ROW0:
            s_selected = (int8_t)(s_scroll + 0);
            usuario_selection_clamp();
            return;

        case UI_VID_USR_ROW1:
            s_selected = (int8_t)(s_scroll + 1);
            usuario_selection_clamp();
            return;

        case UI_VID_USR_ROW2:
            s_selected = (int8_t)(s_scroll + 2);
            usuario_selection_clamp();
            return;

        case UI_VID_USR_ROW3:
            s_selected = (int8_t)(s_scroll + 3);
            usuario_selection_clamp();
            return;

        default:
            return;
    }
}

/* ------------------------------------------------------------------------- */
/* Eventos principales                                                       */
/* ------------------------------------------------------------------------- */

static void usuario_main_on_released(uint16_t id)
{
    int8_t row_idx;
    char c;

    if (s_mode == USR_MODE_PASSWORD)
    {
        if (s_leaf.keyboard_open)
        {
            switch (id)
            {
                case UI_VID_KB_CANCEL:
                    ui_leaf_keyboard_close(&s_leaf);
                    return;

                case UI_VID_KB_OK:
                    usuario_password_try_login();
                    return;

                case UI_VID_KB_BKSP:
                    if (s_leaf.active_row_idx >= 0)
                    {
                        ui_leaf_text_backspace(&s_pwd_rows[s_leaf.active_row_idx]);
                    }
                    return;

                case UI_VID_KB_SPACE:
                    if (s_leaf.active_row_idx >= 0)
                    {
                        ui_leaf_text_append_char(&s_pwd_rows[s_leaf.active_row_idx], ' ');
                    }
                    return;

                case UI_VID_KB_SHIFT:
                    ui_leaf_keyboard_toggle_shift(&s_leaf);
                    return;

                case UI_VID_KB_MODE:
                    ui_leaf_keyboard_toggle_mode(&s_leaf);
                    return;

                default:
                    if (usuario_key_to_char(id, &s_leaf, &c))
                    {
                        if (s_leaf.active_row_idx >= 0)
                        {
                            ui_leaf_text_append_char(&s_pwd_rows[s_leaf.active_row_idx], c);
                        }
                        return;
                    }
                    break;
            }
        }

        switch (id)
        {
            case UI_VID_USR_BACK:
                usuario_password_cancel();
                return;

            case UI_VID_USR_ACCEPT:
                usuario_password_try_login();
                return;

            default:
                break;
        }

        row_idx = usuario_find_pwd_row_by_id(id);
        if (row_idx >= 0)
        {
            ui_leaf_keyboard_open(&s_leaf, row_idx);
            s_leaf.kb_numeric = 0u;
            return;
        }

        return;
    }

    usuario_handle_list_release(id);
}

/* ------------------------------------------------------------------------- */
/* on_enter                                                                  */
/* ------------------------------------------------------------------------- */

static void usuario_on_enter(void)
{
    const UiUserRecord* active = ui_user_get_active();

    s_mode = USR_MODE_LIST;
    s_show_error = 0u;
    s_error_msg[0] = '\0';
    usuario_pwd_clear();

    if (active)
    {
        s_selected = ui_user_store_find_visible_index_by_id(active->id);
        if (s_selected < 0) s_selected = 0;
    }
    else
    {
        s_selected = 0;
    }

    s_scroll = 0;
    usuario_selection_clamp();
}

/* ------------------------------------------------------------------------- */
/* View                                                                      */
/* ------------------------------------------------------------------------- */

const UiView VIEW_USUARIO =
{
    .id = 80,
    .on_enter = usuario_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = usuario_main_draw,
    .on_released = usuario_main_on_released
};
