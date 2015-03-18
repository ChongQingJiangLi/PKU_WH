#include"sci.h"
#include "..\motor.h"
_ONE_RECEIVE _COM_DATA={4,0,5,1500,1500};
short l_motor = 0;
short r_motor = 0;


//receive comport data and depacket

void static print_to_buf(unsigned char *buff, unsigned char size)
{
  if(size==SINGLE_CONTROL){
     char flag=*(char *)&buff[0];
     if(flag==0x01){
       _COM_DATA.steer=*(short *)&buff[1]+Steer_center;//center DJ
			 if(!remote_control_flag)
			 Set_DJ(_COM_DATA.steer-adjust_center);
     }
     else{
       _COM_DATA.velocity=*(short *)&buff[1]+speed_zero;
			 if(!remote_control_flag){
					_COM_DATA.velocity=_COM_DATA.velocity>Protect_duty?Protect_duty:_COM_DATA.velocity;
				 Set_MD(_COM_DATA.velocity);
			 }
			}
		}
  
  else if(size==BOTH_CONTROL){
//    _COM_DATA.steer=*(short *)&buff[0]+Steer_center;
//    _COM_DATA.velocity=*(short *)&buff[2]+speed_zero;
// 		if(!remote_control_flag)
// 		{Set_DJ(_COM_DATA.steer-adjust_center);
// 		 _COM_DATA.velocity=_COM_DATA.velocity>Protect_duty?Protect_duty:_COM_DATA.velocity;
// 		Set_MD(_COM_DATA.velocity);
// 		}  
		l_motor = *(short *)&buff[0];
		r_motor = *(short *)&buff[2];
		//Motor_Speed_Set(MOTOR_NUM1, l_motor);
		//Motor_Speed_Set(MOTOR_NUM2, r_motor);
	}
  
    else if(size==PID_CONTROL){
   _COM_DATA.Kp=*(short *)&buff[0];
   _COM_DATA.Ki=*(short *)&buff[2];
   _COM_DATA.Kd=*(short *)&buff[4];
   _COM_DATA.velocity=*(short *)&buff[6]+speed_zero;
  }
  else
    USART1_Puts("error in receive data");
  
}


int depacket(char rxbyte)
{
	static unsigned char buf[20], pos, size;
	static char state;

	switch (state) {
	case 0:
		if (rxbyte == PACKET_START)
			state = 1;
		break;
	case 1:
		size = rxbyte;
		state = size; //datalength
    pos=0;
		break;
  case 2:
		if(rxbyte == PACKET_END)
			print_to_buf(buf, size);
      state=0;
		break;
	case 3:
		buf[pos++] = rxbyte;  //01 +short or 02+short
		if (pos >= size)
			state = 2;
		break;
  case 4:
    buf[pos++] = rxbyte;  //01 +short or 02+short
		if (pos >= size)
			state =2;
    break;
  case 8:
    buf[pos++] = rxbyte;  //01 +short or 02+short
		if (pos >= size)
			state =2;
                break;
	default:
						state = 0;
            //USART1_Puts("wrong receive data");      
            return 0;
            break;
	}	
        return 1;
}


/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : F8 datasize data 8f
* Output         : None
* Return         : None
*******************************************************************************/

void USART1_IRQHandler(void)
{	
	char re_tmp;
		//receive interrupt
	if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET)
	{
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		re_tmp=USART_ReceiveData(USART1);
		depacket(re_tmp);
		//USART1_Putc(re_tmp);
		//IMU_Read();
	
	}
	
	//overflow-如果发生溢出需要先读SR,再读DR寄存器 则可清除不断入中断的问题
// 	if(USART_GetFlagStatus(USART2,USART_FLAG_ORE)==SET)
// 	{
// 		USART_ClearFlag(USART1,USART_FLAG_ORE);	//读SR
// 		USART_ReceiveData(USART1);				//读DR
// 	}
}

