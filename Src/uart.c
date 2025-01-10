#include "STC15W4Kxx.h"
#include "uart.h"
#include "stdint.h"
#include "delay.h"

bit busy_bit;

void Uart1Init(void)	//115200bps@11.0592MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x01;		//串口1选择定时器2为波特率发生器
	AUXR |= 0x04;		//定时器时钟1T模式
	T2L = 0xE8;			//设置定时初始值
	T2H = 0xFF;			//设置定时初始值
	AUXR |= 0x10;		//定时器2开始计时
    
    ES = 1;
}

void Uart1Isr() interrupt 4 
{   
    if(TI == 1)
    {
        TI = 0;
        busy_bit = 0;
    }
    
    if(RI == 1)
    {
        RI = 0;
    }
}

void Uart1Sendbyte(uint8_t dat)
{
    while(busy_bit);
    busy_bit = 1;
    SBUF = dat;
}

void Uart1SendStr(uint8_t *sendstr)
{   
    while(*sendstr)
    {
        Uart1Sendbyte(*sendstr++);
    }
}

char putchar(char c)  // 串口重定向需要添加头文件stdio.h
{
    SBUF = c;
    while(!TI);
    TI = 0;
    return c;
}