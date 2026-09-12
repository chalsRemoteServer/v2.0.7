/*
 * ui_main_buttons.c
 *
 *  Created on: 12 feb. 2026
 *      Author: oscar
 */

#include "ui_main_buttons.h"
#include "ui_ids.h"
#include "assets_mem.h"
#include "icons.h"
#include <stddef.h>
#include "ui_rects.h"
#include "EVE.h"
#include "colores.h"


static inline void draw_icon_centered_rect(uint32_t mem_addr, uint16_t iw, uint16_t ih, uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  const uint16_t rx0 = x;
  const uint16_t ry0 = y;
  const uint16_t rx1 = (uint16_t)(x + w);
  const uint16_t ry1 = (uint16_t)(y + h);

  const uint16_t bw = (uint16_t)(rx1 - rx0);
  const uint16_t bh = (uint16_t)(ry1 - ry0);

  const uint16_t ix = (uint16_t)(rx0 + (bw > iw ? (bw - iw) / 2 : 0));
  const uint16_t iy = (uint16_t)(ry0 + (bh > ih ? (bh - ih) / 2 : 0));

  EVE_cmd_setbitmap(mem_addr, EVE_ARGB1555, iw, ih);
  EVE_vertex2f(ix * PRESICION, iy * PRESICION);
}


static void draw_btn(uint8_t id, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t font, const char *txt, const UiIcon *icon)
{
	EVE_color_mask(1, 1, 1, 1);
	EVE_color_a(255);
    EVE_tag(id);
    ui_rect_register(id, x, y, w, h);
    EVE_cmd_fgcolor(0x9B6200);
    EVE_color_rgb(WHITE);

    EVE_cmd_button(x, y, w, h, font, 256, txt);    // options=256 lo estabas usando (EVE_OPT_FLAT). Mantén el mismo.

    if (icon)
    {
        EVE_begin(EVE_BITMAPS);
        draw_icon_centered_rect(icon->mem, icon->w, icon->h, x, y, w, h);
        EVE_end();
    }
    EVE_tag(0);
}
static void ui_draw_top_bar(const UiMainLayout *L, const UiIcon *ic0, const UiIcon *ic1, const UiIcon *ic2)
{
  uint16_t x2 = L->top_btn_x2;
  uint16_t x1 = (uint16_t)(x2 - (L->top_btn_w + L->top_btn_gap));
  uint16_t x0 = (uint16_t)(x1 - (L->top_btn_w + L->top_btn_gap));
  uint16_t y  = L->top_btn_y;

  draw_btn(UI_ID_TOP_BTN0, x0, y, L->top_btn_w, L->top_btn_h, L->font, "", ic0);
  draw_btn(UI_ID_TOP_BTN1, x1, y, L->top_btn_w, L->top_btn_h, L->font, "", ic1);
  draw_btn(UI_ID_TOP_BTN2, x2, y, L->top_btn_w, L->top_btn_h, L->font, "", ic2);
}

static void ui_draw_sidebar_buttons(const UiMainLayout *L, const UiIcon *ic0, const UiIcon *ic1, const UiIcon *ic2)
{
  uint16_t sx  = L->side_btn_x;
  uint16_t sy0 = L->side_btn_y0;
  uint16_t sy1 = (uint16_t)(sy0 + L->side_btn_h + L->side_btn_gap);
  uint16_t sy2 = (uint16_t)(sy1 + L->side_btn_h + L->side_btn_gap);

  draw_btn(UI_ID_SIDE_BTN0, sx, sy0, L->side_btn_w, L->side_btn_h, L->font, "REGISTRO"   ,    ic0);
  draw_btn(UI_ID_SIDE_BTN1, sx, sy1, L->side_btn_w, L->side_btn_h, L->font, "PRODUCTO"   ,    ic1);
  draw_btn(UI_ID_SIDE_BTN2, sx, sy2, L->side_btn_w, L->side_btn_h, L->font, "APRENDIZAJE",    ic2);
}

static void ui_draw_side_nav(const UiMainLayout *L, const UiIcon *left, const UiIcon *right)
{

  draw_btn(UI_ID_ARROW_LEFT,  L->arw_x_left,  L->arw_y, L->arw_w, L->arw_h, 25, "<", left);
  draw_btn(UI_ID_ARROW_RIGHT, L->arw_x_right, L->arw_y, L->arw_w, L->arw_h, 25, ">", right);
}

void ui_main_buttons_draw_groups(const UiMainLayout *L, uint8_t groups){
  if (!L) return;
  /* Íconos mapeados a MEM_* (RAM_G) */
  static const UiIcon IC_HOME  = { MEM_ICON_HOME,          64, 64 };
  static const UiIcon IC_USER  = { MEM_ICON_USUARIO,       64, 64 };
  static const UiIcon IC_CONF  = { MEM_ICON_CONFIGURACION, 64, 64 };
  //static const UiIcon IC_FIZQ  = { MEM_ICON_FIZQUIERDA,    64, 64 };
  //static const UiIcon IC_FDER  = { MEM_ICON_FDERECHA,      64, 64 };
  /* Ejemplo: usa los 3 mismos arriba (ajusta a tu UI real) */
  if (groups & UI_GRP_TOPBAR)
    ui_draw_top_bar(L, &IC_HOME, &IC_USER, &IC_CONF);

  if (groups & UI_GRP_SIDEBAR)
    ui_draw_sidebar_buttons(L, NULL, NULL, NULL); // si por ahora no hay íconos, pon NULLs

  if (groups & UI_GRP_ARROWS)
    //ui_draw_side_nav(L, &IC_FIZQ, &IC_FDER); // o con íconos de flechas si los tienes
    ui_draw_side_nav(L, NULL, NULL); // o con íconos de flechas si los tienes

}

void ui_main_buttons_draw(const UiMainLayout *L)
{
	 ui_main_buttons_draw_groups(L, UI_GRP_TOPBAR | UI_GRP_SIDEBAR | UI_GRP_ARROWS);
}

