#ifndef __FLASH_DRIVER_H
#define __FLASH_DRIVER_H

#include "stm32f10x.h"

void ProgramData(uint32_t Address, uint8_t *Data);
void ErasePage(uint32_t StartPageAddr,uint32_t Page);
void JumpToApplication(__IO uint32_t Addr);

#endif 
