#ifndef __SCREEN_H_
#define __SCREEN_H_

#include "stdint.h"

#define COM1_L 0X01
#define COM2_L 0X02
#define COM3_L 0X04
#define COM4_L 0X08
#define COM1_H 0X10
#define COM2_H 0X20
#define COM3_H 0X40
#define COM4_H 0X80

#define ADDR_03 0
#define ADDR_06 1
#define ADDR_07 2
#define ADDR_0A 3
#define ADDR_0B 4
#define ADDR_0E 5
#define ADDR_0F 6

#define DIS_ON  1
#define DIS_OFF 0

extern uint8_t value_03;  //该地址的值
extern uint8_t value_06;
extern uint8_t value_07;
extern uint8_t value_0A;
extern uint8_t value_0B;
extern uint8_t value_0E;
extern uint8_t value_0F;

extern uint8_t addr_tab[7];
extern uint8_t num_tab[10];

sbit TM1722_DIO   = P3^7;
sbit TM1722_CLK   = P4^1;
sbit TM1722_STB   = P3^6;

void lcd_init();
void lcd_clear();
void lcd_display();
void lcd_write_val(uint8_t addr,uint8_t dat);
void TM1722_Write_Byte(unsigned char dat);

void wind_dis(uint8_t num);
void num_dis(uint8_t num);
void channel_dis(uint8_t num);
void sun_dis(bit on_off);
void sync_dis(bit on_off);
void alarm_dis(bit on_off);
void F_dis(bit on_off);
void M_dis(bit on_off);
void R_dis(bit on_off);
void Celsius_dis(bit on_off);
void mode_dis(bit on_off);
void percentage_dis(bit on_off);
void cur_dis(bit on_off);
void set_dis(bit on_off);
void fan_center_dis(bit on_off);
void fan_leaf1_dis(bit on_off);
void fan_leaf2_dis(bit on_off);

#endif 