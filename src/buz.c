#include "..\STM32Lib\\stm32f10x.h"
#include "buz.h"
 

void buz_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		//test
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void delay_ms(unsigned int num)
{
//	int i;
//	for(i=200; i>0; i--)
	   while(num) 
   { 
     num--; 
   } 
}

void bb()
{
	GPIO_SetBits(GPIOB, GPIO_Pin_8);
	delay_ms(50000);
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);
	delay_ms(50000);
}

void beep(unsigned int num)
{
	int i=0;
	for(i=0; i<num; i--)
	{
		bb();
	}
}