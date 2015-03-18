#include "STM32Lib\\stm32f10x.h"
#include "ISR\\isr.h"
/**********************************************

**********************************************/
char buf[12];
void USART_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
		USART_ClockInitTypeDef USART_ClockInitStructure;
	  DMA_InitTypeDef DMA_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;
	//comport1，PA，AFIObus
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD|
            RCC_APB2Periph_AFIO |
            RCC_APB2Periph_USART1 , 
            ENABLE);

    /* A9 USART1_Tx */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//-TX
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		/* PD8 U3_Tx*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* A10 USART1_Rx  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//-RX
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		
		/* PD9 U3_Rx*/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_Init(GPIOD, &GPIO_InitStructure);

		//DMA
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
		DMA_DeInit(DMA1_Channel1);
  	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&(USART1->DR);
  	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)buf;
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;	 
  	//DMA_InitStructure.DMA_BufferSize = 10;  			//imporetant
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;		  
  	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  	DMA_Init(DMA1_Channel4, &DMA_InitStructure);
  	DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
		DMA_Cmd(DMA1_Channel4,DISABLE);
		
	//中断配置
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
		
		USART_InitStructure.USART_BaudRate = 115200;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
		USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
		USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
		USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
		USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;
		
	//USART1
		USART_ClockInit(USART1, &USART_ClockInitStructure);
		USART_Init(USART1, &USART_InitStructure);
		USART_Cmd(USART1, ENABLE);
		//interrupt 
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		//receive interrupt
		
	//USART3	
		USART_ClockInit(USART3, &USART_ClockInitStructure);
		USART_Init(USART3, &USART_InitStructure);
		
		//interrupt
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		//receive interrupt	
		USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);
		//USART_DMACmd(USART1,USART_DMAReq_Rx,DISABLE);
		USART_Cmd(USART1,ENABLE);
	/*
	//comport2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	// A2 做T2X
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // A3 做R2X 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	
	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;
	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;
	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;
	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;

    USART_ClockInit(USART2, &USART_ClockInitStructure);
    USART_Init(USART2, &USART_InitStructure);
    
    USART_Cmd(USART2, ENABLE);
	//串口2使用接收中断
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	*/
}
//DMA中断处理
void DMA1_Channel4_IRQHandler(void)
{
	DMA1->IFCR |= 0X7000;	  //清除标志位
	DMA1_Channel4->CCR &= 0XFFFE;  //失能dma
	
}
void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
void USART1_PutsN (char * buff, u16 len)
{
    int i;
    for(i = 0; i < len; i++)
    {
        USART1_Putc(buff[i]);
    }
}


void USART2_Putc(unsigned char c)
{
    USART_SendData(USART2, c);
    while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET );
}

void USART3_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART3, *str++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
    }

}
