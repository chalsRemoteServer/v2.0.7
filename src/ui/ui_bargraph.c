/*
 * ui_bargraph.c
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#include "ui_bargraph.h"
#include "platform_time.h"
#include <stdlib.h>
#include "EVE.h"
#include "colores.h"
#include "ui_counters.h"
#include "ui_almacena_productos.h"
#include <stdio.h>
#include <string.h>
#include "Posicion_Objetos.h"
#include "system.h"
#include "ComSer.h"
#include "queue1.h"

#define GRAPH_MAX_Y 300U
#define COUNTER_SPLIT_TH 100u
extern volatile struct _DISPLAY_VFD_ comm;

static uint16_t map_value_to_y(uint16_t v, uint16_t gy0, uint16_t gy1)
{
    if (v > GRAPH_MAX_Y) v = GRAPH_MAX_Y;
    uint32_t span = (uint32_t)(gy1 - gy0);
    uint32_t y = (uint32_t)gy1 - (span * (uint32_t)v) / GRAPH_MAX_Y;
    return (uint16_t)y;
}

static void draw_axes_and_labels(uint16_t gx0, uint16_t gy0, uint16_t gx1, uint16_t gy1, uint16_t font)
{
    // Ejes (izquierda y abajo)
    EVE_color_rgb(BLACK);
    EVE_begin(EVE_LINES);
    EVE_line_width(8U * PRESICION);

    // Eje Y
    EVE_vertex2f((int16_t)(gx0 * PRESICION), (int16_t)(gy0 * PRESICION));
    EVE_vertex2f((int16_t)(gx0 * PRESICION), (int16_t)(gy1 * PRESICION));

    // Eje X
    EVE_vertex2f((int16_t)(gx0 * PRESICION), (int16_t)(gy1 * PRESICION));
    EVE_vertex2f((int16_t)(gx1 * PRESICION), (int16_t)(gy1 * PRESICION));
    EVE_end();

    // Etiquetas Y: 300/200/100/0  (la X de labels la dejamos a la izquierda del eje Y)
    const int16_t label_x = (gx0 > 40) ? (int16_t)(gx0 - 45) : (int16_t)gx0;
    char buf[8];

    EVE_color_rgb(BLACK);
    snprintf(buf, sizeof(buf), "300");    // 300
    EVE_cmd_text(label_x, (int16_t)(map_value_to_y(300, gy0, gy1) - 10), 24, 0, buf);

    snprintf(buf, sizeof(buf), "200");    // 200
    EVE_cmd_text(label_x, (int16_t)(map_value_to_y(200, gy0, gy1) - 10), 24, 0, buf);

    snprintf(buf, sizeof(buf), "100");    // 100
    EVE_cmd_text(label_x, (int16_t)(map_value_to_y(100, gy0, gy1) - 10), 24, 0, buf);

    snprintf(buf, sizeof(buf), "0");    // 0
    EVE_cmd_text((int16_t)(label_x + 20), (int16_t)(gy1 - 15), 24, 0, buf);
}

static void draw_threshold_line(uint16_t gx0, uint16_t gy0, uint16_t gx1, uint16_t gy1, uint16_t threshold)
{
    uint16_t yT = map_value_to_y(threshold, gy0, gy1);

    EVE_color_rgb(BLACK);
    EVE_begin(EVE_LINES);
    EVE_line_width(8U * PRESICION);

    EVE_vertex2f((int16_t)(gx0 * PRESICION), (int16_t)(yT * PRESICION));
    EVE_vertex2f((int16_t)((gx1 + 1) * PRESICION), (int16_t)(yT * PRESICION));
    EVE_end();
}

// --- helpers internos ---
static uint16_t clamp_u16(uint16_t v, uint16_t lo, uint16_t hi){
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

static uint8_t ui_bargraph_get_plot_rect(const UiMainLayout *L, uint16_t *gx0, uint16_t *gy0, uint16_t *gx1, uint16_t *gy1){
    if (!L || !gx0 || !gy0 || !gx1 || !gy1) return 0u;
    *gx0 = L->bar_x0;
    *gy0 = L->bar_y0;
    *gx1 = (uint16_t)(L->bar_x0 + L->bar_w);
    *gy1 = (uint16_t)(L->bar_y0 + L->bar_h);
    if (*gx1 <= *gx0 || *gy1 <= *gy0) return 0u;
    return 1u;
}//------------ui_bargraph_get_plot_rect

static void ui_bargraph_build_active_product_text(char* out, uint16_t out_sz)
{
    const UiProductRecord* p;

    if (!out || out_sz == 0u) return;

    p = ui_product_store_get_active();

    if (!p || ui_product_store_count() == 0u)
    {
        snprintf(out, out_sz, "Sin productos activos");
        return;
    }

    snprintf(out, out_sz, "%u: %s", (unsigned)p->id, p->name);
}

void ui_bargraph_init(UiBarGraph *g, uint16_t threshold, uint8_t tol_pct)
{
    if (!g) return;

    for (uint16_t i = 0; i < BAR_HISTORY_MAX; ++i) g->history[i] = 0;
    g->count     = 0;
    g->threshold = clamp_u16(threshold, 0, 300);
    g->tol_pct   = tol_pct;

    g->period_ms = 250;
    g->next_ms   = platform_millis() + g->period_ms;
}

void ui_bargraph_set_threshold(UiBarGraph *g, uint16_t threshold)
{
    if (!g) return;
    g->threshold = clamp_u16(threshold, 0, 300);
}

void ui_bargraph_push(UiBarGraph *g, uint16_t sample_0_300)
{
    if (!g) return;

    uint16_t v = clamp_u16(sample_0_300, 0, 300);

    if (g->count < BAR_HISTORY_MAX) g->count++;

    // shift left
    for (uint16_t i = 0; i + 1 < g->count; ++i) {
        g->history[i] = g->history[i + 1];
    }
    g->history[g->count - 1] = v;
    g->last_value = v;

    // --- contadores (1 incremento por cada muestra nueva) ---
    if (v < COUNTER_SPLIT_TH) {
        ui_counters_increment_error();
    } else {
        ui_counters_increment_metal();
    }

}

void ui_bargraph_update(UiBarGraph *g, uint16_t sample){
    if (!g) return;
    uint32_t now = platform_millis();
    if ((int32_t)(now - g->next_ms) >= 0) {
        g->next_ms = now + g->period_ms;
        ui_bargraph_push(g, sample);}
}//fin de ui_bargraph_update-----------------------------


// Demo: cada period_ms mete una muestra random 0..300
void ui_bargraph_demo_update(UiBarGraph *g)
{
    if (!g) return;

    uint32_t now = platform_millis();
    if ((int32_t)(now - g->next_ms) >= 0) {
        g->next_ms = now + g->period_ms;
        uint16_t v = (uint16_t)(rand() % 301);
        ui_bargraph_push(g, v);
    }
}


static void ui_bargraph_draw_common(const UiBarGraph *g, const UiMainLayout *L, uint16_t gx0, uint16_t gy0, uint16_t gx1, uint16_t gy1){
char bufSignal[32];
char bufUmbral[32];
//char bufX[32], bufY[32];



    draw_axes_and_labels(gx0, gy0, gx1, gy1, L->font);
    draw_threshold_line(gx0, gy0, gx1, gy1, g->threshold);

    EVE_color_rgb(BLACK);

    snprintf(bufSignal, sizeof(bufSignal), "Signal: %u", (unsigned)g->last_value);
    snprintf(bufUmbral, sizeof(bufUmbral), "Umbral: %u", (unsigned)g->threshold);

    //snprintf(bufX, sizeof(bufX), "X: %u", (unsigned)g->rawX);
    //snprintf(bufY, sizeof(bufY), "Y: %u", (unsigned)g->rawY);


    const int16_t textY = (int16_t)(gy1 + 12);
    EVE_cmd_text((int16_t)(gx0 + 10),  textY, 24, 0, bufSignal);
    EVE_cmd_text((int16_t)(gx0 + 160), textY, 24, 0, bufUmbral);
    //EVE_cmd_text((int16_t)(gx0 + 310),  textY, 24, 0, bufX);   // ← agregar
    //EVE_cmd_text((int16_t)(gx0 + 400),  textY, 24, 0, bufY);

    const int16_t prodTextY = (int16_t)(gy0 - 50);
    char bufProd[48];

    ui_bargraph_build_active_product_text(bufProd, sizeof(bufProd));
    EVE_cmd_text_bold((int16_t)(gx0 - 15), prodTextY, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, bufProd);

}//----ui_bargraph_draw_common----------------------------------------


static void ui_bargraph_draw_common2(const UiBarGraph *g,
		                             const UiMainLayout *L,
									 uint16_t gx0,uint16_t gy0,
									 uint16_t gx1, uint16_t gy1){
char bufSignal[32];
char bufUmbral[32];
char bufX[32], bufY[32];


    draw_axes_and_labels(gx0, gy0, gx1, gy1, L->font);
    draw_threshold_line(gx0, gy0, gx1, gy1, g->threshold);
    EVE_color_rgb(BLACK);
    snprintf(bufSignal, sizeof(bufSignal), "Signal: %u", (unsigned)g->last_value);
    snprintf(bufUmbral, sizeof(bufUmbral), "Umbral: %u", (unsigned)g->threshold);
    snprintf(bufX, sizeof(bufX), "X: %u", (unsigned)g->rawX);
    snprintf(bufY, sizeof(bufY), "Y: %u", (unsigned)g->rawY);
    const int16_t textY = (int16_t)(gy1 + 12);
    EVE_cmd_text((int16_t)(gx0 + 10),  textY, 24, 0, bufSignal);
    EVE_cmd_text((int16_t)(gx0 + 160), textY, 24, 0, bufUmbral);
    EVE_cmd_text((int16_t)(gx0 + 310),  textY, 24, 0, bufX);   // ← agregar
    EVE_cmd_text((int16_t)(gx0 + 400),  textY, 24, 0, bufY);
    const int16_t prodTextY = (int16_t)(gy0 - 50);
    char bufProd[48];
    ui_bargraph_build_active_product_text(bufProd, sizeof(bufProd));
    EVE_cmd_text_bold((int16_t)(gx0 - 15), prodTextY, PRODUCT_FONT_SIZE, EVE_OPT_FLAT, bufProd);
}//----ui_bargraph_draw_common-------------------------------------------------------------







void ui_bargraph_draw(const UiBarGraph *g, const UiMainLayout *L){

	ui_bargraph_draw_mode(g, L, UI_BARGRAPH_MODE_BARS);
}//fin de


//estatus, se esta graficando actualmente  o no
#if defined(CMD_BARR_ANS) && (CMD_BARR_ANS == 0x4F) && defined(CMD_DDS_ANS) && (CMD_DDS_ANS == 0x1F)
uint8_t ui_barGraph_status(UiBarGraph *g,uint8_t bandera){
   switch(bandera){
   case  BARRA_ON:
   case    DDS_ON:
   case BARRA_REQ:
   case   DDS_REQ:g->status|=bandera;break;
   case GRAFIC_OFF:
   case    REQ_OFF:g->status&=bandera;break;
   case is_GRAPH_OK:if((g->status&0x0F)==0x0F||(g->status& 0x0F)==0x00){
	                     return TRUE;}//la graficacion es correcto la comunicacion y el menu
   	   	   	   	   	else{return FALSE;}break;
   case FIND_ERROR:uint8_t nibble =g->status & 0x0F;
                   if ((nibble & 0x05)== 0x04){
                           return 0x40;} // BARRAS_OFF;
                   else if((nibble & 0x05) == 0x01) {
                           return 0x4F;}// BARRAS_ON;
                   if((nibble&0x0A)==0x08){
                           return  0x10;}// DDS_OFF;
                   else if((nibble & 0x0A) == 0x02){
                           return 0x1F;} //DDS_ON;
                   break;
   default:return 0;break;}
return 0;
}//fin de status bargrap status
#endif




static void ui_bargraph_draw_as_bars(const UiBarGraph *g, const UiMainLayout *L, uint16_t gx0, uint16_t gy0, uint16_t gx1, uint16_t gy1)
{
    uint16_t th = g->threshold;
    uint16_t tol = (uint16_t)(((uint32_t)th * (uint32_t)g->tol_pct) / 100U);
    uint16_t thLow  = (th > tol) ? (uint16_t)(th - tol) : 0;
    uint16_t thHigh = (uint16_t)(th + tol);
    if (thHigh > 300) thHigh = 300;

    uint16_t barW   = L->bar_w_px;
    uint16_t gap    = L->bar_gap_px;
    uint16_t perBar = (uint16_t)(barW + gap);
    if (perBar == 0) return;

    uint16_t w = (uint16_t)(gx1 - gx0);
    uint16_t maxBarsByWidth = (uint16_t)(w / perBar);
    if (maxBarsByWidth == 0) maxBarsByWidth = 1;

    uint16_t maxBars = g->count;
    if (maxBars > maxBarsByWidth) maxBars = maxBarsByWidth;

    EVE_begin(EVE_RECTS);

    for (uint16_t i = 0; i < maxBars; ++i)
    {
        uint16_t histIdx = (uint16_t)(g->count - 1 - i);
        uint16_t v = g->history[histIdx];
        uint16_t yTop = map_value_to_y(v, gy0, gy1);

        uint16_t x1 = (uint16_t)(gx1 - L->bar_margin_r - i * perBar);
        uint16_t x0 = (uint16_t)(x1 - barW);
        if (x0 < gx0) x0 = gx0;

        if (v < thLow) {
            EVE_color_rgb(GREEN);
        } else if (v > thHigh) {
            EVE_color_rgb(RED);
        } else {
            EVE_color_rgb(YELLOW);
        }

        EVE_vertex2f((int16_t)(x0 * PRESICION), (int16_t)(yTop * PRESICION));
        EVE_vertex2f((int16_t)(x1 * PRESICION), (int16_t)(gy1 * PRESICION));
    }

    EVE_end();
}

static void ui_bargraph_draw_as_line(const UiBarGraph *g, const UiMainLayout *L, uint16_t gx0, uint16_t gy0, uint16_t gx1, uint16_t gy1)
{
    uint16_t plotW;		uint16_t maxPts;		uint16_t startIdx;		uint16_t drawCount;
    uint16_t th;		uint16_t tol;			uint16_t thLow;			uint16_t thHigh;

    if (!g) return;
    if (g->count < 2) return;

    plotW = (uint16_t)(gx1 - gx0);
    if (plotW < 2) return;

    /* máximo de puntos visibles: 1 punto por pixel aprox */
    maxPts = g->count;
    if (maxPts > plotW) maxPts = plotW;
    if (maxPts < 2) return;

    startIdx  = (uint16_t)(g->count - maxPts);
    drawCount = maxPts;

    th = g->threshold;
    tol = (uint16_t)(((uint32_t)th * (uint32_t)g->tol_pct) / 100U);
    thLow  = (th > tol) ? (uint16_t)(th - tol) : 0;
    thHigh = (uint16_t)(th + tol);
    if (thHigh > 300) thHigh = 300;

    EVE_begin(EVE_LINES);
    EVE_line_width(10U * PRESICION);

    for (uint16_t i = 0; i + 1 < drawCount; ++i)
    {
        uint16_t idx0 = (uint16_t)(startIdx + i);
        uint16_t idx1 = (uint16_t)(startIdx + i + 1);

        uint16_t v0 = g->history[idx0];
        uint16_t v1 = g->history[idx1];

        uint16_t x0 = (uint16_t)(gx0 + ((uint32_t)i * (gx1 - gx0)) / (drawCount - 1));
        uint16_t x1 = (uint16_t)(gx0 + ((uint32_t)(i + 1) * (gx1 - gx0)) / (drawCount - 1));

        uint16_t y0 = map_value_to_y(v0, gy0, gy1);
        uint16_t y1 = map_value_to_y(v1, gy0, gy1);

        /* color por banda usando el punto más reciente del segmento */
        if (v1 < thLow) {
            EVE_color_rgb(GREEN);
        } else if (v1 > thHigh) {
            EVE_color_rgb(RED);
        } else {
            EVE_color_rgb(YELLOW);
        }

        EVE_vertex2f((int16_t)(x0 * PRESICION), (int16_t)(y0 * PRESICION));
        EVE_vertex2f((int16_t)(x1 * PRESICION), (int16_t)(y1 * PRESICION));
    }

    EVE_end();
}
/* GRaficador de submenu Dinamico de
 *  barras de deteccion en Portal-Inicio*/
void ui_bargraph_draw_mode(const UiBarGraph *g, const UiMainLayout *L, UiBarGraphMode mode){
uint16_t gx0, gy0, gx1, gy1;

	if (!g || !L) return;
	if (!ui_bargraph_get_plot_rect(L, &gx0, &gy0, &gx1, &gy1)) return;
	ui_bargraph_draw_common(g, L, gx0, gy0, gx1, gy1);
	switch (mode){
		case UI_BARGRAPH_MODE_LINE:
			ui_bargraph_draw_as_line(g, L, gx0, gy0, gx1, gy1);break;
		case UI_BARGRAPH_MODE_BARS:
		default: comm.bits.b.Req_Barras=1;//Se requiere cmd barras
			    ui_bargraph_draw_as_bars(g, L, gx0, gy0, gx1, gy1);break;}
}//fin --------------------------------------------------------------



/*Pantalla de Grafico de DDS en Portal Inicio*/
void ui_DDS_draw(const UiBarGraph *g, const UiMainLayout *L){
uint16_t gx0, gy0, gx1, gy1;
    if (!g || !L) return;
    if (!ui_bargraph_get_plot_rect(L, &gx0, &gy0, &gx1, &gy1)) return;
    ui_bargraph_draw_common2(g, L, gx0, gy0, gx1, gy1);
    ui_bargraph_draw_as_line(g, L, gx0, gy0, gx1, gy1);
}//fin --------------------------------------------------------------












