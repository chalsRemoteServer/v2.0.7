/*
 * view_cfg_sistema_admusers_crear.c
 *
 *  Created on: 25 feb. 2026
 *      Author: oscar
 */

#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_ids.h"
#include "ui_view_mgr.h"
#include "ui_leaf_form.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"

#include <stdint.h>

static UiLeafState s_leaf;

static char s_nombre_usuario[21]   = "";
static char s_numero_nivelUser[11] = "";
static char s_numero_metodo[11]    = "";
static char s_nombre_password[21]  = "";

static UiLeafRow s_rows[] =
{
    { UI_VID_SISTEMA_CREAR_ROW0, "Nombre de usuario"      , UI_LEAF_FIELD_TEXT, 0, 1, s_nombre_usuario  , 20, "" },
    { UI_VID_SISTEMA_CREAR_ROW1, "Nivel de usuario"       , UI_LEAF_FIELD_TEXT, 0, 1, s_numero_nivelUser, 10, "" },
    { UI_VID_SISTEMA_CREAR_ROW2, "Metodo de autorizacion" , UI_LEAF_FIELD_TEXT, 0, 1, s_numero_metodo   , 10, "" },
    { UI_VID_SISTEMA_CREAR_ROW3, "Contrasena"             , UI_LEAF_FIELD_TEXT, 0, 1, s_nombre_password , 20, "" },
};

#define SIST_CREAR_ROWS_N   ((uint8_t)(sizeof(s_rows) / sizeof(s_rows[0])))

static int8_t sist_crear_find_row_by_id(uint16_t id)
{
    uint8_t i;

    for (i = 0u; i < SIST_CREAR_ROWS_N; i++)
    {
        if (s_rows[i].id == id) return (int8_t)i;
    }

    return -1;
}

static uint8_t sist_crear_row_uses_numeric(int8_t row_idx)
{
    if (row_idx == 1) return 1u; /* Nivel de usuario */
    if (row_idx == 2) return 1u; /* Metodo de autorizacion */
    return 0u;
}

static uint8_t sist_crear_row_allows_space(int8_t row_idx)
{
    if (row_idx == 1) return 0u;
    if (row_idx == 2) return 0u;
    return 1u;
}

static char sist_crear_key_to_char(uint16_t id, uint8_t shift)
{
    switch (id)
    {
        case UI_VID_KB_Q: return shift ? 'Q' : 'q';
        case UI_VID_KB_W: return shift ? 'W' : 'w';
        case UI_VID_KB_E: return shift ? 'E' : 'e';
        case UI_VID_KB_R: return shift ? 'R' : 'r';
        case UI_VID_KB_T: return shift ? 'T' : 't';
        case UI_VID_KB_Y: return shift ? 'Y' : 'y';
        case UI_VID_KB_U: return shift ? 'U' : 'u';
        case UI_VID_KB_I: return shift ? 'I' : 'i';
        case UI_VID_KB_O: return shift ? 'O' : 'o';
        case UI_VID_KB_P: return shift ? 'P' : 'p';

        case UI_VID_KB_A: return shift ? 'A' : 'a';
        case UI_VID_KB_S: return shift ? 'S' : 's';
        case UI_VID_KB_D: return shift ? 'D' : 'd';
        case UI_VID_KB_F: return shift ? 'F' : 'f';
        case UI_VID_KB_G: return shift ? 'G' : 'g';
        case UI_VID_KB_H: return shift ? 'H' : 'h';
        case UI_VID_KB_J: return shift ? 'J' : 'j';
        case UI_VID_KB_K: return shift ? 'K' : 'k';
        case UI_VID_KB_L: return shift ? 'L' : 'l';

        case UI_VID_KB_Z: return shift ? 'Z' : 'z';
        case UI_VID_KB_X: return shift ? 'X' : 'x';
        case UI_VID_KB_C: return shift ? 'C' : 'c';
        case UI_VID_KB_V: return shift ? 'V' : 'v';
        case UI_VID_KB_B: return shift ? 'B' : 'b';
        case UI_VID_KB_N: return shift ? 'N' : 'n';
        case UI_VID_KB_M: return shift ? 'M' : 'm';

        case UI_VID_KB_1: return '1';
        case UI_VID_KB_2: return '2';
        case UI_VID_KB_3: return '3';
        case UI_VID_KB_4: return '4';
        case UI_VID_KB_5: return '5';
        case UI_VID_KB_6: return '6';
        case UI_VID_KB_7: return '7';
        case UI_VID_KB_8: return '8';
        case UI_VID_KB_9: return '9';
        case UI_VID_KB_0: return '0';

        default:
            return 0;
    }
}

static uint8_t sist_crear_append_to_active(char c)
{
    if (s_leaf.active_row_idx < 0) return 0u;
    if (s_leaf.active_row_idx >= (int8_t)SIST_CREAR_ROWS_N) return 0u;

    if (sist_crear_row_uses_numeric(s_leaf.active_row_idx))
    {
        if (c < '0' || c > '9') return 0u;
    }

    return ui_leaf_text_append_char(&s_rows[s_leaf.active_row_idx], c);
}

static void sist_crear_backspace_active(void)
{
    if (s_leaf.active_row_idx < 0) return;
    if (s_leaf.active_row_idx >= (int8_t)SIST_CREAR_ROWS_N) return;

    ui_leaf_text_backspace(&s_rows[s_leaf.active_row_idx]);
}

static void sist_crear_open_keyboard_for_row(int8_t row_idx)
{
    ui_leaf_keyboard_open(&s_leaf, row_idx);

    if (sist_crear_row_uses_numeric(row_idx))
        s_leaf.kb_numeric = 1u;
    else
        s_leaf.kb_numeric = 0u;
}

static void opciones_sistema_admusers_crear_on_enter(void)
{
    ui_leaf_state_reset(&s_leaf, 4u);

    s_nombre_usuario[0]   = '\0';
    s_numero_nivelUser[0] = '\0';
    s_numero_metodo[0]    = '\0';
    s_nombre_password[0]  = '\0';
}

static void opciones_sistema_admusers_crear_draw(void)
{
    UiLeafLayout lay;

    ui_leaf_layout_default(&lay, &UI_LAYOUT_MAIN);
    lay.text_box_w = 250;
    lay.text_box_h = 42;

    ui_leaf_draw_form("Crear usuario", s_rows, SIST_CREAR_ROWS_N, &s_leaf, &lay, UI_VID_SISTEMA_CREAR_BACK, UI_VID_SISTEMA_CREAR_ACCEPT, 0, 0);

    if (s_leaf.keyboard_open)
    {
        ui_leaf_draw_keyboard(&s_leaf, s_rows, SIST_CREAR_ROWS_N, &lay);
    }
}

static void opciones_sistema_admusers_crear_on_released(uint16_t id)
{
    int8_t row_idx;
    char c = 0;

    if (s_leaf.keyboard_open)
    {
        switch (id)
        {
            case UI_VID_KB_CANCEL:
                ui_leaf_keyboard_close(&s_leaf);
                return;

            case UI_VID_KB_OK:
                ui_leaf_keyboard_close(&s_leaf);
                return;

            case UI_VID_KB_BKSP:
                sist_crear_backspace_active();
                return;

            case UI_VID_KB_SPACE:
                if (s_leaf.active_row_idx >= 0 &&
                    s_leaf.active_row_idx < (int8_t)SIST_CREAR_ROWS_N &&
                    sist_crear_row_allows_space(s_leaf.active_row_idx))
                {
                    ui_leaf_text_append_char(&s_rows[s_leaf.active_row_idx], ' ');
                }
                return;

            case UI_VID_KB_SHIFT:
                if (!s_leaf.kb_numeric)
                {
                    ui_leaf_keyboard_toggle_shift(&s_leaf);
                }
                return;

            case UI_VID_KB_MODE:
                ui_leaf_keyboard_toggle_mode(&s_leaf);
                return;

            default:
                break;
        }

        c = sist_crear_key_to_char(id, s_leaf.kb_shift);
        if (c != 0)
        {
            sist_crear_append_to_active(c);
            return;
        }
    }

    switch (id)
    {
        case UI_VID_SISTEMA_CREAR_BACK:
            ui_leaf_keyboard_close(&s_leaf);
            ui_view_pop();
            return;

        case UI_VID_SISTEMA_CREAR_ACCEPT:
            ui_leaf_keyboard_close(&s_leaf);
            ui_view_pop();
            return;

        default:
            break;
    }

    row_idx = sist_crear_find_row_by_id(id);
    if (row_idx >= 0)
    {
        sist_crear_open_keyboard_for_row(row_idx);
        return;
    }
}

const UiView VIEW_OPCIONES_SISTEMA_ADMINUSERS_CREAR =
{
    .id = 27,
    .on_enter = opciones_sistema_admusers_crear_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_sistema_admusers_crear_draw,
    .on_released = opciones_sistema_admusers_crear_on_released
};
