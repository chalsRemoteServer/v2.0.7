/*
 * ui_main_buttons.h
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#pragma once
#include "ui_layout.h"

typedef struct {
  uint32_t mem;       // dirección en RAM_G (MEM_ICON_*)
  uint16_t w, h;      // 64x64
} UiIcon;

void ui_main_buttons_draw(const UiMainLayout *L);
void ui_main_buttons_draw_groups(const UiMainLayout *L, uint8_t groups);
//uint8_t ui_get_rect_by_id(uint8_t id, uint16_t *x, uint16_t *y, uint16_t *w, uint16_t *h);

typedef enum {
  UI_GRP_TOPBAR   = (1u << 0),
  UI_GRP_SIDEBAR  = (1u << 1),
  UI_GRP_ARROWS   = (1u << 2),
} UiButtonGroups;

