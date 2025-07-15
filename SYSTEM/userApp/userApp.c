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


//TIMER1���ڱ�����ģʽ��TIMER2����PWM���
void motordrive_init(void)
{
	DRVL298N_Init();
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
//SYNCMotor();

#if 0

	static int count = 0,speed = 0;

	static int count_old = 0;
		float speedrpm = 0;
		float angledeg = 0;
	//count = __HAL_TIM_GET_COUNTER(&htim1);//1---������ߵ��

	count = __HAL_TIM_GET_COUNTER(&htim8);//4---�����ұߵ��
	

	//���ֵ����
	/* if (count > COUNT_MID*2 || count == 0){
		  count = 15000;
		  __HAL_TIM_SET_COUNTER(&htim1, 0);
	  }*/
	  #if 1
	speed++;
	
	 if (speed == COUNT_MID){
		 
		 //DRVL298N_Backward(RIGHTDIR,100,SJMOTOR);//2---�ұߵ����ʱ��ת��

		  DRVL298N_Backward(LEFTDIR,100,SJMOTOR);//1---��ߵ����ʱ��ת��

		 //DRVL298N_Backward(RIGHTDIR,100,KHMOTOR);//4---�ұ߿��ϵ����ʱ��ת��

		 //DRVL298N_Backward(LEFTDIR,50,KHMOTOR);//3---��߿��ϵ����ʱ��ת��

		 //DRVL298N_Backward(RIGHTDIR,100,TJMOTOR);//5---�о���ʱ��
		 
	  }
	 else if(speed == COUNT_MID*2)
	  {
		 //DRVL298N_Forward(RIGHTDIR,100,SJMOTOR);//2---���˳ʱ��ת��
				  

		  DRVL298N_Forward(LEFTDIR,100,SJMOTOR);//1---���˳ʱ��ת��

		 //DRVL298N_Forward(RIGHTDIR,100,KHMOTOR);//4---�ұ߿��ϵ��˳ʱ��ת��

		// DRVL298N_Forward(LEFTDIR,50,KHMOTOR);//3---��߿��ϵ��˳ʱ��ת��

		// DRVL298N_Forward(RIGHTDIR,100,TJMOTOR);//5---�о�˳ʱ��
	  }
	 else if(speed > COUNT_MID*2+2)
	 	speed = 0;

	 #endif

	printf("count:%d,spped:%d,rpm:%f,angle:%f\r\n",count,speed,speedrpm,angledeg);

#endif
}

void remotectrl_init(void)
{
	//������������ʼ��
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
	//6�������ź�1���źŽ�
	//��Ҫȥ����������⵽����2�Σ�����Ϊ�ź���Ч
	int i = 0;
	for(i=0;i<KEYNUM;i++){
		if(ChkFilter(&yk_KEYFilter[i], 0, GetKeyValue(i)) && GetKeyValue(VTKEY)){
			//���㰴�����µ�������ִ�ж�Ӧ�Ŀ�������
			switch(i)
			{
				case KEY1:
					//��
					printf("��\r\n");
					break;
				case KEY2:
					//��
					printf("��\r\n");
					break;
				case KEY3:
					//��
					printf("��\r\n");
					break;
				case KEY4:
					//��
					printf("��\r\n");
					break;
				case KEY5:
					//�о���
					printf("�о���\r\n");
					break;
				case KEY6:
					//�оٽ���
					printf("�оٽ���\r\n");
					break;
				default:
					break;
			}
			
		}
	}
}

