#ifndef __TIMER_H_
#define __TIMER_H_

#include "stdint.h"

extern bit fan_dis_bit;
extern bit temp_dis_bit;
extern bit zero_bit;
extern bit buzzer_bit;
extern bit scan_stop_bit;
extern bit scan_start_bit;
extern bit delay_bit1;
extern bit delay_bit2;
extern bit delay_bit3;

sbit tempchannel1 = P1^7;
sbit tempchannel2 = P2^1;
sbit tempchannel3 = P2^2;

extern bit external_24bit;
extern bit power_bit;
extern bit wind_bit;
extern uint8_t channel_num;
extern bit previous_value;

void Tim0Init();
void Tim1Init();
void Tim3Init();
void power_crl(uint8_t power_num);

#endif