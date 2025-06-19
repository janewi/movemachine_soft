#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "userApp.h" 
#include "timer.h" 

void motordrive_init(void)
{
	TIM3_PWM_Init(500-1,72-1);
}


void motordrive_func(void)
{
	static u8  dir = 1;
	static u16 led0pwmval=0; 

	if(dir)led0pwmval++;				//dir==1 led0pwmval递增
	else led0pwmval--;					//dir==0 led0pwmval递减  
	if(led0pwmval>300)dir=0;			//led0pwmval到达300后，方向为递减
	if(led0pwmval==0)dir=1;				//led0pwmval递减到0后，方向改为递增
	TIM_SetTIM3Compare2(led0pwmval);	//修改比较值，修改占空比

}

void remotectrl_init(void)
{
	
}

void remotectrl_func(void)
{
	static unsigned char  ledst = 0;

	if(ledst == 0)
	{
		LED1=0;
		ledst = 1;
	}
	else
	{
		LED1=1;
		ledst = 0;
	}
}

