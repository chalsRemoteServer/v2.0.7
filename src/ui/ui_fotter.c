/*
 * ui_fotter.c
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#include "ui_footer.h"
#include "platform_time.h"
#include "EVE.h"
#include "colores.h"

#ifndef FOOTER_DEFAULT_ROTATE_MS
#define FOOTER_DEFAULT_ROTATE_MS 1500U
#endif

static const char* footer_label(FooterEstado st){
    switch (st){
        case FOOTER_ESTADO_CONEXION_CU:    return "SIN CONEXION CU";
        case FOOTER_ESTADO_FUNCIONAMIENTO: return "OPERACION NORMAL";
        case FOOTER_ESTADO_ADVERTENCIAS:   return "ADVERTENCIAS";
        default:                           return "";}
}//---------------------------------------------------------------

static uint32_t footer_color(FooterEstado st){
    switch (st){// Usa tus colores existentes en colores.h
        case FOOTER_ESTADO_CONEXION_CU:    return RED;
        case FOOTER_ESTADO_FUNCIONAMIENTO: return GREEN;
        case FOOTER_ESTADO_ADVERTENCIAS:   return YELLOW;
        default:                           return MEDIUM_GRAY;}
}//-------------------------------------------------------------

//deprecated
void ui_footer_init(UiFooter *f, FooterEstado initial_state, uint32_t rotate_ms){
    if (!f) return;

    f->state = initial_state;
    f->rotate_ms = (rotate_ms == 0U) ? FOOTER_DEFAULT_ROTATE_MS : rotate_ms;
    f->next_ms = platform_millis() + f->rotate_ms;
}//FIN  ui_footer_init--------------------------------------

void ui_footer_init_v2(UiFooter *f, FooterEstado initial_state){
    if (!f) return;

    f->state = initial_state;
    //f->rotate_ms = (rotate_ms == 0U) ? FOOTER_DEFAULT_ROTATE_MS : rotate_ms;
    //f->next_ms = platform_millis() + f->rotate_ms;
}//FIN  ui_footer_init--------------------------------------



void ui_footer_update(UiFooter *f){
    if (!f) return;

    uint32_t now = platform_millis();
    if((int32_t)(now - f->next_ms) >= 0){
        f->next_ms = now + f->rotate_ms;
        f->state = (FooterEstado)(((uint8_t)f->state + 1u) % 3u);}
}//fin  ui_footer_update   ------------------------------------------

void ui_footer_draw(const UiFooter *f, const UiMainLayout *L){
    if (!f || !L) return;

    const uint16_t x0 = L->footer_x0;
    const uint16_t y0 = L->footer_y0;
    const uint16_t x1 = (uint16_t)(x0 + L->footer_w);
    const uint16_t y1 = (uint16_t)(y0 + L->footer_h);

    // Franja de color (rect)
    EVE_color_rgb((uint32_t)footer_color(f->state));
    EVE_begin(EVE_RECTS);
    EVE_vertex2f((int16_t)(x0 * PRESICION), (int16_t)(y0 * PRESICION));
    EVE_vertex2f((int16_t)(x1 * PRESICION), (int16_t)(y1 * PRESICION));
    EVE_end();

    // Texto
    EVE_color_mask(1,1,1,1);
    EVE_color_a(255);
    EVE_color_rgb(BLACK);
    const char *label = footer_label(f->state);

    // Centramos verticalmente dentro del footer
    uint16_t ty = (uint16_t)(y0 + (L->footer_h - L->font) / 2);
    EVE_cmd_text_bold((int16_t)(x0 + 300), (int16_t)ty, 24, 0, label);
}//fin  ui_footer_draw-------------------------------------------------

