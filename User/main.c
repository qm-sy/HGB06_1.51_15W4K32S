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
    IT0 = 1;   //�½��ش����������ź�
    EX0 = 1;
    EA = 1;
    P42 = 0;
    P23 = 0;
    P54 = 1;
    PWM5Init(); 
    P05 = 0;
    PWMStop();
      
    printf("======== code start ========\r\n");  

    x = ISP_read(0x0000);
    if((x!=1)&&(x!=2)&&(x!=3)&&(x!=4)&&(x!=5))
    {
        mode_num = 1;
        channel_num = 1;
        sync_bit = 1;
        wind_num = 3;
        power_num = 50;
        temp_num = 100;//��ȡchannel_num��sync_bit��wind_num��power_num��temp_num
        eeprom_mode_save();
        for(y = 1;y<6;y++) 
        
        {
            mode_num = y;
            eeprom_data_write();
         }
    }
    
    restart:
    delay_ms(1000);
    
    while(on_off)
    {
        on_off = power_on();
    }  
    P20 = 0; 

    led_init();    
    lcd_init();  
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