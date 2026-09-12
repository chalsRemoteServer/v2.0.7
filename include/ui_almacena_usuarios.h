/*
 * ui_almacena_usuarios.h
 *
 *  Created on: 18 mar. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

#define UI_USER_MAX_COUNT        20
#define UI_USER_NAME_MAX         20
#define UI_USER_LEVEL_MAX        19
#define UI_USER_METHOD_MAX       19
#define UI_USER_PASSWORD_MAX     20

typedef struct
{
    uint8_t used;                                       /* 0 = libre, 1 = ocupado */
    uint8_t id;                                         /* 1..20 */
    char    username[UI_USER_NAME_MAX + 1];
    char    level[UI_USER_LEVEL_MAX + 1];
    char    auth_method[UI_USER_METHOD_MAX + 1];
    char    password[UI_USER_PASSWORD_MAX + 1];
    uint8_t is_default;

} UiUserRecord;

typedef enum
{
    UI_USER_OK = 0,
    UI_USER_ERR_EMPTY_NAME,
    UI_USER_ERR_FULL,
    UI_USER_ERR_DUPLICATE_NAME,
    UI_USER_ERR_INVALID_DATA,
	UI_USER_ERR_NOT_FOUND,
	UI_USER_ERR_BAD_PASSWORD,
    UI_USER_ERR_PROTECTED,      /* no se puede eliminar/modificar por ser default */
	UI_USER_ERR_NO_ACTIVE,

} UiUserResult;

// FUNCIONES PARA LA INICIALIZACIÓN
void ui_user_store_init(void);
void ui_user_store_load_defaults(void);

uint8_t ui_user_store_count(void);
uint8_t ui_user_store_is_empty(void);
uint8_t ui_user_store_next_free_id(void);

UiUserResult ui_user_store_add_autoid(const char* username, const char* level, const char* auth_method, const char* password, uint8_t* out_id);
uint8_t ui_user_store_delete_by_id(uint8_t id);

const UiUserRecord* ui_user_store_get_by_index(uint8_t index);
const UiUserRecord* ui_user_store_get_by_id(uint8_t id);
const UiUserRecord* ui_user_store_get_by_name(const char* username);
int8_t ui_user_store_find_visible_index_by_id(uint8_t id);

void ui_user_store_load_defaults(void);

UiUserResult ui_user_login_by_name(const char* username, const char* password);
UiUserResult ui_user_login_by_id(uint8_t id, const char* password);
void ui_user_logout(void);

uint8_t ui_user_has_active(void);
const UiUserRecord* ui_user_get_active(void);
const char* ui_user_get_active_name(void);

uint8_t ui_user_password_matches(const UiUserRecord* user, const char* password);
uint8_t ui_user_store_is_default_user(const char* username);
