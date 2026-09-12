/*
 * ui_layout.c
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#include "ui_layout.h"
#include <stdint.h>

/* PARÁMETROS BASE DE PANTALLA  */
#define UI_SCREEN_W                 800u
#define UI_SCREEN_H                 480u

/* MÁRGENES / GAPS GENERALES */
#define UI_TOP_MARGIN_Y             3u
#define UI_RIGHT_MARGIN             6u
#define UI_LEFT_MARGIN_ARROW        5u
#define UI_BOTTOM_MARGIN_PAGER      15u

#define UI_GAP_TOP_TO_FOOTER        2u
#define UI_GAP_FOOTER_TO_CONTENT    5u
#define UI_GAP_LEFT_ARROW_TO_CONTENT 1u
#define UI_GAP_CLOCK_TO_CONTENT     5u
#define UI_GAP_PAGER_TO_CLOCK       10u

/*  BOTONES SUPERIORES */
#define UI_TOP_BTN_W                94u
#define UI_TOP_BTN_H                70u
#define UI_TOP_BTN_GAP              5u

/* botón superior más a la derecha */
#define UI_TOP_BTN_X2               (UI_SCREEN_W - UI_RIGHT_MARGIN - UI_TOP_BTN_W)
#define UI_TOP_BTN_Y                UI_TOP_MARGIN_Y

/* FOOTER */
#define UI_FOOTER_X0                1u
#define UI_FOOTER_H                 50u
#define UI_FOOTER_W                 (UI_SCREEN_W - UI_FOOTER_X0)
#define UI_FOOTER_Y0                (UI_TOP_BTN_Y + UI_TOP_BTN_H + UI_GAP_TOP_TO_FOOTER)

/*  FLECHAS DE NAVEGACIÓN */
#define UI_ARW_W                    94u
#define UI_ARW_H                    70u

#define UI_ARW_X_LEFT               UI_LEFT_MARGIN_ARROW
#define UI_ARW_X_RIGHT              (UI_SCREEN_W - UI_RIGHT_MARGIN - UI_ARW_W)

/* */
#define UI_ARW_Y_FROM_CONTENT       110u

/*  BANDA INFERIOR (PAGERS / RELOJ / USUARIO) */
#define UI_PAGER_X0                 15u
#define UI_PAGER_Y                  (UI_SCREEN_H - UI_BOTTOM_MARGIN_PAGER)
#define UI_PAGER_SPAN               150u
#define UI_PAGER_POINT_SIZE         200u

#define UI_CLOCK_X_FROM_CONTENT     180u
#define UI_USER_X_FROM_CONTENT      290u

#define UI_CLOCK_Y                  (UI_PAGER_Y - UI_GAP_PAGER_TO_CLOCK)
#define UI_USER_Y                   UI_CLOCK_Y

/*  ÁREA ÚTIL DE CONTENIDO */
#define UI_CONTENT_X0               (UI_ARW_X_LEFT + UI_ARW_W + UI_GAP_LEFT_ARROW_TO_CONTENT)
#define UI_CONTENT_X1               UI_ARW_X_RIGHT

#define UI_CONTENT_Y0               (UI_FOOTER_Y0 + UI_FOOTER_H + UI_GAP_FOOTER_TO_CONTENT)
#define UI_CONTENT_Y1               (UI_CLOCK_Y - UI_GAP_CLOCK_TO_CONTENT)

/* GRÁFICA */
#define UI_BAR_W                    280u
#define UI_BAR_H                    140u

/*  */
#define UI_BAR_X_FROM_CONTENT       70u
#define UI_BAR_Y_FROM_CONTENT       90u

#define UI_BAR_X0                   (UI_CONTENT_X0 + UI_BAR_X_FROM_CONTENT)
#define UI_BAR_Y0                   (UI_CONTENT_Y0 + UI_BAR_Y_FROM_CONTENT)

#define UI_BAR_W_PX                 20u
#define UI_BAR_GAP_PX               4u
#define UI_BAR_MARGIN_R             4u

/* BOTONES LATERALES DE LA GRÁFICA */
#define UI_SIDE_BTN_W               150u
#define UI_SIDE_BTN_H               80u
#define UI_SIDE_BTN_GAP             10u

/*  Mantienen exactamente tu geometría actual: */
#define UI_SIDE_BTN_X_FROM_BAR      60u
#define UI_SIDE_BTN_Y_FROM_CONTENT  20u

#define UI_SIDE_BTN_X               (UI_BAR_X0 + UI_BAR_W + UI_SIDE_BTN_X_FROM_BAR)
#define UI_SIDE_BTN_Y0              (UI_CONTENT_Y0 + UI_SIDE_BTN_Y_FROM_CONTENT)

/* FUENTES / TEXTOS */
#define UI_MAIN_FONT                23u

/*  LAYOUT PRINCIPAL  */
const UiMainLayout UI_LAYOUT_MAIN =
{
    /* 3 BOTONES SUPERIOR DERECHA - HOME, USUARIO, CONFIGURACIÓN */
    .top_btn_y   = UI_TOP_BTN_Y,
    .top_btn_x2  = UI_TOP_BTN_X2,
    .top_btn_w   = UI_TOP_BTN_W,
    .top_btn_h   = UI_TOP_BTN_H,
    .top_btn_gap = UI_TOP_BTN_GAP,

    /* RECTÁNGULO/BARRA DE ESTADOS "FOOTER" */
    .footer_x0 = UI_FOOTER_X0,
    .footer_y0 = UI_FOOTER_Y0,
    .footer_w  = UI_FOOTER_W,
    .footer_h  = UI_FOOTER_H,

    /* ÁREA ÚTIL DE CONTENIDO */
    .content_x0 = UI_CONTENT_X0,
    .content_x1 = UI_CONTENT_X1,
    .content_y0 = UI_CONTENT_Y0,
    .content_y1 = UI_CONTENT_Y1,

    /* VARIABLES GRÁFICA */
    .bar_x0 = UI_BAR_X0,
    .bar_y0 = UI_BAR_Y0,
    .bar_w  = UI_BAR_W,
    .bar_h  = UI_BAR_H,

    .bar_w_px     = UI_BAR_W_PX,
    .bar_gap_px   = UI_BAR_GAP_PX,
    .bar_margin_r = UI_BAR_MARGIN_R,

    /* 3 BOTONES APILADOS A LA DERECHA DE LA GRÁFICA */
    .side_btn_w   = UI_SIDE_BTN_W,
    .side_btn_h   = UI_SIDE_BTN_H,
    .side_btn_gap = UI_SIDE_BTN_GAP,
    .side_btn_x   = UI_SIDE_BTN_X,
    .side_btn_y0  = UI_SIDE_BTN_Y0,

    /* FLECHAS DE NAVEGACIÓN IZQUIERDA / DERECHA */
    .arw_w       = UI_ARW_W,
    .arw_h       = UI_ARW_H,
    .arw_y       = (UI_CONTENT_Y0 + UI_ARW_Y_FROM_CONTENT),
    .arw_x_left  = UI_ARW_X_LEFT,
    .arw_x_right = UI_ARW_X_RIGHT,

    /* PAGERS */
    .pager_x0         = UI_PAGER_X0,
    .pager_y          = UI_PAGER_Y,
    .pager_span       = UI_PAGER_SPAN,
    .pager_point_size = UI_PAGER_POINT_SIZE,

    /* RELOJ / USUARIO */
    .clock_x = (UI_CONTENT_X0 + UI_CLOCK_X_FROM_CONTENT),
    .clock_y = UI_CLOCK_Y,
    .user_x  = (UI_CONTENT_X0 + UI_USER_X_FROM_CONTENT),
    .user_y  = UI_USER_Y,

    .font = UI_MAIN_FONT,
};
