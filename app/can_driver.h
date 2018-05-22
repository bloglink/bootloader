#ifndef __CAN_DRIVER_H
#define __CAN_DRIVER_H

#include "stm32f10x.h"

void CAN_Config(void);

uint8_t CAN_WriteData(uint8_t *Message,uint8_t size);

void CAN_ReceiveData(void);

#endif 
