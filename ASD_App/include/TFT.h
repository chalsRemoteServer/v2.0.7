/*
 * TFT.h
 *
 *  Created on: Nov 24, 2025
 *      Author: desarrollo_1
 */

#pragma once

#include <stdint.h>

void vStart_Pantalla_TFT(void);
void TFT_init(void);
void TFT_display(void);

uint16_t ui_touch_release_x(void);
uint16_t ui_touch_release_y(void);
uint8_t ui_monitor_graficadores(void);

