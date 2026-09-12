/*
 * ui_almacena_productos.h
 *
 *  Created on: 11 mar. 2026
 *      Author: oscar
 */

#pragma once

#include <stdint.h>

#define UI_PRODUCT_MAX_COUNT   30
#define UI_PRODUCT_NAME_MAX    20

typedef struct
{
    uint8_t used;                              /* 0 = libre, 1 = ocupado */
    uint8_t id;                                /* 1..30 */
    char    name[UI_PRODUCT_NAME_MAX + 1];     /* 20 + '\0' */
} UiProductRecord;

typedef enum
{
    UI_PROD_OK = 0,
    UI_PROD_ERR_EMPTY_NAME,
    UI_PROD_ERR_FULL
} UiProductResult;

/* Inicialización / limpieza RAM */
void ui_product_store_init(void);
void ui_product_store_clear_ram(void);

/* Estado general */
uint8_t ui_product_store_count(void);
uint8_t ui_product_store_is_empty(void);
uint8_t ui_product_store_next_free_id(void);

/* Altas / bajas */
UiProductResult ui_product_store_add_autoid(const char* name, uint8_t* out_id);

/* Esta función se usará para restaurar desde flash respetando el ID guardado */
UiProductResult ui_product_store_add_with_id(const char* name, uint8_t fixed_id);

uint8_t ui_product_store_delete_by_id(uint8_t id);

/* Consultas */
const UiProductRecord* ui_product_store_get_by_index(uint8_t index);
const UiProductRecord* ui_product_store_get_by_id(uint8_t id);

/* Producto activo */
uint8_t ui_product_store_get_active_id(void);
const UiProductRecord* ui_product_store_get_active(void);
uint8_t ui_product_store_set_active_by_id(uint8_t id);
int8_t ui_product_store_find_visible_index_by_id(uint8_t id);

//#pragma once
//
//#include <stdint.h>
//
//#define UI_PRODUCT_MAX_COUNT   30
//#define UI_PRODUCT_NAME_MAX    20
//
//typedef struct
//{
//    uint8_t used;                              // 0 = libre, 1 = ocupado
//    uint8_t id;                                // 1..30
//    char    name[UI_PRODUCT_NAME_MAX + 1];     // 20 + '\0'
//} UiProductRecord;
//
//typedef enum
//{
//    UI_PROD_OK = 0,
//    UI_PROD_ERR_EMPTY_NAME,
//    UI_PROD_ERR_FULL
//} UiProductResult;
//
//void ui_product_store_init(void);
//
//uint8_t ui_product_store_count(void);
//uint8_t ui_product_store_next_free_id(void);
//uint8_t ui_product_store_delete_by_id(uint8_t id);
//uint8_t ui_product_store_is_empty(void);
//
//UiProductResult ui_product_store_add_autoid(const char* name, uint8_t* out_id);
//
//const UiProductRecord* ui_product_store_get_by_index(uint8_t index);
//const UiProductRecord* ui_product_store_get_by_id(uint8_t id);
//
//uint8_t ui_product_store_get_active_id(void);
//const UiProductRecord* ui_product_store_get_active(void);
//uint8_t ui_product_store_set_active_by_id(uint8_t id);
//int8_t ui_product_store_find_visible_index_by_id(uint8_t id);
//
////uint8_t ui_product_store_save_to_flash(void);
////uint8_t ui_product_store_load_from_flash(void);
//void ui_product_store_clear_ram(void);
//UiProductResult ui_product_store_add_with_id(const char* name, uint8_t fixed_id);
