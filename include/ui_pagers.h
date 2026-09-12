/*
 * ui_pagers.h
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>
#include "ui_layout.h"

// Dibuja N pagers, con uno activo (verde) y el resto gris
void ui_pagers_draw(const UiMainLayout *L, uint8_t active, uint8_t total);

