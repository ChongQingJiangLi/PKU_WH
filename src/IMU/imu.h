#ifndef imu_h
#define imu_h

#include "..\STM32Lib\\stm32f10x.h"
#include "..\hal.h"
#include "..\sci\\sci.h"
#include  <math.h>    //Keil library 


#define   uchar unsigned char
#define   uint unsigned int	
#define   u32  float
#define   bool char

//********
/*ģ��IIC�˿�������붨��*/

/****************************************
 MPU6050
****************************************/

#define	SMPLRT_DIV		0x19	//�����ǲ�����(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲���Ƶ��(5Hz)
#define	GYRO_CONFIG		0x1B	//�Լ췶Χ  0x18   (2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ٶ��Լ�0x01(5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42
#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ����(����)
#define	WHO_AM_I			0x75	//IIC��ַ�Ĵ����� 0x68 
//#define	SlaveAddress	0xD0	//IICд���ַ��  +1Ϊ��ȡ����
#define	MPU6050_Addr   0xD0	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�

//************************************/
/*ģ��IIC�˿�������붨��*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_6
#define SCL_L         GPIOB->BRR  = GPIO_Pin_6
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_7
#define SDA_L         GPIOB->BRR  = GPIO_Pin_7

#define SCL_read      GPIOB->IDR  & GPIO_Pin_6
#define SDA_read      GPIOB->IDR  & GPIO_Pin_7

//���ֺ�������

//void Delayms(u32 m);  



void Delayms(vu32 m);  


extern void IMU_Read(void);
void READ_MPU6050(void);
void Init_MPU6050(void);
int GetData(uchar REG_Address);
void MPU6050_Calibration(void);
float getGyroRate(short gyro);
float getGyroAngleX(void);
float getGyroAngleY(void);
float getGyroAngleZ(void);

bool Single_Write(unsigned char SlaveAddress, unsigned char REG_Address, unsigned char REG_data);
unsigned char Single_Read(unsigned char SlaveAddress,unsigned char REG_Address);
//only for proper use
//the fact is that theses things does not really matter

#endif

