#include "..\STM32Lib\\stm32f10x.h"
#include "motor.h"

unsigned char can_tx_success_flag=0;
void USB_HP_CAN1_TX_IRQHandler(void) //CAN TX
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
       can_tx_success_flag=1;
    }
}

/*************************************************************************
                          CAN1_RX0_IRQHandler
??úCAN1???????
*************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	{
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
    }
}


/*************************************************************************
                            Motor_Reset
??:??????????????
????: int Motor_ID     ????: MOTOR_NUM1   MOTOR_NUM2  
                                       MOTOR_NUM3   MOTOR_NUM4
                                       MOTOR_NUM5   MOTOR_NUM6
                                       MOTOR_NUM7   MOTOR_NUM8
*************************************************************************/
void Motor_Reset(int Motor_ID)
{
    CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    //???
    tx_message.RTR = CAN_RTR_DATA;  //???
    tx_message.DLC = 0x01;          //????1
    tx_message.StdId = 0x00;        //?ID?0x00

    switch (Motor_ID)
    {
        case MOTOR_NUM1: tx_message.Data[0] = ID_MOTOR1_CHOOSE_MODE; break;  //???????1: ??0?0x10
        case MOTOR_NUM2: tx_message.Data[0] = ID_MOTOR2_CHOOSE_MODE; break;  //???????2: ??0?0x20
        case MOTOR_NUM3: tx_message.Data[0] = ID_MOTOR3_CHOOSE_MODE; break;  //???????3: ??0?0x30
        case MOTOR_NUM4: tx_message.Data[0] = ID_MOTOR4_CHOOSE_MODE; break;  //???????4: ??0?0x40
        case MOTOR_NUM5: tx_message.Data[0] = ID_MOTOR5_CHOOSE_MODE; break;  //???????5: ??0?0x50
        case MOTOR_NUM6: tx_message.Data[0] = ID_MOTOR6_CHOOSE_MODE; break;  //???????6: ??0?0x60
        case MOTOR_NUM7: tx_message.Data[0] = ID_MOTOR7_CHOOSE_MODE; break;  //???????7: ??0?0x70
        case MOTOR_NUM8: tx_message.Data[0] = ID_MOTOR8_CHOOSE_MODE; break;  //???????8: ??0?0x80
        default: 
        {
            while(1)      //????????????8,??????
            {
                //delay_ms(200);
                #ifdef __DEBUG
                   // printf("ERROR Motor_Reset Motor_ID choise!\r\n");
                #endif
            }
        }
    }
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    //while(can_tx_success_flag == 0);
}

/*************************************************************************
                            Motor_Init
??:???????????????
????: int Motor_ID     ????: MOTOR_NUM1   MOTOR_NUM2  
                                       MOTOR_NUM3   MOTOR_NUM4
                                       MOTOR_NUM5   MOTOR_NUM6
                                       MOTOR_NUM7   MOTOR_NUM8
????: int Motor_Mode   ????: PWM_MODE 
                                       SPEED_MODE 
                                       PWM_LOCATION_MODE 
                                       SPEED_LOCATION_MODE
*************************************************************************/
void Motor_Init(int Motor_ID,int Motor_Mode)
{
    CanTxMsg tx_message;
    
    tx_message.RTR = CAN_RTR_DATA;  //???
    tx_message.IDE = CAN_ID_STD;    //???
    tx_message.DLC = 0x04;          //????4      
    
    tx_message.Data[0] = 0x59;
    tx_message.Data[3] = 0xae;
    
    switch (Motor_ID)
    {
        case MOTOR_NUM1: tx_message.StdId = ID_MOTOR1_CHOOSE_MODE; break; //??????1: ?ID?0x10
        case MOTOR_NUM2: tx_message.StdId = ID_MOTOR2_CHOOSE_MODE; break; //??????2: ?ID?0x20
        case MOTOR_NUM3: tx_message.StdId = ID_MOTOR3_CHOOSE_MODE; break; //??????3: ?ID?0x30
        case MOTOR_NUM4: tx_message.StdId = ID_MOTOR4_CHOOSE_MODE; break; //??????4: ?ID?0x40
        case MOTOR_NUM5: tx_message.StdId = ID_MOTOR5_CHOOSE_MODE; break; //??????5: ?ID?0x50
        case MOTOR_NUM6: tx_message.StdId = ID_MOTOR6_CHOOSE_MODE; break; //??????6: ?ID?0x60
        case MOTOR_NUM7: tx_message.StdId = ID_MOTOR7_CHOOSE_MODE; break; //??????7: ?ID?0x70
        case MOTOR_NUM8: tx_message.StdId = ID_MOTOR8_CHOOSE_MODE; break; //??????8: ?ID?0x80
        default: 
        {
            while(1)      //????????????8,??????
            {
                //delay_ms(200);
                #ifdef __DEBUG
                //    printf("ERROR Motor_Init Motor_ID choise!\r\n");
                #endif
            }
        }
    }
    
    switch (Motor_Mode)
    {
        case PWM_MODE:              tx_message.Data[1] = 0x56; //???????? PWM_MODE
                                    tx_message.Data[2] = 0xab; //??1?0x56,??2?0xab
                                    break;
        case SPEED_MODE:            tx_message.Data[1] = 0x55; //???????? SPEED_MODE
                                    tx_message.Data[2] = 0xaa; //??1?0x55,??2?0xaa
                                    break;
        case PWM_LOCATION_MODE:     tx_message.Data[1] = 0x58; //???????? PWM_LOCATION_MODE
                                    tx_message.Data[2] = 0xad; //??1?0x58,??2?0xad
                                    break;
        case SPEED_LOCATION_MODE:   tx_message.Data[1] = 0x57; //???????? SPEED_LOCATION_MODE
                                    tx_message.Data[2] = 0xac; //??1?0x57,??2?0xac
                                    break;
        default:
        {
            while(1)      //??????????????4,??????
            {
                //delay_ms(200);
                #ifdef __DEBUG
                    printf("ERROR Motor_Init Motor_Mode choise!\r\n");
                #endif
            }
        }
    }

    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    //while(can_tx_success_flag == 0);
}

/*************************************************************************
                           MOTOR_PWM_Set
??:PWM?????????
????: int Motor_ID     ????: MOTOR_NUM1   MOTOR_NUM2  
                                       MOTOR_NUM3   MOTOR_NUM4
                                       MOTOR_NUM5   MOTOR_NUM6
                                       MOTOR_NUM7   MOTOR_NUM8
????: int Give_PWM   ?? -5000 -- 0 -- 5000
*************************************************************************/
void Motor_PWM_Set(int Motor_ID,int Give_PWM)
{
    CanTxMsg tx_message;
    
    tx_message.RTR = CAN_RTR_DATA;  //???
    tx_message.IDE = CAN_ID_STD;    //???
    tx_message.DLC = 0x04;          //????4    
  
    switch (Motor_ID)
    {
        case MOTOR_NUM1: tx_message.StdId = ID_MOTOR1_PWM_MODE; break; //??????1,PWM????ID?0x11
        case MOTOR_NUM2: tx_message.StdId = ID_MOTOR2_PWM_MODE; break; //??????2,PWM????ID?0x21
        case MOTOR_NUM3: tx_message.StdId = ID_MOTOR3_PWM_MODE; break; //??????3,PWM????ID?0x31
        case MOTOR_NUM4: tx_message.StdId = ID_MOTOR4_PWM_MODE; break; //??????4,PWM????ID?0x41
        case MOTOR_NUM5: tx_message.StdId = ID_MOTOR5_PWM_MODE; break; //??????5,PWM????ID?0x51
        case MOTOR_NUM6: tx_message.StdId = ID_MOTOR6_PWM_MODE; break; //??????6,PWM????ID?0x61
        case MOTOR_NUM7: tx_message.StdId = ID_MOTOR7_PWM_MODE; break; //??????7,PWM????ID?0x71
        case MOTOR_NUM8: tx_message.StdId = ID_MOTOR8_PWM_MODE; break; //??????8,PWM????ID?0x81
        default: 
        {
            while(1)      //????????????8,??????
            {
                //delay_ms(200);
                #ifdef __DEBUG
                   // printf("ERROR Motor_PWM_Set Motor_ID choise!\r\n");
                #endif
            }
        }
    }
    
    if(Give_PWM >= 3500)
    {
        Give_PWM = 3500;
    }
    
    if(Give_PWM <= -3500)
    {
        Give_PWM = -3500;
    }
    
    tx_message.Data[0] = (unsigned char)((Give_PWM >> 24)&0xff);
    tx_message.Data[1] = (unsigned char)((Give_PWM >> 16)&0xff);
    tx_message.Data[2] = (unsigned char)((Give_PWM >> 8)&0xff);
    tx_message.Data[3] = (unsigned char)(Give_PWM&0xff);
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    //while(can_tx_success_flag == 0);
}

/*************************************************************************
                           MOTOR_Speed_Set
??:Speed?????????
????: int Motor_ID     ????: MOTOR_NUM1   MOTOR_NUM2  
                                       MOTOR_NUM3   MOTOR_NUM4
                                       MOTOR_NUM5   MOTOR_NUM6
                                       MOTOR_NUM7   MOTOR_NUM8
????: int Give_Speed  -500 -- 0 -- 500
*************************************************************************/
void Motor_Speed_Set(int Motor_ID,int Give_Speed)
{
    CanTxMsg tx_message;
    
    tx_message.RTR = CAN_RTR_DATA;  //???
    tx_message.IDE = CAN_ID_STD;    //???
    tx_message.DLC = 0x04;          //????4    
  
    switch (Motor_ID)
    {
        case MOTOR_NUM1: tx_message.StdId = ID_MOTOR1_SPEED_MODE; break; //??????1,Speed????ID?0x12
        case MOTOR_NUM2: tx_message.StdId = ID_MOTOR2_SPEED_MODE; break; //??????2,Speed????ID?0x22
        case MOTOR_NUM3: tx_message.StdId = ID_MOTOR3_SPEED_MODE; break; //??????3,Speed????ID?0x32
        case MOTOR_NUM4: tx_message.StdId = ID_MOTOR4_SPEED_MODE; break; //??????4,Speed????ID?0x42
        case MOTOR_NUM5: tx_message.StdId = ID_MOTOR5_SPEED_MODE; break; //??????5,Speed????ID?0x52
        case MOTOR_NUM6: tx_message.StdId = ID_MOTOR6_SPEED_MODE; break; //??????6,Speed????ID?0x62
        case MOTOR_NUM7: tx_message.StdId = ID_MOTOR7_SPEED_MODE; break; //??????7,Speed????ID?0x72
        case MOTOR_NUM8: tx_message.StdId = ID_MOTOR8_SPEED_MODE; break; //??????8,Speed????ID?0x82
        default: 
        {
            while(1)      //????????????8,??????
            {
                //delay_ms(200);
                #ifdef __DEBUG
                 //   printf("ERROR Motor_Speed_Set Motor_ID choise!\r\n");
                #endif
            }
        }
    }
    
    if(Give_Speed >= 500)
    {
        Give_Speed = 500;
    }
    
    if(Give_Speed <= -500)
    {
        Give_Speed = -500;
    }
    
    tx_message.Data[0] = (unsigned char)((Give_Speed >> 24)&0xff);
    tx_message.Data[1] = (unsigned char)((Give_Speed >> 16)&0xff);
    tx_message.Data[2] = (unsigned char)((Give_Speed >> 8)&0xff);
    tx_message.Data[3] = (unsigned char)(Give_Speed&0xff);
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0);   
}

/*************************************************************************
                         MOTOR_PWM_Location_Set
??:???PWM?????????????
????: int Motor_ID     ????: MOTOR_NUM1   MOTOR_NUM2  
                                       MOTOR_NUM3   MOTOR_NUM4
                                       MOTOR_NUM5   MOTOR_NUM6
                                       MOTOR_NUM7   MOTOR_NUM8
????: int Give_PWM      0 -- 5000
????: int Give_PWM_Location  
*************************************************************************/
void Motor_PWM_Location_Set(int Motor_ID,int Give_PWM,int Give_PWM_Location)
{
    CanTxMsg tx_message;
    
    tx_message.RTR = CAN_RTR_DATA;  //???
    tx_message.IDE = CAN_ID_STD;    //???
    tx_message.DLC = 0x08;          //????4    
  
    switch (Motor_ID)
    {
        case MOTOR_NUM1: tx_message.StdId = ID_MOTOR1_PWM_LOCATION_MODE; break; //??????1,PWM_LOCATION????ID?0x13
        case MOTOR_NUM2: tx_message.StdId = ID_MOTOR2_PWM_LOCATION_MODE; break; //??????2,PWM_LOCATION????ID?0x23
        case MOTOR_NUM3: tx_message.StdId = ID_MOTOR3_PWM_LOCATION_MODE; break; //??????3,PWM_LOCATION????ID?0x33
        case MOTOR_NUM4: tx_message.StdId = ID_MOTOR4_PWM_LOCATION_MODE; break; //??????4,PWM_LOCATION????ID?0x43
        case MOTOR_NUM5: tx_message.StdId = ID_MOTOR5_PWM_LOCATION_MODE; break; //??????5,PWM_LOCATION????ID?0x53
        case MOTOR_NUM6: tx_message.StdId = ID_MOTOR6_PWM_LOCATION_MODE; break; //??????6,PWM_LOCATION????ID?0x63
        case MOTOR_NUM7: tx_message.StdId = ID_MOTOR7_PWM_LOCATION_MODE; break; //??????7,PWM_LOCATION????ID?0x73
        case MOTOR_NUM8: tx_message.StdId = ID_MOTOR8_PWM_LOCATION_MODE; break; //??????8,PWM_LOCATION????ID?0x83     
        default: 
        {
            while(1)      //????????????8,??????
            {
                //delay_ms(200);
                #ifdef __DEBUG
                 //   printf("ERROR Motor_PWM_Location_Set Motor_ID choise!\r\n");
                #endif
            }
        }
    }
    
    Give_PWM = abs(Give_PWM);
    
    if(Give_PWM >= 5000)
    {
        Give_PWM = 5000;
    }
    
    tx_message.Data[0] = (unsigned char)((Give_PWM >> 24)&0xff);
    tx_message.Data[1] = (unsigned char)((Give_PWM >> 16)&0xff);
    tx_message.Data[2] = (unsigned char)((Give_PWM >> 8)&0xff);
    tx_message.Data[3] = (unsigned char)(Give_PWM&0xff);
    tx_message.Data[4] = (unsigned char)((Give_PWM_Location >> 24)&0xff);
    tx_message.Data[5] = (unsigned char)((Give_PWM_Location >> 16)&0xff);
    tx_message.Data[6] = (unsigned char)((Give_PWM_Location >> 8)&0xff);
    tx_message.Data[7] = (unsigned char)(Give_PWM_Location&0xff); 
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0);   
}

/*************************************************************************
                        Motor_Speed_Location_Set
??:???Speed?????????????
????: int Motor_ID     ????: MOTOR_NUM1   MOTOR_NUM2  
                                       MOTOR_NUM3   MOTOR_NUM4
                                       MOTOR_NUM5   MOTOR_NUM6
                                       MOTOR_NUM7   MOTOR_NUM8
????: int Give_Speed   0 - 500
????: int Give_Speed_Location 
*************************************************************************/
void Motor_Speed_Location_Set(int Motor_ID,int Give_Speed,int Give_Speed_Location)
{
    CanTxMsg tx_message;
    
    tx_message.RTR = CAN_RTR_DATA;  //???
    tx_message.IDE = CAN_ID_STD;    //???
    tx_message.DLC = 0x08;          //????8 
  
    switch (Motor_ID)
    {
        case MOTOR_NUM1: tx_message.StdId = ID_MOTOR1_SPEED_LOCATION_MODE; break; //?????1,Speed_LOCATION???ID=0x14
        case MOTOR_NUM2: tx_message.StdId = ID_MOTOR2_SPEED_LOCATION_MODE; break; //?????2,Speed_LOCATION???ID=0x24
        case MOTOR_NUM3: tx_message.StdId = ID_MOTOR3_SPEED_LOCATION_MODE; break; //?????3,Speed_LOCATION???ID=0x34
        case MOTOR_NUM4: tx_message.StdId = ID_MOTOR4_SPEED_LOCATION_MODE; break; //?????4,Speed_LOCATION???ID=0x44
        case MOTOR_NUM5: tx_message.StdId = ID_MOTOR5_SPEED_LOCATION_MODE; break; //?????5,Speed_LOCATION???ID=0x54
        case MOTOR_NUM6: tx_message.StdId = ID_MOTOR6_SPEED_LOCATION_MODE; break; //?????6,Speed_LOCATION???ID=0x64
        case MOTOR_NUM7: tx_message.StdId = ID_MOTOR7_SPEED_LOCATION_MODE; break; //?????7,Speed_LOCATION???ID=0x74
        case MOTOR_NUM8: tx_message.StdId = ID_MOTOR8_SPEED_LOCATION_MODE; break; //?????8,Speed_LOCATION???ID=0x84   
        default: 
        {
            while(1)      //????????????8,??????
            {
                //delay_ms(200);
                #ifdef __DEBUG
                 //   printf("ERROR Motor_Speed_Location_Set Motor_ID choise!\r\n");
                #endif
            }
        }
    }
    
    Give_Speed = abs(Give_Speed);
    
    if(Give_Speed >= 500)
    {
        Give_Speed = 5000;
    }
    
    tx_message.Data[0] = (unsigned char)((Give_Speed >> 24)&0xff);
    tx_message.Data[1] = (unsigned char)((Give_Speed >> 16)&0xff);
    tx_message.Data[2] = (unsigned char)((Give_Speed >> 8)&0xff);
    tx_message.Data[3] = (unsigned char)(Give_Speed&0xff);
    tx_message.Data[4] = (unsigned char)((Give_Speed_Location >> 24)&0xff);
    tx_message.Data[5] = (unsigned char)((Give_Speed_Location >> 16)&0xff);
    tx_message.Data[6] = (unsigned char)((Give_Speed_Location >> 8)&0xff);
    tx_message.Data[7] = (unsigned char)(Give_Speed_Location&0xff); 
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0);   
}
