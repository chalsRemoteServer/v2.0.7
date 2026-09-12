/*
 * ui_submenus.c
 *
 *  Created on: 18 feb. 2026
 *      Author: oscar
 */


#include "ui_submenus.h"
#include "ui_layout.h"
#include "EVE.h"
#include "colores.h"

static UiSubmenuId s_sub = UI_SUB_NONE;

void ui_submenu_set(UiSubmenuId id)   { s_sub = id; }
void ui_submenu_clear(void)          { s_sub = UI_SUB_NONE; }
uint8_t ui_submenu_is_active(void)   { return (s_sub != UI_SUB_NONE); }
UiSubmenuId ui_submenu_get(void)     { return s_sub; }

static const char* sub_title(UiSubmenuId id)
{
    switch (id)
    {
        case UI_SUB_PRODUCTO:     return "PRODUCTO";
        case UI_SUB_APRENDIZAJE:  return "APRENDIZAJE";
        case UI_SUB_REGISTRO:     return "REGISTRO";
        case UI_SUB_USUARIO:      return "USUARIO";
        case UI_SUB_CONFIG:       return "CONFIGURACION";
        default:                  return "";
    }
}

void ui_submenu_draw(void){
    const UiMainLayout* L = &UI_LAYOUT_MAIN;

    const uint16_t x = (uint16_t)(L->content_x0 + 30);
    const uint16_t y = (uint16_t)(L->content_y0 + 30);

    EVE_color_rgb(BLACK);
    EVE_cmd_text_bold(x, y, 30, EVE_OPT_FLAT, sub_title(s_sub));
}
