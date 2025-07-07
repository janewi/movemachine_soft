#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "userApp.h" 
#include "timer.h" 
#include "drvL298N.h"

#define COUNT_MID 1000

//TIMER1���ڱ�����ģʽ��TIMER2����PWM���
void motordrive_init(void)
{
	DRVL298N_Init();

	//time1pwm_Init();
	//��������
	//��һ������ı�����
	 HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);
  	__HAL_TIM_SET_COUNTER(&htim1, COUNT_MID);

	//�ڶ�������ı�����
	 HAL_TIM_Encoder_Start(&htim8, TIM_CHANNEL_ALL);
	__HAL_TIM_SET_COUNTER(&htim8, COUNT_MID);
}


void motordrive_func(void)
{
#if 0
	static u8  dir = 1;
	static u16 led0pwmval=0; 

	if(dir)led0pwmval++;				//dir==1 led0pwmval����
	else led0pwmval--;					//dir==0 led0pwmval�ݼ�  
	if(led0pwmval>300)dir=0;			//led0pwmval����300�󣬷���Ϊ�ݼ�
	if(led0pwmval==0)dir=1;				//led0pwmval�ݼ���0�󣬷����Ϊ����
	//TIM_SetTIM3Compare2(led0pwmval);	//�޸ıȽ�ֵ���޸�ռ�ձ�
#endif

	static int count = 0,speed = 0;


	//count = __HAL_TIM_GET_COUNTER(&htim1);//1---������ߵ��

	//count = __HAL_TIM_GET_COUNTER(&htim8);//4---�����ұߵ��
	
	//���ֵ����
	/* if (count > COUNT_MID*2 || count == 0){
		  count = 15000;
		  __HAL_TIM_SET_COUNTER(&htim1, 0);
	  }*/
	  #if 1
	speed++;
	
	 if (speed == COUNT_MID){
		 
		 //DRVL298N_Backward(RIGHTDIR,100,SJMOTOR);//2---�ұߵ����ʱ��ת��

		 // DRVL298N_Backward(LEFTDIR,100,SJMOTOR);//1---��ߵ����ʱ��ת��

		 //DRVL298N_Backward(RIGHTDIR,100,KHMOTOR);//4---�ұ߿��ϵ����ʱ��ת��

		 //DRVL298N_Backward(LEFTDIR,100,KHMOTOR);//3---��߿��ϵ����ʱ��ת��

		 DRVL298N_Backward(RIGHTDIR,100,TJMOTOR);//5---�о���ʱ��
		 
	  }
	 else if(speed == COUNT_MID*2)
	  {
		 //DRVL298N_Forward(RIGHTDIR,100,SJMOTOR);//2---���˳ʱ��ת��
				  

		  //DRVL298N_Forward(LEFTDIR,100,SJMOTOR);//1---���˳ʱ��ת��

		 //DRVL298N_Forward(RIGHTDIR,100,KHMOTOR);//4---�ұ߿��ϵ��˳ʱ��ת��

		 //DRVL298N_Forward(LEFTDIR,100,KHMOTOR);//3---��߿��ϵ��˳ʱ��ת��

		 DRVL298N_Forward(RIGHTDIR,100,TJMOTOR);//5---�о�˳ʱ��
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

