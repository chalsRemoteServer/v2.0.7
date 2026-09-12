/*
 * g_screen_5.c
 *
 *  Created on: 30 mar. 2026
 *      Author: oscar
 */

#include "ui_layout.h"
#include "ui_pantallas.h"
#include "ui_bargraph.h"
#include "EVE.h"
#include "EVE_commands.h"
#include "colores.h"
#include "Posicion_Objetos.h"
#include <stdint.h>

extern UiBarGraph g_bar;

static UiMainLayout s_screen5_layout;
static uint8_t s_screen5_layout_init = 0u;

#define SC5_GRAPH_W   420u
#define SC5_GRAPH_H   170u
#define SC5_TITLE_X   20u

static void screen5_build_layout(void)
{
    const UiMainLayout *B = &UI_LAYOUT_MAIN;

    s_screen5_layout = *B;   // copiar layout base

    /* área útil disponible dentro del content */
    const uint16_t content_w = (uint16_t)(B->content_x1 - B->content_x0);
    const uint16_t content_h = (uint16_t)(B->content_y1 - B->content_y0);

    /* reservar espacio para título */
    const uint16_t title_y   = (uint16_t)(B->content_y0 + 10u);
    const uint16_t title_h   = 34u;
    const uint16_t title_gap = 18u;

    const uint16_t plot_area_y0 = (uint16_t)(title_y + title_h + title_gap);
    const uint16_t plot_area_y1 = B->content_y1;

    const uint16_t plot_area_h = (uint16_t)(plot_area_y1 - plot_area_y0);

    uint16_t gw = SC5_GRAPH_W;
    uint16_t gh = SC5_GRAPH_H;

    /* protección por si luego cambias content o tamaño */
    if (gw > content_w)    gw = (uint16_t)(content_w - 10u);
    if (gh > plot_area_h)  gh = (uint16_t)(plot_area_h - 10u);

    s_screen5_layout.bar_w  = gw;
    s_screen5_layout.bar_h  = gh;

    /* centrado horizontal dentro de content */
    s_screen5_layout.bar_x0 = (uint16_t)(B->content_x0 + ((content_w - gw) / 2u));

    /* centrado vertical dentro del área disponible bajo el título */
    s_screen5_layout.bar_y0 = (uint16_t)(plot_area_y0 + ((plot_area_h - gh) / 2u));

    /* parámetros internos de la gráfica por si luego los quieres afinar */
    s_screen5_layout.bar_w_px     = 10u;
    s_screen5_layout.bar_gap_px   = 2u;
    s_screen5_layout.bar_margin_r = 2u;

    s_screen5_layout_init = 1u;
}
//Pantalla Grafica DDS, SubMenu Portal Inicio
static void scr5_draw(void){
const UiMainLayout *L;

    if (!s_screen5_layout_init) {
        screen5_build_layout(); }
    L = &s_screen5_layout;
    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold((int16_t)SC5_TITLE_X, (int16_t)(UI_LAYOUT_MAIN.content_y0 + 10u), PRODUCT_FONT_SIZE, EVE_OPT_FLAT, "Metal signal");
    ui_DDS_draw(&g_bar, L);
    ui_barGraph_status(&g_bar,DDS_ON);

}// fin screen5--------------------------------------------------

static void scr5_on_enter(void){
    if (!s_screen5_layout_init) {
        screen5_build_layout();}
}//---------------------------------------

static void scr5_update(uint32_t now_ms){
    (void)now_ms;
}//-----------------------------------------

const UiScreen g_screen_5 = {
    .on_enter = scr5_on_enter,
    .on_exit  = 0,
    .update   = scr5_update,
    .draw     = scr5_draw
};
