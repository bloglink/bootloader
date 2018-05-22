#include <stm32f10x.h>
#include <stm32f10x_conf.h>
#include "led_driver.h"
#include "usart_driver.h"
#include "flash_driver.h"
#include "can_driver.h"

#define APP_ADDR 0x8004000

extern volatile uint32_t UsartTime;
extern volatile uint32_t UsartCount;
extern uint8_t  UsartBuffer[1028];
void USART_ExecuteCmd ( void );
void USART_TimeOut ( void );

extern volatile uint32_t CanTime;
extern volatile uint32_t CanCount;
extern uint8_t CanBuffer[1028];
void CAN_ExecuteCmd ( void );

uint32_t PageCount;
uint32_t TimeOut;
uint8_t isBoot;

int main()
{
    isBoot = 1;
    TimeOut = 0;
    USART1_Config ( 115200 );

    while ( 1 )
    {
        USART1_ReceiveData();

        if ( UsartTime > 10000 )
        {
            UsartTime = 0;

            if ( UsartCount > 0 )
            {
                USART_ExecuteCmd();
                UsartCount = 0;
            }

            if ( isBoot )
                USART_TimeOut();
        }

        UsartTime++;
    }
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.29
 * brief:      USART执行命令
 ******************************************************************************/
void USART_ExecuteCmd ( void )
{
    uint8_t cmd;
    uint8_t crc;
    uint32_t i;
    cmd = UsartBuffer[0];

    switch ( cmd )
    {
    case 'C'://同步信号
        USART1_WriteData ( ( uint8_t* ) "C", 1 );
        break;

    case 'E'://传输完成
        USART1_WriteData ( ( uint8_t* ) "E", 1 );
        PageCount = 0;
        break;

    case 'G'://跳转指令
        JumpToApplication ( APP_ADDR );
        break;

    case 'S'://一包数据接收完成
        if ( UsartCount < 1028 ) //丢帧
        {
            USART1_WriteData ( ( uint8_t* ) "D", 1 );
            break;
        }

        crc = 0;

        for ( i = 0; i < 1024; i++ ) //校验和
            crc += UsartBuffer[i + 4];

        if ( crc != UsartBuffer[3] )
        {
            USART1_WriteData ( ( uint8_t* ) "N", 1 );
            break;
        }

        ProgramData ( ( uint32_t ) APP_ADDR + PageCount * 0x400, UsartBuffer ); //写入
        USART1_WriteData ( ( uint8_t* ) "A", 1 ); //写入完成
        PageCount++;
        break;

    case 'R'://读数据
        USART1_WriteData ( ( uint8_t* ) "R", 1 );
        isBoot = 0;
        break;

    default:
        USART1_WriteData ( UsartBuffer, 1 );
        break;
    }

    UsartCount = 0;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.29
 * brief:      USART计时跳转
 ******************************************************************************/
void USART_TimeOut ( void )
{
    TimeOut++;

    if ( TimeOut > 80 )
    {
        JumpToApplication ( APP_ADDR );
    }
}
