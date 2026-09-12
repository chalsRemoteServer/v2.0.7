/*
 * g_screen_2.c
 *
 *  Created on: 16 feb. 2026
 *      Author: oscar
 */

#include "ui_pantallas.h"
#include "ui_layout.h"
#include "EVE.h"
#include "colores.h"
#include <stdio.h>
#include "ui_counters.h"
#include "EVE_commands.h"
#include "EVE_arduino.h"
#include "Posicion_Objetos.h"

static void cont_on_enter(void) { }

static void cont_update(uint32_t now_ms)
{
  (void)now_ms;  // Aquí normalmente nada; los contadores ya viven en tu lógica global.
}

static inline void draw_card(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const char* title, uint32_t color_fill)
{
  EVE_color_rgb(color_fill);
  EVE_begin(EVE_RECTS);
  EVE_vertex2f(x * PRESICION, y * PRESICION);
  EVE_vertex2f((x + w) * PRESICION, (y + h) * PRESICION);

  EVE_end();

  EVE_color_rgb(BLACK);
  EVE_cmd_text_bold((uint16_t)(x + w/2), (uint16_t)(y-10), 24, EVE_OPT_CENTERX, title);
}

static inline uint16_t clamp_left(const UiMainLayout* L, int32_t x)
{
    const int32_t minX = (int32_t)L->content_x0;
    if (x < minX) x = minX;
    return (uint16_t)x;
}

static void cont_draw(void)
{
    const UiMainLayout* L = &UI_LAYOUT_MAIN;
    const uint16_t title_x = 20;
	const uint16_t title_y   = (uint16_t)(L->content_y0 + 10);

    EVE_color_mask(1,1,1,1);
    EVE_color_a(255);
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(title_x, title_y, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Contadores");

    // 2) parámetros (equivalentes Arduino, pero en px fijos)
    const uint16_t TITLE_ROWS_DY = 70;
    const uint16_t TITLE_CARD_DY = 40;
    const uint16_t cards_dx = 5;

    const uint16_t gapY = 60;
    const uint16_t row0 = (uint16_t)(title_y + TITLE_ROWS_DY);

    const uint16_t xLabel_base = (uint16_t)(L->content_x0 + 40);
    const uint16_t xLabel      = clamp_left(L, (int32_t)xLabel_base - (int32_t)cards_dx);

    const uint16_t boxW  = 140;
    const uint16_t boxH  = 170;

    const uint16_t xErr0 = (uint16_t)(L->content_x0 + 250);
    const uint16_t xMet0 = (uint16_t)(xErr0 + 160);

    const uint16_t xErr  = (uint16_t)(clamp_left(L, (int32_t)xErr0 - (int32_t)cards_dx));
    const uint16_t xMet  = (uint16_t)(clamp_left(L, (int32_t)xMet0 - (int32_t)cards_dx));

    const uint16_t yCard = (uint16_t)(title_y + TITLE_CARD_DY);

    // 3) etiquetas izquierda
    EVE_color_rgb(BLACK);
    EVE_cmd_text(xLabel, 			row0,             24, 	EVE_OPT_FLAT, 	"Disp.");
    EVE_cmd_text(xLabel, (uint16_t)(row0 + gapY),     24, 	EVE_OPT_FLAT, 	"Usuario");
    EVE_cmd_text(xLabel, (uint16_t)(row0 + 2*gapY),   24, 	EVE_OPT_FLAT, 	"Producto");

    // 4) tarjetas
    draw_card(xErr, yCard, boxW, (uint16_t)(boxH + 60), "Error", 0x0000FF);
    draw_card(xMet, yCard, boxW, (uint16_t)(boxH + 60), "Metal", 0xE0FFFF);

    // 5) valores
    uint32_t cntMetal = 0, cntError = 0;
    ui_get_counters(&cntMetal, &cntError);

    char errBuf[12], metBuf[12];
    snprintf(errBuf, sizeof(errBuf), "%lu", (unsigned long)cntError);
    snprintf(metBuf, sizeof(metBuf), "%lu", (unsigned long)cntMetal);

    const uint16_t y1 = row0;
    const uint16_t y2 = (uint16_t)(row0 + gapY);
    const uint16_t y3 = (uint16_t)(row0 + 2*gapY);

    const uint16_t cxErr = (uint16_t)(xErr + boxW/2);
    const uint16_t cxMet = (uint16_t)(xMet + boxW/2);

    EVE_color_rgb(BLACK);
    EVE_cmd_text(cxErr, y1, 24, EVE_OPT_CENTERX, errBuf);
    EVE_cmd_text(cxErr, y2, 24, EVE_OPT_CENTERX, errBuf);
    EVE_cmd_text(cxErr, y3, 24, EVE_OPT_CENTERX, errBuf);

    EVE_cmd_text(cxMet, y1, 24, EVE_OPT_CENTERX, metBuf);
    EVE_cmd_text(cxMet, y2, 24, EVE_OPT_CENTERX, metBuf);
    EVE_cmd_text(cxMet, y3, 24, EVE_OPT_CENTERX, metBuf);
}


const UiScreen g_screen_2 = {
  .on_enter = cont_on_enter,
  .on_exit  = 0,
  .update   = cont_update,
  .draw     = cont_draw
};


