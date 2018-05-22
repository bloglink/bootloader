/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.06.16
 * brief:       USART驱动程序
 ******************************************************************************/
#include "usart_driver.h"

uint8_t  UsartBuffer[1028];
uint32_t UsartCount;
uint32_t UsartTime;
uint32_t Usart;
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART的设置
 ******************************************************************************/
void USARTx_Config(uint32_t bitrate)
{
	Usart = 0;
	USART1_Config(bitrate);
	USART2_Config(bitrate);
	USART3_Config(bitrate);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART1的设置
 ******************************************************************************/
void USART1_Config(uint32_t bitrate)
{	
	NVIC_InitTypeDef NVIC_InitStructure;  
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = bitrate;
	USART_Init(USART1, &USART_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	//USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 

	USART_Cmd(USART1, ENABLE);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART2的设置
 ******************************************************************************/
void USART2_Config(uint32_t bitrate)
{
	NVIC_InitTypeDef NVIC_InitStructure;  
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = bitrate;
	USART_Init(USART2, &USART_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); 

	USART_Cmd(USART2, ENABLE);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART3的设置
 ******************************************************************************/
void USART3_Config(uint32_t bitrate)
{
	NVIC_InitTypeDef NVIC_InitStructure;  
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = bitrate;
	USART_Init(USART3, &USART_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);  
	//USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); 

	USART_Cmd(USART3, ENABLE);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART写数据
 ******************************************************************************/
void USARTx_WriteData(uint8_t *Buffer, uint32_t Length)
{
	switch (Usart) {
		case 0:
			USART1_WriteData(Buffer,Length);
			USART2_WriteData(Buffer,Length);
			USART3_WriteData(Buffer,Length);
			break;
		case 1:
			USART1_WriteData(Buffer,Length);
			break;
		case 2:
			USART2_WriteData(Buffer,Length);
			break;
		case 3:
			USART3_WriteData(Buffer,Length);
			break;
		default:
			break;
	}
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART1写数据
 ******************************************************************************/
void USART1_WriteData(uint8_t *Buffer, uint32_t Length)
{
	uint32_t i;
	for (i=0; i<Length; i++) {
		USART_SendData(USART1,(uint8_t)Buffer[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}

/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART2写数据
 ******************************************************************************/
void USART2_WriteData(uint8_t *Buffer, uint32_t Length)
{
	uint32_t i;
	for (i=0; i<Length; i++) {
		USART_SendData(USART2,(uint8_t)Buffer[i]);
		while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
	}
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART3写数据
 ******************************************************************************/
void USART3_WriteData(uint8_t *Buffer, uint32_t Length)
{
	uint32_t i;
	for (i=0; i<Length; i++) {
		USART_SendData(USART3,(uint8_t)Buffer[i]);
		while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
	}
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART1接收数据
 ******************************************************************************/
void USARTx_ReceiveData(void)
{
	switch (Usart) {
		case 0:
			if (USART1_ReceiveData())
				break;
			if (USART2_ReceiveData())
				break;
			if (USART3_ReceiveData())
				break;
		break;
		case 1:
			USART1_ReceiveData();
			break;
		case 2:
			USART2_ReceiveData();
			break;
		case 3:
			USART3_ReceiveData();
			break;
		default:
			break;
	}
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART1接收数据
 ******************************************************************************/
uint8_t USART1_ReceiveData(void)
{
	uint8_t temp;
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		temp = USART_ReceiveData(USART1);
		UsartBuffer[UsartCount] = temp;
		UsartCount++;
		UsartTime = 0;
		Usart = 1;
		return 1;
	}
	return 0;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART2接收数据
 ******************************************************************************/
uint8_t USART2_ReceiveData(void)
{
	uint8_t temp;
	if (USART_GetFlagStatus(USART2,USART_FLAG_RXNE) != RESET) {
		USART_ClearFlag(USART2,USART_FLAG_RXNE);
		temp = USART_ReceiveData(USART2);
		UsartBuffer[UsartCount] = temp;
		UsartCount++;
		UsartTime = 0;
		Usart = 2;
		return 1;
	}
	return 0;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART3接收数据
 ******************************************************************************/
uint8_t USART3_ReceiveData(void)
{
	uint8_t temp;
	if (USART_GetFlagStatus(USART3,USART_FLAG_RXNE) != RESET) {
		USART_ClearFlag(USART3,USART_FLAG_RXNE);
		temp = USART_ReceiveData(USART3);
		UsartBuffer[UsartCount] = temp;
		UsartCount++;
		UsartTime = 0;
		Usart = 3;
		return 1;
	}
	return 0;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART1接收中断
 ******************************************************************************/
void USART1_IRQHandler(void)
{
	uint8_t temp;
	if (USART_GetITStatus(USART1,USART_IT_RXNE) != RESET) {
		temp = USART_ReceiveData(USART1);
		UsartBuffer[UsartCount] = temp;
		UsartCount++;
		UsartTime = 0;
	}
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART2接收中断
 ******************************************************************************/
void USART2_IRQHandler(void)
{
	uint8_t temp;
	if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET) {
		temp = USART_ReceiveData(USART2);
		UsartBuffer[UsartCount] = temp;
		UsartCount++;
		UsartTime = 0;
	}
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      USART3接收中断
 ******************************************************************************/
void USART3_IRQHandler(void)
{
	uint8_t temp;
	if (USART_GetITStatus(USART3,USART_IT_RXNE) != RESET) {
		temp = USART_ReceiveData(USART3);
		UsartBuffer[UsartCount] = temp;
		UsartCount++;
		UsartTime = 0;
	}
}
/*******************************************************************************
 *								END 
 ******************************************************************************/
