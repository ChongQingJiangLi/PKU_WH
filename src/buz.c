#include "..\STM32Lib\\stm32f10x.h"
#include "buz.h"
#include "delay.h"

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

void beep(unsigned int num)
{
	int i;
	for(i=0; i<num; i++)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_8);
		delay_ms(150);
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);
		delay_ms(150);		
	}

}