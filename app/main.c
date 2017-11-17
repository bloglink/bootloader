/*******************************************************************************
 * Copyright (c) 2016,青岛艾普智能仪器有限公司
 * All rights reserved.
 *
 * version:     0.4
 * author:      link
 * date:        2016.06.29
 * brief:       增加开机自动上传版本号功能 
 * version:     0.5
 * author:      link
 * date:        2016.09.18
 * brief:       增加读保护功能 
 * version		0.6
 * author		link
 * date			2016.11.14
 * brief		增加多串口支持
 * version		0.7
 * author		link
 * date			2016.12.03
 * brief		去除开机自动上传版本号，增加ISP跳转升级
 * version		0.8
 * author		link
 * date			2016.12.16
 * brief		修正读保护不能开启的BUG，优化ISP跳转升级
*******************************************************************************/
#include "led_driver.h"
#include "can_driver.h"
#include "usart_driver.h"
#include "flash_driver.h"

#define APP_ADDR 0x8004000
#define ISP_ADDR 0x1FFFF000
extern volatile uint32_t UsartTime;
extern volatile uint32_t UsartCount;
extern uint8_t  UsartBuffer[1028];
void USART_ExecuteCmd(void);

extern volatile uint32_t CanTime;
extern volatile uint32_t CanCount;
extern uint8_t CanBuffer[1028];
void CAN_ExecuteCmd(void);

void WaitTimeOut(void);
volatile uint32_t PageCount;
volatile uint32_t TimeOut;
volatile uint8_t isBoot;
/*******************************************************************************
 * version:     0.5
 * author:      link
 * date:        2016.09.18
 * brief:       主程序
 ******************************************************************************/
int main()
{
	
	isBoot  = 1;
	TimeOut = 0;
	FlashInit();
	CAN_Config();
	USARTx_Config(115200);
	CAN_WriteData((uint8_t*)"V0.8",4);
	
	while(1) {
		CAN_ExecuteCmd();
		CanTime++;
		USART_ExecuteCmd();
		UsartTime++;
		WaitTimeOut();
	}
}
/*******************************************************************************
 * version:     0.5
 * author:      link
 * date:        2016.09.18
 * brief:       CAN执行命令
 ******************************************************************************/
void CAN_ExecuteCmd(void)
{
	uint8_t cmd;
	uint8_t crc;
	uint32_t i;
	CAN_ReceiveData();
	if (CanTime < 10000)
		return;
	CanTime = 0;
	if (CanCount == 0)
		return;
	cmd = CanBuffer[0];
	switch (cmd) {
		case 'C'://同步信号
			CAN_WriteData((uint8_t*)"C",1);
			break;
		case 'E'://传输完成
			CAN_WriteData((uint8_t*)"E",1);
			PageCount = 0;
			break;
		case 'G'://跳转指令
			JumpToApplication(APP_ADDR);
			break;
		case 'S'://一包数据接收完成
			if (CanCount < 1028) {//丢帧
				CAN_WriteData((uint8_t*)"N",1);
				break;
			}
			crc = 0;
			for (i=0; i<1024; i++)//校验和
				crc += CanBuffer[i+4];
			if (crc != CanBuffer[3]) {
				CAN_WriteData((uint8_t*)"N",1);
				break;
			}
			ProgramData((uint32_t)APP_ADDR+PageCount*0x400,CanBuffer);//写入
			CAN_WriteData((uint8_t*)"A",1);//写入完成
			PageCount++;
			break;
		case 'R'://心跳包
			CAN_WriteData((uint8_t*)"R",1);
			isBoot = 0;
			break;
		case 'P'://开启读保护
			FLASH_Unlock();
			FLASH_ReadOutProtection(ENABLE);
			FLASH_Lock();
			CAN_WriteData((uint8_t*)"P",1);//
			break;
		default:
			//CAN_WriteData(CanBuffer,CanCount);
			break;
	}
	CanCount = 0;
}
/*******************************************************************************
 * version:     0.5
 * author:      link
 * date:        2016.09.18
 * brief:       USART执行命令
 ******************************************************************************/
void USART_ExecuteCmd(void)
{
	uint8_t cmd;
	uint8_t crc;
	uint32_t i;
	USARTx_ReceiveData();
	if (UsartTime < 10000)
		return;
	UsartTime = 0;
	if (UsartCount == 0)
		return;
	cmd = UsartBuffer[0];
	switch (cmd) {
		case 'C'://同步信号
			USARTx_WriteData((uint8_t*)"C",1);
			break;
		case 'E'://传输完成
			USARTx_WriteData((uint8_t*)"E",1);
			PageCount = 0;
			break;
		case 'G'://跳转指令
			JumpToApplication(APP_ADDR);
			break;
		case 'S'://一包数据接收完成
			if (UsartCount < 1028) {//丢帧
				USARTx_WriteData((uint8_t*)"N",1);
				break;
			}
			crc = 0;
			for (i=0; i<1024; i++)//校验和
				crc += UsartBuffer[i+4];
			if (crc != UsartBuffer[3]) {
				USARTx_WriteData((uint8_t*)"N",1);
				break;
			}
			ProgramData((uint32_t)APP_ADDR+PageCount*0x400,UsartBuffer);//写入
			USARTx_WriteData((uint8_t*)"A",1);//写入完成
			PageCount++;
			break;
		case 'R'://心跳包
			USARTx_WriteData((uint8_t*)"R",1);
			isBoot = 0;
			break;
		case 'I'://转到ISP
			if(FLASH_GetReadOutProtectionStatus() != SET) {
				USARTx_DeInit();
				JumpToApplication(ISP_ADDR);
			}
			USARTx_WriteData((uint8_t*)"N",1);
			break;
		case 'P'://开启写保护
			if(FLASH_GetReadOutProtectionStatus() != SET) {
				FLASH_Unlock();
				FLASH_ReadOutProtection(ENABLE);   
				FLASH_Lock();
			}
			USARTx_WriteData((uint8_t*)"P",1);
			break;
		case 'V':
			USARTx_WriteData((uint8_t*)"V0.8",4);
			break;
		default:
			break;
	}
	UsartCount = 0;
}
/*******************************************************************************
 * version:     0.4
 * author:      link
 * date:        2016.07.28
 * brief:       超时跳转 
 ******************************************************************************/
void WaitTimeOut(void)
{
	if (isBoot) {
		TimeOut++;
		if (TimeOut > 200000) { //约0.6s
			JumpToApplication(APP_ADDR);
		}
	}
}
