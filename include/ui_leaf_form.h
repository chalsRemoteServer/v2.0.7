/*
 * ui_leaf_form.h
 *
 *  Created on: 9 mar. 2026
 *      Author: oscar
 */

#pragma once

#include <stdint.h>
#include "ui_layout.h"

/* Tipo de campo */
typedef enum
{
    UI_LEAF_FIELD_BOOL = 0,
    UI_LEAF_FIELD_TEXT = 1,
    UI_LEAF_FIELD_INT  = 2
} UiLeafFieldType;

/* Fila individual del formulario */
typedef struct
{
    uint16_t id;               /* TAG / ID táctil de la fila */
    const char* label;         /* texto mostrado a la izquierda */
    UiLeafFieldType type;      /* BOOL / TEXT / INT */

    uint8_t checked;           /* para BOOL */
    uint8_t enabled;           /* 0 = deshabilitado, 1 = habilitado */

    char* text_buf;            /* usado por TEXT */
    uint8_t text_max_len;      /* longitud máxima sin '\0' */
    const char* placeholder;   /* texto guía opcional */

} UiLeafRow;

/* Estado local del formulario */
typedef struct
{
    int16_t scroll;            /* índice de la primera fila visible */
    uint8_t visible_rows;      /* cuántas filas visibles caben */

    uint8_t keyboard_open;     /* 0 = cerrado, 1 = abierto */
    int8_t active_row_idx;     /* fila actualmente en edición, -1 = ninguna */

    uint8_t kb_shift;          /* 0 = minúsculas, 1 = mayúsculas */
    uint8_t kb_numeric;        /* 0 = abc, 1 = 123 */

} UiLeafState;

/* Layout calculado de la hoja */
typedef struct
{
    /* Título */
    uint16_t title_x;
    uint16_t title_y;

    /* Área de lista */
    uint16_t list_x;
    uint16_t list_y;
    uint16_t list_w;
    uint16_t list_h;

    /* Fila */
    uint16_t row_h;
    uint16_t row_gap;

    /* Texto interno */
    uint16_t text_pad_x;
    uint16_t text_pad_y;
    uint16_t label_font;

    /* Checkbox */
    uint16_t check_w;
    uint16_t check_h;
    uint16_t check_margin_right;

    /* Barra lateral */
    uint16_t side_x;
    uint16_t side_y;
    uint16_t side_btn_w;
    uint16_t side_btn_h;
    uint16_t side_gap_y;

    /* Campo de texto */
    uint16_t text_box_w;
    uint16_t text_box_h;
    uint16_t text_box_margin_right;
    uint16_t text_value_pad_x;
    uint16_t text_value_pad_y;
    uint16_t value_font;

    /* Teclado */
    uint16_t kb_x;
    uint16_t kb_y;
    uint16_t kb_w;
    uint16_t kb_h;

    uint16_t kb_key_w;
    uint16_t kb_key_h;
    uint16_t kb_gap_x;
    uint16_t kb_gap_y;
    uint16_t kb_font;

    /* Preview */
    uint16_t kb_preview_x;
    uint16_t kb_preview_y;
    uint16_t kb_preview_w;
    uint16_t kb_preview_h;
    uint16_t kb_preview_gap;

    /* anchos especiales */
    uint16_t kb_special_w_shift;
    uint16_t kb_special_w_bksp;
    uint16_t kb_special_w_mode;
    uint16_t kb_special_w_space;
    uint16_t kb_special_w_ok;
    uint16_t kb_special_w_cancel;

} UiLeafLayout;

/* -------- helpers de estado / layout -------- */

void ui_leaf_layout_default(UiLeafLayout* out, const UiMainLayout* L);

void ui_leaf_state_reset(UiLeafState* st, uint8_t visible_rows);

void ui_leaf_scroll_clamp(UiLeafState* st, uint8_t total_rows);
void ui_leaf_scroll_up(UiLeafState* st, uint8_t total_rows);
void ui_leaf_scroll_down(UiLeafState* st, uint8_t total_rows);
uint8_t ui_leaf_need_scroll(const UiLeafState* st, uint8_t total_rows);

void ui_leaf_text_clear(UiLeafRow* row);
uint8_t ui_leaf_text_append_char(UiLeafRow* row, char c);
uint8_t ui_leaf_text_backspace(UiLeafRow* row);

void ui_leaf_keyboard_open(UiLeafState* st, int8_t row_idx);
void ui_leaf_keyboard_close(UiLeafState* st);
void ui_leaf_keyboard_toggle_shift(UiLeafState* st);
void ui_leaf_keyboard_toggle_mode(UiLeafState* st);

/* opcional: se deja por compatibilidad aunque ya no sea necesario */
void ui_leaf_keyboard_get_rect(const UiLeafLayout* lay, uint16_t* x, uint16_t* y, uint16_t* w, uint16_t* h);

/* -------- draw principal -------- */

void ui_leaf_draw_bool_form(const char* title, UiLeafRow* rows, uint8_t total_rows, UiLeafState* st, const UiLeafLayout* lay,
                            uint16_t id_cancel, uint16_t id_accept, uint16_t id_up, uint16_t id_down);

void ui_leaf_draw_form(const char* title, UiLeafRow* rows, uint8_t total_rows, UiLeafState* st, const UiLeafLayout* lay,
                       uint16_t id_cancel, uint16_t id_accept, uint16_t id_up, uint16_t id_down);

void ui_leaf_draw_keyboard(UiLeafState* st, UiLeafRow* rows, uint8_t total_rows, const UiLeafLayout* lay);
