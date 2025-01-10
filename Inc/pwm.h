#ifndef __PWM_H_
#define __PWM_H_

#define CYCLE 4096

void PWM5Init(void);
void PWMSet(unsigned int dat);
void PWMStart(void);
void PWMStop(void);
void wind_crl(unsigned char wind_num);

#endif 