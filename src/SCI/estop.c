#include "sci.h"
char ESTOP_flag=0;

static void print_to_buf_estop(unsigned char *buff, unsigned char size)
{
  if(size==SINGLE_CONTROL){
     char flag=*(char *)&buff[0];
     if(flag==0x01){
       _COM_DATA.steer=*(short *)&buff[1]+Steer_center;//center DJ
       FTM_PWM_Duty(FTM1, CH0, _COM_DATA.steer); 
     }
     else{ 
       _COM_DATA.velocity=*(short *)&buff[1];
         ESTOP_flag=0;
     }
  }
  
  else if(size==BOTH_CONTROL){
  //here to estop
          ESTOP_flag=0;
        FTM_PWM_Duty(FTM0, CH0, 0); 
        FTM_PWM_Duty(FTM0, CH1, 0); 
  }
    else if(size==PID_CONTROL){
         ESTOP_flag=1;
        FTM_PWM_Duty(FTM0, CH0, 0); 
        FTM_PWM_Duty(FTM0, CH1, 0); 
        delayms(100);
        FTM_PWM_Duty(FTM0, CH0, 0); 
        FTM_PWM_Duty(FTM0, CH1, 300);
        delayms(500);
        FTM_PWM_Duty(FTM0, CH0, 0);
        FTM_PWM_Duty(FTM0, CH1, 0);
  }
  else
    printf("error in receive data");
  
}


int depacket_ESTOP(char rxbyte)
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
                  print_to_buf_estop(buf, size);
                 // UART1_PFIFO|=0x40;//clear receive FIFO
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
                printf("wrong receive data");
                return 0;
                break;
	}	
        return 1;
}
