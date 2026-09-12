/*
 * ui_layout.h
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>


typedef struct
{
    // Topbar
    uint16_t clock_x, clock_y;
    uint16_t user_x,  user_y;
    uint16_t font;

    // Bar graph
    uint16_t bar_x0, bar_y0;
    uint16_t bar_w,  bar_h;

    uint16_t bar_w_px;
    uint16_t bar_gap_px;
    uint16_t bar_margin_r;

    // Top-right 3 botones
    uint16_t top_btn_y;
    uint16_t top_btn_w;
    uint16_t top_btn_h;
    uint16_t top_btn_gap;
    uint16_t top_btn_x2;  // x del botón más a la derecha (btn2), los demás se derivan

    // 2 botones al lado de la gráfica (apilados)
    uint16_t side_btn_w;
    uint16_t side_btn_h;
    uint16_t side_btn_gap;
    uint16_t side_btn_x;
    uint16_t side_btn_y0; // y del botón superior (btn0)

    // Flechas izq/der
    uint16_t arw_w;
    uint16_t arw_h;
    uint16_t arw_y;
    uint16_t arw_x_left;
    uint16_t arw_x_right;

    // Footer
    uint16_t footer_x0;
    uint16_t footer_y0;
    uint16_t footer_w;
    uint16_t footer_h;

    // Pagers (indicadores de pantalla)
    uint16_t pager_x0;          // x del primer punto
    uint16_t pager_y;           // y (centro) de los puntos
    uint16_t pager_span;        // ancho total donde se distribuyen los 5 puntos
    uint16_t pager_point_size;  // tamaño para EVE_point_size (en unidades 1/16 px)

    // Content area (zona donde dibujan las pantallas/submenús)
    uint16_t content_x0;
    uint16_t content_y0;
    uint16_t content_x1;
    uint16_t content_y1;


} UiMainLayout;

extern const UiMainLayout UI_LAYOUT_MAIN;
