#include "STC15W4Kxx.h"
#include "key.h"
#include "stdint.h"
#include "timer.h"
#include "pwm.h"
#include "screen.h"
#include "delay.h"
#include "eeprom.h"
#include "stdio.h"
#include "sync.h"

bit on_off = 1;

bit sync_bit = 0;

uint8_t wind_num = 0;
uint8_t power_num = 0;
uint8_t mode_num = 0;
uint8_t temp_num = 80;

uint8_t key_val = 0;

uint8_t mode_info[5];

bit sync_delay_bit = 0;
bit temp_delay_bit = 0;


void button_scan()
{
    uint8_t continue_cnt1;
    uint8_t continue_cnt2;
    if(scan_start_bit == 1)
    {
        switch(key_val)
        {
//            case PowerKey:
//                power_key();
//                break;
            
            case UpKey:
                up_key();
                delay_ms(50);
                break;
            
            case ChannelChoose:
                continue_cnt1 = 100;
                while((key_val==ChannelChoose)&&(continue_cnt1>0))
                {
                    continue_cnt1--; 
                    delay_ms(10);
                }
                if(continue_cnt1 == 0)           //长按1s判断，开启同步 
                {
                    temp_set();
                }

                if(temp_delay_bit == 0)         //短按，mode切换
                {
                    channel_choose();
                }
                delay_ms(50);
                break;
//            case FunctionKey:
            
            case ModeChoose:                    //复用同步切换、mode切换
                continue_cnt2 = 100;
                while((key_val==ModeChoose)&&(continue_cnt2>0))
                {
                    continue_cnt2--; 
                    delay_ms(10);
                }
                
                if(continue_cnt2 == 0)           //长按1s判断，开启同步 
                {
                    sync_choose();
                }

                if(sync_delay_bit == 0)         //短按，mode切换
                {
                    mode_choose();
                }
                
                delay_ms(50);
                break;
                
            case DownKey:
                down_key();
                delay_ms(50);
                break;
            
            case FanUp:
                fan_up();   
                delay_ms(50); 
                break;
            
            case FanDown:
                fan_down();          
                delay_ms(50);
                break;

        }
        if(power_bit==1)
        {
            sun_dis(DIS_ON);
        }
        scan_stop_bit = 1;
        scan_start_bit = 0;
    }
}

void fan_up()
{
    char up_num1 = 0;
    up_num1 = wind_num;
    
    buzzer=buzzer_bit=0;
    
    up_num1 += 1;
    if(up_num1>6)
    {
        up_num1 = 6;
    }
    
    wind_num = up_num1;
    wind_crl(wind_num);
    wind_dis(wind_num);
    
    eeprom_data_write();
}

void fan_down()
{
    char down_num1 = 0;
    down_num1 = wind_num;
    
    buzzer=buzzer_bit=0;
    
    down_num1 -= 1;
    if(down_num1<0)
    {
        down_num1 = 0;
    }

    wind_num = down_num1;
    wind_crl(wind_num);
    wind_dis(wind_num);
    
    eeprom_data_write();
}

void up_key()
{
    char up_num2 = 0;
    up_num2 = power_num;
    
    buzzer=buzzer_bit=0;
    
    up_num2 += 5;
    if(up_num2>100)
    {
        up_num2 = 100;
    }
    
    power_num = up_num2;
    power_crl(power_num);
    num_dis(power_num);
    channel_dis(channel_num);
    percentage_dis(DIS_ON);
    
    eeprom_data_write();
}

void down_key()
{
    char down_num2 = 0;
    down_num2 = power_num;
    
    buzzer=buzzer_bit=0;
    
    down_num2 -= 5;
    if(down_num2<0)
    {
        down_num2 = 0;
    }
    
    power_num = down_num2;
    power_crl(power_num);
    num_dis(power_num);
    channel_dis(channel_num);
    percentage_dis(DIS_ON);
    
    eeprom_data_write();
}

void channel_choose()
{
    buzzer=buzzer_bit=0;
    
    if(channel_num==7)
    {
        channel_num = 1;
    }
    else
    {
        channel_num += 1;
    }
    channel_dis(channel_num);
    if(power_bit==1)
    {
        sun_dis(DIS_ON);
    }
    eeprom_data_write();
}

void temp_set()
{
    unsigned char num3 = 0;
    num3 = temp_num;
    
    buzzer=buzzer_bit=0;
    delay_ms(500);
    while(key_val!=ChannelChoose)
    {
        if(temp_dis_bit)
        {
            num_dis(temp_num);
            Celsius_dis(DIS_ON);
            
        }
        if(~temp_dis_bit)
        {
            lcd_write_val(addr_tab[ADDR_0B],0x00);

            lcd_write_val(addr_tab[ADDR_0A],0X00);

            lcd_write_val(addr_tab[ADDR_07],0X00);

            lcd_write_val(addr_tab[ADDR_06],0X00);
            
            Celsius_dis(DIS_ON);
            
        }
        if(key_val == UpKey)
        {
            buzzer=buzzer_bit=0;
            num3+=5;
            if(num3>200)
            {
                num3 = 200;
            }
            temp_num = num3;

            delay_ms(100);
        }
        if(key_val == DownKey)
        {
            buzzer=buzzer_bit=0;
            num3-=5;
            if(num3<50)
            {
                num3 = 50;
            }
            temp_num = num3;

            delay_ms(100);
        }
    }
    
    temp_delay_bit = 1;         //延时1s  防止长按、短按误触 
    
    num_dis(power_num);           
    channel_dis(channel_num);
    Celsius_dis(DIS_OFF);
    percentage_dis(DIS_ON);
    if(power_bit==1)
    {
        sun_dis(DIS_ON);
    }
    eeprom_data_write();
}

void mode_choose()
{
    buzzer=buzzer_bit=0;
    mode_num += 1;
    if(mode_num==6)
    {
        mode_num = 1;
    }
    
    lcd_clear();                //一次清屏
    
    percentage_dis(DIS_OFF);
    mode_dis(DIS_ON);       
    num_dis(mode_num);           //显示模式序号和标志
      
    eeprom_data_read(mode_num);  //读取channel_num、sync_bit、wind_num、power_num
        
    delay_ms(500);
    
    lcd_clear();                 //二次清屏
    
    mode_dis(DIS_OFF);
    sun_dis(~sync_bit);
    sync_dis(sync_bit);
    num_dis(power_num);
    channel_dis(channel_num);
    percentage_dis(DIS_ON);
    wind_dis(wind_num);          //刷新界面：加热符号、通道、功率、百分比、风力、同步标志
    delay_bit1 = 0;
    delay_bit2 = 0;

    sync_judge();               //判断同步状态

    temp_listen();

    eeprom_mode_save();         //记录模式序号
}

void sync_choose()
{
    buzzer=buzzer_bit=0;

    sync_delay_bit = 1;         //延时1s  防止长按、短按误触 

    sync_bit = ~sync_bit;       //同步开or关
    sync_dis(sync_bit);

    sync_judge();

    eeprom_data_write();        //记录同步状态
}

//void power_key()
//{
//   lcd_clear();
//}

void fan_rotate()
{
    fan_center_dis(DIS_ON);
    if((wind_bit == 1)&&(wind_num>0))
    {
        fan_leaf1_dis(fan_dis_bit);
        fan_leaf2_dis(~fan_dis_bit);
    }
    else
    {
        fan_leaf1_dis(DIS_ON);
        fan_leaf2_dis(DIS_OFF);
    }
}

uint8_t power_on()
{
    if(scan_start_bit == 1)
    {
        if(key_val == PowerKey)
        {
            return 0;
        }
    }
    return 1;
}

void power_off()
{
    uint8_t continue_cnt;
    if(scan_start_bit == 1)
    {
        continue_cnt = 100;
        while((key_val==PowerKey)&&(continue_cnt>0))
        {
            continue_cnt--; 
            delay_ms(10);
        }
        if(continue_cnt == 0)           //长按1s判断，开启同步 
        {
            lcd_clear();
            power_bit = 0;
            sun_dis(DIS_OFF);
            PWMStop();
            on_off = 1;
        }
    }

}