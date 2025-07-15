#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "userApp.h" 
#include "timer.h" 
#include "drvL298N.h"
#include "Syncmotor.h"	
#include "filter.h"

#define COUNT_MID 1000

FilterItem_T yk_KEYFilter[KEYNUM];


//TIMER1用于编码器模式，TIMER2用于PWM输出
void motordrive_init(void)
{
	DRVL298N_Init();
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
//SYNCMotor();

#if 0

	static int count = 0,speed = 0;

	static int count_old = 0;
		float speedrpm = 0;
		float angledeg = 0;
	//count = __HAL_TIM_GET_COUNTER(&htim1);//1---升降左边电机

	count = __HAL_TIM_GET_COUNTER(&htim8);//4---升降右边电机
	

	//最大值清零
	/* if (count > COUNT_MID*2 || count == 0){
		  count = 15000;
		  __HAL_TIM_SET_COUNTER(&htim1, 0);
	  }*/
	  #if 1
	speed++;
	
	 if (speed == COUNT_MID){
		 
		 //DRVL298N_Backward(RIGHTDIR,100,SJMOTOR);//2---右边电机逆时针转动

		  DRVL298N_Backward(LEFTDIR,100,SJMOTOR);//1---左边电机逆时针转动

		 //DRVL298N_Backward(RIGHTDIR,100,KHMOTOR);//4---右边开合电机逆时针转动

		 //DRVL298N_Backward(LEFTDIR,50,KHMOTOR);//3---左边开合电机逆时针转动

		 //DRVL298N_Backward(RIGHTDIR,100,TJMOTOR);//5---托举逆时针
		 
	  }
	 else if(speed == COUNT_MID*2)
	  {
		 //DRVL298N_Forward(RIGHTDIR,100,SJMOTOR);//2---电机顺时针转动
				  

		  DRVL298N_Forward(LEFTDIR,100,SJMOTOR);//1---电机顺时针转动

		 //DRVL298N_Forward(RIGHTDIR,100,KHMOTOR);//4---右边开合电机顺时针转动

		// DRVL298N_Forward(LEFTDIR,50,KHMOTOR);//3---左边开合电机顺时针转动

		// DRVL298N_Forward(RIGHTDIR,100,TJMOTOR);//5---托举顺时针
	  }
	 else if(speed > COUNT_MID*2+2)
	 	speed = 0;

	 #endif

	printf("count:%d,spped:%d,rpm:%f,angle:%f\r\n",count,speed,speedrpm,angledeg);

#endif
}

void remotectrl_init(void)
{
	//按键过滤器初始化
	//Filter_Init(&yk_KEYFilter[0],KEYNUM,TRUE,FALSE,KEYFILTER,0);
	int i = 0;
	for(i = 0;i<KEYNUM;i++){
		yk_KEYFilter[i].IsCountFilter = TRUE;
		yk_KEYFilter[i].CountFilterThread = KEYFILTER;
		ResetFilterCount(&yk_KEYFilter[i]);
	}
}

bool GetKeyValue(u8 index)
{
	bool retvalue = FALSE;
	switch(index)
	{
		case KEY1:
			retvalue = GetKEYQ1();
			break;
		case KEY2:
			retvalue = GetKEYQ2();
			break;
		case KEY3:
			retvalue = GetKEYQ3();
			break;
		case KEY4:
			retvalue = GetKEYQ4();
			break;
		case KEY5:
			retvalue = GetKEYQ5();
			break;
		case KEY6:
			retvalue = GetKEYQ6();
			break;
		case VTKEY:
			retvalue = GetKEYVT();
			break;
		default:
			break;
	}
	return retvalue;
}
void remotectrl_func(void)
{
	//6个按键脚和1个信号脚
	//需要去抖，连续检测到大于2次，才认为信号有效
	int i = 0;
	for(i=0;i<KEYNUM;i++){
		if(ChkFilter(&yk_KEYFilter[i], 0, GetKeyValue(i)) && GetKeyValue(VTKEY)){
			//满足按键按下的条件，执行对应的控制命令
			switch(i)
			{
				case KEY1:
					//升
					printf("升\r\n");
					break;
				case KEY2:
					//降
					printf("降\r\n");
					break;
				case KEY3:
					//开
					printf("开\r\n");
					break;
				case KEY4:
					//合
					printf("合\r\n");
					break;
				case KEY5:
					//托举升
					printf("托举升\r\n");
					break;
				case KEY6:
					//托举降低
					printf("托举降低\r\n");
					break;
				default:
					break;
			}
			
		}
	}
}

