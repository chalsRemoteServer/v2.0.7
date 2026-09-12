/*
 * ui_counters.c
 *
 *  Created on: 17 feb. 2026
 *      Author: oscar
 */

#include "ui_counters.h"

static uint32_t s_cnt_metal = 0;
static uint32_t s_cnt_error = 0;

void ui_counters_init(void)
{
    s_cnt_metal = 0;
    s_cnt_error = 0;
}

void ui_counters_increment_metal(void)
{
    s_cnt_metal++;
}

void ui_counters_increment_error(void)
{
    s_cnt_error++;
}

void ui_get_counters(uint32_t* metal, uint32_t* error)
{
    if (metal) *metal = s_cnt_metal;
    if (error) *error = s_cnt_error;
}

