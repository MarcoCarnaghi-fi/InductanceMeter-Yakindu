/*
 * delay.h
 *
 *  Created on: 31 dic. 2018
 *      Author: pdaccs
 */
#include "TM4C123GH6PM.h"

#ifndef DELAY_H_
#define DELAY_H_

void TimeTick_Decrement(void);
void delay_n10us(uint32_t n);
void delay_1ms(void);
void delay_nms(uint32_t n);
uint32_t count_sec(uint32_t enable);
uint32_t count_msec(uint32_t enable);
uint32_t softpwm_8bit(uint8_t threshold);


#endif /* DELAY_H_ */
