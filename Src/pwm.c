#include "STC15W4Kxx.h"
#include "pwm.h"
#include "timer.h"

void PWM5Init(void)
{
    PWMCFG = 0x08;              //����PWM5�������ʼ��ƽΪ�ߵ�ƽ
    PWMCKS = 0x0b;              //ѡ��PWM��ʱ��ΪFosc/12
    
    PWMC = CYCLE;               //����PWM���� PWMCH PWMCL
    
    PWM5T1 = 0;                 //����PWM5��1�η�ת��PWM����
    PWM5T2 = 10;                //����PWM5��2�η�ת��PWM���� ռ�ձ�Ϊ(PWM5T2-PWM5T1)/PWMC
                         
    PWM5CR = 0x00;              //ѡ��PWM5�����P2.3,��ʹ��PWM5�ж�          
}

void PWMSet(unsigned int dat)
{
    PWMStart();
    PWM5T2 = dat;
}

void PWMStart(void)
{
    PWMCR = 0X88;
}

void PWMStop(void)
{
    
    PWMCR = 0X00;
    P23 = 1;
}

void wind_crl(unsigned char wind_num)
{
    if(wind_bit == 1)       
    {
        switch(wind_num)
        {
            case 0:
                PWMStop();
                break;
            
            case 1:
                PWMSet(2000);
                break;
            
            case 2:
                PWMSet(2400);
                break;
            
            case 3:
                PWMSet(2800);
                break;
            
            case 4:
                PWMSet(3200);
                break;
            
            case 5:
                PWMSet(3600);
                break;
            
            case 6:
                PWMSet(4000);
                break;
        }
        
    }
    
}