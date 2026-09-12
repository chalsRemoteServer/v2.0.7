/*
 * ui_reloj.h
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>
#include <ui_layout.h>

typedef struct
{
    uint16_t h, m, s, ms;
    uint32_t prev_ms;
} UiReloj;

void ui_reloj_init(UiReloj *r, uint16_t h, uint16_t m, uint16_t s);
void ui_reloj_update(UiReloj *r);
//void ui_reloj_draw(const UiReloj *r, const UiTopbarLayout *L, const char *user);

void ui_reloj_draw(const UiReloj *r, const UiMainLayout *L, const char *user);
