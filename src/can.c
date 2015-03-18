#include "can.h"

//float DataFromCan[5];

void CAN_configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	CAN_InitTypeDef CAN_InitStructure;
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	
	/* GPIO clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	/* Configure CAN pin: RX */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  	/* Configure CAN pin: TX */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
//  	GPIO_Init(GPIOB, &GPIO_InitStructure);
//    GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);
  
  	/* CANx Periph clock enable */
  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
  
  	/* CAN register init */
  	CAN_DeInit(CAN1);
  	CAN_StructInit(&CAN_InitStructure);

  	/* CAN cell init */
  	CAN_InitStructure.CAN_TTCM = DISABLE;
  	CAN_InitStructure.CAN_ABOM = DISABLE;
  	CAN_InitStructure.CAN_NART = DISABLE;
  	CAN_InitStructure.CAN_AWUM = DISABLE;
  	CAN_InitStructure.CAN_RFLM = DISABLE;
  	CAN_InitStructure.CAN_TXFP = DISABLE;
  	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  
  	/* CAN Baudrate = 1MBps*/
  	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  	CAN_InitStructure.CAN_BS1 = CAN_BS1_5tq;
  	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
  	CAN_InitStructure.CAN_Prescaler = 4; 				//8-500K, 16-250K, 32-125K (1,3,5)
														//4-1M(1,5,3)
  	CAN_Init(CAN1, &CAN_InitStructure);
/*滤波器0，过滤左小腿数据，ID=0x123由FIFO0接收*/
  	CAN_FilterInitStructure.CAN_FilterNumber = 0;
  	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0xffff;
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0xffff;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
  	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
		CAN_FilterInit(&CAN_FilterInitStructure);


   /*接受中断*/
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);  
		
		
   /*发送中断*/
  	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);  
		
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE); 
}

/*can线，传输32位浮点数据
**ID can线ID，angle浮点数据的指针
*/
// void CAN_Transmit_data(uint32_t ID, float angle)
// {
// 	CanTxMsg TxMessage;
// 	int32_t Temp;

// 	Temp = angle*10;

//    	TxMessage.StdId = ID;
// 	TxMessage.ExtId	= 0x00;
// 	TxMessage.RTR 	= CAN_RTR_DATA;
// 	TxMessage.IDE	= CAN_ID_STD;
// 	TxMessage.DLC	= 4;

// 	TxMessage.Data[0] = Temp&0xff;
// 	TxMessage.Data[1] = (Temp>>8)&0xff;
//     TxMessage.Data[2] = (Temp>>16)&0xff;
//     TxMessage.Data[3] = (Temp>>24)&0xff;
// 		
// 	CAN_Transmit(CAN1, &TxMessage);
// }
// /*接收can1的FIFO0消息，因为接收的数据有五个，而一个FIFO只有三级，所以后两个数据由FIFO1处理*/
// void CAN_RcvMsg_FIFO0(void)
// {
//  	char temp[4];
// 	CanRxMsg RxMessage;
//   	RxMessage.StdId=0x00;
//   	RxMessage.ExtId=0x00;
//   	RxMessage.IDE=0;
//   	RxMessage.DLC=0;
//   	RxMessage.FMI=0;
//   	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
// 	
// 	temp[0] = RxMessage.Data[0];
// 	temp[1] = RxMessage.Data[1];
// 	temp[2] = RxMessage.Data[2];
// 	temp[3] = RxMessage.Data[3];
// 	switch(RxMessage.StdId)
// 	{
// 		case(0x123):	  				//接收左小腿数据
// 		DataFromCan[0] = *((float*)temp);
// 		break;

// 		case(0x124):					//接收左大腿数据
// 		DataFromCan[1] = *((float*)temp);
// 		break;

// 		case(0x125):					//接收右小腿数据
// 		DataFromCan[2] = *((float*)temp);
// 		break;

// 	}
// }
// void CAN_RcvMsg_FIFO1(void)
// {
//  	char temp[4];
// 	CanRxMsg RxMessage;
//   	RxMessage.StdId=0x00;
//   	RxMessage.ExtId=0x00;
//   	RxMessage.IDE=0;
//   	RxMessage.DLC=0;
//   	RxMessage.FMI=0;
//   	CAN_Receive(CAN1, CAN_FIFO1, &RxMessage);
// 	
// 	temp[0] = RxMessage.Data[0];
// 	temp[1] = RxMessage.Data[1];
// 	temp[2] = RxMessage.Data[2];
// 	temp[3] = RxMessage.Data[3];
// 	switch(RxMessage.StdId)
// 	{
// 		case(0x126):					//接收右大腿数据
// 		DataFromCan[3] = *((float*)temp);
// 		break;

// 		case(0x127):					//接收背部数据
// 		DataFromCan[4] = *((float*)temp);
// 		break;
// 	}		
// }
/*can线的中断处理函数函数，stm32的can接收中断有两个，而每一个有3级fifo*/
// void USB_LP_CAN1_RX0_IRQHandler(void) 
// {
// 	CAN_RcvMsg_FIFO0();
// }
// void  CAN1_RX1_IRQHandler(void)
// {
// 	CAN_RcvMsg_FIFO1();
// }  
