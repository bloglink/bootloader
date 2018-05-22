#ifndef _USART_DRIVER_H
#define _USART_DRIVER_H

#include "stm32f10x.h"

void USART1_Config(uint32_t bitrate);
void USART2_Config(uint32_t bitrate);

void USART1_WriteData(uint8_t *Buffer, uint32_t Length);
void USART2_WriteData(uint8_t *Buffer, uint32_t Length);

void USART1_ReceiveData(void);
void USART2_ReceiveData(void);

void USART1_IRQHandler(void);
void USART2_IRQHandler(void);

#endif 

