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
        if(master_pwm_bit == 1)
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
    wind_crl(wind_num);         //ˢ�·����͹���
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
        printf("here");
        wind_bit = 0;
        PWMStop();
        delay_bit3 = 0;
    }
}
void led_init()
{
    P24 = P25 = P26 = 0;
    P43 = P44 = 0;
}

void master_pwm_adc_listen()
{
    static now_value = 0;
    if( now_value != previous_value )
    {
        sync_judge();
        now_value = previous_value;
    }
}

void temp_listen()
{   
    static uint8_t i = 0;
    static uint16_t leijia = 0;
    static uint16_t ntc6_val = 0;
    static uint8_t ntc6_temp = 0;
    leijia = leijia + ntc6_adc_val;
    i++;
    if(i == 10)
    {
        ntc6_val = leijia/i;
        i = 0;
        leijia = 0;
    }

        ntc6_temp = LookupTable(temp_table,121,ntc6_val);

        printf("THE value of ntc6_val is %d \r\n",(int)ntc6_temp);


    if(ntc6_temp>=temp_num)
    {
        alarm_dis(DIS_ON);
        P05 = 1;
        P54 = temp_dis_bit;
    }
    
    if(ntc6_temp<temp_num)    
    {
        alarm_dis(DIS_OFF);
        P05 = 0;
        P54 = 1;

    }
}
    
//    if(sync_bit==1)
//        
//    {
//        if(master_pwm_bit==1)
//        {
//            //alarm_dis(DIS_OFF);
//            power_bit = 1; 
//        }
//    }
//}