#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "userApp.h" 
#include "timer.h" 
#include "drvL298N.h"

#define COUNT_MID 1000

//TIMER1用于编码器模式，TIMER2用于PWM输出
void motordrive_init(void)
{
	DRVL298N_Init();

	//time1pwm_Init();
	//升降功能
	//第一个电机的编码器
	 HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  	__HAL_TIM_SET_COUNTER(&htim1, COUNT_MID);

	//第二个电机的编码器
	 HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
	__HAL_TIM_SET_COUNTER(&htim8, COUNT_MID);
}


void motordrive_func(void)
{
#if 0
	static u8  dir = 1;
	static u16 led0pwmval=0; 

	if(dir)led0pwmval++;				//dir==1 led0pwmval递增
	else led0pwmval--;					//dir==0 led0pwmval递减  
	if(led0pwmval>300)dir=0;			//led0pwmval到达300后，方向为递减
	if(led0pwmval==0)dir=1;				//led0pwmval递减到0后，方向改为递增
	//TIM_SetTIM3Compare2(led0pwmval);	//修改比较值，修改占空比
#endif

	static int count = 0,speed = 0;


	//count = __HAL_TIM_GET_COUNTER(&htim1);//1---升降左边电机

	//count = __HAL_TIM_GET_COUNTER(&htim8);//4---升降右边电机
	
	//最大值清零
	/* if (count > COUNT_MID*2 || count == 0){
		  count = 15000;
		  __HAL_TIM_SET_COUNTER(&htim1, 0);
	  }*/
	  #if 1
	speed++;
	
	 if (speed == COUNT_MID){
		 
		 //DRVL298N_Backward(RIGHTDIR,100,SJMOTOR);//2---右边电机逆时针转动

		 // DRVL298N_Backward(LEFTDIR,100,SJMOTOR);//1---左边电机逆时针转动

		 //DRVL298N_Backward(RIGHTDIR,100,KHMOTOR);//4---右边开合电机逆时针转动

		 //DRVL298N_Backward(LEFTDIR,100,KHMOTOR);//3---左边开合电机逆时针转动

		 DRVL298N_Backward(RIGHTDIR,100,TJMOTOR);//5---托举逆时针
		 
	  }
	 else if(speed == COUNT_MID*2)
	  {
		 //DRVL298N_Forward(RIGHTDIR,100,SJMOTOR);//2---电机顺时针转动
				  

		  //DRVL298N_Forward(LEFTDIR,100,SJMOTOR);//1---电机顺时针转动

		 //DRVL298N_Forward(RIGHTDIR,100,KHMOTOR);//4---右边开合电机顺时针转动

		 //DRVL298N_Forward(LEFTDIR,100,KHMOTOR);//3---左边开合电机顺时针转动

		 DRVL298N_Forward(RIGHTDIR,100,TJMOTOR);//5---托举顺时针
	  }
	 else if(speed > COUNT_MID*2+2)
	 	speed = 0;

	 #endif

	printf("count:%d,spped:%d\r\n",count,speed);

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

