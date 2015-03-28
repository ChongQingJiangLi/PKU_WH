#include "STM32Lib\\stm32f10x.h"
#include "hal.h"
/*********************************
Configure Timer 1 and 8 as output of the 
DJ&MD control,  frequence 50hz,
duty 5%-10%    middle 7.5%   1.5ms
*********************************/
void TIM1_8_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	TIM_DeInit(TIM1);	TIM_DeInit(TIM8);
	/*TIM1*/
	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;						//prescaler 72M/72=1M
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//up count
	TIM_TimeBaseStructure.TIM_Period = 10000;							//reload 1M/20K=50hz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);

	/* Channel 1 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//PWM mode 2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//正向通道有效
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//反向通道无效
	TIM_OCInitStructure.TIM_Pulse = Center_duty; 							//占空时间
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//输出极性
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     //互补端的极性  
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;  
	
	TIM_OC1Init(TIM1,&TIM_OCInitStructure); 			//CH1
	//TIM_OCInitStructure.TIM_Pulse = Center_duty 	
	TIM_OC1Init(TIM8,&TIM_OCInitStructure); 			//CH1
	/* TIM1 counter enable */
	TIM_Cmd(TIM1,ENABLE);	TIM_Cmd(TIM8,ENABLE);	
	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1,ENABLE);TIM_CtrlPWMOutputs(TIM8,ENABLE);
}



/**************************
TIMER2  as the clock for signal capture
**************************/
void TIM2_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	
	/* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Prescaler = 72-1;    	//72M/72=1M
	TIM_TimeBaseStructure.TIM_Period = 40000;			//40ms  overflow
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	
	/*load */
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	//clear IT bit
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update); 
	/* overflow interrupt*/
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	/* allow*/
	TIM_Cmd(TIM2, ENABLE);
}

/**************************
TIMER3  as the encoder accumulator
**************************/
void TIM3_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM3);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 30000;	//6w RELOAD  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
  TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
   TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  //TIM_ITConfig(ENCODER_TIMER, TIM_IT_Update, ENABLE);
	 TIM_Cmd(TIM3, ENABLE); 
}

/**************************
TIMER4  as the encoder accumulator
**************************/
void TIM4_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  /* Timer configuration in Encoder mode */
  TIM_DeInit(TIM4);
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
  
  TIM_TimeBaseStructure.TIM_Prescaler = 0x0;  // No prescaling 
  TIM_TimeBaseStructure.TIM_Period = 30000;	//6w RELOAD  
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
 
  TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, 
                             TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_ICStructInit(&TIM_ICInitStructure);
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
 // Clear all pending interrupts
   TIM_ClearFlag(TIM4, TIM_FLAG_Update);
  //TIM_ITConfig(ENCODER_TIMER, TIM_IT_Update, ENABLE);
	 TIM_Cmd(TIM4, ENABLE); 
}



