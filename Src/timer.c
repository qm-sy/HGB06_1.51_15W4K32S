#include "STC15W4Kxx.h"
#include "timer.h"
#include "stdint.h"
#include "key.h"

bit fan_dis_bit = 0;
bit temp_dis_bit = 0;
bit zero_bit = 0;
bit buzzer_bit = 1;
bit scan_stop_bit = 1;
bit scan_start_bit = 0;  
bit external_24bit = 0;
bit power_bit = 1;
bit wind_bit = 1;
bit previous_value = 0;
bit delay_bit1 = 0;
bit delay_bit2 = 0;
bit delay_bit3 = 0;
uint8_t channel_num = 1;
uint16_t tim1_t = 59880;
void Tim0Init()         //11.0592Mhz  1ms
{
    AUXR |= 0X80;       //修改计数速率，定时器时钟设置为12T模式
	
	TMOD &= 0XF0;       //清空TIM0需要配置的低四位（低四位是TIM0,高四位是TIM1）

	TH0 = 0XCD;         // TH0 高四位  TL0 低四位
	TL0 = 0XD4;         //通过计算得出的值，计算公式在下面
	
	TF0 = 0;            //清除TC0N-TF0  中断溢出标志，溢出后自动置1，CPU响应中断后置硬件清0
	TR0 = 1;            //TCON-TR0  TIM0开始计数启动

	ET0 = 1;            //打开IE-ET0，TIM0中断
}


void Tim1Init()         //11.0592Mhz  10ms  16位手动装载
{   
	AUXR &= 0xBF;       //定时器时钟12T模式
	TMOD &= 0x0F;       //设置定时器模式
	TMOD |= 0x10;       //设置定时器模式   手动/自动开关
}

void Tim3Init(void)		//10毫秒@11.0592MHz
{
	T4T3M &= 0xFD;      //定时器时钟12T模式
	T3L = 0x00;         //设置定时初始值
	T3H = 0xDC;         //设置定时初始值
	T4T3M |= 0x08;      //定时器3开始计时
    
    IE2 |= 0x20; //打开IE2-ET2，TIM3中断
}

void ET0ISR(void) interrupt 0 
{
    tempchannel1 = tempchannel2 = tempchannel3 = 1;    
        /*延时移相*/
    TL1 = tim1_t;				//设置定时初始值
	TH1 = tim1_t>>8;				//设置定时初始值

    zero_bit = 1;
    
    TR1 = 1;				//定时器1开始计时      
    ET1 = 1; 
}

void Tim0Isr(void) interrupt 1 
{

}

void Tim1Isr(void) interrupt 3 
{

    if((zero_bit == 1)&&(power_bit == 1))
    {
        switch(channel_num)
        {
            case 1: {tempchannel1=0; tempchannel2=1; tempchannel3=1;}break;
            case 2: {tempchannel1=1; tempchannel2=0; tempchannel3=1;}break;
            case 3: {tempchannel1=1; tempchannel2=1; tempchannel3=0;}break;
            case 4: {tempchannel1=0; tempchannel2=0; tempchannel3=1;}break;
            case 5: {tempchannel1=1; tempchannel2=0; tempchannel3=0;}break;
            case 6: {tempchannel1=0; tempchannel2=1; tempchannel3=0;}break;
            case 7: {tempchannel1=0; tempchannel2=0; tempchannel3=0;}break;
        }
            /*发送一个10us的脉冲*/
            zero_bit = 0; 

            TL1 = 0xF7;				//设置定时初始值
            TH1 = 0xFF;				//设置定时初始值
    }
  
    else
    {
        tempchannel1 = tempchannel2 = tempchannel3 = 1;     //1-0-1的脉冲 2us
        TR1 = 0;				//定时器关闭计时      
        ET1 = 0; 
    }
}

void Tim3Isr(void) interrupt 19 
{
    static uint8_t buzzer_continue_cnt = 0;
    static uint8_t sacn_cnt = 0;
    static uint8_t key_cnt = 0;
    static uint8_t sync_delay_cnt = 0;
    static uint8_t temp_delay_cnt = 0;
    static uint8_t fan_rotate_cnt = 0;
    static uint8_t temp_rotate_cnt = 0;
    static uint16_t fan_delay_cnt = 0;
    
    if((delay_bit1 == 1)&&(delay_bit2 == 1))
    {
        fan_delay_cnt++;
        if(fan_delay_cnt==12000)
        {
            fan_delay_cnt = 0;
            delay_bit1 = 0;
            delay_bit2 = 0;
            delay_bit3 = 1;
        }
    }
    if((delay_bit1&delay_bit2)==0)
    {
        fan_delay_cnt = 0;
    }
//    if((delay_bit1 == 1)&&(delay_bit2 == 0))
//    {
//        fan_delay_cnt = 0;
//    }
    fan_rotate_cnt++;
    if(fan_rotate_cnt>15)
    {
        fan_dis_bit = ~fan_dis_bit;
        fan_rotate_cnt = 0;
    }
    
    temp_rotate_cnt++;
    if(temp_rotate_cnt>50)
    {
        temp_dis_bit = ~temp_dis_bit;
        temp_rotate_cnt = 0;
    }
    
    key_cnt++;
    if(key_cnt>5)
    {
        key_cnt=0; 
        key_val = (P0&0X0f);
    }
    
    if(sync_delay_bit == 1)
    {
        sync_delay_cnt++;
        if(sync_delay_cnt>150)
        {
            sync_delay_bit = 0;
            sync_delay_cnt = 0;
        }
    }

    if(temp_delay_bit == 1)
    {
        temp_delay_cnt++;
        if(temp_delay_cnt>150)
        {
            temp_delay_bit = 0;
            temp_delay_cnt = 0;
        }
    }
    
    if(buzzer_bit==0)
    {
        buzzer_continue_cnt++;
        if(buzzer_continue_cnt>10)
        {
            buzzer_continue_cnt=0; 
            buzzer=buzzer_bit=1;
        }
    } 
    
    if(scan_stop_bit == 1)
    {
        sacn_cnt++;
        if(sacn_cnt>10)
        {
            scan_start_bit = 1;
            scan_stop_bit = 0;
            sacn_cnt = 0;
        }
    }
       
    if(P07==0)
    {
        external_24bit = 1;   //24V接入
        previous_value = 1;
    }
    if(P07==1)
    {
        external_24bit = 0;
        previous_value = 0;
    }
}

void power_crl(uint8_t power_num)
{ 
    if(power_bit==1)
    {
        ET1 = 1;
    }
    else
    {
        ET1 = 0;
    }
    
    tim1_t = power_num*39+59880;
    
//    TL1 = tim1_t;                   //设置定时初始值
//    TH1 = tim1_t>>8;				//设置定时初始值

}