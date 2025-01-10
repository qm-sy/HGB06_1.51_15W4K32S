#ifndef __KEY_H_
#define __KEY_H_

#include "stdint.h"

#define   ChannelChoose     2
#define   ModeChoose        4
#define   FunctionKey       3
#define   PowerKey          0
#define   UpKey             1
#define   DownKey           5
#define   FanUp             9
#define   FanDown           13

sbit buzzer = P5^4;

extern bit sync_delay_bit;
extern bit temp_delay_bit; 
extern bit DIS_Variable;
extern bit on_off;


extern uint8_t wind_num;
extern uint8_t power_num;
extern uint8_t mode_num;
extern uint8_t temp_num;
extern bit sync_bit;

extern uint8_t key_val;

extern uint8_t mode_info[5];

void button_scan();
void fan_up();
void fan_down();
void up_key();
void down_key();
void channel_choose();
void temp_set();
void mode_choose();
void sync_choose();
void power_key();
void fan_rotate();
uint8_t power_on();
void power_off();

#endif 