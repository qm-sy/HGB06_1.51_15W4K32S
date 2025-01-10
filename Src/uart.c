#include "STC15W4Kxx.h"
#include "uart.h"
#include "stdint.h"
#include "delay.h"

bit busy_bit;

void Uart1Init(void)	//115200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��ʱ��1Tģʽ
	T2L = 0xE8;			//���ö�ʱ��ʼֵ
	T2H = 0xFF;			//���ö�ʱ��ʼֵ
	AUXR |= 0x10;		//��ʱ��2��ʼ��ʱ
    
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

char putchar(char c)  // �����ض�����Ҫ���ͷ�ļ�stdio.h
{
    SBUF = c;
    while(!TI);
    TI = 0;
    return c;
}