/*
 * ui_rects.c
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */


#include "ui_rects.h"

#ifndef UI_RECTS_MAX
#define UI_RECTS_MAX 80   // suficiente para tu meta (50+). Ajusta si quieres.
#endif

typedef struct {
    uint16_t id, x, y, w, h;
} UiRect;

static UiRect s_rects[UI_RECTS_MAX];
static uint8_t s_n = 0;

void ui_rects_begin_frame(void)
{
    s_n = 0;
}

void ui_rect_register(uint16_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    if (id == 0) return;
    if (s_n >= UI_RECTS_MAX) return;
    s_rects[s_n++] = (UiRect){ id, x, y, w, h };
}

uint8_t ui_get_rect_by_id(uint16_t id, uint16_t *x, uint16_t *y, uint16_t *w, uint16_t *h)
{
    // Busca de atrás hacia adelante (último registrado gana)
    for (int16_t i = (int16_t)s_n - 1; i >= 0; --i)
    {
        if (s_rects[i].id == id)
        {
            if (x) *x = s_rects[i].x;
            if (y) *y = s_rects[i].y;
            if (w) *w = s_rects[i].w;
            if (h) *h = s_rects[i].h;
            return 1;
        }
    }
    return 0;
}
