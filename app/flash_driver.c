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

#if defined (STM32F10X_MD) || defined (STM32F10X_MD_VL)
 #define PAGE_SIZE                         (0x400)    /* 1 Kbyte */
 #define FLASH_SIZE                        (0x20000)  /* 128 KBytes */
#elif defined STM32F10X_CL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x40000)  /* 256 KBytes */
#elif defined STM32F10X_HD
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x80000)  /* 512 KBytes */
#elif defined STM32F10X_XL
 #define PAGE_SIZE                         (0x800)    /* 2 Kbytes */
 #define FLASH_SIZE                        (0x100000) /* 1 MByte */
#else 
 #error "Please select first the STM32 device to be used (in stm32f10x.h)"    
#endif  


typedef  void (*pFunction)(void);
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
	if (Address%PAGE_SIZE == 0)
		FLASH_ErasePage(Address);

	for(i=0; i<PAGE_SIZE/4; i++) {
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
 * brief:      擦出指定扇区区间的Flash数
 ******************************************************************************/
void ErasePage(uint32_t StartPageAddr,uint32_t Page)
{
	uint32_t i;

	FLASH_Unlock();

	for(i=0; i<Page; i++){
		FLASH_ErasePage(StartPageAddr + i*PAGE_SIZE);
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
