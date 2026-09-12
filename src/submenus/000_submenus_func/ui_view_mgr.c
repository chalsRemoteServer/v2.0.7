/*
 * ui_view_mgr.c
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */


#include "ui_view_mgr.h"

#ifndef UI_VIEW_STACK_MAX
#define UI_VIEW_STACK_MAX 6
#endif

static const UiView* s_stack[UI_VIEW_STACK_MAX];
static int8_t s_top = -1;

void ui_view_init(void)
{
    s_top = -1;
}

uint8_t ui_view_is_active(void){
    return (s_top >= 0);
}//*****************************************

const UiView* ui_view_top(void)
{
    return (s_top >= 0) ? s_stack[s_top] : (const UiView*)0;
}

void ui_view_push(const UiView* v)
{
    if (!v) return;
    if (s_top >= (UI_VIEW_STACK_MAX - 1)) return;

    s_stack[++s_top] = v;
    if (v->on_enter) v->on_enter();
}

void ui_view_pop(void)
{
    if (s_top < 0) return;

    const UiView* v = s_stack[s_top];
    if (v && v->on_exit) v->on_exit();

    s_stack[s_top] = 0;
    s_top--;
}

void ui_view_clear(void)
{
    while (s_top >= 0) ui_view_pop();
}

void ui_view_update(uint32_t now_ms)
{
    const UiView* v = ui_view_top();
    if (v && v->update) v->update(now_ms);
}

void ui_view_draw(void){
    const UiView* v = ui_view_top();
    if (v && v->draw) v->draw();
}

void ui_view_handle_released(uint16_t released_id)
{
    const UiView* v = ui_view_top();
    if (v && v->on_released) v->on_released(released_id);
}
