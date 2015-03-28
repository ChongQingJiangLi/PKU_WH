
/*******************************************************************************
// GY-52 MPU3050 IIC���Գ���
// ʹ�õ�Ƭ��STM32F103C8T6
// ����8.00M
// ���뻷�� Keil uVision4

// ��ģ������ GPIOE12->SCL GPIOE13->SDA      
// ʹ�ã�STM32F103VCT6����1���ӵ���
// ���Դ���������ʾ�������� 38400
// QQ��531389319
*******************************************************************************/

#include "imu.h" 

#define 		abs(num)  					(((num)>0)?(num):-(num))

#define GYRO_RATION  0.060976//0.064753//0.060976 = 1/16.4,
#define GYRO_RATIOP  0.060976//0.061890
#define INT_SCALE			0.04
#define SIMPLE_TIME 0.04

#define MAX_DataNum 450

unsigned char TX_DATA[4];  	 //��ʾ�ݻ�����

short T_X,T_Y,T_Z,T_T;		 //X,Y,Z�ᣬ�¶�
float Gyro_X;             //�����ǵĽ��ٶ�

float Gyro_Angle_X,Gyro_Angle_Y,Gyro_Angle_Z;

char init_flag = 0;



float OffsetData_X[MAX_DataNum];
float OffsetData_Y[MAX_DataNum];
float OffsetData_Z[MAX_DataNum];

float Gyro_Offset_X;
float Gyro_Offset_Y;
float Gyro_Offset_Z;

double  sum_data_x = 0, sum_data_y=0, sum_data_z=0;

int cur_index = 0, cur_index_x=0, cur_index_y=0, cur_index_z=0;

  /*******************************/
void DATA_printf(uchar *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //ȡ������
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //ȡ������
    *++s =temp_data+0x30; 	
}


//��ʼ��MPU6050��������Ҫ��ο�pdf�����޸�************************
void Init_MPU6050(void)
{
  int i = 1000;
  unsigned char ret[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; //������
  
  while(i--)
  {
    Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
    Single_Write(MPU6050_Addr,CONFIG, 0x03);
    Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
    Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);
    Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x03);
    //�����¼��ͨ���Ƿ�����
    ret[0] = Single_Read(MPU6050_Addr,SMPLRT_DIV);
    ret[1] = Single_Read(MPU6050_Addr,CONFIG);
    ret[2] = Single_Read(MPU6050_Addr,GYRO_CONFIG);
    ret[3] = Single_Read(MPU6050_Addr,ACCEL_CONFIG); 
    ret[4] = Single_Read(MPU6050_Addr,PWR_MGMT_1); 
    if(ret[0]==0x07 &&ret[1]==0x03 
      &&ret[2]==0x18 &&ret[3]==0x01 &&ret[4]==0x03)
    {
      break;
    }
  }
  
//  kal_flt_init(&kal_filter);  //��ʼ���˲���
  MPU6050_Calibration(); //��һ��У��
  
  init_flag = 1;  //MPU6050��ʼ�����,�������ж��ﴦ��������
}
/*
***������Ĵ������һ��У�����
*/
void MPU6050_Calibration(void)
{
  int times = 0;
  short data_x, data_y, data_z;
  for(times=0; times<MAX_DataNum; times++)
  {
      data_z = GetData(GYRO_ZOUT_H);
			data_x = GetData(GYRO_XOUT_H);
			data_y = GetData(GYRO_YOUT_H);
 
      OffsetData_X[cur_index] = data_x;
			OffsetData_Y[cur_index] = data_y;
			OffsetData_Z[cur_index] = data_z;
      cur_index++;
    
      sum_data_x += data_x;
			sum_data_y += data_y;
			sum_data_z += data_z;
      Delay(20);
  }
	
	cur_index_x = cur_index_y = cur_index_z = cur_index;
	
	Gyro_Offset_X = sum_data_x/MAX_DataNum;
	Gyro_Offset_Y = sum_data_y/MAX_DataNum;
	Gyro_Offset_Z = sum_data_z/MAX_DataNum;

}
/*
* ���������ǵ����
*/
float updateOffset_x(float input)
{
    float Gyro_Offset;
    if(cur_index_x < MAX_DataNum)
    {
      sum_data_x -= OffsetData_X[cur_index_x];
      OffsetData_X[cur_index_x] = input;
      sum_data_x += OffsetData_X[cur_index_x];
      cur_index_x++;
      Gyro_Offset = sum_data_x/MAX_DataNum;
    }
    else 
    {
      cur_index_x = 0;
      sum_data_x -= OffsetData_X[cur_index_x];
      OffsetData_X[cur_index] = input;
      sum_data_x += OffsetData_X[cur_index_x];
      cur_index_x++;  
      Gyro_Offset = sum_data_x/MAX_DataNum;      
    }
    return Gyro_Offset;
}

/*
*** ��ȡԭʼ����,�˲�������ٻ���
*/
short input = 0;
float getGyroAngleX()
{  
  float GyroRate = 0;
  float filtedRate = 0;
  input = GetData(GYRO_XOUT_H);
  
//  GyroRate = kalman_filter(&kal_filter, input); //kalman�˲�
  GyroRate = input;
  if(abs(GyroRate-Gyro_Offset_X)>5) //С����������ת�˶�
  {
    filtedRate = (GyroRate-Gyro_Offset_X)*SIMPLE_TIME;
    if(filtedRate > 0)
      Gyro_Angle_X = Gyro_Angle_X + filtedRate*GYRO_RATIOP;
    else
      Gyro_Angle_X = Gyro_Angle_X + filtedRate*GYRO_RATION;
  }
  else //С��û����ת�͸������
  {
     Gyro_Offset_X = updateOffset_x(GyroRate);
  }
   if(Gyro_Angle_X>360)
  	  Gyro_Angle_X-=360;
   else if(Gyro_Angle_X<0)
	  Gyro_Angle_X+=360;

	  return Gyro_Angle_X*10;
	
}

/*
* ����������y������
*/
float updateOffset_y(float input)
{
    float Gyro_Offset;
    if(cur_index_y < MAX_DataNum)
    {
      sum_data_y -= OffsetData_Y[cur_index_y];
      OffsetData_Y[cur_index_y] = input;
      sum_data_y += OffsetData_Y[cur_index_y];
      cur_index_y++;
      Gyro_Offset = sum_data_y/MAX_DataNum;
    }
    else 
    {
      cur_index_y = 0;
      sum_data_y -= OffsetData_Y[cur_index_y];
      OffsetData_Y[cur_index_y] = input;
      sum_data_y += OffsetData_Y[cur_index_y];
      cur_index_y++;  
      Gyro_Offset = sum_data_y/MAX_DataNum;      
    }
    return Gyro_Offset;
}

/*
*** ��ȡԭʼ����,�˲�������ٻ���
*/
float getGyroAngleY()
{ 
  float GyroRate = 0;
  float filtedRate = 0;
  input = GetData(GYRO_YOUT_H);
  
//  GyroRate = kalman_filter(&kal_filter, input); //kalman�˲�
  GyroRate = input;
  if(abs(GyroRate-Gyro_Offset_Y)>5) //С����������ת�˶�
  {
    filtedRate = (GyroRate-Gyro_Offset_Y)*SIMPLE_TIME;
    if(filtedRate > 0)
      Gyro_Angle_Y = Gyro_Angle_Y + filtedRate*GYRO_RATIOP;
    else
      Gyro_Angle_Y = Gyro_Angle_Y + filtedRate*GYRO_RATION;
  }
  else //С��û����ת�͸������
  {
     Gyro_Offset_Y = updateOffset_y(GyroRate);
  }
   if(Gyro_Angle_Y>360)
  	  Gyro_Angle_Y-=360;
   else if(Gyro_Angle_Y<0)
	  Gyro_Angle_Y+=360;

	  return Gyro_Angle_Y*10;
	
}
/*
* ����������z������
*/
float updateOffset_z(float input)
{
    float Gyro_Offset;
    if(cur_index_z < MAX_DataNum)
    {
      sum_data_z -= OffsetData_Z[cur_index_z];
      OffsetData_Z[cur_index_z] = input;
      sum_data_z += OffsetData_Z[cur_index_z];
      cur_index_z++;
      Gyro_Offset = sum_data_z/MAX_DataNum;
    }
    else 
    {
      cur_index_z = 0;
      sum_data_z -= OffsetData_Z[cur_index_z];
      OffsetData_Z[cur_index_z] = input;
      sum_data_z += OffsetData_Z[cur_index_z];
      cur_index_z++;  
      Gyro_Offset = sum_data_z/MAX_DataNum;      
    }
    return Gyro_Offset;
}

/*
*** ��ȡԭʼ����,�˲�������ٻ���
*/
float getGyroAngleZ()
{ 
  float GyroRate = 0;
  float filtedRate = 0;
  input = GetData(GYRO_ZOUT_H);
  
//  GyroRate = kalman_filter(&kal_filter, input); //kalman�˲�
  GyroRate = input;
  if(abs(GyroRate-Gyro_Offset_Z)>5) //С����������ת�˶�
  {
    filtedRate = (GyroRate-Gyro_Offset_Z)*SIMPLE_TIME;
    if(filtedRate > 0)
      Gyro_Angle_Z = Gyro_Angle_Z + filtedRate*GYRO_RATIOP;
    else
      Gyro_Angle_Z = Gyro_Angle_Z + filtedRate*GYRO_RATION;
  }
  else //С��û����ת�͸������
  {
     Gyro_Offset_Z = updateOffset_z(GyroRate);
  }
   if(Gyro_Angle_Z>360)
  	  Gyro_Angle_Z-=360;
   else if(Gyro_Angle_Z<0)
	  Gyro_Angle_Z+=360;

	  return Gyro_Angle_Z*10;
	
}
/*
*** ��ȡ������ԭʼ����
*/
int GetData(uchar REG_Address)
{
	uchar H,L;
	H = Single_Read(MPU6050_Addr,REG_Address);
	L = Single_Read(MPU6050_Addr,REG_Address+1);
	return ((H<<8)|L);   //�ϳ�����
}

	
//******��ȡMPU6050�� ������
void READ_MPU6050(void)
{									
	//accelerator
//  A_X=	GetData(ACCEL_XOUT_H); 						   //��ȡ����X������
//  A_Y=	GetData(ACCEL_YOUT_H);
// 	A_Z=	GetData(ACCEL_ZOUT_H);; 
	
// 	T_T=	GetData(TEMP_OUT_H);
// T_T = 35+ ((double) (T_T + 13200)) / 280;// ��ȡ������¶�
// T_T/=64; 	  
	//gyro
   T_X=	GetData(GYRO_XOUT_H);
   T_Y=	GetData(GYRO_YOUT_H);
	 T_Z=	GetData(GYRO_ZOUT_H);  
//  return T_Z;
   Gyro_X = T_Z/16.4;
}
//������
void IMU_Read()
 {
// 	READ_MPU6050();
//   DATA_printf(TX_DATA,T_X);//ת��X�����ݵ�����
// 	USART1_Puts("TX=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,T_Y);//ת��Y�����ݵ�����
// 	USART1_Puts("  TY=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,T_Z);//ת��Z�����ݵ�����
// 	USART1_Puts("  TZ=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,T_T);//ת���¶����ݵ�����
// 	USART1_Puts("  TT=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	USART1_Putc(0x0d);
// 	USART1_Putc(0X0A);	 //�س�
// 	 
// 	DATA_printf(TX_DATA,A_X);//ת��X�����ݵ�����
// 	USART1_Puts("AX=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,A_Y);//ת��Y�����ݵ�����
// 	USART1_Puts("  AY=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,A_Z);//ת��Z�����ݵ�����
// 	USART1_Puts("  AZ=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	USART1_Putc(0x0d);
// 	USART1_Putc(0X0A);	 //�س� 
 }

/*************����***************/
