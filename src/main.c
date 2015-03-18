/************************************************************
	POSSMINI  

All right reserved
								Peking university
											2013..
*************************************************************/

#include "STM32Lib\\stm32f10x.h"
#include "hal.h"

int main(void)
{
	
	ChipHalInit();			
	ChipOutHalInit();
	Delay(10);
	//Init_MPU6050(); 
	
	for(;;)
	{
	IWDG_ReloadCounter();	
	}
	
	
}

//delay
void Delay(u16 speed)
{
	u16 i;
	while(speed!=0)
	{
		speed--;
		for(i=0;i<400;i++);
	}
}



