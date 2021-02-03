/*
 * lcd1602.h
 *
 *  Created on: 2 ene. 2019
 *      Author: pdaccs
 */
#include "TM4C123GH6PM.h"

#ifndef INC_LCD1602_H_
#define INC_LCD1602_H_

/* LCD Display interface on TivaC TM4C123GXL */
//        V0 PD0
//        RS PD2
//        RW PD1
//        E  PD3
//
//        D4 PB5
//        D5 PB0
//        D6 PB1
//        D7 PE4
//        A  PB5
//        K  PB4


void lcd1602_init(void); // Execute initialization sequence
void lcd1602_senddata(uint32_t RS, uint32_t RW, uint32_t lcd_data); // lcd data is encoded in 8 bits, sent over 4 bits interface
void lcd1602_backlight(uint32_t backlight_on); // Manages backlight
void lcd1602_print(uint32_t line, char string[16]);
void lcd1602_clear(void);

#endif /* INC_LCD1602_H_ */
