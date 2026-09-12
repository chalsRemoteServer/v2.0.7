/*
 * ui_pagers.c
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#include "ui_pagers.h"
#include "EVE.h"
#include "colores.h"
#include <stdio.h>
#include <string.h>

void ui_pagers_draw(const UiMainLayout *L, uint8_t active, uint8_t total){
    if (!L) return;
    if (total == 0) return;
    // Protección: si active >= total, clamp a 0
    if (active >= total) active = 0;
    const uint16_t x0 = L->pager_x0;
    const uint16_t y  = L->pager_y;
    const uint16_t span = L->pager_span;
    const uint16_t pointSize = L->pager_point_size;

    EVE_color_mask(1, 1, 1, 1);
    EVE_color_a(255);
    EVE_begin(EVE_POINTS);
    EVE_point_size(pointSize);

    if (total == 1){// Un solo pager centrado
        uint16_t cx = (uint16_t)(x0 + span / 2);
        EVE_color_rgb(0x00FF00); // verde activo
        EVE_vertex2f((int16_t)(cx * PRESICION), (int16_t)(y * PRESICION));}
    else{
        const uint16_t range = (uint16_t)(total - 1);
        for (uint16_t i = 0; i < total; ++i){
            // Distribución equidistante en [x0 .. x0 + span]
            uint16_t cx = (uint16_t)(x0 + ((uint32_t)i * span) / range);
            if (i == active) EVE_color_rgb(0x00FF00);     // verde
            else EVE_color_rgb(MEDIUM_GRAY);  // gris
            EVE_vertex2f((int16_t)(cx * PRESICION), (int16_t)(y * PRESICION));
            }}
    EVE_end();
    EVE_color_rgb(BLACK);
    EVE_color_a(255);
    for (uint16_t i = 0; i < total; ++i){
    	uint16_t cx = (uint16_t)(x0 + ((uint32_t)i * span) / (total - 1));
    	char lbl[4];
    	snprintf(lbl, sizeof(lbl), "%u", (unsigned)(i + 1));
    	EVE_cmd_text((int16_t)cx, (int16_t)(y - 24), 23, EVE_OPT_CENTER, lbl);}
}//fin  ui_pagers_draw------------------------------------------------------------------

