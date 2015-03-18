
#include "isr.h"

/*****************
Signal capture and output control
******************/
char remote_control_flag;
int adjust_center=0;
#define counter_reload 5000
/*******************************************************************************
* Function Name  : EXTI2_IRQHandler
* Description    : This function handles External interrupt Line 2 request.
* Input          : None
* Output         : None
* Return         : MD input
*******************************************************************************/
int ddd=0;
void EXTI2_IRQHandler(void)
{
		 	u16 temp;
			static u16 PE2_Counter;
			static u16 PE2_last_count;
	if(EXTI_GetITStatus(EXTI_Line2) != RESET)
   {
		 	EXTI_ClearITPendingBit(EXTI_Line2);
			temp=TIM_GetCounter(TIM2);
			//Delay(1);
		 
	     if(!(GPIOE->IDR & GPIO_Pin_2))
			{ // PE2=0   falling	
				  PE2_Counter=temp;
					PE2_Counter=(PE2_Counter-PE2_last_count)>0?
					(PE2_Counter-PE2_last_count):(PE2_Counter-PE2_last_count+counter_reload);
						 //begin to control
						if(remote_control_flag == 1)
						{
							ddd = PE2_Counter - adjust_center;
							if((ddd < Max_duty) && (ddd > Min_duty))
								Set_DJ(ddd);
						}

		   }
			 else{	//Rising
					PE2_last_count=temp; 
			 }
   }
	 
}

/*******************************************************************************
* Function Name  : EXTI3_IRQHandler
* Description    : This function handles External interrupt Line 3 request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI3_IRQHandler(void)
{
	
			 	u16 temp;
			static u16 PE3_Counter;
			static u16 PE3_last_count;
			static	u16 C_tmp;
	if(EXTI_GetITStatus(EXTI_Line3) != RESET)
   {
		 	EXTI_ClearITPendingBit(EXTI_Line3);
			temp=TIM_GetCounter(TIM2);
			Delay(1);
		 
	     if(!(GPIOE->IDR & GPIO_Pin_3))
			{ // PE3=0   falling	
				  PE3_Counter=temp;
		
					PE3_Counter=(PE3_Counter-PE3_last_count)>0?
					(PE3_Counter-PE3_last_count):(PE3_Counter-PE3_last_count+counter_reload);
							//begin to control
				 //need not to check the remote flg
						if(PE3_Counter<Max_duty&&PE3_Counter>Min_duty&&remote_control_flag)
						{  //remote_control_flag&&
							C_tmp= PE3_Counter>Protect_duty?Protect_duty:PE3_Counter;
							Set_MD(C_tmp);
						}
// 						else
// 							Set_MD(Stop_duty);
		   }
			else
			{	//Rising
					PE3_last_count=temp; 
			}
   }
}


/*******************************************************************************
* Function Name  : EXTI9_5_IRQHandler
* Description    : This function handles External lines 9 to 5 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI9_5_IRQHandler(void)
{
	u16 temp;
	static int16_t PB6_Counter;
	static u16 PB6_last_count;
	static u16 PE5_Counter;
	static u16 PE5_last_count;
	if(EXTI_GetITStatus(EXTI_Line6) != RESET)
   {
		 	EXTI_ClearITPendingBit(EXTI_Line6);
			temp=TIM_GetCounter(TIM2);
			Delay(1);
		 
	     if(!(GPIOB->IDR & GPIO_Pin_6)){ // PB6=0   falling	
				 PB6_Counter=temp;
				
					PB6_Counter=(PB6_Counter-PB6_last_count)>0?
					(PB6_Counter-PB6_last_count):(PB6_Counter-PB6_last_count+counter_reload);
					  //2 choice    near 1W   near 2W
				 if(PB6_Counter<1300&&PB6_Counter>800){
						//	USART1_Putc('1');
					 remote_control_flag=0;
					 }
				 else if(PB6_Counter>1400&&PB6_Counter<2200){
						//	USART1_Putc('2');
					 remote_control_flag=1;
					 }		 
					  //else
						//USART1_Putc('3');
		   }
			 else{	//Rising
				PB6_last_count=temp; 
			
			 }
   }	
	 
	 if(EXTI_GetITStatus(EXTI_Line5) != RESET)
   {//×ªÏòÎ¢µ÷
 		 	EXTI_ClearITPendingBit(EXTI_Line5);
// 			//AFIO->EXTICR[1]    //correspond to EXTI4567  low 4bit determin
// 		 // which port to be selected   0000 0000 0101 0000 then PE5
// 		 
// 			temp=TIM_GetCounter(TIM2);
// 			Delay(1);	 
// 	     if(!(GPIOE->IDR & GPIO_Pin_5)){ // PE5=0   falling	
// 				PE5_Counter=temp;		
// 				PE5_Counter=(PE5_Counter-PE5_last_count)>0?
// 				(PE5_Counter-PE5_last_count):(PE5_Counter-PE5_last_count+counter_reload);
// 				//if(remote_control_flag)
// 				 adjust_center=PE5_Counter-Center_duty;
// 		   }
// 			 else{	//Rising
// 				PE5_last_count=temp; 		
// 			 }
   }	
}
