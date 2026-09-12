#include <stdint.h>
#include "platform_time.h"

#include <ft900_interrupt.h>
#include <ft900_timers.h>

static volatile uint32_t s_ms = 0;

static void isr_timers(void){
    /* Como el vector es "interrupt_timers" (agregado),
       detectamos qué timer lo disparó.
       IMPORTANTE: timer_is_interrupted() limpia el bit de interrupción. */

    if (timer_is_interrupted(timer_select_a) == 1){
        s_ms++;}  // 1 tick = 1 ms (si el timer está calibrado a 1ms)

    /* Si en el futuro usas B/C/D, aquí los atenderías también:
    if (timer_is_interrupted(timer_select_b) == 1) { ... }
    if (timer_is_interrupted(timer_select_c) == 1) { ... }
    if (timer_is_interrupted(timer_select_d) == 1) { ... }
    */
}//-----------------------------------------------------------

void platform_time_init(void){
    s_ms = 0;
    /* 1) Inicializa Timer A
       NOTA: necesitamos que el timer genere un "evento" cada 1 ms.
       Como tu API no tiene set_frequency(), se ajusta con prescaler + initial.
    */

    /* Prescaler: lo habilitamos y ponemos un valor razonable inicial.
       Si el prescaler no se usa en tu chip/SDK, puedes dejar prescaler OFF,
       pero aquí lo dejo ON porque tu API lo contempla. */
    (void)timer_prescaler_ex(timer_select_a, 100); // <-- ajuste inicial (ver calibración abajo)
    (void)timer_init(timer_select_a,
                     1000,                    // <-- ajuste inicial (ver calibración abajo)
                     timer_direction_down,
                     timer_prescaler_select_on,
                     timer_mode_continuous);

    /* 2) OBLIGATORIO: attach ANTES de enable del módulo */
    (void)interrupt_attach(interrupt_timers, 0, isr_timers); // prioridad 0 (alta). Ajusta si lo requieres.
    /* 3) Ahora sí habilita la interrupción del timer A */
    (void)timer_enable_interrupt(timer_select_a);
    /* 4) Habilita interrupciones globales */
    interrupt_enable_globally();
    /* 5) Arranca el timer */
    (void)timer_start(timer_select_a);
}//-------------------------------------------------

uint32_t platform_millis(void){
    return s_ms;
}//-----------------------------------------

uint32_t millis(void){
    return s_ms;
}//-----------------------------------------

