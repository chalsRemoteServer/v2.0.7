/*
 * ui_almacena_productos.c
 *
 *  Created on: 11 mar. 2026
 *      Author: oscar
 */

/*
 * ui_almacena_productos.c
 *
 *  Created on: 11 mar. 2026
 *      Author: oscar
 */

#include "ui_almacena_productos.h"
#include <string.h>
#include <ft900_memctl.h>

static UiProductRecord g_products[UI_PRODUCT_MAX_COUNT];
static uint8_t g_product_count = 0u;
static uint8_t g_active_product_id = 0u;

static uint8_t ui_product_name_is_empty(const char* name)
{
    uint16_t i;

    if (!name) return 1u;

    for (i = 0; name[i] != '\0'; i++)
    {
        if (name[i] != ' ')
        {
            return 0u;
        }
    }

    return 1u;
}

static uint8_t ui_product_store_id_exists(uint8_t id)
{
    uint8_t i;

    if (id == 0u || id > UI_PRODUCT_MAX_COUNT) return 0u;

    for (i = 0; i < UI_PRODUCT_MAX_COUNT; i++)
    {
        if (g_products[i].used && g_products[i].id == id)
        {
            return 1u;
        }
    }

    return 0u;
}

static uint8_t ui_product_store_find_first_used_id(void)
{
    uint8_t i;

    for (i = 0; i < UI_PRODUCT_MAX_COUNT; i++)
    {
        if (g_products[i].used)
        {
            return g_products[i].id;
        }
    }

    return 0u;
}

void ui_product_store_clear_ram(void)
{
    memset(g_products, 0, sizeof(g_products));
    g_product_count = 0u;
    g_active_product_id = 0u;
}

void ui_product_store_init(void)
{
    ui_product_store_clear_ram();
}

uint8_t ui_product_store_count(void)
{
    return g_product_count;
}

uint8_t ui_product_store_is_empty(void)
{
    return (g_product_count == 0u) ? 1u : 0u;
}

uint8_t ui_product_store_next_free_id(void)
{
    uint8_t id;
    uint8_t i;
    uint8_t used;

    for (id = 1; id <= UI_PRODUCT_MAX_COUNT; id++)
    {
        used = 0u;

        for (i = 0; i < UI_PRODUCT_MAX_COUNT; i++)
        {
            if (g_products[i].used && g_products[i].id == id)
            {
                used = 1u;
                break;
            }
        }

        if (!used)
        {
            return id;
        }
    }

    return 0u;   /* no hay IDs libres */
}

UiProductResult ui_product_store_add_autoid(const char* name, uint8_t* out_id)
{
    uint8_t slot;
    uint8_t new_id;

    if (ui_product_name_is_empty(name))
    {
        return UI_PROD_ERR_EMPTY_NAME;
    }

    if (g_product_count >= UI_PRODUCT_MAX_COUNT)
    {
        return UI_PROD_ERR_FULL;
    }

    new_id = ui_product_store_next_free_id();
    if (new_id == 0u)
    {
        return UI_PROD_ERR_FULL;
    }

    for (slot = 0; slot < UI_PRODUCT_MAX_COUNT; slot++)
    {
        if (!g_products[slot].used)
        {
            g_products[slot].used = 1u;
            g_products[slot].id   = new_id;

            strncpy(g_products[slot].name, name, UI_PRODUCT_NAME_MAX);
            g_products[slot].name[UI_PRODUCT_NAME_MAX] = '\0';

            g_product_count++;

            if (g_active_product_id == 0u)
            {
                g_active_product_id = new_id;
            }

            if (out_id) *out_id = new_id;
            return UI_PROD_OK;
        }
    }

    return UI_PROD_ERR_FULL;
}

UiProductResult ui_product_store_add_with_id(const char* name, uint8_t fixed_id)
{
    uint8_t slot;

    if (ui_product_name_is_empty(name))
    {
        return UI_PROD_ERR_EMPTY_NAME;
    }

    if (fixed_id == 0u || fixed_id > UI_PRODUCT_MAX_COUNT)
    {
        return UI_PROD_ERR_FULL;
    }

    if (g_product_count >= UI_PRODUCT_MAX_COUNT)
    {
        return UI_PROD_ERR_FULL;
    }

    if (ui_product_store_id_exists(fixed_id))
    {
        return UI_PROD_ERR_FULL;
    }

    for (slot = 0; slot < UI_PRODUCT_MAX_COUNT; slot++)
    {
        if (!g_products[slot].used)
        {
            g_products[slot].used = 1u;
            g_products[slot].id   = fixed_id;

            strncpy(g_products[slot].name, name, UI_PRODUCT_NAME_MAX);
            g_products[slot].name[UI_PRODUCT_NAME_MAX] = '\0';

            g_product_count++;

            return UI_PROD_OK;
        }
    }

    return UI_PROD_ERR_FULL;
}

const UiProductRecord* ui_product_store_get_by_index(uint8_t index)
{
    uint8_t found = 0u;
    uint8_t i;

    for (i = 0; i < UI_PRODUCT_MAX_COUNT; i++)
    {
        if (g_products[i].used)
        {
            if (found == index)
            {
                return &g_products[i];
            }
            found++;
        }
    }

    return 0;
}

const UiProductRecord* ui_product_store_get_by_id(uint8_t id)
{
    uint8_t i;

    for (i = 0; i < UI_PRODUCT_MAX_COUNT; i++)
    {
        if (g_products[i].used && g_products[i].id == id)
        {
            return &g_products[i];
        }
    }

    return 0;
}

uint8_t ui_product_store_get_active_id(void)
{
    return g_active_product_id;
}

const UiProductRecord* ui_product_store_get_active(void)
{
    return ui_product_store_get_by_id(g_active_product_id);
}

uint8_t ui_product_store_set_active_by_id(uint8_t id)
{
    const UiProductRecord* p = ui_product_store_get_by_id(id);
    if (!p) return 0u;

    g_active_product_id = id;
    return 1u;
}

int8_t ui_product_store_find_visible_index_by_id(uint8_t id)
{
    uint8_t found = 0u;
    uint8_t i;

    for (i = 0; i < UI_PRODUCT_MAX_COUNT; i++)
    {
        if (g_products[i].used)
        {
            if (g_products[i].id == id)
            {
                return (int8_t)found;
            }
            found++;
        }
    }

    return -1;
}

uint8_t ui_product_store_delete_by_id(uint8_t id)
{
    uint8_t i;

    for (i = 0; i < UI_PRODUCT_MAX_COUNT; i++)
    {
        if (g_products[i].used && g_products[i].id == id)
        {
            g_products[i].used = 0u;
            g_products[i].id = 0u;
            g_products[i].name[0] = '\0';

            if (g_product_count > 0u)
            {
                g_product_count--;
            }

            if (g_active_product_id == id)
            {
                g_active_product_id = ui_product_store_find_first_used_id();
            }

            return 1u;
        }
    }

    return 0u;
}
