#include "STC15W4Kxx.h"
#include "pwm.h"
#include "timer.h"

void PWM5Init(void)
{
    PWMCFG = 0x08;              //配置PWM5的输出初始电平为高电平
    PWMCKS = 0x0b;              //选择PWM的时钟为Fosc/12
    
    PWMC = CYCLE;               //设置PWM周期 PWMCH PWMCL
    
    PWM5T1 = 0;                 //设置PWM5第1次反转的PWM计数
    PWM5T2 = 10;                //设置PWM5第2次反转的PWM计数 占空比为(PWM5T2-PWM5T1)/PWMC
                         
    PWM5CR = 0x00;              //选择PWM5输出到P2.3,不使能PWM5中断          
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