#include "STC15W4Kxx.h"
#include "adc.h"
#include "delay.h"
#include "stdint.h"


#define ADC_POWER   0x80            //ADC��Դ����λ
#define ADC_SPEED0  0x20            //360��ʱ��
#define ADC_START   0x08            //ADC��ʼ����λ
#define ADC_FLAG    0x10            //ADC��ɱ�־
 /**************************************************************************
����������ADC�ڳ�ʼ��
��ڲ�������
����ֵ����
 *************************************************************************/
void ADCInit(void)
{	
    P1ASF = 0x27;                   
    ADC_RES = 0;                    
    ADC_RESL=0;
    ADC_CONTR = ADC_POWER | ADC_SPEED0;
}

/**************************************
����������ADC�ڼ��ADת��ֵ����
��ڲ�������
����ֵ��ADC 12λ����
***************************************/
uint16_t Get_ADC12bitResult(uint8_t channel_x)	
{
    uint16_t adc_value = 0;

    ADC_CONTR |= ADC_POWER | ADC_SPEED0 | ADC_START | (channel_x-1) ; // ����ADCת��
    while (!(ADC_CONTR & ADC_FLAG));  // �ȴ�ADCת�����
    ADC_CONTR &= ~ADC_FLAG; 
    
    adc_value = ADC_RESL;
    adc_value |= (ADC_RES << 2);

    return adc_value;
  
}



