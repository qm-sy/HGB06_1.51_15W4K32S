#include "STC15W4Kxx.h"
#include "eeprom.h"
#include "stdint.h"
#include "delay.h"
#include "key.h"
#include "timer.h"
#include "stdio.h"
#include "screen.h"
#include "sync.h"
#include "pwm.h"

void ISPIdle()
{
    IAP_CONTR = 0X00;
    IAP_CMD = 0X00;
    IAP_TRIG = 0X00;
    IAP_ADDRH = 0X80;
    IAP_ADDRL = 0X00; 
}

uint8_t ISP_read(uint16_t addr)
{
    uint8_t dat;
    
    IAP_CONTR = 0X83;
    IAP_CMD = 0X01;
    IAP_ADDRL = addr;
    IAP_ADDRH = addr>>8;
    
    IAP_TRIG = 0X5A;
    IAP_TRIG = 0XA5;
    
    delay_ms(10);
    
    dat = IAP_DATA;
    
    ISPIdle();
    
    return dat;
}

void ISP_write(uint16_t addr,uint8_t dat)
{
    IAP_CONTR = 0X83;
    IAP_CMD = 0X02;
    IAP_ADDRL = addr;
    IAP_ADDRH = addr>>8;
    IAP_DATA = dat;
    
    IAP_TRIG = 0X5A;
    IAP_TRIG = 0XA5;
    
    delay_ms(10);
    
    ISPIdle();
}

void ISP_earse(uint16_t addr)
{   
    IAP_CONTR = 0X83;   //  WT2:WT1:WT0 = 0 1 1  CPU等待时间>=12MHz
    IAP_CMD = 0X03;
    IAP_ADDRL = addr;
    IAP_ADDRH = addr>>8;
    
    IAP_TRIG = 0X5A;
    IAP_TRIG = 0XA5;
    
    delay_ms(10);
    
    ISPIdle();
}

//void eeprom_date_write()
//{
//    if(eeprom_bit == 1)
//    { 
//        ISP_earse(0x0000);
//        
//        ISP_write(0x0000,init_temp1);       //温度1

//        
//        eeprom_bit = 0;
//    }
//}


void eeprom_mode_save()
{
    uint16_t addr = 0x0000;
    
    ISP_earse(addr);
    ISP_write(addr,mode_num);  
    
    buzzer=buzzer_bit=0;
}

void eeprom_data_write()
{
    uint8_t i;
    uint16_t addr;
    
    mode_info[0] = channel_num;         //通道
    mode_info[1] = sync_bit;           //同步标志位
    mode_info[2] = wind_num;            //风力
    mode_info[3] = power_num;           //功率
    mode_info[4] = temp_num;            //温度
    mode_info[5] = fengshan_delay;

    addr = (mode_num * 2)<<8;
    
    ISP_earse(addr);
    
    for(i=0;i<6;i++)
    {
        ISP_write(addr,mode_info[i]);
        addr++;
    }
    
    buzzer=buzzer_bit=0;
}

void ISP_data_init()
{  
    mode_num = ISP_read(0x0000);
    eeprom_data_read(mode_num);
    
    sun_dis(power_bit);
    percentage_dis(DIS_ON);   
    sync_dis(sync_bit);
    num_dis(power_num);
    channel_dis(channel_num);
    wind_dis(wind_num);          //刷新界面：加热符号、通道、功率、百分比、风力、同步标志
    
    sync_judge();               //判断同步状态
}

void eeprom_data_read(uint8_t mode)     
{
    uint8_t i;
    uint16_t addr;
    
    addr = (mode * 2)<<8;
    for(i=0;i<6;i++)
    {
        mode_info[i] = ISP_read(addr);
        addr++;
    }
    
    channel_num = mode_info[0];
    sync_bit = mode_info[1];
    wind_num = mode_info[2];
    power_num = mode_info[3];
    temp_num = mode_info[4];//读取channel_num、sync_bit、wind_num、power_num、temp_num
    fengshan_delay = mode_info[5];
}
    
