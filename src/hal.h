#ifndef HAL_H
#define HAL_H

//Intial Hardware
extern void  ChipHalInit(void);
extern void  ChipOutHalInit(void);

#define Center_duty 1500  //1.5ms
#define Max_duty 2000		//2ms
#define Min_duty 1000		//1ms

#define Protect_duty 1560
#define Stop_duty 1500

#define Set_DJ(duty) TIM1->CCR1=duty
#define Set_MD(duty) TIM8->CCR1=duty

extern int adjust_center;
extern void Delay(u16 n); //5 us delay

extern int adjust_center;
extern char remote_control_flag;//estop flag
extern void USART1_Putc(unsigned char c);
extern void USART1_Puts(char * str);
extern void USART1_PutsN (char * buff, u16 len);


#endif
