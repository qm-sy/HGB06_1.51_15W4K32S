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
uint8_t temp_num = 45;

uint8_t key_val = 0;

uint8_t mode_info[6];
uint8_t alarm_dis_flag = 0;
bit sync_delay_bit = 0;
bit temp_delay_bit = 0;


void button_scan()
{
    uint8_t continue_cnt1;
    uint8_t continue_cnt2;
    uint8_t continue_cnt3;
    if(scan_start_bit == 1)
    {
        switch(key_val)
        {
//            case PowerKey:
//                power_key();
//                break;
            
            case UpKey:
                up_key();
                button_scan_flag = 1;
                button_scan_cnt = 0;
                delay_ms(50);
                break;
            
            case ChannelChoose:
                continue_cnt1 = 100;
                while((key_val==ChannelChoose)&&(continue_cnt1>0))
                {
                    continue_cnt1--; 
                    delay_ms(10);
                }
                if(continue_cnt1 == 0)           //����1s�жϣ�����ͬ�� 
                {
                    fan_delay_set();
                }

                if(temp_delay_bit == 0)         //�̰���mode�л�
                {
                    channel_choose();
                }
                button_scan_flag = 1;
                button_scan_cnt = 0;
                delay_ms(50);
                break;

            case FunctionKey:
                continue_cnt3 = 100;
                while((key_val==FunctionKey)&&(continue_cnt3>0))
                {
                    continue_cnt3--; 
                    delay_ms(10);
                }
                if(continue_cnt3 == 0)           //����1s�жϣ�����ͬ�� 
                {
                    temp_set();
                }
                button_scan_flag = 1;
                button_scan_cnt = 0;
                break;

            case ModeChoose:                    //����ͬ���л���mode�л�
                continue_cnt2 = 100;
                while((key_val==ModeChoose)&&(continue_cnt2>0))
                {
                    continue_cnt2--; 
                    delay_ms(10);
                }
                
                if(continue_cnt2 == 0)           //����1s�жϣ�����ͬ�� 
                {
                    sync_choose();
                }

                if(sync_delay_bit == 0)         //�̰���mode�л�
                {
                    mode_choose();
                }
                button_scan_flag = 1;
                button_scan_cnt = 0;
                delay_ms(50);
                break;
                
            case DownKey:
                down_key();
                button_scan_flag = 1;
                button_scan_cnt = 0;
                delay_ms(50);
                break;
            
            case FanUp:
                fan_up();   
                button_scan_flag = 1;
                button_scan_cnt = 0;
                delay_ms(50); 
                break;
            
            case FanDown:
                fan_down();   
                button_scan_flag = 1;     
                button_scan_cnt = 0;  
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
    
    if(channel_num==3)
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

void fan_delay_set()
{
    unsigned char num3 = 0;
    num3 = fengshan_delay;
    
    buzzer=buzzer_bit=0;
    delay_ms(500);
    while(key_val!=ChannelChoose)
    {
        if(temp_dis_bit)
        {
            num_dis(fengshan_delay);
            //Celsius_dis(DIS_ON);
            
        }
        if(~temp_dis_bit)
        {
            lcd_write_val(addr_tab[ADDR_0B],0x00);

            lcd_write_val(addr_tab[ADDR_0A],0X00);

            lcd_write_val(addr_tab[ADDR_07],0X00);

            lcd_write_val(addr_tab[ADDR_06],0X00);
            
            //Celsius_dis(DIS_ON);
            
        }
        if(key_val == UpKey)
        {
            buzzer=buzzer_bit=0;
            num3+=1;
            if(num3>200)
            {
                num3 = 200;
            }
            fengshan_delay = num3;

            delay_ms(100);
        }
        if(key_val == DownKey)
        {
            buzzer=buzzer_bit=0;
            num3-=1;
            if(num3<2)
            {
                num3 = 1;
            }
            fengshan_delay = num3;

            delay_ms(100);
        }
        button_scan_flag = 1;
        button_scan_cnt = 0;
    }
    temp_delay_bit = 1;
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

void temp_set()
{
    unsigned char num3 = 0;
    num3 = temp_num;
    
    buzzer=buzzer_bit=0;
    alarm_dis(alarm_dis_flag);
    delay_ms(500);
    while(key_val!=FunctionKey)
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
            num3+=1;
            if(num3>120)
            {
                num3 = 120;
            }
            temp_num = num3;

            delay_ms(100);
        }
        if(key_val == DownKey)
        {
            buzzer=buzzer_bit=0;
            num3-=1;
            if(num3<20)
            {
                num3 = 20;
            }
            temp_num = num3;

            delay_ms(100);
        }
        if(key_val == ModeChoose)
        {
            buzzer=buzzer_bit=0;
            alarm_dis_flag = 1 - alarm_dis_flag;
            alarm_dis(alarm_dis_flag);
            delay_ms(100);
        }
        button_scan_flag = 1;
        button_scan_cnt = 0;
    }
    
    temp_delay_bit = 1;         //��ʱ1s  ��ֹ�������̰��� 
    
    num_dis(power_num);           
    channel_dis(channel_num);
    Celsius_dis(DIS_OFF);
    percentage_dis(DIS_ON);
    alarm_dis(DIS_OFF);
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
    
    lcd_clear();                //һ������
    
    percentage_dis(DIS_OFF);
    mode_dis(DIS_ON);       
    num_dis(mode_num);           //��ʾģʽ��źͱ��?
      
    eeprom_data_read(mode_num);  //��ȡchannel_num��sync_bit��wind_num��power_num
        
    delay_ms(500);
    
    lcd_clear();                 //��������
    
    mode_dis(DIS_OFF);
    sun_dis(~sync_bit);
    sync_dis(sync_bit);
    num_dis(power_num);
    channel_dis(channel_num);
    percentage_dis(DIS_ON);
    wind_dis(wind_num);          //ˢ�½��棺���ȷ��š�ͨ�������ʡ��ٷֱȡ�������ͬ����־
    delay_bit1 = 0;
    delay_bit2 = 0;

    sync_judge();               //�ж�ͬ��״̬

    temp_listen();

    eeprom_mode_save();         //��¼ģʽ���?
}

void sync_choose()
{
    buzzer=buzzer_bit=0;

    sync_delay_bit = 1;         //��ʱ1s  ��ֹ�������̰��� 

    sync_bit = ~sync_bit;       //ͬ����or��
    sync_dis(sync_bit);

    sync_judge();

    eeprom_data_write();        //��¼ͬ��״̬
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
            button_scan_flag = 1;
            button_scan_cnt = 0;
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
        if(continue_cnt == 0)           //����1s�жϣ�����ͬ�� 
        {
            lcd_clear();
            power_bit = 0;
            sun_dis(DIS_OFF);
            PWMStop();
            on_off = 1;
        }
    }

}