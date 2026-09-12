/*
 * ui_footer.h
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>
#include "ui_layout.h"

typedef enum{
    FOOTER_ESTADO_CONEXION_CU    = 0,  // Rojo
    FOOTER_ESTADO_FUNCIONAMIENTO = 1,  // Verde
    FOOTER_ESTADO_ADVERTENCIAS   = 2   // Amarillo
} FooterEstado;

typedef struct{
    FooterEstado state;
    uint32_t next_ms;
    uint32_t rotate_ms;
} UiFooter;

void ui_footer_init(UiFooter *f, FooterEstado initial_state, uint32_t rotate_ms);
void ui_footer_update(UiFooter *f);
void ui_footer_draw(const UiFooter *f, const UiMainLayout *L);
void ui_footer_init_v2(UiFooter *f, FooterEstado initial_state);
