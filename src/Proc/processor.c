/**************************
**************************/
#include "..\STM32Lib\\stm32f10x.h"
#include "..\hal.h"
#include "imu.h"
#include "..\sci\\sci.h"
#include "..\can.h"
#include "..\motor.h"
/********************
Increase PID Control algorithm
Simple 
********************/
#define period 0.01    //10ms
static int last_err;
extern char buf[12]; //defined in USART.c
int  PID_Control(int Kp, int Ki, int Kd, int Aim_speed, int temp_speed)
{
    int err,err_d,delta;
    float dt;
    err=(Aim_speed-temp_speed+last_err)/2;    
    err_d=err-last_err;   
    last_err=err;
    
    dt=0.01;
    delta=Kp*err+0.1*Kd*err_d/dt;  
    delta*=0.01;     //time...  

    return delta;
}




#define Pack_header (char)0xa2
#define Pack_tail   (char)0x2a

void send_status(short gyroX, short gyroY, short gyroZ, unsigned short l_encoder, unsigned char r_encoder)
{
//	static unsigned char buf[6] = {0xa2};
//	*(short *)&buf[1] = angle;
//	*(unsigned short *)&buf[3] = count;
//	*(char *)&buf[5] = (char)0x2a;
	//static unsigned char buf[12] = {0xa2};
	buf[0] = 0xa2;
	*(short *)&buf[1] = gyroX;
	*(short *)&buf[3] = gyroY;
	*(short *)&buf[5] = gyroZ;
	*(unsigned short *)&buf[7] = l_encoder;
	*(unsigned short *)&buf[9] = r_encoder;
	buf[11] = 0x2a;
	//USART1_PutsN (buf, sizeof(buf));
	DMA1_Channel4->CNDTR = 12;
	DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

/*******************************************************************************
* Function Name  : TIM2_IRQHandler TIM2ÖÐ¶Ï
* Description    : This function handles TIM2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define MAXSPEED_OFFSET			200  //MD SPEED
#define TIM3_flow_period    30000
#define MAX_DEVIA   25000

int duty=Center_duty;
static int reverse_flag;
int led_flag = 1;
void TIM2_IRQHandler(void)
{	
	static u16 encoder_left_count, encoder_right_count;
	static u16 last_count;
	static short gyroX, gyroY, gyroZ;
	static int PID_err;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
			
		//re_data=IMU_Inital();
      //get the encoder   forward backward reserve
    encoder_left_count = TIM_GetCounter(TIM3);
		encoder_right_count = TIM_GetCounter(TIM4);
      //send the data
		gyroZ = (short)getGyroAngleZ();
// 		gyroX = (short)getGyroAngleX();
// 		gyroY = (short)getGyroAngleY();
		
		//CAN_Transmit_data(0x123, 23.6);
		Motor_PWM_Set(0x02, 20);
		Motor_PWM_Set(0x01, 50);
    //send_status(re_data.angle_speed, encoder_count);
		send_status(gyroX, gyroY, gyroZ, encoder_left_count, encoder_right_count);
		
		if(led_flag == 1)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_15);
			//GPIO_SetBits(GPIOE, GPIO_Pin_4);
			led_flag = 0;
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_15);
			//GPIO_ResetBits(GPIOE, GPIO_Pin_4);
			led_flag = 1;
		}
	}
}

