/*
 * ui_almacena_usuarios.c
 *
 *  Created on: 18 mar. 2026
 *      Author: oscar
 */

#include "ui_almacena_usuarios.h"
#include <string.h>

static UiUserRecord g_users[UI_USER_MAX_COUNT];
static uint8_t g_user_count = 0u;

/* 0 = sin usuario activo */
static uint8_t g_active_user_id = 0u;

/* =========================
   Helpers internos
   ========================= */

static uint8_t ui_user_text_is_empty(const char* s)
{
    uint16_t i;

    if (!s) return 1u;

    for (i = 0; s[i] != '\0'; i++)
    {
        if (s[i] != ' ')
        {
            return 0u;
        }
    }

    return 1u;
}

static uint8_t ui_user_password_len_ok(const char* s)
{
    size_t n;

    if (!s) return 0u;

    n = strlen(s);
    return (n >= 5u && n <= UI_USER_PASSWORD_MAX) ? 1u : 0u;
}

static uint8_t ui_user_name_exists(const char* username)
{
    uint8_t i;

    if (!username) return 0u;

    for (i = 0; i < UI_USER_MAX_COUNT; i++)
    {
        if (g_users[i].used)
        {
            if (strcmp(g_users[i].username, username) == 0)
            {
                return 1u;
            }
        }
    }

    return 0u;
}

static UiUserResult ui_user_store_add_internal(const char* username,
                                               const char* level,
                                               const char* auth_method,
                                               const char* password,
                                               uint8_t is_default,
                                               uint8_t* out_id)
{
    uint8_t slot;
    uint8_t new_id;

    if (ui_user_text_is_empty(username))
    {
        return UI_USER_ERR_EMPTY_NAME;
    }

    if (ui_user_text_is_empty(level) ||
        ui_user_text_is_empty(auth_method) ||
        !ui_user_password_len_ok(password))
    {
        return UI_USER_ERR_INVALID_DATA;
    }

    if (ui_user_name_exists(username))
    {
        return UI_USER_ERR_DUPLICATE_NAME;
    }

    if (g_user_count >= UI_USER_MAX_COUNT)
    {
        return UI_USER_ERR_FULL;
    }

    new_id = ui_user_store_next_free_id();
    if (new_id == 0u)
    {
        return UI_USER_ERR_FULL;
    }

    for (slot = 0; slot < UI_USER_MAX_COUNT; slot++)
    {
        if (!g_users[slot].used)
        {
            g_users[slot].used = 1u;
            g_users[slot].id   = new_id;
            g_users[slot].is_default = is_default ? 1u : 0u;

            strncpy(g_users[slot].username, username, UI_USER_NAME_MAX);
            g_users[slot].username[UI_USER_NAME_MAX] = '\0';

            strncpy(g_users[slot].level, level, UI_USER_LEVEL_MAX);
            g_users[slot].level[UI_USER_LEVEL_MAX] = '\0';

            strncpy(g_users[slot].auth_method, auth_method, UI_USER_METHOD_MAX);
            g_users[slot].auth_method[UI_USER_METHOD_MAX] = '\0';

            strncpy(g_users[slot].password, password, UI_USER_PASSWORD_MAX);
            g_users[slot].password[UI_USER_PASSWORD_MAX] = '\0';

            g_user_count++;

            if (out_id) *out_id = new_id;
            return UI_USER_OK;
        }
    }

    return UI_USER_ERR_FULL;
}

/* =========================
   API pública
   ========================= */

void ui_user_store_init(void)
{
    memset(g_users, 0, sizeof(g_users));
    g_user_count = 0u;
    g_active_user_id = 0u;

    ui_user_store_load_defaults();
}

void ui_user_store_load_defaults(void)
{
    uint8_t dummy_id;

    /* Usuarios base del sistema */
    (void)ui_user_store_add_internal("Servicio",
                                     "Servicio",
                                     "Password",
                                     "servicio123",
                                     1u,
                                     &dummy_id);

    (void)ui_user_store_add_internal("Administrador",
                                     "Administrador",
                                     "Password",
                                     "admin123",
                                     1u,
                                     &dummy_id);

    (void)ui_user_store_add_internal("Operador",
                                     "Operador",
                                     "Password",
                                     "operador123",
                                     1u,
                                     &dummy_id);
}

uint8_t ui_user_store_count(void)
{
    return g_user_count;
}

uint8_t ui_user_store_is_empty(void)
{
    return (g_user_count == 0u) ? 1u : 0u;
}

uint8_t ui_user_store_next_free_id(void)
{
    uint8_t id;
    uint8_t i;
    uint8_t used;

    for (id = 1; id <= UI_USER_MAX_COUNT; id++)
    {
        used = 0u;

        for (i = 0; i < UI_USER_MAX_COUNT; i++)
        {
            if (g_users[i].used && g_users[i].id == id)
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

    return 0u;
}

UiUserResult ui_user_store_add_autoid(const char* username, const char* level, const char* auth_method, const char* password, uint8_t* out_id)
{
    return ui_user_store_add_internal(username, level, auth_method, password, 0u, out_id);
}

const UiUserRecord* ui_user_store_get_by_index(uint8_t index)
{
    uint8_t found = 0u;
    uint8_t i;

    for (i = 0; i < UI_USER_MAX_COUNT; i++)
    {
        if (g_users[i].used)
        {
            if (found == index)
            {
                return &g_users[i];
            }
            found++;
        }
    }

    return 0;
}

const UiUserRecord* ui_user_store_get_by_id(uint8_t id)
{
    uint8_t i;

    for (i = 0; i < UI_USER_MAX_COUNT; i++)
    {
        if (g_users[i].used && g_users[i].id == id)
        {
            return &g_users[i];
        }
    }

    return 0;
}

const UiUserRecord* ui_user_store_get_by_name(const char* username)
{
    uint8_t i;

    if (!username) return 0;

    for (i = 0; i < UI_USER_MAX_COUNT; i++)
    {
        if (g_users[i].used && strcmp(g_users[i].username, username) == 0)
        {
            return &g_users[i];
        }
    }

    return 0;
}

int8_t ui_user_store_find_visible_index_by_id(uint8_t id)
{
    uint8_t found = 0u;
    uint8_t i;

    for (i = 0; i < UI_USER_MAX_COUNT; i++)
    {
        if (g_users[i].used)
        {
            if (g_users[i].id == id)
            {
                return (int8_t)found;
            }
            found++;
        }
    }

    return -1;
}

uint8_t ui_user_store_delete_by_id(uint8_t id)
{
    uint8_t i;

    for (i = 0; i < UI_USER_MAX_COUNT; i++)
    {
        if (g_users[i].used && g_users[i].id == id)
        {
            if (g_users[i].is_default)
            {
                return 0u; /* protegido */
            }

            if (g_active_user_id == id)
            {
                g_active_user_id = 0u;
            }

            g_users[i].used = 0u;
            g_users[i].id = 0u;
            g_users[i].is_default = 0u;
            g_users[i].username[0] = '\0';
            g_users[i].level[0] = '\0';
            g_users[i].auth_method[0] = '\0';
            g_users[i].password[0] = '\0';

            if (g_user_count > 0u)
            {
                g_user_count--;
            }

            return 1u;
        }
    }

    return 0u;
}

/* =========================
   Login / sesión
   ========================= */

uint8_t ui_user_password_matches(const UiUserRecord* user, const char* password)
{
    if (!user || !password) return 0u;
    return (strcmp(user->password, password) == 0) ? 1u : 0u;
}

UiUserResult ui_user_login_by_id(uint8_t id, const char* password)
{
    const UiUserRecord* user;

    user = ui_user_store_get_by_id(id);
    if (!user)
    {
        return UI_USER_ERR_NOT_FOUND;
    }

    if (!ui_user_password_matches(user, password))
    {
        return UI_USER_ERR_BAD_PASSWORD;
    }

    g_active_user_id = id;
    return UI_USER_OK;
}

UiUserResult ui_user_login_by_name(const char* username, const char* password)
{
    const UiUserRecord* user;

    user = ui_user_store_get_by_name(username);
    if (!user)
    {
        return UI_USER_ERR_NOT_FOUND;
    }

    if (!ui_user_password_matches(user, password))
    {
        return UI_USER_ERR_BAD_PASSWORD;
    }

    g_active_user_id = user->id;
    return UI_USER_OK;
}

void ui_user_logout(void)
{
    g_active_user_id = 0u;
}

uint8_t ui_user_has_active(void)
{
    return (g_active_user_id != 0u) ? 1u : 0u;
}

const UiUserRecord* ui_user_get_active(void)
{
    if (g_active_user_id == 0u)
    {
        return 0;
    }

    return ui_user_store_get_by_id(g_active_user_id);
}

const char* ui_user_get_active_name(void)
{
    const UiUserRecord* user = ui_user_get_active();

    if (!user)
    {
        return "Sin usuario";
    }

    return user->username;
}

uint8_t ui_user_store_is_default_user(const char* username)
{
    const UiUserRecord* user = ui_user_store_get_by_name(username);

    if (!user) return 0u;
    return user->is_default ? 1u : 0u;
}
