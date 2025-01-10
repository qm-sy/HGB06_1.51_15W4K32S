#include "STC15W4Kxx.h"
#include "stdint.h"
#include "sync.h"
#include "timer.h"
#include "key.h"
#include "pwm.h"
#include "ntc.h"
#include "screen.h"
#include "stdio.h"

void sync_judge()
{
    if(sync_bit == 1)
    {
        
        if(external_24bit == 1)
        {
            power_bit = 1;
            sun_dis(DIS_ON);
            wind_bit = 1;
            delay_bit1 = 1;
            delay_bit2 = 0;
        }
        else
        {
            delay_bit2 = 1;
            power_bit = 0;
            sun_dis(DIS_OFF);
            wind_bit = 0;
            PWMStop(); 
        }
    }
    else
    {
        //delay_bit1 = 0;
        power_bit = 1;
        sun_dis(DIS_ON);
        wind_bit = 1; 
    }
    
    power_crl(power_num);
    wind_crl(wind_num);         //刷新风力和功率
}

void sync_fan_delay_listen()
{
    if((delay_bit1 == 1)&&(delay_bit2 == 1))
    {
        wind_bit = 1;  
        wind_crl(wind_num);  
    }
    
//    if((sync_bit == 1)&&(delay_bit2 == 0))
//    {
//        wind_bit = 1;  
//        wind_crl(wind_num); 
//    }
    
    if(delay_bit3 == 1)
    {  
        wind_bit = 0; 
        printf("here");
        PWMStop();
        delay_bit3 = 0;
    }
}
void led_init()
{
    P24 = P25 = P26 = 0;
    P43 = P44 = 0;
}

void external_24V_listen()
{
    static now_value = 0;
    if(now_value!=previous_value)
    {
        sync_judge();
        now_value = previous_value;
    }
}

void temp_listen()
{   
    static uint8_t temp; 
    temp = (uint8_t)get_temp(6);
    if(temp>=temp_num)
    {
        alarm_dis(DIS_ON);
        P05 = 1;
        P54 = temp_dis_bit;
    }
    
    if(temp<temp_num)    
    {

            alarm_dis(DIS_OFF);
            P05 = 0;
            P54 = 1;
 
    }
}

    
//    if(sync_bit==1)
//        
//    {
//        if(external_24bit==1)
//        {
//            //alarm_dis(DIS_OFF);
//            power_bit = 1; 
//        }
//    }
