#include "can_driver.h"
#include "stm32f10x_can.h"
 
uint8_t CanBuffer[1028];
volatile uint32_t CanCount;
volatile uint32_t CanTime;
volatile uint8_t MailBox;
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      CAN��GPIO����
 ******************************************************************************/
void CAN_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	             // ��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		         // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      CAN��NVIC����
 ******************************************************************************/
void CAN_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      CAN���˲�������
 ******************************************************************************/
void CAN_ConfigFilter(uint8_t FilterNumber,uint8_t can_addr)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	CAN_FilterInitStructure.CAN_FilterNumber		 = FilterNumber;
	CAN_FilterInitStructure.CAN_FilterMode			 = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale			 = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh		 = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow			 = (can_addr<<8);
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh	 = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow		 = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation	 = ENABLE;//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);	
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      CAN������
 ******************************************************************************/
void CAN_Config(void)
{
	CAN_InitTypeDef        CAN_InitStructure;           //����CAN��ʼ���ṹ��        
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;     //�����������ʼ���ṹ��                     

	CAN_GPIO_Config();

	CAN_DeInit(CAN1);                                    //��λ��������                
	CAN_StructInit(&CAN_InitStructure);
	CAN_InitStructure.CAN_TTCM = DISABLE;				//ʧ��ʱ�䴥��
	CAN_InitStructure.CAN_ABOM = DISABLE;              	//ʹ�����߹���                
	CAN_InitStructure.CAN_AWUM = DISABLE;				//ʹ���Զ�����
	CAN_InitStructure.CAN_NART = ENABLE;              	//ʹ���Զ��ش�                
	CAN_InitStructure.CAN_RFLM = DISABLE;				//ʧ��FIFO����ģʽ
	CAN_InitStructure.CAN_TXFP = ENABLE;               	//���ȼ��������˳�����            
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;       //����Ϊ����ģʽ              
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;      		//������=36M/((1+4+3)*45)=100K             
	CAN_InitStructure.CAN_BS1 = CAN_BS1_4tq;			//������=36M/((1+4+3)*9)=500K 
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 9;
	CAN_Init(CAN1, &CAN_InitStructure);                 //��������                  

	/* CAN filter init *///	FIFO0�յ�ַƥ��ģ���ַ��Χ1-63
	CAN_FilterInitStructure.CAN_FilterNumber=0;  
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh=0x00;
	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=0;
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure); 
	
	//CAN_NVIC_Config();
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      CANд����
 ******************************************************************************/
uint8_t CAN_WriteData(uint8_t *Message,uint8_t size)
{
	uint8_t i;

	CanTxMsg TxMsg;
	TxMsg.StdId = 0x40;
	TxMsg.RTR	= CAN_RTR_DATA;
	TxMsg.IDE	= CAN_ID_STD;
	TxMsg.DLC	= size;

	for(i=0; i<TxMsg.DLC; i++)
		TxMsg.Data[i] = Message[i];

	MailBox = CAN_Transmit(CAN1,&TxMsg);
	while(CAN_TransmitStatus(CAN1,MailBox) != CAN_TxStatus_Ok);
	return 0;
}

/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      CAN����
 ******************************************************************************/
void CAN_ReceiveData(void)
{
	uint8_t i;
	CanRxMsg RxMsg;
	if (CAN_MessagePending(CAN1,CAN_FIFO0) < 1)
		return;
	CAN_Receive(CAN1,CAN_FIFO0,&RxMsg);
	for (i=0; i<RxMsg.DLC; i++) {
		CanBuffer[CanCount] = RxMsg.Data[i];
		CanCount++;
	}
	CanTime = 0;
}
/*******************************************************************************
 * version:    1.0
 * author:     link
 * date:       2016.06.16
 * brief:      CAN�жϽ���
 ******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg RxMsg;

	CAN_Receive(CAN1,CAN_FIFO0, &RxMsg);
	CAN_ClearITPendingBit(CAN1,CAN_IT_FMP0);
	for (i=0; i<RxMsg.DLC; i++) {
		CanBuffer[CanCount] = RxMsg.Data[i];
		CanCount++;
	}
	CanTime = 0;
}

/*******************************************************************************
 *								END 
 ******************************************************************************/
