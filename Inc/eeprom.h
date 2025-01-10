#ifndef __EEPROM_H_
#define __EEPROM_H_

#include "stdint.h"

void ISPIdle();

uint8_t ISP_read(uint16_t addr);

void ISP_write(uint16_t addr,uint8_t dat);

void ISP_earse(uint16_t addr);

void eeprom_data_write();

void eeprom_mode_save();

void ISP_data_init();

void eeprom_data_read(uint8_t mode);
    
#endif