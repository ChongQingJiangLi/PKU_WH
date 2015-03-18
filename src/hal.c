/***************************************************
**HAL.c

***************************************************/

#include "STM32Lib\\stm32f10x.h"
#include "imu.h"
#include "can.h"
#include "buz.h"
#include "motor.h"
//function for each mode
extern void GPIO_Configuration(void);			//GPIO
extern void RCC_Configuration(void);			//RCC
extern void TIM1_8_Configuration(void);			//TIM1,8
extern void TIM2_Configuration(void);     //TIM2
extern void TIM3_Configuration(void);     //TIM3
extern void TIM4_Configuration(void);     //TIM4
extern void USART_Configuration(void);		//COM
extern void NVIC_Configuration(void);			//NVIC
extern void IWDG_Configuration(void);			//IWDG
 /*
********************************************************************************
** 函数名称 ： WWDG_Configuration(void)
** 函数功能 ： 看门狗初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void WWDG_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
  WWDG_SetPrescaler(WWDG_Prescaler_8);	              //  WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  
  WWDG_SetWindowValue(0x41);		                 // Set Window value to 0x41
  WWDG_Enable(0x50);		       // Enable WWDG and set counter value to 0x7F, WWDG timeout = ~4 ms * 64 = 262 ms 
  WWDG_ClearFlag();			       // Clear EWI flag
  WWDG_EnableIT();			       // Enable EW interrupt
}
 /*
********************************************************************************
** 函数名称 ： IWDG_Configuration(void)
** 函数功能 ： 独立看门狗初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void IWDG_Configuration(void)
{
	/* 写入0x5555,用于允许狗狗寄存器写入功能 */
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	
	/* 狗狗时钟分频,40K/16=2500HZ(0.4ms)*/
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	
	/* 喂狗时间 200ms/0.4ms=500 .注意不能大于0xfff*/
	IWDG_SetReload(500);
	
	/* 喂狗*/
	IWDG_ReloadCounter();
	
	/* 使能狗狗*/
	//IWDG_Enable();
}
/*******************************
**ChipHalInit()
**In_chip  configure
*******************************/
void  ChipHalInit(void)
{
	//source of clock
	RCC_Configuration();
	
	//GPIO
	GPIO_Configuration();
	
	//interrupt
	NVIC_Configuration();
	
	//uart
	USART_Configuration();	
	

}



/*********************************
**ChipOutHalInit()
**out_chip intial
*********************************/
void  ChipOutHalInit(void)
{
	Init_MPU6050();
	buz_init();
	beep(3);
	//timer
	//TIM1_8_Configuration();
	TIM2_Configuration();
	TIM3_Configuration();
	TIM4_Configuration();
	
	//can1
	CAN_configuration();
	
	Motor_Reset(0x01);
	Motor_Init(0x01, PWM_MODE);
	Motor_Reset(0x02);
	Motor_Init(0x02, PWM_MODE);
	//watch dog
	IWDG_Configuration();
	//WWDG_Configuration();	
}
