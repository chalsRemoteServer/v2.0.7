/*
 * ui_rects.h
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

void ui_rects_begin_frame(void);
void ui_rect_register(uint16_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h);
uint8_t ui_get_rect_by_id(uint16_t id, uint16_t *x, uint16_t *y, uint16_t *w, uint16_t *h);
