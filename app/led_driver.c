/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.06.16
 * brief:       LED驱动程序
 ******************************************************************************/
#include "led_driver.h"
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      LED的GPIO设置
 ******************************************************************************/
void LED_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      LED闪烁loop次 
 ******************************************************************************/
void LED_Blink(uint32_t loop)
{
	uint32_t i;
	for (i=0; i<loop; i++) {
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		LED_Delay(100000);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		LED_Delay(100000);
	}
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      延时程序 
 ******************************************************************************/
void LED_Delay (uint32_t t) 
{
	volatile uint32_t i,j;
	for (i=0 ; i < t ; i++)
		j++;
}

/*******************************************************************************
 *								END 
 ******************************************************************************/
