/*
 * view_cfg_producto_admin_crear.c
 *
 *  Created on: 24 feb. 2026
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
#include "ui_almacena_productos.h"
#include "ui_persist_productos.h"
#include <stdio.h>
#include <string.h>

static UiLeafState s_leaf;

static char s_nombre_producto[21] = "";
static char s_numero_producto[11] = "";
static char s_status_msg[40] = "";
static uint8_t s_last_created_id = 0u;


#define  UI_VID_PROD_CREAR_POP_CANCEL  			 10u
#define	 UI_VID_PROD_CREAR_POP_ACCEPT  			 11u
#define	 UI_VID_PROD_CREAR_FAST_CANCEL 			 12u
#define	 UI_VID_PROD_CREAR_FAST_ACCEPT 			 13u

typedef enum
{
    PROD_CREAR_MODE_FORM = 0,
    PROD_CREAR_MODE_POPUP_APRENDIZAJE,
    PROD_CREAR_MODE_APRENDIZAJE_RAPIDO
} ProdCrearMode;

static ProdCrearMode s_mode = PROD_CREAR_MODE_FORM;

static UiLeafRow s_rows[] =
{
    { UI_VID_PROD_CREAR_ROW0, "Nombre del producto", UI_LEAF_FIELD_TEXT, 0, 1, s_nombre_producto, 20, "" },
    { UI_VID_PROD_CREAR_ROW1, "Numero de producto",  UI_LEAF_FIELD_TEXT, 0, 0, s_numero_producto, 10, "" },
};

#define PROD_CREAR_ROWS_N   ((uint8_t)(sizeof(s_rows) / sizeof(s_rows[0])))

static int8_t prod_crear_find_row_by_id(uint16_t id)
{
    uint8_t i;
    for (i = 0; i < PROD_CREAR_ROWS_N; i++)
    {
        if (s_rows[i].id == id) return (int8_t)i;
    }
    return -1;
}

static uint8_t prod_crear_key_to_char(uint16_t id, UiLeafState* st, char* out_c)
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

static void prod_crear_refresh_next_id(void)
{
    uint8_t next_id = ui_product_store_next_free_id();

    if (next_id == 0u)
    {
        strcpy(s_numero_producto, "LLENO");
    }
    else
    {
        snprintf(s_numero_producto, sizeof(s_numero_producto), "%u", next_id);
    }
}

static void prod_crear_draw_popup_aprendizaje(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t box_x = (uint16_t)(L->footer_x0);
    const uint16_t box_w = 695;
    const uint16_t box_h = 320;

    const uint16_t box_y = (uint16_t)(L->content_y0);

    const uint16_t BTN_W = 94;
    const uint16_t BTN_H = 94;
    const uint16_t BTN_GAP = 5;

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    const UiSubmSecondary side[] =
    {
        { UI_VID_PROD_CREAR_POP_CANCEL, "X"  },
        { UI_VID_PROD_CREAR_POP_ACCEPT, "", MEM_ICON_ACEPTAR },
    };

    EVE_color_rgb(BLUE);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(box_x * PRESICION, box_y * PRESICION);
    EVE_vertex2f((box_x + box_w) * PRESICION, (box_y + box_h) * PRESICION);
    EVE_end();

    EVE_color_rgb(WHITE);
    EVE_cmd_text((int16_t)(box_x + (box_w / 2u)), 250, 25, EVE_OPT_CENTER, "Producto creado - aprendizaje ? ");

    ui_subm_draw_side_buttons(xR0, yR0, BTN_W, BTN_H, BTN_GAP, side, 2);
}

static void prod_crear_draw_aprendizaje_rapido(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t BTN_W = 94;
    const uint16_t BTN_H = 94;
    const uint16_t BTN_GAP = 5;

    const uint16_t box_w = 695;
    const uint16_t box_h = 270;
    const uint16_t box_x = (uint16_t)(L->footer_x0);

    const uint16_t box_y = (uint16_t)(L->content_y0 + 50);

    const uint16_t xR0 = (uint16_t)(L->content_x1);
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    const UiSubmSecondary side[] =
    {
        { UI_VID_PROD_CREAR_FAST_CANCEL, "X"                    },
        { UI_VID_PROD_CREAR_FAST_ACCEPT, "", MEM_ICON_INIMANUAL },
    };

    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(20, (int16_t)(L->content_y0 + 10), PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Aprendizaje rapido");

    EVE_color_rgb(BLUE);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(box_x * PRESICION, box_y * PRESICION);
    EVE_vertex2f((box_x + box_w) * PRESICION, (box_y + box_h) * PRESICION);
    EVE_end();

    EVE_color_rgb(WHITE);
    EVE_cmd_text(20, 240, 25, EVE_OPT_FLAT, "Preparar productos");
    EVE_cmd_text(20, 290, 25, EVE_OPT_FLAT, "e iniciar aprendizaje");

    ui_subm_draw_side_buttons(xR0, yR0, BTN_W, BTN_H, BTN_GAP, side, 2);
}

static void opciones_producto_admin_crear_on_enter(void)
{
    ui_leaf_state_reset(&s_leaf, 4);

    s_nombre_producto[0] = '\0';
    s_numero_producto[0] = '\0';
    s_status_msg[0] = '\0';
    s_last_created_id = 0u;

    prod_crear_refresh_next_id();

    s_mode = PROD_CREAR_MODE_FORM;
}

static void opciones_producto_adm_crear_draw(void)
{
    UiLeafLayout lay;

    ui_leaf_layout_default(&lay, &UI_LAYOUT_MAIN);

    lay.text_box_w = 285;
    lay.text_box_h = 46;

    switch (s_mode)
    {
        case PROD_CREAR_MODE_FORM:
            ui_leaf_draw_form("Crear producto", s_rows, PROD_CREAR_ROWS_N, &s_leaf, &lay, UI_VID_OPT_PROD_LEAF_BACK, UI_VID_OPT_PROD_LEAF_ACCEPT, 0, 0);

            if (s_leaf.keyboard_open)
            {
                ui_leaf_draw_keyboard(&s_leaf, s_rows, PROD_CREAR_ROWS_N, &lay);
            }

            if (s_status_msg[0] != '\0')
            {
                EVE_color_rgb(BLACK);
                EVE_cmd_text(360, 430, USER_FONT_SIZE, EVE_OPT_CENTER, s_status_msg);
            }
            break;

        case PROD_CREAR_MODE_POPUP_APRENDIZAJE:
            prod_crear_draw_popup_aprendizaje();
            break;

        case PROD_CREAR_MODE_APRENDIZAJE_RAPIDO:
            prod_crear_draw_aprendizaje_rapido();
            break;

        default:
            break;
    }
}

static void opciones_producto_admin_crear_on_released(uint16_t id)
{
    int8_t row_idx;
    char c;

    if (s_mode == PROD_CREAR_MODE_POPUP_APRENDIZAJE)
    {
        switch (id)
        {
            case UI_VID_PROD_CREAR_POP_CANCEL:
                ui_view_pop();
                return;

            case UI_VID_PROD_CREAR_POP_ACCEPT:
                s_mode = PROD_CREAR_MODE_APRENDIZAJE_RAPIDO;
                return;

            default:
                return;
        }
    }

    if (s_mode == PROD_CREAR_MODE_APRENDIZAJE_RAPIDO)
    {
        switch (id)
        {
            case UI_VID_PROD_CREAR_FAST_CANCEL:
                ui_view_pop();
                return;

            case UI_VID_PROD_CREAR_FAST_ACCEPT:
                /* Por el momento no hace nada */
                return;

            default:
                return;
        }
    }

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
                if (s_leaf.active_row_idx >= 0 && s_leaf.active_row_idx < PROD_CREAR_ROWS_N)
                {
                    ui_leaf_text_backspace(&s_rows[s_leaf.active_row_idx]);
                }
                return;

            case UI_VID_KB_SPACE:
                if (s_leaf.active_row_idx >= 0 && s_leaf.active_row_idx < PROD_CREAR_ROWS_N)
                {
                    if (s_leaf.active_row_idx != 1)
                    {
                        ui_leaf_text_append_char(&s_rows[s_leaf.active_row_idx], ' ');
                    }
                }
                return;

            case UI_VID_KB_SHIFT:
                ui_leaf_keyboard_toggle_shift(&s_leaf);
                return;

            case UI_VID_KB_MODE:
                ui_leaf_keyboard_toggle_mode(&s_leaf);
                return;

            default:
                if (prod_crear_key_to_char(id, &s_leaf, &c))
                {
                    if (s_leaf.active_row_idx >= 0 && s_leaf.active_row_idx < PROD_CREAR_ROWS_N)
                    {
                        if (s_leaf.active_row_idx == 1)
                        {
                            if (c >= '0' && c <= '9')
                            {
                                ui_leaf_text_append_char(&s_rows[s_leaf.active_row_idx], c);
                            }
                        }
                        else
                        {
                            ui_leaf_text_append_char(&s_rows[s_leaf.active_row_idx], c);
                        }
                    }
                    return;
                }
                break;
        }
    }

    switch (id)
    {
        case UI_VID_OPT_PROD_LEAF_BACK:
            ui_view_pop();
            return;

        case UI_VID_OPT_PROD_LEAF_ACCEPT:
        {
            UiProductResult rc;
            uint8_t new_id = 0u;
            int8_t persist_rc;

            rc = ui_product_store_add_autoid(s_nombre_producto, &new_id);

            if (rc == UI_PROD_OK)
            {
                s_last_created_id = new_id;
                s_status_msg[0] = '\0';

                ui_leaf_keyboard_close(&s_leaf);

                s_nombre_producto[0] = '\0';
                prod_crear_refresh_next_id();

                s_mode = PROD_CREAR_MODE_POPUP_APRENDIZAJE;
                return;
            }
            else if (rc == UI_PROD_ERR_EMPTY_NAME)
            {
                strcpy(s_status_msg, "Error - Nombre vacio");
                return;
            }
            else if (rc == UI_PROD_ERR_FULL)
            {
                strcpy(s_status_msg, "Limite de 30 productos");
                return;
            }

            strcpy(s_status_msg, "Error al crear");
            return;
        }

        default:
            break;
    }

    row_idx = prod_crear_find_row_by_id(id);
    if (row_idx >= 0)
    {
        if (row_idx == 1)
        {
            /* fila solo informativa: no abrir teclado */
            return;
        }

        ui_leaf_keyboard_open(&s_leaf, row_idx);
        s_leaf.kb_numeric = 0u;
        return;
    }
}

const UiView VIEW_CFG_PROD_ADMIN_CREAR =
{
    .id = 10,
    .on_enter = opciones_producto_admin_crear_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_producto_adm_crear_draw,
    .on_released = opciones_producto_admin_crear_on_released
};
