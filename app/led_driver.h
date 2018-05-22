#ifndef __LED_DRIVER_H
#define __LED_DRIVER_H

#include "stm32f10x.h"

void LED_Config(void);
void LED_Blink(uint32_t loop);
void LED_Delay(uint32_t t);

#endif 
