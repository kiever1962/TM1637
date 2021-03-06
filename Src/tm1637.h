#ifndef TM1637_H
#define TM1637_H
#include "stdint.h"
#define I2C_DELAY  100
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44
#define STARTADDR  0xc0 
/**************definitions for brightness***********************/
#define  BRIGHT_DARKEST 0
#define  BRIGHT_TYPICAL 2
#define  BRIGHTEST      7
/************definitions for type of the 4-Digit Display*********************/
#define D4036B 0
#define D4056A 1
//Special characters index of tube table
#define INDEX_NEGATIVE_SIGN	16
#define INDEX_BLANK			     0 // 17 nothing ,0  zero beter for clock
void TM1637_display(uint8_t Seg_N,int8_t DispData);
void TM1637_display_all(uint16_t Decimal);
void TM1637_displayTime(uint8_t hours,uint8_t minutes);
void TM1637_brightness(uint8_t brightness);
void TM1637_clearDisplay(void);
void tik_delay(uint32_t i);
#endif
