/*
 * ui_reloj.c
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#include <platform_time.h>
#include <stdio.h>
#include <string.h>
#include <ui_reloj.h>
#include "EVE.h"
#include "colores.h"

void ui_reloj_init(UiReloj *r, uint16_t h, uint16_t m, uint16_t s)
{
    if (!r) return;
    r->h = h % 24;
    r->m = m % 60;
    r->s = s % 60;
    r->ms = 0;
    r->prev_ms = platform_millis();
}

void ui_reloj_update(UiReloj *r)
{
    if (!r) return;

    uint32_t now = platform_millis();
    uint32_t delta = now - r->prev_ms;
    r->prev_ms = now;

    r->ms += (uint16_t)(delta % 1000U);
    r->s  += (uint16_t)(delta / 1000U);

    if (r->ms >= 1000U) { r->ms -= 1000U; r->s++; }
    if (r->s  >= 60U)   { r->s  -= 60U;   r->m++; }
    if (r->m  >= 60U)   { r->m  -= 60U;   r->h++; }
    if (r->h  >= 24U)   { r->h  -= 24U; }
}

void ui_reloj_draw(const UiReloj *r, const UiMainLayout *L, const char *user)
{
    if (!r || !L) return;
    if (!user) user = "Sin usuario";

    char buf[12];
    snprintf(buf, sizeof(buf), "%02u:%02u:%02u", (unsigned)r->h, (unsigned)r->m, (unsigned)r->s);

    EVE_color_rgb(BLACK);
    EVE_cmd_bgcolor(MEDIUM_GRAY);
    EVE_cmd_fgcolor(MEDIUM_GRAY);

    EVE_cmd_text(L->clock_x, L->clock_y, 24, EVE_OPT_FLAT, buf);    // Reloj
    EVE_cmd_text(L->user_x,  L->user_y,  24, EVE_OPT_FLAT, user);    // Usuario

}

