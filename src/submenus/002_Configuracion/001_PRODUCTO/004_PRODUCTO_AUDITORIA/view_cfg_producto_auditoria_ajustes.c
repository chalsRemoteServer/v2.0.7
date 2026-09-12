/*
 * view_cfg_producto_auditoria_ajustes.c
 *
 *  Created on: 24 feb. 2026
 *      Author: oscar
 */

/*
 * view_cfg_producto_auditoria_ajustes.c
 *
 *  Created on: 24 feb. 2026
 *      Author: oscar
 */

#include "ui_submenu_widgets.h"
#include "ui_layout.h"
#include "ui_view_mgr.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "ui_views.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* =========================================================================
 * IDs LOCALES
 * ========================================================================= */

/* Popup modo */
#define UI_VID_PROD_AUD_AJ_POP_ROW0        10
#define UI_VID_PROD_AUD_AJ_POP_ROW1        11
#define UI_VID_PROD_AUD_AJ_POP_ROW2        12
#define UI_VID_PROD_AUD_AJ_POP_ROW3        13

/* Main */
#define UI_VID_PROD_AUD_AJ_ROW_MODO        14
#define UI_VID_PROD_AUD_AJ_ROW_TPREP       15

#define UI_VID_PROD_AUD_AJ_BACK            16
#define UI_VID_PROD_AUD_AJ_ACCEPT          17

/* Popup modo - botones laterales */
#define UI_VID_PROD_AUD_AJ_POP_BACK        18
#define UI_VID_PROD_AUD_AJ_POP_ACCEPT      19
#define UI_VID_PROD_AUD_AJ_POP_UP          20
#define UI_VID_PROD_AUD_AJ_POP_DOWN        21

/* Popup tiempo - keypad */
#define UI_VID_PROD_AUD_AJ_TP_NUM7         22
#define UI_VID_PROD_AUD_AJ_TP_NUM8         23
#define UI_VID_PROD_AUD_AJ_TP_NUM9         24

#define UI_VID_PROD_AUD_AJ_TP_NUM4         25
#define UI_VID_PROD_AUD_AJ_TP_NUM5         26
#define UI_VID_PROD_AUD_AJ_TP_NUM6         27

#define UI_VID_PROD_AUD_AJ_TP_NUM1         28
#define UI_VID_PROD_AUD_AJ_TP_NUM2         29
#define UI_VID_PROD_AUD_AJ_TP_NUM3         30

#define UI_VID_PROD_AUD_AJ_TP_NUM0         31

/* Popup tiempo - controles */
#define UI_VID_PROD_AUD_AJ_TP_UP           32
#define UI_VID_PROD_AUD_AJ_TP_DOWN         33
#define UI_VID_PROD_AUD_AJ_TP_CLEAR        34
#define UI_VID_PROD_AUD_AJ_TP_BACK         35
#define UI_VID_PROD_AUD_AJ_TP_ACCEPT       36

#define PROD_AUD_AJ_VISIBLE_POP_ROWS       4

/* =========================================================================
 * DATOS
 * ========================================================================= */

static const char* s_modos[] =
{
    "Desactivado",
    "Cada hora",
    "A diario",
    "Intervalo",
    "Externo",
    "Intervalo y cambio de producto",
    "Cambio de producto",
};

#define PROD_AUD_AJ_MODOS_N   ((uint8_t)(sizeof(s_modos) / sizeof(s_modos[0])))

/* Estado de pantalla interna */
typedef enum
{
    PROD_AUD_AJ_SCREEN_MAIN = 0,
    PROD_AUD_AJ_SCREEN_POP_MODO,
    PROD_AUD_AJ_SCREEN_POP_TPREP
} ProdAudAjScreen;

static ProdAudAjScreen s_screen = PROD_AUD_AJ_SCREEN_MAIN;

/* Modo */
static int8_t  s_modo_actual = 4;              /* Externo por defecto */
static int8_t  s_modo_popup_selected = 4;
static int16_t s_popup_scroll = 0;

/* Tiempo preparación */
static uint8_t s_tprep_value = 0u;             /* valor confirmado: 0 = vacío/no definido */
static uint8_t s_tprep_edit_value = 0u;        /* valor temporal mientras popup está abierto */
static char    s_tprep_digits[3] = "";         /* máximo 2 dígitos + '\0' */
static char    s_tprep_text[24] = "";          /* texto mostrado en fila principal */

/* Estado general */
static char s_status_msg[50] = "";

/* =========================================================================
 * HELPERS GENERALES
 * ========================================================================= */

static void prod_aud_aj_clamp_scroll(void)
{
    int16_t maxScroll = (int16_t)PROD_AUD_AJ_MODOS_N - (int16_t)PROD_AUD_AJ_VISIBLE_POP_ROWS;

    if (maxScroll < 0) maxScroll = 0;
    if (s_popup_scroll < 0) s_popup_scroll = 0;
    if (s_popup_scroll > maxScroll) s_popup_scroll = maxScroll;
}

static uint16_t prod_aud_aj_popup_row_id_from_visible(uint8_t vis)
{
    switch (vis)
    {
        case 0: return UI_VID_PROD_AUD_AJ_POP_ROW0;
        case 1: return UI_VID_PROD_AUD_AJ_POP_ROW1;
        case 2: return UI_VID_PROD_AUD_AJ_POP_ROW2;
        case 3: return UI_VID_PROD_AUD_AJ_POP_ROW3;
        default: return 0;
    }
}

static int8_t prod_aud_aj_visible_from_popup_row_id(uint16_t id)
{
    switch (id)
    {
        case UI_VID_PROD_AUD_AJ_POP_ROW0: return 0;
        case UI_VID_PROD_AUD_AJ_POP_ROW1: return 1;
        case UI_VID_PROD_AUD_AJ_POP_ROW2: return 2;
        case UI_VID_PROD_AUD_AJ_POP_ROW3: return 3;
        default: return -1;
    }
}

static void prod_aud_aj_draw_textbox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* txt)
{
    EVE_color_rgb(WHITE);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);
    EVE_end();

    EVE_color_rgb(0x707070);
    EVE_begin(EVE_LINES);

    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, y * PRESICION);

    EVE_vertex2f(x * PRESICION, (y + h) * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);

    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f(x * PRESICION, (y + h) * PRESICION);

    EVE_vertex2f((x + w) * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);

    EVE_end();

    EVE_color_rgb(BLACK);
    EVE_cmd_text((int16_t)(x + 10), (int16_t)(y + 10), 24, EVE_OPT_FLAT, txt ? txt : "");
}

//static void prod_aud_aj_draw_fill_button(uint16_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* txt, uint8_t fill_color)
static void prod_aud_aj_draw_fill_button(uint16_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* txt, int fill_color)

{
    EVE_tag((uint8_t)id);

    EVE_color_rgb(fill_color);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);
    EVE_end();

    EVE_color_rgb(WHITE);
    EVE_begin(EVE_LINES);

    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, y * PRESICION);

    EVE_vertex2f(x * PRESICION, (y + h) * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);

    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f(x * PRESICION, (y + h) * PRESICION);

    EVE_vertex2f((x + w) * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);

    EVE_end();

    EVE_color_rgb(WHITE);
    EVE_cmd_text((int16_t)(x + w / 2u), (int16_t)(y + h / 2u), USER_FONT_SIZE, EVE_OPT_CENTER, txt ? txt : "");

    EVE_tag(0);
}


static void prod_aud_aj_draw_icon_button(uint16_t id, uint32_t icon_addr, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    EVE_tag((uint8_t)id);

    EVE_color_rgb(BLUE);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);
    EVE_end();


    EVE_begin(EVE_BITMAPS);
    EVE_cmd_setbitmap(icon_addr, EVE_ARGB1555, 64, 64);
    EVE_vertex2f((x + (w - 64u) / 2u) * PRESICION, (y + (h - 64u) / 2u) * PRESICION);
    EVE_end();

    EVE_tag(0);
}


/* =========================================================================
 * HELPERS TIEMPO
 * ========================================================================= */

static void prod_aud_aj_update_tprep_main_text(void)
{
    if (s_tprep_value >= 1u && s_tprep_value <= 30u)
        snprintf(s_tprep_text, sizeof(s_tprep_text), "%u min", (unsigned)s_tprep_value);
    else
        s_tprep_text[0] = '\0';
}

static void prod_aud_aj_sync_digits_from_edit_value(void)
{
    if (s_tprep_edit_value >= 1u && s_tprep_edit_value <= 30u)
        snprintf(s_tprep_digits, sizeof(s_tprep_digits), "%u", (unsigned)s_tprep_edit_value);
    else
        s_tprep_digits[0] = '\0';
}

static void prod_aud_aj_rebuild_edit_value_from_digits(void)
{
    int v;

    if (s_tprep_digits[0] == '\0')
    {
        s_tprep_edit_value = 0u;
        return;
    }

    v = atoi(s_tprep_digits);

    if (v < 1 || v > 30)
    {
        s_tprep_edit_value = 0u;
        return;
    }

    s_tprep_edit_value = (uint8_t)v;
}

static void prod_aud_aj_tprep_clear(void)
{
    s_tprep_digits[0] = '\0';
    s_tprep_edit_value = 0u;
}

static void prod_aud_aj_tprep_append_digit(char c)
{
    uint16_t len;
    char tmp[3];
    int v;

    if (c < '0' || c > '9') return;

    len = (uint16_t)strlen(s_tprep_digits);
    if (len >= 2u) return;

    strcpy(tmp, s_tprep_digits);
    tmp[len] = c;
    tmp[len + 1u] = '\0';

    v = atoi(tmp);

    if (v < 0 || v > 30)
        return;

    strcpy(s_tprep_digits, tmp);
    prod_aud_aj_rebuild_edit_value_from_digits();
}

static void prod_aud_aj_tprep_inc(void)
{
    if (s_tprep_edit_value == 0u)
        s_tprep_edit_value = 1u;
    else if (s_tprep_edit_value < 30u)
        s_tprep_edit_value++;

    prod_aud_aj_sync_digits_from_edit_value();
}

static void prod_aud_aj_tprep_dec(void)
{
    if (s_tprep_edit_value > 1u)
        s_tprep_edit_value--;
    else if (s_tprep_edit_value == 1u)
        s_tprep_edit_value = 1u;
    else
        return;

    prod_aud_aj_sync_digits_from_edit_value();
}

static void prod_aud_aj_open_popup_modo(void)
{
    s_screen = PROD_AUD_AJ_SCREEN_POP_MODO;
    s_modo_popup_selected = s_modo_actual;

    if (s_modo_popup_selected < 0)
        s_modo_popup_selected = 0;

    s_popup_scroll = s_modo_popup_selected;
    prod_aud_aj_clamp_scroll();
}

static void prod_aud_aj_close_popup_modo(void)
{
    s_screen = PROD_AUD_AJ_SCREEN_MAIN;
    s_popup_scroll = 0;
}

static void prod_aud_aj_open_popup_tprep(void)
{
    s_screen = PROD_AUD_AJ_SCREEN_POP_TPREP;
    s_tprep_edit_value = s_tprep_value;
    prod_aud_aj_sync_digits_from_edit_value();
    s_status_msg[0] = '\0';
}

static void prod_aud_aj_close_popup_tprep_cancel(void)
{
    s_screen = PROD_AUD_AJ_SCREEN_MAIN;
    s_tprep_edit_value = 0u;
    s_tprep_digits[0] = '\0';
    s_status_msg[0] = '\0';
}

static uint8_t prod_aud_aj_accept_popup_tprep(void)
{
    if (s_tprep_edit_value < 1u || s_tprep_edit_value > 30u)
    {
        snprintf(s_status_msg, sizeof(s_status_msg), "Selecciona un valor entre 1 y 30 min");
        return 0u;
    }

    s_tprep_value = s_tprep_edit_value;
    prod_aud_aj_update_tprep_main_text();

    s_tprep_edit_value = 0u;
    s_tprep_digits[0] = '\0';
    s_status_msg[0] = '\0';
    s_screen = PROD_AUD_AJ_SCREEN_MAIN;

    return 1u;
}

static void prod_aud_aj_get_tprep_current_text(char* out, uint16_t out_sz)
{
    if (!out || out_sz == 0u) return;

    if (s_tprep_edit_value >= 1u && s_tprep_edit_value <= 30u)
        snprintf(out, out_sz, "%u min", (unsigned)s_tprep_edit_value);
    else
        snprintf(out, out_sz, "-- min");
}

/* =========================================================================
 * ENTER
 * ========================================================================= */

static void opciones_producto_auditoria_ajustes_on_enter(void)
{
    s_screen = PROD_AUD_AJ_SCREEN_MAIN;
    s_popup_scroll = 0;
    s_status_msg[0] = '\0';

    if (s_modo_actual < 0 || s_modo_actual >= (int8_t)PROD_AUD_AJ_MODOS_N)
        s_modo_actual = 4;

    if (s_tprep_value == 0u)
        s_tprep_value = 1u;

    prod_aud_aj_update_tprep_main_text();
}

/* =========================================================================
 * DRAW MAIN
 * ========================================================================= */

static void prod_aud_aj_draw_main(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t title_x = 20;
    const uint16_t title_y = 140;

    const uint16_t row_x   = 3;
    const uint16_t row_y0  = 190;
    const uint16_t row_w   = 700;
    const uint16_t row_h   = 62;
    const uint16_t row_gap = 5;

    const uint16_t label_x = row_x + 18;
    const uint16_t text_y_off = 21;

    const uint16_t box_w = 265;
    const uint16_t box_h = 58;
    const uint16_t box_x = (uint16_t)(row_x + row_w - box_w - 12);

    const uint16_t y0 = row_y0;
    const uint16_t y1 = (uint16_t)(row_y0 + row_h + row_gap);

    const uint16_t BTN = 84;
    const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    EVE_color_a(255);
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Ajustes");

    /* Fila 1: modo */
    EVE_color_rgb(MEDIUM_GRAY);
    EVE_tag(UI_VID_PROD_AUD_AJ_ROW_MODO);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(row_x * PRESICION, y0 * PRESICION);
    EVE_vertex2f((row_x + row_w) * PRESICION, (y0 + row_h) * PRESICION);
    EVE_end();
    EVE_tag(0);

    EVE_color_rgb(BLACK);
    EVE_cmd_text((int16_t)label_x, (int16_t)(y0 + text_y_off), 25, EVE_OPT_FLAT, "Modo");
    prod_aud_aj_draw_textbox(box_x, (uint16_t)(y0 + (row_h - box_h) / 2u), box_w, box_h, s_modos[s_modo_actual]);

    /* Fila 2: tiempo preparación */
    EVE_color_rgb(MEDIUM_GRAY);
    EVE_tag(UI_VID_PROD_AUD_AJ_ROW_TPREP);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(row_x * PRESICION, y1 * PRESICION);
    EVE_vertex2f((row_x + row_w) * PRESICION, (y1 + row_h) * PRESICION);
    EVE_end();
    EVE_tag(0);

    EVE_color_rgb(BLACK);
    EVE_cmd_text((int16_t)label_x, (int16_t)(y1 + text_y_off), 25, EVE_OPT_FLAT, "Tiempo para preparacion");
    prod_aud_aj_draw_textbox(box_x, (uint16_t)(y1 + (row_h - box_h) / 2u), box_w, box_h, s_tprep_text);

    {
        const UiSubmSecondary side[] =
        {
            { UI_VID_PROD_AUD_AJ_BACK,   "X" },
            { UI_VID_PROD_AUD_AJ_ACCEPT, "", MEM_ICON_ACEPTAR },
        };

        ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 3, side, 2);
    }

    if (s_status_msg[0] != '\0')
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text(350, 430, 27, EVE_OPT_CENTER, s_status_msg);
    }
}

/* =========================================================================
 * DRAW POPUP MODO
 * ========================================================================= */

static void prod_aud_aj_draw_popup_modo(void)
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
    const uint16_t text_y  = 18;

    uint8_t i;

    const uint16_t BTN = 84;
    const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Modo");

    prod_aud_aj_clamp_scroll();

    for (i = 0; i < PROD_AUD_AJ_VISIBLE_POP_ROWS; i++)
    {
        uint8_t idx = (uint8_t)(s_popup_scroll + i);
        uint16_t y;
        uint8_t is_selected = 0u;

        if (idx >= PROD_AUD_AJ_MODOS_N) break;

        y = (uint16_t)(table_y + i * (row_h + row_gap));

        if (s_modo_popup_selected == (int8_t)idx)
            is_selected = 1u;

        if (is_selected)
            EVE_color_rgb(GREEN);
        else
            EVE_color_rgb(MEDIUM_GRAY);

        EVE_tag((uint8_t)prod_aud_aj_popup_row_id_from_visible(i));
        EVE_begin(EVE_RECTS);
        EVE_vertex2f(table_x * PRESICION, y * PRESICION);
        EVE_vertex2f((table_x + table_w) * PRESICION, (y + row_h - 6) * PRESICION);
        EVE_end();
        EVE_tag(0);

        EVE_color_rgb(BLACK);
        EVE_cmd_text((int16_t)(table_x + pad_x), (int16_t)(y + text_y), USER_FONT_SIZE, EVE_OPT_FLAT, s_modos[idx]);
    }

    {
        const UiSubmSecondary side[] =
        {
            { UI_VID_PROD_AUD_AJ_POP_BACK,   "X" },
            { UI_VID_PROD_AUD_AJ_POP_ACCEPT, "", MEM_ICON_ACEPTAR },
            { UI_VID_PROD_AUD_AJ_POP_UP,     "", MEM_ICON_FARRIBA },
            { UI_VID_PROD_AUD_AJ_POP_DOWN,   "", MEM_ICON_FABAJO  },
        };

        ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 3, side, 4);
    }
}

/* =========================================================================
 * DRAW POPUP TIEMPO
 * ========================================================================= */

static void prod_aud_aj_draw_popup_tprep(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;
    char cur_txt[24];

    const uint16_t title_x = 20;
    const uint16_t title_y = 140;

    /* keypad izquierda */
    const uint16_t key_x0 = 10;
    const uint16_t key_y0 = 195;
    const uint16_t key_w  = 84;
    const uint16_t key_h  = 60;
    const uint16_t key_gap_x = 5;
    const uint16_t key_gap_y = 5;

    /* bloque derecho */
    const uint16_t ctrl_x = (uint16_t)(key_x0 + 3u * key_w + 2u * key_gap_x + 30u);
    const uint16_t ctrl_btn = 84;
    const uint16_t ctrl_text_x_gap = 10;
    const uint16_t ctrl_clear_gap = 30;
    const uint16_t top_y = key_y0;
    const uint16_t bottom_y = (uint16_t)(top_y + 160u);

    const uint16_t clear_btn_w = 110;
    const uint16_t clear_btn_h = 60;

    const uint16_t BTN = 84;
    const uint16_t xR0 = (uint16_t)(L->content_x1) + 10;
    const uint16_t yR0 = (uint16_t)(L->content_y0);

    prod_aud_aj_get_tprep_current_text(cur_txt, sizeof(cur_txt));

    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Tiempo maximo de ejecucion");

    /* keypad */
   // EVE_color_rgb(BLUE);
   // static void prod_aud_aj_draw_fill_button(uint16_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* txt, uint8_t fill_color)

    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM7, key_x0,                               key_y0,                               key_w, key_h,            "7", BLUE);
    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM8, (uint16_t)(key_x0 + key_w + key_gap_x), key_y0,                               key_w, key_h,          "8", BLUE);
    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM9, (uint16_t)(key_x0 + 2u * (key_w + key_gap_x)), key_y0,                         key_w, key_h,         "9", BLUE);

    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM4, key_x0,                               (uint16_t)(key_y0 + key_h + key_gap_y), key_w, key_h,          "4", BLUE);
    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM5, (uint16_t)(key_x0 + key_w + key_gap_x), (uint16_t)(key_y0 + key_h + key_gap_y), key_w, key_h,        "5", BLUE);
    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM6, (uint16_t)(key_x0 + 2u * (key_w + key_gap_x)), (uint16_t)(key_y0 + key_h + key_gap_y), key_w, key_h, "6", BLUE);

    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM1, key_x0,                               (uint16_t)(key_y0 + 2u * (key_h + key_gap_y)), key_w, key_h,   "1", BLUE);
    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM2, (uint16_t)(key_x0 + key_w + key_gap_x), (uint16_t)(key_y0 + 2u * (key_h + key_gap_y)), key_w, key_h, "2", BLUE);
    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM3, (uint16_t)(key_x0 + 2u * (key_w + key_gap_x)), (uint16_t)(key_y0 + 2u * (key_h + key_gap_y)), key_w, key_h, "3", BLUE);

    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_NUM0, (uint16_t)(key_x0 + key_w + key_gap_x), (uint16_t)(key_y0 + 3u * (key_h + key_gap_y)), key_w, key_h, "0", BLUE);

   //static void prod_aud_aj_draw_icon_button(uint16_t id, uint32_t icon_addr, uint16_t x, uint16_t y, uint16_t w, uint16_t h)

    /* Flecha arriba */
    prod_aud_aj_draw_icon_button(UI_VID_PROD_AUD_AJ_TP_UP, MEM_ICON_FARRIBA, ctrl_x, top_y, ctrl_btn, ctrl_btn);

    EVE_color_rgb(BLACK);
    EVE_cmd_text((int16_t)(ctrl_x + ctrl_btn + ctrl_text_x_gap), (int16_t)(top_y + 26), PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "30 min");

    /* Borrar */
    prod_aud_aj_draw_fill_button(UI_VID_PROD_AUD_AJ_TP_CLEAR,
                                 (uint16_t)(ctrl_x + ctrl_btn + ctrl_text_x_gap + 110u + ctrl_clear_gap), (uint16_t)(top_y + 12u),
                                 clear_btn_w, clear_btn_h, "Borrar", BLUE);

    /* Valor actual al centro */
    EVE_color_rgb(BLACK);
    EVE_cmd_text((int16_t)(ctrl_x + 95), (int16_t)(top_y + 105), PRODUCT_FONT_SIZE, EVE_OPT_FLAT, cur_txt);

    /* Flecha abajo */
    prod_aud_aj_draw_icon_button(UI_VID_PROD_AUD_AJ_TP_DOWN, MEM_ICON_FABAJO, ctrl_x, bottom_y, ctrl_btn, ctrl_btn);

    EVE_color_rgb(BLACK);
    EVE_cmd_text((int16_t)(ctrl_x + ctrl_btn + ctrl_text_x_gap), (int16_t)(bottom_y + 26), PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "1 min");

    {
        const UiSubmSecondary side[] =
        {
            { UI_VID_PROD_AUD_AJ_TP_BACK,   "X" },
            { UI_VID_PROD_AUD_AJ_TP_ACCEPT, "", MEM_ICON_ACEPTAR },
        };

        ui_subm_draw_side_buttons(xR0, yR0, BTN, BTN, 3, side, 2);
    }

    if (s_status_msg[0] != '\0')
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text(350, 430, 27, EVE_OPT_CENTER, s_status_msg);
    }
}

/* =========================================================================
 * DRAW GENERAL
 * ========================================================================= */

static void opciones_producto_auditoria_ajustes_draw(void)
{
    switch (s_screen)
    {
        case PROD_AUD_AJ_SCREEN_POP_MODO:
            prod_aud_aj_draw_popup_modo();
            break;

        case PROD_AUD_AJ_SCREEN_POP_TPREP:
            prod_aud_aj_draw_popup_tprep();
            break;

        case PROD_AUD_AJ_SCREEN_MAIN:
        default:
            prod_aud_aj_draw_main();
            break;
    }
}

/* =========================================================================
 * EVENTOS
 * ========================================================================= */

static void opciones_producto_auditoria_ajustes_on_released(uint16_t id)
{
    int8_t vis;

    if (s_screen == PROD_AUD_AJ_SCREEN_POP_MODO)
    {
        switch (id)
        {
            case UI_VID_PROD_AUD_AJ_POP_BACK:
                prod_aud_aj_close_popup_modo();
                return;

            case UI_VID_PROD_AUD_AJ_POP_ACCEPT:
                if (s_modo_popup_selected >= 0 && s_modo_popup_selected < (int8_t)PROD_AUD_AJ_MODOS_N)
                    s_modo_actual = s_modo_popup_selected;

                prod_aud_aj_close_popup_modo();
                return;

            case UI_VID_PROD_AUD_AJ_POP_UP:
                if (s_popup_scroll > 0) s_popup_scroll--;
                return;

            case UI_VID_PROD_AUD_AJ_POP_DOWN:
            {
                int16_t maxScroll = (int16_t)PROD_AUD_AJ_MODOS_N - (int16_t)PROD_AUD_AJ_VISIBLE_POP_ROWS;
                if (maxScroll < 0) maxScroll = 0;
                if (s_popup_scroll < maxScroll) s_popup_scroll++;
                return;
            }

            default:
                break;
        }

        vis = prod_aud_aj_visible_from_popup_row_id(id);
        if (vis >= 0)
        {
            uint8_t idx = (uint8_t)(s_popup_scroll + vis);

            if (idx < PROD_AUD_AJ_MODOS_N)
                s_modo_popup_selected = (int8_t)idx;

            return;
        }

        return;
    }

    if (s_screen == PROD_AUD_AJ_SCREEN_POP_TPREP)
    {
        switch (id)
        {
            case UI_VID_PROD_AUD_AJ_TP_BACK:
                prod_aud_aj_close_popup_tprep_cancel();
                return;

            case UI_VID_PROD_AUD_AJ_TP_ACCEPT:
                (void)prod_aud_aj_accept_popup_tprep();
                return;

            case UI_VID_PROD_AUD_AJ_TP_UP:
                prod_aud_aj_tprep_inc();
                return;

            case UI_VID_PROD_AUD_AJ_TP_DOWN:
                prod_aud_aj_tprep_dec();
                return;

            case UI_VID_PROD_AUD_AJ_TP_CLEAR:
                prod_aud_aj_tprep_clear();
                return;

            case UI_VID_PROD_AUD_AJ_TP_NUM7:
                prod_aud_aj_tprep_append_digit('7');
                return;
            case UI_VID_PROD_AUD_AJ_TP_NUM8:
                prod_aud_aj_tprep_append_digit('8');
                return;
            case UI_VID_PROD_AUD_AJ_TP_NUM9:
                prod_aud_aj_tprep_append_digit('9');
                return;

            case UI_VID_PROD_AUD_AJ_TP_NUM4:
                prod_aud_aj_tprep_append_digit('4');
                return;
            case UI_VID_PROD_AUD_AJ_TP_NUM5:
                prod_aud_aj_tprep_append_digit('5');
                return;
            case UI_VID_PROD_AUD_AJ_TP_NUM6:
                prod_aud_aj_tprep_append_digit('6');
                return;

            case UI_VID_PROD_AUD_AJ_TP_NUM1:
                prod_aud_aj_tprep_append_digit('1');
                return;
            case UI_VID_PROD_AUD_AJ_TP_NUM2:
                prod_aud_aj_tprep_append_digit('2');
                return;
            case UI_VID_PROD_AUD_AJ_TP_NUM3:
                prod_aud_aj_tprep_append_digit('3');
                return;

            case UI_VID_PROD_AUD_AJ_TP_NUM0:
                prod_aud_aj_tprep_append_digit('0');
                return;

            default:
                return;
        }
    }

    /* MAIN */
    switch (id)
    {
        case UI_VID_PROD_AUD_AJ_BACK:
            ui_view_pop();
            return;

        case UI_VID_PROD_AUD_AJ_ACCEPT:
            ui_view_pop();
            return;

        case UI_VID_PROD_AUD_AJ_ROW_MODO:
            prod_aud_aj_open_popup_modo();
            return;

        case UI_VID_PROD_AUD_AJ_ROW_TPREP:
            prod_aud_aj_open_popup_tprep();
            return;

        default:
            break;
    }
}

/* =========================================================================
 * VIEW
 * ========================================================================= */

const UiView VIEW_OPCIONES_PRODUCTO_AUDITORIA_AJUSTES =
{
    .id = 16,
    .on_enter = opciones_producto_auditoria_ajustes_on_enter,
    .on_exit = 0,
    .update = 0,
    .draw = opciones_producto_auditoria_ajustes_draw,
    .on_released = opciones_producto_auditoria_ajustes_on_released
};
