/*
 * bsp.c
 *
 *  Created on: 11 feb. 2019
 *      Author: Marco
 */

#include "inc/bsp.h"
#include "inc/tm4c123gh6pm.h"

__attribute__((naked)) void assert_failed (char const *file, int line) {
    /* TBD: damage control */
    NVIC_SystemReset(); /* reset the system */
}

uint32_t sw_debounce(uint32_t sw) {
    static uint32_t sw_state;
    static uint32_t sw_lowcount;
    static uint32_t sw_highcount;

    if (sw != 0) { //  Test 1
        sw_highcount++;
        sw_lowcount=0;
        if (sw_highcount >= DEBOUNCEDELAY) {
            sw_highcount = DEBOUNCEDELAY;
            sw_state = 1;
        }
    } else {  //  Test 0
        sw_highcount=0;
        sw_lowcount++;
        if (sw_lowcount >= DEBOUNCEDELAY) {
            sw_lowcount = DEBOUNCEDELAY;
            sw_state = 0;
        }
    }
    return sw_state;
}

void SysTick_Init(uint32_t ticks) {

    SysTick->LOAD = ticks; // Systick reload value (this is the value that is load when counter reaches 0)
    SysTick->VAL  = 0U; // Clears STCURRENT register by writing with any value
    SysTick->CTRL = (1U << 2) | (1U << 1) | 1U; // Configures: CLK_SRC=system clock - INTEN=enables interruption - ENABLE (enables counter)

}
