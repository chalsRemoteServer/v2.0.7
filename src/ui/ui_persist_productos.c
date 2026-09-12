/*
 * ui_persist_productos.c
 *
 *  Created on: 2 abr. 2026
 *      Author: oscar
 */

/*
 * ui_persist_productos.c
 *
 *  Persistencia de productos en flash interna usando dlog
 */

#include "ui_persist_productos.h"
#include "ui_almacena_productos.h"
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "EVE_config.h"
#include "EVE.h"
#include "HAL.h"
#include <ft900_dlog.h>
#include "eve_app.h"
#include "eve.h"
#include "EVE_arduino.h"


/*
 * Igual que en eve_app.c, el área de datalogger en flash viene definida
 * por la infraestructura del proyecto / startup.
 */
extern __flash__ uint32_t __dlog_partition[];

/*
 * IMPORTANTE:
 * - Esta versión piloto usa la partición dlog para productos.
 * - Como dlog_erase() borra la partición completa, no conviene usar a la vez
 *   otra persistencia basada en dlog (por ejemplo touch persistente) hasta
 *   unificar ambas en un solo "storage manager".
 */

#define UI_PROD_PERSIST_MAGIC        0x50524F44UL  /* 'PROD' */
#define UI_PROD_PERSIST_VERSION      0x0001u
#define UI_PROD_PERSIST_FIRST_PAGE   1u

/*
 * Tamaño máximo temporal de página soportado por este piloto.
 * En la práctica, para FT900 + dlog normalmente el tamaño es mucho menor
 * (por ejemplo ~256 bytes), pero aquí dejamos margen.
 */
#define UI_PROD_PERSIST_MAX_PAGEBUF  1024u

typedef struct
{
    uint32_t magic;
    uint16_t version;
    uint16_t total_size;

    uint8_t  product_count;
    uint8_t  active_product_id;
    uint16_t reserved;

    UiProductRecord products[UI_PRODUCT_MAX_COUNT];

    uint32_t crc;
} UiProductPersistBlock;

static uint8_t ui_persist_checksum32(const uint8_t* data, uint32_t len, uint32_t seed)
{
    uint32_t i;
    uint32_t hash = seed;

    if (!data) return 0u;

    /* FNV-1a simple para esta primera versión */
    for (i = 0u; i < len; i++)
    {
        hash ^= (uint32_t)data[i];
        hash *= 16777619UL;
    }

    return (uint8_t)(hash != 0u);
}

/* Devuelve el hash real en 32 bits */
static uint32_t ui_persist_checksum32_full(const uint8_t* data, uint32_t len)
{
    uint32_t i;
    uint32_t hash = 2166136261UL;

    if (!data) return 0u;

    for (i = 0u; i < len; i++)
    {
        hash ^= (uint32_t)data[i];
        hash *= 16777619UL;
    }

    return hash;
}

static uint8_t ui_persist_products_flash_init(int* pgsz_out, int* pages_out)
{
    int ret;
    int pgsz = 0;
    int pages = 0;

    ret = dlog_init(__dlog_partition, &pgsz, &pages);

    if (ret < 0)
    {
        return 0u;
    }

    if (pgsz <= 0 || pages <= 0)
    {
        return 0u;
    }

    if (pgsz_out)  *pgsz_out = pgsz;
    if (pages_out) *pages_out = pages;

    return 1u;
}

static uint8_t ui_persist_products_build_block(UiProductPersistBlock* blk)
{
    uint8_t i;
    uint8_t count;
    const UiProductRecord* p;

    if (!blk) return 0u;

    memset(blk, 0, sizeof(UiProductPersistBlock));

    blk->magic            = UI_PROD_PERSIST_MAGIC;
    blk->version          = UI_PROD_PERSIST_VERSION;
    blk->total_size       = (uint16_t)sizeof(UiProductPersistBlock);
    blk->product_count    = ui_product_store_count();
    blk->active_product_id = ui_product_store_get_active_id();
    blk->reserved         = 0u;

    count = ui_product_store_count();
    if (count > UI_PRODUCT_MAX_COUNT)
    {
        return 0u;
    }

    for (i = 0u; i < count; i++)
    {
        p = ui_product_store_get_by_index(i);
        if (!p) return 0u;

        blk->products[i] = *p;
    }

    blk->crc = 0u;
    blk->crc = ui_persist_checksum32_full((const uint8_t*)blk,
                                          (uint32_t)offsetof(UiProductPersistBlock, crc));

    return 1u;
}

static uint8_t ui_persist_products_block_validate(const UiProductPersistBlock* blk)
{
    uint32_t crc_calc;
    uint8_t i;
    uint8_t active_ok = 0u;

    if (!blk) return 0u;

    if (blk->magic != UI_PROD_PERSIST_MAGIC) return 0u;
    if (blk->version != UI_PROD_PERSIST_VERSION) return 0u;
    if (blk->total_size != (uint16_t)sizeof(UiProductPersistBlock)) return 0u;
    if (blk->product_count > UI_PRODUCT_MAX_COUNT) return 0u;

    crc_calc = ui_persist_checksum32_full((const uint8_t*)blk,
                                          (uint32_t)offsetof(UiProductPersistBlock, crc));
    if (crc_calc != blk->crc) return 0u;

    if (blk->product_count == 0u)
    {
        if (blk->active_product_id != 0u) return 0u;
        return 1u;
    }

    if (blk->active_product_id == 0u)
    {
        return 0u;
    }

    for (i = 0u; i < blk->product_count; i++)
    {
        if (!blk->products[i].used) return 0u;
        if (blk->products[i].id == 0u || blk->products[i].id > UI_PRODUCT_MAX_COUNT) return 0u;
        if (blk->products[i].name[0] == '\0') return 0u;

        if (blk->products[i].id == blk->active_product_id)
        {
            active_ok = 1u;
        }
    }

    if (!active_ok) return 0u;

    return 1u;
}

//uint8_t ui_persist_productos_init(void)
//{
//    int pgsz, pages;
//    return ui_persist_products_flash_init(&pgsz, &pages);
//}

int8_t ui_persist_productos_init(void)
{
    int pgsz, pages;

    if (!ui_persist_products_flash_init(&pgsz, &pages))
    {
        return UI_PERSIST_PROD_ERR_DLOG_INIT;
    }

    return UI_PERSIST_PROD_OK;
}

int8_t ui_persist_productos_save(void)
{
    UiProductPersistBlock blk;
    uint8_t pagebuf[UI_PROD_PERSIST_MAX_PAGEBUF] __attribute__((aligned(4)));
    int pgsz = 0;
    int pages = 0;
    uint32_t total_size;
    uint32_t pages_needed;
    uint32_t page_idx;
    uint32_t offset = 0u;

//    if (!ui_persist_products_flash_init(&pgsz, &pages))
//    {
//        return 0u;
//    }
//
    if (!ui_persist_products_flash_init(&pgsz, &pages))
    {
        return UI_PERSIST_PROD_ERR_DLOG_INIT;
    }

    if ((uint32_t)pgsz > UI_PROD_PERSIST_MAX_PAGEBUF)
    {
        return UI_PERSIST_PROD_ERR_PAGE_SIZE;
    }

    if (!ui_persist_products_build_block(&blk))
    {
        return UI_PERSIST_PROD_ERR_BUILD_BLOCK;
    }

//    if ((uint32_t)pgsz > UI_PROD_PERSIST_MAX_PAGEBUF)
//    {
//        return 0u;
//    }

//    if (!ui_persist_products_build_block(&blk))
//    {
//        return 0u;
//    }

    total_size = (uint32_t)sizeof(UiProductPersistBlock);
    pages_needed = (total_size + (uint32_t)pgsz - 1u) / (uint32_t)pgsz;

    if ((UI_PROD_PERSIST_FIRST_PAGE + pages_needed - 1u) > (uint32_t)pages)
    {
        return UI_PERSIST_PROD_ERR_PAGE_RANGE;
    }

//    if ((UI_PROD_PERSIST_FIRST_PAGE + pages_needed - 1u) > (uint32_t)pages)
//    {
//        return 0u;
//    }

    /* OJO: esto borra toda la partición dlog */

    if (dlog_erase() < 0)
    {
        return UI_PERSIST_PROD_ERR_DLOG_ERASE;
    }
//    if (dlog_erase() < 0)
//    {
//        return 0u;
//    }

    for (page_idx = 0u; page_idx < pages_needed; page_idx++)
    {
        uint32_t bytes_left;
        uint32_t chunk;

        memset(pagebuf, 0xFF, (size_t)pgsz);

        bytes_left = total_size - offset;
        chunk = (bytes_left > (uint32_t)pgsz) ? (uint32_t)pgsz : bytes_left;

        memcpy(pagebuf, ((const uint8_t*)&blk) + offset, (size_t)chunk);

        if (dlog_prog((int)(UI_PROD_PERSIST_FIRST_PAGE + page_idx), (uint32_t*)pagebuf) < 0)
        {
            return UI_PERSIST_PROD_ERR_DLOG_PROG;
        }

//        if (dlog_prog((int)(UI_PROD_PERSIST_FIRST_PAGE + page_idx), (uint32_t*)pagebuf) < 0)
//        {
//            return 0u;
//        }

        offset += chunk;
    }

    //return 1u;
    return UI_PERSIST_PROD_OK;
}

int8_t ui_persist_productos_load(void)
{
    UiProductPersistBlock blk;
    uint8_t pagebuf[UI_PROD_PERSIST_MAX_PAGEBUF] __attribute__((aligned(4)));
    int pgsz = 0;
    int pages = 0;
    uint32_t total_size;
    uint32_t pages_needed;
    uint32_t page_idx;
    uint32_t offset = 0u;
    uint8_t i;

//    if (!ui_persist_products_flash_init(&pgsz, &pages))
//    {
//        return 0u;
//    }

    if (!ui_persist_products_flash_init(&pgsz, &pages))
    {
        return UI_PERSIST_PROD_ERR_DLOG_INIT;
    }

    if ((uint32_t)pgsz > UI_PROD_PERSIST_MAX_PAGEBUF)
    {
        return UI_PERSIST_PROD_ERR_PAGE_SIZE;
    }

//    if ((uint32_t)pgsz > UI_PROD_PERSIST_MAX_PAGEBUF)
//    {
//        return 0u;
//    }

    memset(&blk, 0, sizeof(UiProductPersistBlock));

    total_size = (uint32_t)sizeof(UiProductPersistBlock);
    pages_needed = (total_size + (uint32_t)pgsz - 1u) / (uint32_t)pgsz;

    if ((UI_PROD_PERSIST_FIRST_PAGE + pages_needed - 1u) > (uint32_t)pages)
    {
        return UI_PERSIST_PROD_ERR_PAGE_RANGE;
    }

//    if ((UI_PROD_PERSIST_FIRST_PAGE + pages_needed - 1u) > (uint32_t)pages)
//    {
//        return 0u;
//    }

    for (page_idx = 0u; page_idx < pages_needed; page_idx++)
    {
        uint32_t bytes_left;
        uint32_t chunk;

        memset(pagebuf, 0x00, (size_t)pgsz);

        if (dlog_read((int)(UI_PROD_PERSIST_FIRST_PAGE + page_idx), (uint32_t*)pagebuf) < 0)
        {
            return UI_PERSIST_PROD_ERR_DLOG_READ;
        }
//
//        if (dlog_read((int)(UI_PROD_PERSIST_FIRST_PAGE + page_idx), (uint32_t*)pagebuf) < 0)
//        {
//            return 0u;
//        }

        bytes_left = total_size - offset;
        chunk = (bytes_left > (uint32_t)pgsz) ? (uint32_t)pgsz : bytes_left;

        memcpy(((uint8_t*)&blk) + offset, pagebuf, (size_t)chunk);

        offset += chunk;
    }

    if (!ui_persist_products_block_validate(&blk))
    {
        return UI_PERSIST_PROD_ERR_INVALID_BLOCK;
    }

//    if (!ui_persist_products_block_validate(&blk))
//    {
//        return 0u;
//    }

    ui_product_store_clear_ram();

    for (i = 0u; i < blk.product_count; i++)
    {
        if (blk.products[i].used)
        {
        	if (ui_product_store_add_with_id(blk.products[i].name, blk.products[i].id) != UI_PROD_OK)
        	{
        	    ui_product_store_clear_ram();
        	    return UI_PERSIST_PROD_ERR_RESTORE_ITEM;
        	}
//            if (ui_product_store_add_with_id(blk.products[i].name, blk.products[i].id) != UI_PROD_OK)
//            {
//                ui_product_store_clear_ram();
//                return 0u;
//            }
        }
    }

    if (blk.product_count > 0u)
    {

    	if (!ui_product_store_set_active_by_id(blk.active_product_id))
    	{
    	    ui_product_store_clear_ram();
    	    return UI_PERSIST_PROD_ERR_RESTORE_ACTIVE;
    	}
//        if (!ui_product_store_set_active_by_id(blk.active_product_id))
//        {
//            ui_product_store_clear_ram();
//            return 0u;
//        }
    }

    //return 1u;
    return UI_PERSIST_PROD_OK;
}

int8_t ui_persist_productos_clear(void)
{
    int pgsz = 0;
    int pages = 0;

    if (!ui_persist_products_flash_init(&pgsz, &pages))
    {
        return UI_PERSIST_PROD_ERR_DLOG_INIT;
    }

    if (dlog_erase() < 0)
    {
        return UI_PERSIST_PROD_ERR_DLOG_ERASE;
    }

//    if (!ui_persist_products_flash_init(&pgsz, &pages))
//    {
//        return 0u;
//    }

//    if (dlog_erase() < 0)
//    {
//        return 0u;
//    }

    //return 1u;
    return UI_PERSIST_PROD_OK;
}

//
//#include "ui_persist_productos.h"
//#include "ui_almacena_productos.h"
//#include <string.h>
//#include <stdint.h>
//#include <ft900_dlog.h>
//#include "ui_persist_productos.h"
//#include "ui_almacena_productos.h"
//
//
//extern __flash__ uint32_t __dlog_partition[];
