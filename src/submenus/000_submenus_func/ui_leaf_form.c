/*
 * ui_leaf_form.c
 *
 *  Created on: 9 mar. 2026
 *      Author: oscar
 */

#include "ui_layout.h"
#include "ui_leaf_form.h"
#include "ui_submenu_widgets.h"
#include "ui_rects.h"
#include "EVE.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include "assets_mem.h"
#include "EVE_arduino.h"
#include "EVE_commands.h"
#include "ui_view_ids.h"

#include <stdio.h>
#include <string.h>

#define UI_LEAF_PREVIEW_CURSOR "|"

/* Helpers internos*/

static void ui_leaf_draw_checkbox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t checked, uint8_t enabled)
{
    if (enabled) EVE_color_rgb(WHITE);
    else         EVE_color_rgb(0xD8D8D8);

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

    if (checked)
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text((int16_t)(x + w / 2u), (int16_t)(y + h / 2u), 28, EVE_OPT_CENTER, "X");
    }
}

static void ui_leaf_draw_textbox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t enabled, uint8_t active)
{
    if (enabled) EVE_color_rgb(WHITE);
    else         EVE_color_rgb(0xD8D8D8);

    EVE_begin(EVE_RECTS);
    EVE_vertex2f(x * PRESICION, y * PRESICION);
    EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);
    EVE_end();

    if (active) EVE_color_rgb(BLACK);
    else        EVE_color_rgb(0x707070);

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
}

static void ui_leaf_draw_key(uint16_t id, const char* txt, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font)
{
    EVE_tag((uint8_t)id);
    ui_rect_register(id, x, y, w, h);

    EVE_color_rgb(BLUE);
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
    EVE_cmd_text((int16_t)(x + w / 2u), (int16_t)(y + h / 2u), font, EVE_OPT_CENTER, txt);

    EVE_tag(0);
}

static void ui_leaf_build_preview_text(const UiLeafRow* row, char* out, uint16_t out_sz)
{
    size_t len;

    if (!out || out_sz == 0u) return;
    out[0] = '\0';

    if (!row) return;

    if (row->text_buf && row->text_buf[0] != '\0')
    {
        len = strlen(row->text_buf);

        if (len + 2u < out_sz)
        {
            strcpy(out, row->text_buf);
            out[len] = '|';
            out[len + 1u] = '\0';
        }
        else
        {
            strncpy(out, row->text_buf, out_sz - 1u);
            out[out_sz - 1u] = '\0';
        }
    }
    else
    {
        strncpy(out, UI_LEAF_PREVIEW_CURSOR, out_sz - 1u);
        out[out_sz - 1u] = '\0';
    }
}

static void ui_leaf_draw_keyboard_preview(const UiLeafState* st, UiLeafRow* rows, uint8_t total_rows, const UiLeafLayout* lay)
{
    const UiLeafRow* row;
    char shown_text[96];
    uint16_t box_x, box_y, box_w, box_h;

    if (!st || !rows || !lay) return;
    if (!st->keyboard_open) return;
    if (st->active_row_idx < 0) return;
    if ((uint8_t)st->active_row_idx >= total_rows) return;

    row = &rows[st->active_row_idx];

    box_x = lay->kb_preview_x;
    box_y = lay->kb_preview_y;
    box_w = lay->kb_preview_w;
    box_h = lay->kb_preview_h;

    ui_leaf_draw_textbox(box_x, box_y, box_w, box_h, 1u, 1u);

    if (row->text_buf && row->text_buf[0] != '\0')
    {
        ui_leaf_build_preview_text(row, shown_text, sizeof(shown_text));
        EVE_color_rgb(BLACK);
        EVE_cmd_text((int16_t)(box_x + lay->text_value_pad_x),
                     (int16_t)(box_y + lay->text_value_pad_y),
                     lay->value_font,
                     EVE_OPT_FLAT,
                     shown_text);
    }
    else if (row->placeholder && row->placeholder[0] != '\0')
    {
        char tmp[96];
        snprintf(tmp, sizeof(tmp), "%s|", row->placeholder);
        EVE_color_rgb(0x606060);
        EVE_cmd_text((int16_t)(box_x + lay->text_value_pad_x),
                     (int16_t)(box_y + lay->text_value_pad_y),
                     lay->value_font,
                     EVE_OPT_FLAT,
                     tmp);
    }
    else
    {
        EVE_color_rgb(BLACK);
        EVE_cmd_text((int16_t)(box_x + lay->text_value_pad_x),
                     (int16_t)(box_y + lay->text_value_pad_y),
                     lay->value_font,
                     EVE_OPT_FLAT,
                     "|");
    }
}

/* Dibuja una sola fila tipo BOOL */
static void ui_leaf_draw_bool_row(const UiLeafRow* row, uint16_t row_x, uint16_t row_y, uint16_t row_w, uint16_t row_h, const UiLeafLayout* lay)
{
    if (row->enabled) EVE_color_rgb(BLUE);
    else              EVE_color_rgb(BLUE);

    EVE_tag((uint8_t)row->id);
    ui_rect_register(row->id, row_x, row_y, row_w, row_h);

    EVE_begin(EVE_RECTS);
    EVE_vertex2f(row_x * PRESICION, row_y * PRESICION);
    EVE_vertex2f((row_x + row_w) * PRESICION, (row_y + row_h) * PRESICION);
    EVE_end();

    EVE_color_rgb(WHITE);
    EVE_cmd_text((int16_t)(row_x + lay->text_pad_x), (int16_t)(row_y + lay->text_pad_y), lay->label_font, EVE_OPT_FLAT, (row->label ? row->label : ""));

    {
    const uint16_t cb_w = lay->check_w;
    const uint16_t cb_h = lay->check_h;
    const uint16_t cb_x = (uint16_t)(row_x + row_w - lay->check_margin_right - cb_w);
    const uint16_t cb_y = (uint16_t)(row_y + (row_h - cb_h) / 2u);

    ui_leaf_draw_checkbox(cb_x, cb_y, cb_w, cb_h, row->checked, row->enabled);
    }

    EVE_tag(0);
}

static void ui_leaf_draw_text_row(const UiLeafRow* row, uint16_t row_x, uint16_t row_y, uint16_t row_w, uint16_t row_h, const UiLeafLayout* lay, uint8_t active)
{
    const char* shown_text;
    uint16_t box_w, box_h, box_x, box_y;

    if (row->enabled) EVE_color_rgb(BLUE);
    else              EVE_color_rgb(BLUE);

    EVE_tag((uint8_t)row->id);
    ui_rect_register(row->id, row_x, row_y, row_w, row_h);

    EVE_begin(EVE_RECTS);
    EVE_vertex2f(row_x * PRESICION, row_y * PRESICION);
    EVE_vertex2f((row_x + row_w) * PRESICION, (row_y + row_h) * PRESICION);
    EVE_end();

    EVE_color_rgb(WHITE);
    EVE_cmd_text((int16_t)(row_x + lay->text_pad_x), (int16_t)(row_y + lay->text_pad_y), lay->label_font, EVE_OPT_FLAT, (row->label ? row->label : ""));

    box_w = lay->text_box_w;
    box_h = lay->text_box_h;
    box_x = (uint16_t)((row_x + row_w) - box_w);
    box_y = (uint16_t)(row_y + (row_h - box_h) / 2u);

    ui_leaf_draw_textbox(box_x, box_y, box_w, box_h, 1u, active);

    shown_text = "";
    if (row->text_buf && row->text_buf[0] != '\0')
    {
        shown_text = row->text_buf;
        EVE_color_rgb(BLACK);
    }
    else if (row->placeholder)
    {
        shown_text = row->placeholder;
        EVE_color_rgb(0x606060);
    }
    else
    {
        shown_text = "";
        EVE_color_rgb(BLACK);
    }

    EVE_cmd_text((int16_t)(box_x + lay->text_value_pad_x), (int16_t)(box_y + lay->text_value_pad_y), lay->value_font, EVE_OPT_FLAT, shown_text);

    EVE_tag(0);
}

static void ui_leaf_draw_row(const UiLeafRow* row, uint16_t row_x, uint16_t row_y, uint16_t row_w, uint16_t row_h,
                             const UiLeafLayout* lay, uint8_t active)
{
    if (!row || !lay) return;

    switch (row->type)
    {
        case UI_LEAF_FIELD_BOOL:
            ui_leaf_draw_bool_row(row, row_x, row_y, row_w, row_h, lay);
            break;

        case UI_LEAF_FIELD_TEXT:
            ui_leaf_draw_text_row(row, row_x, row_y, row_w, row_h, lay, active);
            break;

        default:
            ui_leaf_draw_bool_row(row, row_x, row_y, row_w, row_h, lay);
            break;
    }
}

/*  API pública */

void ui_leaf_layout_default(UiLeafLayout* out, const UiMainLayout* L)
{
    (void)L;

    if (!out) return;

    out->title_x = 20;
    out->title_y = 135;

    out->list_x  = 3; // 20
    out->list_y  = 190;
    out->list_w  = 600;
    out->list_h  = 260;

    out->row_h   = 56;
    out->row_gap = 5;

    out->text_pad_x = 18;
    out->text_pad_y = 12;
    out->label_font = 25;

    out->check_w = 50;
    out->check_h = 50;
    out->check_margin_right = 20;

    out->side_x = 710;
    out->side_y = 130;
    out->side_btn_w = 84;
    out->side_btn_h = 84;
    out->side_gap_y = 2;

    out->text_box_w = 220;
    out->text_box_h = 42;
    out->text_box_margin_right = 20;
    out->text_value_pad_x = 10;
    out->text_value_pad_y = 8;
    out->value_font = 24;

    /* bloque total del teclado */
    out->kb_x = 1;
    out->kb_y = out->list_y;   /* ahora el teclado inicia directamente aquí */
    out->kb_w = (uint16_t)(out->side_x - out->kb_x - 5u);
    out->kb_h = out->list_h;

    /* preview a la derecha del título */
    out->kb_preview_x = (uint16_t)(out->title_x + 250u);
    out->kb_preview_y = (uint16_t)(out->title_y - 2u);
    out->kb_preview_w = (uint16_t)(out->side_x - out->kb_preview_x - 20u);
    out->kb_preview_h = 44u;

    out->kb_gap_x = 6;
    out->kb_gap_y = 6;
    out->kb_font  = 24;

    /* altura de tecla: 4 filas ocupando toda la altura del teclado */
    {
        uint16_t free_h = out->kb_h;
        out->kb_key_h = (uint16_t)((free_h - 3u * out->kb_gap_y) / 4u);
    }

    out->kb_key_w = (uint16_t)((out->kb_w - 9u * out->kb_gap_x) / 10u);    /* fila base de 10 teclas */

    /* especiales */
    out->kb_special_w_shift  = (uint16_t)(out->kb_key_w + 18u);
    out->kb_special_w_bksp   = (uint16_t)(out->kb_key_w + 34u);  // 18
    out->kb_special_w_mode   = (uint16_t)(out->kb_key_w + 12u);
    out->kb_special_w_ok     = (uint16_t)(out->kb_key_w + 12u);
    out->kb_special_w_cancel = (uint16_t)(out->kb_key_w + 12u);

    out->kb_special_w_space = (uint16_t)(out->kb_w - out->kb_special_w_mode - out->kb_special_w_ok - out->kb_special_w_cancel - 3u * out->kb_gap_x);

}

void ui_leaf_state_reset(UiLeafState* st, uint8_t visible_rows)
{
    if (!st) return;

    st->scroll = 0;
    st->visible_rows = visible_rows;
    st->keyboard_open = 0u;
    st->active_row_idx = -1;
    st->kb_shift = 0u;
    st->kb_numeric = 0u;
}

void ui_leaf_scroll_clamp(UiLeafState* st, uint8_t total_rows)
{
    int16_t maxScroll;

    if (!st) return;

    maxScroll = (int16_t)total_rows - (int16_t)st->visible_rows;
    if (maxScroll < 0) maxScroll = 0;

    if (st->scroll < 0) st->scroll = 0;
    if (st->scroll > maxScroll) st->scroll = maxScroll;
}

void ui_leaf_scroll_up(UiLeafState* st, uint8_t total_rows)
{
    (void)total_rows;

    if (!st) return;
    if (st->scroll > 0) st->scroll--;
}

void ui_leaf_scroll_down(UiLeafState* st, uint8_t total_rows)
{
    int16_t maxScroll;

    if (!st) return;

    maxScroll = (int16_t)total_rows - (int16_t)st->visible_rows;
    if (maxScroll < 0) maxScroll = 0;

    if (st->scroll < maxScroll) st->scroll++;
}

uint8_t ui_leaf_need_scroll(const UiLeafState* st, uint8_t total_rows)
{
    if (!st) return 0u;
    return (total_rows > st->visible_rows) ? 1u : 0u;
}

void ui_leaf_keyboard_open(UiLeafState* st, int8_t row_idx)
{
    if (!st) return;

    st->keyboard_open = 1u;
    st->active_row_idx = row_idx;
}

void ui_leaf_keyboard_close(UiLeafState* st)
{
    if (!st) return;

    st->keyboard_open = 0u;
    st->active_row_idx = -1;
    st->kb_shift = 0u;
    st->kb_numeric = 0u;
}

void ui_leaf_keyboard_toggle_shift(UiLeafState* st)
{
    if (!st) return;
    st->kb_shift = (uint8_t)!st->kb_shift;
}

void ui_leaf_keyboard_toggle_mode(UiLeafState* st)
{
    if (!st) return;
    st->kb_numeric = (uint8_t)!st->kb_numeric;
    st->kb_shift = 0u;
}

void ui_leaf_text_clear(UiLeafRow* row)
{
    if (!row || !row->text_buf) return;
    row->text_buf[0] = '\0';
}

uint8_t ui_leaf_text_append_char(UiLeafRow* row, char c)
{
    size_t len;

    if (!row || !row->text_buf) return 0u;

    len = strlen(row->text_buf);
    if (len >= row->text_max_len) return 0u;

    row->text_buf[len] = c;
    row->text_buf[len + 1u] = '\0';
    return 1u;
}

uint8_t ui_leaf_text_backspace(UiLeafRow* row)
{
    size_t len;

    if (!row || !row->text_buf) return 0u;

    len = strlen(row->text_buf);
    if (len == 0u) return 0u;

    row->text_buf[len - 1u] = '\0';
    return 1u;
}

void ui_leaf_keyboard_get_rect(const UiLeafLayout* lay,
                               uint16_t* x, uint16_t* y,
                               uint16_t* w, uint16_t* h)
{
    if (!lay || !x || !y || !w || !h) return;

    *x = lay->kb_x;
    *y = lay->kb_y;
    *w = lay->kb_w;
    *h = lay->kb_h;
}

void ui_leaf_draw_bool_form(const char* title, UiLeafRow* rows, uint8_t total_rows, UiLeafState* st, const UiLeafLayout* lay,
                            uint16_t id_cancel, uint16_t id_accept,
                            uint16_t id_up,uint16_t id_down)
{
    ui_leaf_draw_form(title, rows, total_rows, st, lay, id_cancel, id_accept, id_up, id_down);
}

void ui_leaf_draw_keyboard(UiLeafState* st, UiLeafRow* rows, uint8_t total_rows, const UiLeafLayout* lay)
{
    uint16_t x, y, w, h, gx, gy, f;
    uint16_t row2_y, row3_y, row4_y;
    uint16_t x4;

    if (!st || !rows || !lay) return;
    if (!st->keyboard_open) return;

    x  = lay->kb_x;
    y  = lay->kb_y;
    w  = lay->kb_key_w;
    h  = lay->kb_key_h;
    gx = lay->kb_gap_x;
    gy = lay->kb_gap_y;
    f  = lay->kb_font;

    row2_y = (uint16_t)(y + (h + gy));
    row3_y = (uint16_t)(row2_y + (h + gy));
    row4_y = (uint16_t)(row3_y + (h + gy));

    /* fondo completo del teclado */
    EVE_color_rgb(WHITE);
    EVE_begin(EVE_RECTS);
    EVE_vertex2f(lay->kb_x * PRESICION, lay->kb_y * PRESICION);
    EVE_vertex2f((lay->kb_x + lay->kb_w) * PRESICION, (lay->kb_y + lay->kb_h) * PRESICION);
    EVE_end();

    ui_leaf_draw_keyboard_preview(st, rows, total_rows, lay);

    if (!st->kb_numeric)
    {
        /* Fila 1: q w e r t y u i o p */
        ui_leaf_draw_key(UI_VID_KB_Q, st->kb_shift ? "Q" : "q", x + 0*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_W, st->kb_shift ? "W" : "w", x + 1*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_E, st->kb_shift ? "E" : "e", x + 2*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_R, st->kb_shift ? "R" : "r", x + 3*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_T, st->kb_shift ? "T" : "t", x + 4*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_Y, st->kb_shift ? "Y" : "y", x + 5*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_U, st->kb_shift ? "U" : "u", x + 6*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_I, st->kb_shift ? "I" : "i", x + 7*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_O, st->kb_shift ? "O" : "o", x + 8*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_P, st->kb_shift ? "P" : "p", x + 9*(w+gx), y, w, h, f);

        /* Fila 2: a s d f g h j k l */
        ui_leaf_draw_key(UI_VID_KB_A, st->kb_shift ? "A" : "a", x + 0*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_S, st->kb_shift ? "S" : "s", x + 1*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_D, st->kb_shift ? "D" : "d", x + 2*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_F, st->kb_shift ? "F" : "f", x + 3*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_G, st->kb_shift ? "G" : "g", x + 4*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_H, st->kb_shift ? "H" : "h", x + 5*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_J, st->kb_shift ? "J" : "j", x + 6*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_K, st->kb_shift ? "K" : "k", x + 7*(w+gx), row2_y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_L, st->kb_shift ? "L" : "l", x + 8*(w+gx), row2_y, w, h, f);

        /* Fila 3: shift z x c v b n m bksp */
        {
            uint16_t x3 = x;

            ui_leaf_draw_key(UI_VID_KB_SHIFT, "SHIFT", x3, row3_y, lay->kb_special_w_shift, h, 18);
            x3 = (uint16_t)(x3 + lay->kb_special_w_shift + gx);

            ui_leaf_draw_key(UI_VID_KB_Z, st->kb_shift ? "Z" : "z", x3, row3_y, w, h, f); x3 = (uint16_t)(x3 + w + gx);
            ui_leaf_draw_key(UI_VID_KB_X, st->kb_shift ? "X" : "x", x3, row3_y, w, h, f); x3 = (uint16_t)(x3 + w + gx);
            ui_leaf_draw_key(UI_VID_KB_C, st->kb_shift ? "C" : "c", x3, row3_y, w, h, f); x3 = (uint16_t)(x3 + w + gx);
            ui_leaf_draw_key(UI_VID_KB_V, st->kb_shift ? "V" : "v", x3, row3_y, w, h, f); x3 = (uint16_t)(x3 + w + gx);
            ui_leaf_draw_key(UI_VID_KB_B, st->kb_shift ? "B" : "b", x3, row3_y, w, h, f); x3 = (uint16_t)(x3 + w + gx);
            ui_leaf_draw_key(UI_VID_KB_N, st->kb_shift ? "N" : "n", x3, row3_y, w, h, f); x3 = (uint16_t)(x3 + w + gx);
            ui_leaf_draw_key(UI_VID_KB_M, st->kb_shift ? "M" : "m", x3, row3_y, w, h, f); x3 = (uint16_t)(x3 + w + gx);

            ui_leaf_draw_key(UI_VID_KB_BKSP, "BORRAR", x3, row3_y, lay->kb_special_w_bksp, h, 18);
        }

        /* Fila 4: 123 | SPACE | OK | X */
        x4 = x;
        ui_leaf_draw_key(UI_VID_KB_MODE, "123", x4, row4_y, lay->kb_special_w_mode, h, f);
        x4 = (uint16_t)(x4 + lay->kb_special_w_mode + gx);

        ui_leaf_draw_key(UI_VID_KB_SPACE, "", x4, row4_y, lay->kb_special_w_space, h, 22);
        x4 = (uint16_t)(x4 + lay->kb_special_w_space + gx);

        ui_leaf_draw_key(UI_VID_KB_OK, "OK", x4, row4_y, lay->kb_special_w_ok, h, f);
        x4 = (uint16_t)(x4 + lay->kb_special_w_ok + gx);

        ui_leaf_draw_key(UI_VID_KB_CANCEL, "X", x4, row4_y, lay->kb_special_w_cancel, h, f);
    }
    else
    {
        /* Fila 1: 1 2 3 4 5 6 7 8 9 0 */
        ui_leaf_draw_key(UI_VID_KB_1, "1", x + 0*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_2, "2", x + 1*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_3, "3", x + 2*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_4, "4", x + 3*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_5, "5", x + 4*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_6, "6", x + 5*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_7, "7", x + 6*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_8, "8", x + 7*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_9, "9", x + 8*(w+gx), y, w, h, f);
        ui_leaf_draw_key(UI_VID_KB_0, "0", x + 9*(w+gx), y, w, h, f);

        /* Fila 2 vacía visualmente: puedes dejarla en blanco */
        /* Fila 3: solo borrar */
        ui_leaf_draw_key(UI_VID_KB_BKSP, "BKSP", x + 8*(w+gx), row3_y, lay->kb_special_w_bksp, h, 18);

        /* Fila 4: ABC | SPACE | OK | X */
        x4 = x;
        ui_leaf_draw_key(UI_VID_KB_MODE, "ABC", x4, row4_y, lay->kb_special_w_mode, h, f);
        x4 = (uint16_t)(x4 + lay->kb_special_w_mode + gx);

        ui_leaf_draw_key(UI_VID_KB_SPACE, "SPACE", x4, row4_y, lay->kb_special_w_space, h, 18);
        x4 = (uint16_t)(x4 + lay->kb_special_w_space + gx);

        ui_leaf_draw_key(UI_VID_KB_OK, "OK", x4, row4_y, lay->kb_special_w_ok, h, f);
        x4 = (uint16_t)(x4 + lay->kb_special_w_ok + gx);

        ui_leaf_draw_key(UI_VID_KB_CANCEL, "X", x4, row4_y, lay->kb_special_w_cancel, h, f);
    }
}

void ui_leaf_draw_form(const char* title, UiLeafRow* rows, uint8_t total_rows, UiLeafState* st, const UiLeafLayout* lay,
                       uint16_t id_cancel, uint16_t id_accept, uint16_t id_up, uint16_t id_down)
{
    uint8_t i;
    uint8_t need_scroll;

    if (!lay || !st || !rows) return;

    ui_leaf_scroll_clamp(st, total_rows);
    need_scroll = ui_leaf_need_scroll(st, total_rows);

    EVE_color_a(255);
    EVE_color_rgb(BLACK);

    if (title && title[0] != '\0')
    {
        EVE_cmd_text_bold(lay->title_x, lay->title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, title);
    }

    for (i = 0u; i < st->visible_rows; i++)
    {
        uint8_t idx = (uint8_t)(st->scroll + i);
        uint16_t row_x, row_y, row_w, row_h;
        uint8_t active = 0u;

        if (idx >= total_rows) break;

        row_x = lay->list_x;
        row_y = (uint16_t)(lay->list_y + i * (lay->row_h + lay->row_gap));

        if (lay->side_x > row_x + 5u)
            row_w = (uint16_t)(lay->side_x - row_x - 5u);
        else
            row_w = lay->list_w;

        row_h = lay->row_h;

        if (st->keyboard_open && st->active_row_idx == (int8_t)idx)
            active = 1u;

        ui_leaf_draw_row(&rows[idx], row_x, row_y, row_w, row_h, lay, active);
    }

    if (need_scroll)
    {
        const UiSubmSecondary side4[] =
        {
            { id_cancel, "X", 0u },
            { id_accept, "",  MEM_ICON_ACEPTAR },
            { id_up,     "",  MEM_ICON_FARRIBA },
            { id_down,   "",  MEM_ICON_FABAJO  },
        };
        ui_subm_draw_side_buttons(lay->side_x, lay->side_y, lay->side_btn_w, lay->side_btn_h, lay->side_gap_y, side4, 4);
    }
    else
    {
        const UiSubmSecondary side2[] =
        {
            { id_cancel, "X", 0u },
            { id_accept, "",  MEM_ICON_ACEPTAR },
        };
        ui_subm_draw_side_buttons(lay->side_x, lay->side_y, lay->side_btn_w, lay->side_btn_h, lay->side_gap_y, side2, 2);
    }
}

