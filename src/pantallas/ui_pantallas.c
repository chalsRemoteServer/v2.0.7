/*
 * ui_pantallas.c
 *
 *  Created on: 16 feb. 2026
 *      Author: oscar
 */

#include "ui_pantallas.h"
#include "ui_submenus.h"

extern const UiScreen g_screen_main;
extern const UiScreen g_screen_2;
extern const UiScreen g_screen_3;
extern const UiScreen g_screen_4;
extern const UiScreen g_screen_5;

static const UiScreen* s_screens[] = {
    &g_screen_main,
    &g_screen_2,
	&g_screen_3,
	&g_screen_4,
	&g_screen_5
};

#define SCREENS_COUNT ((uint8_t)(sizeof(s_screens)/sizeof(s_screens[0])))
static uint8_t s_idx = 0;

uint8_t ui_screens_count(void) { return SCREENS_COUNT; }

void ui_screens_init(void){
    s_idx = 0;
    if (s_screens[s_idx] && s_screens[s_idx]->on_enter) s_screens[s_idx]->on_enter();
}

void ui_screens_set(uint8_t idx){
    if (idx >= SCREENS_COUNT) return;
    if (idx == s_idx) return;

    if (s_screens[s_idx] && s_screens[s_idx]->on_exit) s_screens[s_idx]->on_exit();
    s_idx = idx;
    if (s_screens[s_idx] && s_screens[s_idx]->on_enter) s_screens[s_idx]->on_enter();
}

uint8_t ui_screens_get(void){ return s_idx; }

void ui_screens_next(void){ ui_screens_set((uint8_t)((s_idx + 1u) % SCREENS_COUNT)); }
void ui_screens_prev(void){ ui_screens_set((uint8_t)((s_idx + SCREENS_COUNT - 1u) % SCREENS_COUNT)); }

void ui_screens_update(uint32_t now_ms){
    const UiScreen* sc = s_screens[s_idx];
    if (sc && sc->update) sc->update(now_ms);
}

void ui_screens_draw(void){
    if (ui_submenu_is_active()){
        ui_submenu_draw();   // overlay de submenú (no afecta pagers)
        return;}
    const UiScreen* sc = s_screens[s_idx];
    if (sc && sc->draw) sc->draw();
}//fin  dibujar pantallas-----------------------

