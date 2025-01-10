#ifndef __NTC_H_
#define __NTC_H_

#include "stdint.h"

extern uint16_t temp_table[121];
uint16_t get_temp(uint8_t channel_x);
uint8_t LookupTable(uint16_t *temptab,uint8_t tablenum,uint16_t temp);

#endif