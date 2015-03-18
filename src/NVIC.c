#include "STM32Lib\\stm32f10x.h" 

void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* Configure one bit for preemption priority */
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);


	/* Timer2 interrupt*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;			
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
// 	/* PB6 PE5 interrupt  */
// 	 NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//highest priority
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// 	
// 		/* PE2 interrupt  */
// 	 NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;	
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// 	/* PE3 interrupt  */
// 	 NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;	
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;	
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
	
		/*UART1*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//lower priority
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	  /* Enable the TIM3 Update Interrupt 
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);
	*/
	
		/*  WWDG  configure*/ 
  //NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  //NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  //NVIC_Init(&NVIC_InitStructure);
	
}


