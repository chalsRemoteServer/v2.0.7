/*
 * ui_pantallas.h
 *
 *  Created on: 16 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

typedef struct{
    void (*on_enter)(void);
    void (*on_exit)(void);
    void (*update)(uint32_t now_ms);   // lógica/estado
    void (*draw)(void);               // dibuja en DL
} UiScreen;


uint8_t ui_screens_count(void);


void ui_screens_init(void);
void ui_screens_set(uint8_t idx);
uint8_t ui_screens_get(void);
void ui_screens_next(void);
void ui_screens_prev(void);

void ui_screens_update(uint32_t now_ms);
void ui_screens_draw(void);
