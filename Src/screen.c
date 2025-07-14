#include "STC15W4Kxx.h"
#include "stdint.h"
#include "delay.h"
#include "intrins.h" 
#include "screen.h"

uint8_t value_03 = 0;  //该地址的值
uint8_t value_06 = 0;
uint8_t value_07 = 0;
uint8_t value_0A = 0;
uint8_t value_0B = 0;
uint8_t value_0E = 0;
uint8_t value_0F = 0; 
 
uint8_t addr_tab[7] = { 0xc3,0xc6,0xc7,0xca,0xcb,0xce,0xcf};

uint8_t num_tab[10] = {0xF5,0x05,0xD3,0x97,0x27,0xB6,0xF6,0x15,0xF7,0xB7};
                     /*  0    1    2    3    4    5    6    7    8    9  */

void lcd_init()
{
    TM1722_STB=1;            //端口配置初始化
    TM1722_CLK=1;
    TM1722_DIO=1;
    TM1722_Write_Byte(0x00); //工作模式
    TM1722_STB=1;
    TM1722_Write_Byte(0x44);   //固定地址模式
    TM1722_STB=1;
    
    lcd_clear();
    
    lcd_display();
}

void lcd_clear()  //清显示缓存
{
    uint8_t i;
                 
    for(i=0;i<7;i++)
    {
        TM1722_Write_Byte(addr_tab[i]);
        TM1722_Write_Byte(0);
        TM1722_STB=1;    
    }
}

void lcd_display()  //显示开
{
    TM1722_Write_Byte(0x93); 
    TM1722_STB=1;
}

void lcd_write_val(uint8_t addr,uint8_t dat)
{
    TM1722_Write_Byte(addr); 
    TM1722_Write_Byte(dat); 
    TM1722_STB=1;
}

/******************************************
函数：写一字节函数
参数：dat---写入的一字节数据
返回值：无
******************************************/
void TM1722_Write_Byte(uint8_t dat)
{
  uint8_t i;
         
  delay_us(50); //用于片选信号的延时
  TM1722_STB=0;     //有效的片选信号
  for(i=0;i<8;i++)
  {
   TM1722_CLK = 0;
   TM1722_DIO = dat&0x01;
   TM1722_CLK = 1;    //时钟上升沿，送入一位数据
   dat>>=1;      
  }
  delay_us(50);   //用于片选信号的延时
}

void num_dis(uint8_t num)
{
    uint8_t hundreds = num / 100;  // 获取百位数
    uint8_t tens = (num / 10) % 10;  // 获取十位数
    uint8_t ones = num % 10;  // 获取个位数
    
    value_0B &= 0x00;
    value_0A &= 0X00;
    value_07 &= 0X00;
    value_06 &= 0X00;
    
    value_0B |= (num_tab[hundreds]>>4);
    value_0A |= num_tab[hundreds];
    value_07 |= num_tab[tens];
    value_06 |= num_tab[ones]; 

    if((num<10)&&(num>=0))
    {
        lcd_write_val(addr_tab[ADDR_0B],0x00);

        lcd_write_val(addr_tab[ADDR_0A],0X00);
        
        lcd_write_val(addr_tab[ADDR_07],0X00);

        lcd_write_val(addr_tab[ADDR_06],value_06);  
    }
    else if((num<100)&&(num>=10))
    {
        lcd_write_val(addr_tab[ADDR_0B],0x00);

        lcd_write_val(addr_tab[ADDR_0A],0X00);
        
        lcd_write_val(addr_tab[ADDR_07],value_07);

        lcd_write_val(addr_tab[ADDR_06],value_06);   
    }
    else
    {
        lcd_write_val(addr_tab[ADDR_0B],value_0B);
        
        lcd_write_val(addr_tab[ADDR_0A],value_0A);
        
        lcd_write_val(addr_tab[ADDR_07],value_07);

        lcd_write_val(addr_tab[ADDR_06],value_06);
    }
    
}

void wind_dis(uint8_t num)
{
    value_0F &= 0x30;
    
    switch(num)
    {
        case 0:
            value_0F |= 0x00;
            break;
        
        case 1:
            value_0F |= 0x40;
            break;
        
        case 2:
            value_0F |= 0xc0;
            break;
        
        case 3:
            value_0F |= 0xc8;
            break;
        
        case 4:
            value_0F |= 0xcc;
            break;
        
        case 5:
            value_0F |= 0xce;
            break;
        
        case 6:
            value_0F |= 0xcf;
            break;
        
        default:
            break;
    }
    
    lcd_write_val(addr_tab[ADDR_0F],value_0F);
}

void channel_dis(uint8_t num)
{
    value_0E &= 0x80;
    value_0B &= 0x00;
    
    switch(num)
    {
        case 1:
            value_0E |= 0x04;
            value_0B |= 0x40;
            break;
        
        case 2:
            value_0E |= 0x02;
            value_0B |= 0x20;
            break;
        
        case 3:
            // value_0E |= 0x01;
            // value_0B |= 0x10;
            value_0E |= 0x06;
            value_0B |= 0x60;
            break;
        
        // case 4:
        //     value_0E |= 0x06;
        //     value_0B |= 0x60;
        //     break;
        
        // case 5:
        //     value_0E |= 0x03;
        //     value_0B |= 0x30;
        //     break;
        
        // case 6:
        //     value_0E |= 0x05;
        //     value_0B |= 0x50;
        //     break;
        
        // case 7:
        //     value_0E |= 0x07;
        //     value_0B |= 0x70;
        //     break;
        
        default:
            break;
    }
    
    lcd_write_val(addr_tab[ADDR_0E],value_0E);
    lcd_write_val(addr_tab[ADDR_0B],value_0B);
}

void sun_dis(bit on_off)
{
    value_0E &= 0xf7;
    if(on_off==DIS_ON)
    {
        value_0E |= 0x08;   
    }
    lcd_write_val(addr_tab[ADDR_0E],value_0E);
}

void sync_dis(bit on_off)
{
    value_03 &= 0xef;
    if(on_off==DIS_ON)
    {
        value_03 |= 0x10;   
    }
    lcd_write_val(addr_tab[ADDR_03],value_03);
}

void alarm_dis(bit on_off)
{
    value_03 &= 0xdf;
    if(on_off==DIS_ON)
    {
        value_03 |= 0x20;   
    }
    lcd_write_val(addr_tab[ADDR_03],value_03);
}

//void F_dis(bit on_off)
//{
//    value_0E &= 0x7f;
//    if(on_off==DIS_ON)
//    {
//        value_0E |= 0x80;   
//    }
//    lcd_write_val(addr_tab[ADDR_0E],value_0E);
//}

//void M_dis(bit on_off)
//{
//    value_0E &= 0xbf;
//    if(on_off==DIS_ON)
//    {
//        value_0E |= 0x40;   
//    }
//    lcd_write_val(addr_tab[ADDR_0E],value_0E);
//}

//void R_dis(bit on_off)
//{
//    value_0E &= 0xdf;
//    if(on_off==DIS_ON)
//    {
//        value_0E |= 0x20;   
//    }
//    lcd_write_val(addr_tab[ADDR_0E],value_0E);
//}

void Celsius_dis(bit on_off)
{
    value_03 &= 0xbf;
    if(on_off==DIS_ON)
    {
        value_03 |= 0x40;   
    }
    lcd_write_val(addr_tab[ADDR_03],value_03);
}

void mode_dis(bit on_off)
{
    value_03 &= 0x7f;
    if(on_off==DIS_ON)
    {
        value_03 |= 0x80;   
    }
    lcd_write_val(addr_tab[ADDR_03],value_03);
}

void percentage_dis(bit on_off)
{
    value_06 &= 0xf7;
    if(on_off==DIS_ON)
    {
        value_06 |= 0x08;   
    }
    lcd_write_val(addr_tab[ADDR_06],value_06);
}

//void cur_dis(bit on_off)
//{
//    value_03 &= 0xf7;
//    if(on_off==DIS_ON)
//    {
//        value_03 |= 0x08;   
//    }
//    lcd_write_val(addr_tab[ADDR_03],value_03);
//}

//void set_dis(bit on_off)
//{
//    value_03 &= 0xfb;
//    if(on_off==DIS_ON)
//    {
//        value_03 |= 0x04;   
//    }
//    lcd_write_val(addr_tab[ADDR_03],value_03);
//}

void fan_center_dis(bit on_off)
{
    value_0F &= 0xdf;
    if(on_off==DIS_ON)
    {
        value_0F |= 0x20;   
    }
    lcd_write_val(addr_tab[ADDR_0F],value_0F);
}

void fan_leaf1_dis(bit on_off)
{
    value_0E &= 0xef;
    if(on_off==DIS_ON)
    {
        value_0E |= 0x10;   
    }
    lcd_write_val(addr_tab[ADDR_0E],value_0E);
}

void fan_leaf2_dis(bit on_off)
{
    value_0F &= 0xef;
    if(on_off==DIS_ON)
    {
        value_0F |= 0x10;   
    }
    lcd_write_val(addr_tab[ADDR_0F],value_0F);
}
