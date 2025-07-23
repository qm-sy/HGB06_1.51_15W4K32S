#include "STC15W4Kxx.h"
#include <stdio.h> 
#include "gpio.h"
#include "delay.h"
#include "adc.h"
#include "uart.h"
#include "ntc.h"
#include "timer.h"
#include "screen.h"
#include "stdint.h"
#include "pwm.h"
#include "key.h"
#include "eeprom.h"
#include "sync.h"

void main()
{
    uint8_t x;
    uint8_t y;
    P_SW2 |= 0x80;
    
    GPIOInit();
    ADCInit();
    Uart1Init();
    Tim0Init();
    Tim1Init();
    Tim3Init();
    IT0 = 1;   //下降沿触发过零检测信号
    EX0 = 1;
    EA = 1;
    P42 = 0;
    P23 = 0;
    P54 = 1;
    PWM5Init(); 
    P05 = 0;
    PWMStop();
      
    printf("======== code start ========\r\n");  
    lcd_init();  
    x = ISP_read(0x0000);
    power_statu = ISP_read(0x0002);
    if((x!=1)&&(x!=2)&&(x!=3)&&(x!=4)&&(x!=5))
    {
        mode_num = 1;
        power_statu = 0;
        channel_num = 1;
        sync_bit = 1;
        wind_num = 3;
        power_num = 50;
        temp_num = 100;//读取channel_num、sync_bit、wind_num、power_num、temp_num
        eeprom_mode_save();
        for(y = 1;y<6;y++) 
        
        {
            mode_num = y;
            eeprom_data_write();
         }
    }else
    {
        mode_num = x;
    }
    
    restart:
    delay_ms(1000);

    if( power_statu == 1 )
    {
        on_off = 0;
    }else
    {
        on_off = 1;
    }

    while(on_off)
    {
        on_off = power_on(); 
    }  
    power_statu = 1;
    eeprom_mode_save();
    P20 = 0; 
    delay_ms(10);
    led_init();    

    ISP_data_init();
    percentage_dis(DIS_ON);
    if(power_bit==1)
    {
        sun_dis(DIS_ON);
    }
    buzzer=buzzer_bit=0;
    
    while(1)
    {
        button_scan();
        fan_rotate();
        external_24V_listen();
        temp_listen();
        sync_fan_delay_listen();
        power_off();
        if(on_off == 1)
        {
            buzzer=buzzer_bit=0;
            P24 = P25 = P26 = 1;
            P43 = P44 = 1;
            P20 = 1;
            goto restart;   
        }
        
    }
}