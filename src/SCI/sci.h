#ifndef SCI_H
#define SCI_H

#include "..\STM32Lib\\stm32f10x.h"
#include "..\hal.h"

typedef struct 
{
  short Kp;
  short Ki;
  short Kd;
  short steer;
  short velocity;
}_ONE_RECEIVE;

#define PACKET_START (char)0xf8
#define PACKET_END   (char)0x8f
#define SINGLE_CONTROL 3
#define BOTH_CONTROL 4
#define PID_CONTROL 8
#define Steer_center 1500		//center 1500ms
#define speed_zero  1500		//stop duty 

extern int depacket(char rxbyte);
extern _ONE_RECEIVE _COM_DATA;

#endif

