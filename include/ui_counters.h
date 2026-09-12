/*
 * ui_counters.h
 *
 *  Created on: 17 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

void ui_counters_init(void);
void ui_counters_increment_metal(void);
void ui_counters_increment_error(void);
void ui_get_counters(uint32_t* metal, uint32_t* error);

