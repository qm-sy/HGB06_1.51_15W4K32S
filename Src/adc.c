#include "STC15W4Kxx.h"
#include "adc.h"
#include "delay.h"
#include "stdint.h"


#define ADC_POWER   0x80            //ADC电源控制位
#define ADC_SPEED0  0x20            //360个时钟
#define ADC_START   0x08            //ADC起始控制位
#define ADC_FLAG    0x10            //ADC完成标志
 /**************************************************************************
功能描述：ADC口初始化
入口参数：无
返回值：无
 *************************************************************************/
void ADCInit(void)
{	
    P1ASF = 0x27;                   
    ADC_RES = 0;                    
    ADC_RESL=0;
    ADC_CONTR = ADC_POWER | ADC_SPEED0;
}

/**************************************
功能描述：ADC口检测AD转换值函数
入口参数：无
返回值：ADC 12位数据
***************************************/
uint16_t Get_ADC12bitResult(uint8_t channel_x)	
{
    uint16_t adc_value = 0;

    ADC_CONTR |= ADC_POWER | ADC_SPEED0 | ADC_START | (channel_x-1) ; // 启动ADC转换
    while (!(ADC_CONTR & ADC_FLAG));  // 等待ADC转换完成
    ADC_CONTR &= ~ADC_FLAG; 
    
    adc_value = ADC_RESL;
    adc_value |= (ADC_RES << 2);

    return adc_value;
  
}



