#include "delay.h"
#include "intrins.h"

void delay_ms(unsigned int time)	//@11.0592MHz
{
	unsigned char data i, j;
    for(time;time>0;time--)
    {
        _nop_();
        _nop_();
        _nop_();
        i = 11;
        j = 190;
        do
        {
            while (--j);
        } while (--i);
    }
}

void delay_us(unsigned int time)	//@11.0592MHz
{
    for(;time>0;time--)
    {
        _nop_();
        _nop_();
        _nop_();
    }
}