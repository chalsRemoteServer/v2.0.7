/*
 * ui_bargraph.h
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>
#include "ui_layout.h"

#ifndef BAR_HISTORY_MAX
#define BAR_HISTORY_MAX 16
#endif

//control de Graficacion y su comunicacion
#define BARRA_ON    0x01
#define DDS_ON      0x02
#define BARRA_REQ   0x04//xxxx x1xx
#define DDS_REQ     0x08
#define GRAFIC_OFF  0xFC  //xxxx xx00
#define REQ_OFF     0xF3 //xxxx 00xx Requerimeinto de graficacion apagado
#define is_GRAPH_OK 0x7A//esta ok la graficacion?
#define FIND_ERROR  0x7B //encuentra el comando que hace falta mandar
//fin de graficacion y su comunicacion

#define STATUS_ON  0x7C
#define STATUS_OFF 0x7D

typedef struct
{
    uint16_t history[BAR_HISTORY_MAX];
    uint16_t count;

    uint16_t threshold;     // 0..300
    uint8_t  tol_pct;       // ej. 10 (%)

    uint16_t last_value;    // <-- NUEVO: último valor mostrado (para texto "Signal")
    uint16_t rawX;//valores de X de deteccion en raw
    uint16_t rawY;//valor de y en raw de deteccion real

    // Para demo/updates temporizados
    uint32_t next_ms;
    uint32_t period_ms;

    uint8_t status;//indica 1|0 si se esta graficando actualmente, o no. si estas corriendo en el menu
} UiBarGraph;

typedef enum
{
    UI_BARGRAPH_MODE_BARS = 0,
    UI_BARGRAPH_MODE_LINE = 1
} UiBarGraphMode;

void ui_bargraph_init(UiBarGraph *g, uint16_t threshold, uint8_t tol_pct);
void ui_bargraph_set_threshold(UiBarGraph *g, uint16_t threshold);
void ui_bargraph_push(UiBarGraph *g, uint16_t sample_0_300);

// demo opcional
void ui_bargraph_demo_update(UiBarGraph *g);
void ui_bargraph_update(UiBarGraph *g, uint16_t sample);

void ui_bargraph_draw(const UiBarGraph *g, const UiMainLayout *L);
void ui_bargraph_draw_mode(const UiBarGraph *g, const UiMainLayout *L, UiBarGraphMode mode);

void ui_DDS_draw(const UiBarGraph *g, const UiMainLayout *L);
uint8_t ui_barGraph_status(UiBarGraph *g,uint8_t bandera);
