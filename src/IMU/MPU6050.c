
/*******************************************************************************
// GY-52 MPU3050 IIC测试程序
// 使用单片机STM32F103C8T6
// 晶振：8.00M
// 编译环境 Keil uVision4

// 与模块连接 GPIOE12->SCL GPIOE13->SDA      
// 使用：STM32F103VCT6串口1连接电脑
// 电脑串口助手显示，波特率 38400
// QQ：531389319
*******************************************************************************/

#include "imu.h" 

#define 		abs(num)  					(((num)>0)?(num):-(num))

#define GYRO_RATION  0.060976//0.064753//0.060976 = 1/16.4,
#define GYRO_RATIOP  0.060976//0.061890
#define INT_SCALE			0.04
#define SIMPLE_TIME 0.04

#define MAX_DataNum 450

unsigned char TX_DATA[4];  	 //显示据缓存区

short T_X,T_Y,T_Z,T_T;		 //X,Y,Z轴，温度
float Gyro_X;             //陀螺仪的角速度

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
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30; 	
}


//初始化MPU6050，根据需要请参考pdf进行修改************************
void Init_MPU6050(void)
{
  int i = 1000;
  unsigned char ret[6] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; //测试用
  
  while(i--)
  {
    Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
    Single_Write(MPU6050_Addr,CONFIG, 0x03);
    Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
    Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);
    Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x03);
    //调试下检查通信是否正常
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
  
//  kal_flt_init(&kal_filter);  //初始化滤波器
  MPU6050_Calibration(); //第一次校正
  
  init_flag = 1;  //MPU6050初始化完成,可以在中断里处理数据了
}
/*
***配置完寄存器后第一次校正零点
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
* 更新陀螺仪的零点
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
*** 读取原始数据,滤波处理后再积分
*/
short input = 0;
float getGyroAngleX()
{  
  float GyroRate = 0;
  float filtedRate = 0;
  input = GetData(GYRO_XOUT_H);
  
//  GyroRate = kalman_filter(&kal_filter, input); //kalman滤波
  GyroRate = input;
  if(abs(GyroRate-Gyro_Offset_X)>5) //小车发生了旋转运动
  {
    filtedRate = (GyroRate-Gyro_Offset_X)*SIMPLE_TIME;
    if(filtedRate > 0)
      Gyro_Angle_X = Gyro_Angle_X + filtedRate*GYRO_RATIOP;
    else
      Gyro_Angle_X = Gyro_Angle_X + filtedRate*GYRO_RATION;
  }
  else //小车没有旋转就更新零点
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
* 更新陀螺仪y轴的零点
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
*** 读取原始数据,滤波处理后再积分
*/
float getGyroAngleY()
{ 
  float GyroRate = 0;
  float filtedRate = 0;
  input = GetData(GYRO_YOUT_H);
  
//  GyroRate = kalman_filter(&kal_filter, input); //kalman滤波
  GyroRate = input;
  if(abs(GyroRate-Gyro_Offset_Y)>5) //小车发生了旋转运动
  {
    filtedRate = (GyroRate-Gyro_Offset_Y)*SIMPLE_TIME;
    if(filtedRate > 0)
      Gyro_Angle_Y = Gyro_Angle_Y + filtedRate*GYRO_RATIOP;
    else
      Gyro_Angle_Y = Gyro_Angle_Y + filtedRate*GYRO_RATION;
  }
  else //小车没有旋转就更新零点
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
* 更新陀螺仪z轴的零点
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
*** 读取原始数据,滤波处理后再积分
*/
float getGyroAngleZ()
{ 
  float GyroRate = 0;
  float filtedRate = 0;
  input = GetData(GYRO_ZOUT_H);
  
//  GyroRate = kalman_filter(&kal_filter, input); //kalman滤波
  GyroRate = input;
  if(abs(GyroRate-Gyro_Offset_Z)>5) //小车发生了旋转运动
  {
    filtedRate = (GyroRate-Gyro_Offset_Z)*SIMPLE_TIME;
    if(filtedRate > 0)
      Gyro_Angle_Z = Gyro_Angle_Z + filtedRate*GYRO_RATIOP;
    else
      Gyro_Angle_Z = Gyro_Angle_Z + filtedRate*GYRO_RATION;
  }
  else //小车没有旋转就更新零点
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
*** 读取陀螺仪原始数据
*/
int GetData(uchar REG_Address)
{
	uchar H,L;
	H = Single_Read(MPU6050_Addr,REG_Address);
	L = Single_Read(MPU6050_Addr,REG_Address+1);
	return ((H<<8)|L);   //合成数据
}

	
//******读取MPU6050数 测试用
void READ_MPU6050(void)
{									
	//accelerator
//  A_X=	GetData(ACCEL_XOUT_H); 						   //读取计算X轴数据
//  A_Y=	GetData(ACCEL_YOUT_H);
// 	A_Z=	GetData(ACCEL_ZOUT_H);; 
	
// 	T_T=	GetData(TEMP_OUT_H);
// T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
// T_T/=64; 	  
	//gyro
   T_X=	GetData(GYRO_XOUT_H);
   T_Y=	GetData(GYRO_YOUT_H);
	 T_Z=	GetData(GYRO_ZOUT_H);  
//  return T_Z;
   Gyro_X = T_Z/16.4;
}
//测试用
void IMU_Read()
 {
// 	READ_MPU6050();
//   DATA_printf(TX_DATA,T_X);//转换X轴数据到数组
// 	USART1_Puts("TX=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,T_Y);//转换Y轴数据到数组
// 	USART1_Puts("  TY=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,T_Z);//转换Z轴数据到数组
// 	USART1_Puts("  TZ=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,T_T);//转换温度数据到数组
// 	USART1_Puts("  TT=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	USART1_Putc(0x0d);
// 	USART1_Putc(0X0A);	 //回车
// 	 
// 	DATA_printf(TX_DATA,A_X);//转换X轴数据到数组
// 	USART1_Puts("AX=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,A_Y);//转换Y轴数据到数组
// 	USART1_Puts("  AY=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	DATA_printf(TX_DATA,A_Z);//转换Z轴数据到数组
// 	USART1_Puts("  AZ=");
// 	USART1_PutsN (TX_DATA, sizeof(TX_DATA)); 
// 	USART1_Putc(0x0d);
// 	USART1_Putc(0X0A);	 //回车 
 }

/*************结束***************/
