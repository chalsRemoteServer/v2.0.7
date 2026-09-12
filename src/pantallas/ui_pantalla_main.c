#include "ui_pantallas.h"
#include "ui_main_buttons.h"
#include "ui_footer.h"
#include "ui_bargraph.h"
#include "ui_pagers.h"
#include "ui_layout.h"

extern UiBarGraph g_bar;

static void main_on_enter(void){

}

static void main_update(uint32_t now_ms){
    (void)now_ms;
}


static void main_draw(void){
    ui_main_buttons_draw_groups(&UI_LAYOUT_MAIN, UI_GRP_SIDEBAR); // PRODUCTO/APRENDIZAJE/REGISTRO
    ui_bargraph_draw(&g_bar, &UI_LAYOUT_MAIN);                    // gráfica visible solo en main
    ui_barGraph_status(&g_bar,BARRA_ON);
}//--------------main draw-------------------------------





const UiScreen g_screen_main = {
    .on_enter = main_on_enter,
    .on_exit  = 0,
    .update   = main_update,
    .draw     = main_draw
};
