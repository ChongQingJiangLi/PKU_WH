#include "STM32Lib\\stm32f10x.h"
#include "hal.h"
/*******************************************************************************
* Function Name  : GPIO_Configuration

*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	/*允许总线CLOCK,在使用GPIO之前必须允许相应端的时钟.
	从STM32的设计角度上说,没被允许的端将不接入时钟,也就不会耗能,
	这是STM32节能的一种技巧,*/
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	
//configure input...  mainly detect the signal come from receiver
	
	/* Configure PA.06,07 as encoder input */
	GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Configure PD.12,13 as encoder input */
	GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);
	
	/* PB6  input,  receive the ON/OFF signal from receiver */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* PE2,3,5 input,  receive the control signal from receiver 
	PE2 CH1 DJ  //	PE3 CH2 MD	//  PE5 CH6 adjust */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M
	GPIO_Init(GPIOE, &GPIO_InitStructure);

		//configure interrupt
		   /* PE2,3,5  and PB6 */
			 //collect correspond pin to interrupt	
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource2);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource5);
		GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource6);
		
		
    EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 |EXTI_Line5|EXTI_Line6;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


	
//configure output signal,  2 CH  PWM

	/*PA8,output   as DJ PWM*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //PP
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*PC6 ,output  as MD PWM*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* IIC   PB6 SCL   PB7 SDA*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	//继电器
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	//test
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}


