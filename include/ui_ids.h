/*
 * ui_ids.h
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

typedef enum
{
    UI_ID_NONE        = 0,

    // Top-right (3)
    UI_ID_TOP_BTN0    = 1,
    UI_ID_TOP_BTN1    = 2,
    UI_ID_TOP_BTN2    = 3,

    // Side of graph (3 apilados)
    UI_ID_SIDE_BTN0   = 4,
    UI_ID_SIDE_BTN1   = 5,
    UI_ID_SIDE_BTN2   = 6,

    // Arrows
    UI_ID_ARROW_RIGHT = 7,
    UI_ID_ARROW_LEFT  = 8
} UiId;

