
/*******************************************************************************
*  �������ܣ�HMC5883L + ADXL345 + ITG3205

*******************************************************************************/

#include "imu.h"

//��������
// ����ȫ����printf    �ض��嵽���ǵ�����Ϳ�����
void USART1_SendData(uchar SendData)  
{
  uart_putchar (UART1, SendData);
}

void USART_SendData(uchar SendData)  
{
  uart_putchar (UART1, SendData);
}

  /*
********************************************************************************
IMU���ݹ�����
********************************************************************************
*/
_RECIEVE_DATA IMU_Inital(void)
{ 
  SDA_OUT;
  SCL_OUT;
  _RECIEVE_DATA tmp;
  /*
  Init_HMC5883L();
   read_HMC5883l();
 Send_HMC5883L();  

  Init_HMC1022();
  read_HMC1022();
  Send_HMC1022();
  
  */
  //accelerator
  Init_ADXL345();
  tmp.gravity=read_ADXL345();
 //Send_ADXL345();
 //angle speed
  Init_ITG3205(); 
  tmp.angle_speed=read_ITG3205();	   
 // Send_ITG3205();
  
  

  return tmp;
}

/*************����***************/
