/*
 * ui_view_mgr.h
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

typedef struct UiView UiView;

struct UiView
{
    uint16_t id;
    void (*on_enter)(void);
    void (*on_exit)(void);
    void (*update)(uint32_t now_ms);
    void (*draw)(void);
    void (*on_released)(uint16_t released_id);
};

void ui_view_init(void);

/* Stack control */
void ui_view_push(const UiView* v);
void ui_view_pop(void);
void ui_view_clear(void);

/* Status */
uint8_t ui_view_is_active(void);
const UiView* ui_view_top(void);

/* Runtime */
void ui_view_update(uint32_t now_ms);
void ui_view_draw(void);
void ui_view_handle_released(uint16_t released_id);
