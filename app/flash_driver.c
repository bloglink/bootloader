/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     1.0
 * author:      link
 * date:        2016.06.16
 * brief:       Flash驱动程序
 ******************************************************************************/
#include "flash_driver.h"

volatile uint32_t PageSize;

typedef  void (*pFunction)(void);
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.07.28
 * brief:      读取页面大小
 ******************************************************************************/
 void FlashInit(void)
 {
	 uint8_t flash;
	 flash =  *(__IO u8 *)(0X1FFFF7E0);
	 if (flash == 0x00)
		 PageSize = 0x800;
	 else
		 PageSize = 0x400;
 }
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      写入一页数据 
 ******************************************************************************/
void ProgramData(uint32_t Address, uint8_t *Data)
{
	uint32_t i;
	uint32_t temp,temp1,temp2,temp3,temp4;

	FLASH_Unlock();
	if (Address%PageSize == 0)
		FLASH_ErasePage(Address);

	for(i=0; i<0x400/4; i++) {
		temp1 = Data[i*4+4]; 
		temp2 = Data[i*4+5]; 
		temp3 = Data[i*4+6]; 
		temp4 = Data[i*4+7]; 
		temp = temp1+(temp2<<8)+(temp3<<16)+(temp4<<24);
		FLASH_ProgramWord(Address+i*4, temp);
	}
	FLASH_Lock();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      控制程序跳转到指定位置开始执
 ******************************************************************************/
void JumpToApplication(__IO uint32_t Addr)
{
	pFunction Jump_To_Application;
	__IO uint32_t JumpAddress; 

	if (((*(__IO uint32_t*)Addr) & 0x2FFE0000 ) == 0x20000000) { 
		JumpAddress = *(__IO uint32_t*) (Addr + 4);
		Jump_To_Application = (pFunction) JumpAddress;
	//	__set_PRIMASK(1);//关闭所有中断
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000); //重新映射中断向量表
		__set_MSP(*(__IO uint32_t*)Addr);
		Jump_To_Application();
	}
}
/*******************************************************************************
 *								END 
 ******************************************************************************/
