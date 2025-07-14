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
    uint8_t temp_val;
    if( temp_listen_bit == 1)
    {
        temp_val = LookupTable(temp_table,121,ntc6_val);
        printf("THE value of ntc6_val is %d \r\n",(int)temp_val);

        if(temp_val>=temp_num)
        {
            if( alarm_dis_flag == 0 )
            {
                tempchannel3 = 1;
            }
        }
        
        if(temp_val<temp_num-1)
        {
            if( alarm_dis_flag == 0 )
            {
                tempchannel3 = 0;
            }
            if( alarm_dis_flag == 1 )
            {
                if( power_bit == 1 )
                {
                    tempchannel3 = 0;
                }else
                {
                    tempchannel3 = 1;
                }
            }  
        }
        temp_listen_bit = 0;
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