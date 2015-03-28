#include "delay.h"

void delay_ms(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=10300; //at 72MHz 10300 is ok
 		while(a--);
	}
}

void delay_us(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a=9;  //at 72MHz 9 is ok,the higher the number the more timing precise
		while(a--);
	}
}
