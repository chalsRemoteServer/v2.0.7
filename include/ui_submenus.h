/*
 * ui_submenus.h
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */

#pragma once
#include <stdint.h>

typedef enum {
    UI_SUB_NONE = 0,
    UI_SUB_PRODUCTO,
    UI_SUB_APRENDIZAJE,
    UI_SUB_REGISTRO,
    UI_SUB_USUARIO,
    UI_SUB_CONFIG
} UiSubmenuId;

void ui_submenu_set(UiSubmenuId id);
void ui_submenu_clear(void);
uint8_t ui_submenu_is_active(void);
UiSubmenuId ui_submenu_get(void);

/* Dibuja SOLO el contenido central del sub-menú (un título por ahora) */
void ui_submenu_draw(void);
