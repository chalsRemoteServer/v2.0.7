/*
 * ui_persist_productos.h
 *
 *  Created on: 2 abr. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

typedef enum
{
    UI_PERSIST_PROD_OK = 1,

    UI_PERSIST_PROD_ERR_DLOG_INIT      = -1,
    UI_PERSIST_PROD_ERR_PAGE_SIZE      = -2,
    UI_PERSIST_PROD_ERR_PAGE_RANGE     = -3,
    UI_PERSIST_PROD_ERR_DLOG_READ      = -4,
    UI_PERSIST_PROD_ERR_INVALID_BLOCK  = -5,
    UI_PERSIST_PROD_ERR_RESTORE_ITEM   = -6,
    UI_PERSIST_PROD_ERR_RESTORE_ACTIVE = -7,

    UI_PERSIST_PROD_ERR_BUILD_BLOCK    = -8,
    UI_PERSIST_PROD_ERR_DLOG_ERASE     = -9,
    UI_PERSIST_PROD_ERR_DLOG_PROG      = -10
} UiPersistProductosResult;

int8_t ui_persist_productos_init(void);
int8_t ui_persist_productos_save(void);
int8_t ui_persist_productos_load(void);
int8_t ui_persist_productos_clear(void);
