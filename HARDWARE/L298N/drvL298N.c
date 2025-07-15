/**
 * @file drvL298N.c
 * @brief DRVL298N˫H�ŵ������������ʵ��
 */

#include "drvL298N.h"
#include "timer.h"

//��������·���
//��һ·���
/** @brief IN1���ŵĶ�ʱ�� */
#define IN1_TIM &htim2
/** @brief IN2���ŵĶ�ʱ�� */
#define IN2_TIM &htim2

/** @brief IN1���ŵĶ�ʱ��ͨ�� */
#define IN1_CH TIM_CHANNEL_2
/** @brief IN2���ŵĶ�ʱ��ͨ�� */
#define IN2_CH TIM_CHANNEL_3

//�ڶ�·���
/** @brief IN1���ŵĶ�ʱ�� */
#define IN1_2_TIM &htim3
/** @brief IN2���ŵĶ�ʱ�� */
#define IN2_2_TIM &htim3

/** @brief IN1���ŵĶ�ʱ��ͨ�� */
#define IN1_2_CH TIM_CHANNEL_1
/** @brief IN2���ŵĶ�ʱ��ͨ�� */
#define IN2_2_CH TIM_CHANNEL_2

//���ϵ���·���
//��һ·���
/** @brief IN1���ŵĶ�ʱ�� */
#define KHIN1_TIM &htim4
/** @brief IN2���ŵĶ�ʱ�� */
#define KHIN2_TIM &htim4

/** @brief IN1���ŵĶ�ʱ��ͨ�� */
#define KHIN1_CH TIM_CHANNEL_1
/** @brief IN2���ŵĶ�ʱ��ͨ�� */
#define KHIN2_CH TIM_CHANNEL_2

//�ڶ�·���
/** @brief IN1���ŵĶ�ʱ�� */
#define KHIN1_2_TIM &htim4
/** @brief IN2���ŵĶ�ʱ�� */
#define KHIN2_2_TIM &htim4

/** @brief IN1���ŵĶ�ʱ��ͨ�� */
#define KHIN1_2_CH TIM_CHANNEL_3
/** @brief IN2���ŵĶ�ʱ��ͨ�� */
#define KHIN2_2_CH TIM_CHANNEL_4

//�оٵ�һ·���
/** @brief IN1���ŵĶ�ʱ�� */
#define TJIN1_TIM &htim3
/** @brief IN2���ŵĶ�ʱ�� */
#define TJIN2_TIM &htim3

/** @brief IN1���ŵĶ�ʱ��ͨ�� */
#define TJIN1_CH TIM_CHANNEL_3
/** @brief IN2���ŵĶ�ʱ��ͨ�� */
#define TJIN2_CH TIM_CHANNEL_4





/** @brief ��ǰ˥��ģʽ */
static DecayMode currentDecayMode = SLOW_DECAY;

/**
 * @brief ����IN1���ŵ�PWMռ�ձ�
 * @param duty ռ�ձ�ֵ
 */
 void __SetIn1PWM(u8 motordir,uint8_t duty,u8 index)
{
	if(motordir == RIGHTDIR)
	{
		switch(index)
		{
			case SJMOTOR:
				__HAL_TIM_SET_COMPARE(IN1_TIM, IN1_CH, duty);
				break;
			case KHMOTOR:
				__HAL_TIM_SET_COMPARE(KHIN1_TIM, KHIN1_CH, duty);
				break;
			case TJMOTOR:
				__HAL_TIM_SET_COMPARE(TJIN1_TIM, TJIN1_CH, duty);
				break;
			default:
				break;
		}
   		
	}
	else
	{
		switch(index)
		{
			case SJMOTOR:
				__HAL_TIM_SET_COMPARE(IN1_2_TIM, IN1_2_CH, duty);
				break;
			case KHMOTOR:
				__HAL_TIM_SET_COMPARE(KHIN1_2_TIM, KHIN1_2_CH, duty);
				break;
			case TJMOTOR:
				__HAL_TIM_SET_COMPARE(TJIN1_TIM, TJIN1_CH, duty);
				break;
			default:
				break;
		}
		
	}
}

/**
 * @brief ����IN2���ŵ�PWMռ�ձ�
 * @param duty ռ�ձ�ֵ
 */
void __SetIn2PWM(u8 motordir,uint8_t duty,u8 index)
{
	if(motordir == RIGHTDIR)
	{
		switch(index)
		{
			case SJMOTOR:
				__HAL_TIM_SET_COMPARE(IN2_TIM, IN2_CH, duty);
				break;
			case KHMOTOR:
				__HAL_TIM_SET_COMPARE(KHIN2_TIM, KHIN2_CH, duty);
				break;
			case TJMOTOR:
				__HAL_TIM_SET_COMPARE(TJIN2_TIM, TJIN2_CH, duty);
				break;
			default:
				break;
		}
	}
	else
	{
		switch(index)
		{
			case SJMOTOR:
				__HAL_TIM_SET_COMPARE(IN2_2_TIM, IN2_2_CH, duty);
				break;
			case KHMOTOR:
				__HAL_TIM_SET_COMPARE(KHIN2_2_TIM, KHIN2_2_CH, duty);
				break;
			case TJMOTOR:
				__HAL_TIM_SET_COMPARE(TJIN2_TIM, TJIN2_CH, duty);
				break;
			default:
				break;
		}
	}
}

//test io
void time1pwm_Init(void)
{
	GPIO_InitTypeDef GPIO_Initure;

   __HAL_RCC_GPIOE_CLK_ENABLE();			   //����GPIOEʱ��
   
   GPIO_Initure.Pin=GPIO_PIN_13 | GPIO_PIN_14; 			   //PB5
   GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;	   //�������
   GPIO_Initure.Pull=GPIO_PULLUP;			   //����
   GPIO_Initure.Speed=GPIO_SPEED_HIGH;		   //����
   HAL_GPIO_Init(GPIOE,&GPIO_Initure);

   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);   //PE5��1��Ĭ�ϳ�ʼ�������
   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);   //PE5��1��Ĭ�ϳ�ʼ�������

}


/**
 * @brief ��ʼ��DRVL298N
 */
void DRVL298N_Init(void)
{

	//MX_TIM1_Init();
  	MX_TIM2_Init();

	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM6_Init();
	//MX_TIM8_Init();


	//����������
	//���ǵ�һ·�������
    HAL_TIM_PWM_Start(IN1_TIM, IN1_CH);
    HAL_TIM_PWM_Start(IN2_TIM, IN2_CH);


	//���ǵڶ�·�������
	HAL_TIM_PWM_Start(IN1_2_TIM, IN1_2_CH);
    HAL_TIM_PWM_Start(IN2_2_TIM, IN2_2_CH);


	//���ǿ��ϵ�
	//���ǵ�һ·�������
    HAL_TIM_PWM_Start(KHIN1_TIM, KHIN1_CH);
    HAL_TIM_PWM_Start(KHIN2_TIM, KHIN2_CH);
	//���ǵڶ�·�������
	HAL_TIM_PWM_Start(KHIN1_2_TIM, KHIN1_2_CH);
    HAL_TIM_PWM_Start(KHIN2_2_TIM, KHIN2_2_CH);

	//�����оٵ����
	//���ǵ�һ·�������
    HAL_TIM_PWM_Start(TJIN1_TIM, TJIN1_CH);
    HAL_TIM_PWM_Start(TJIN2_TIM, TJIN2_CH);

}

/**
 * @brief ����˥��ģʽ
 * @param mode ˥��ģʽ
 */
void DRVL298N_SetDecayMode(DecayMode mode)
{
    currentDecayMode = mode;
}

/**
 * @brief ���Ƶ��ǰ��
 * @param speed �ٶ�ֵ��0-100��
 */
void DRVL298N_Forward(u8 motordir,uint8_t speed,u8 index)
{
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    
    if (currentDecayMode == FAST_DECAY) {
        __SetIn1PWM(motordir,speed,index);
        __SetIn2PWM(motordir,0,index);
    } else {
        __SetIn1PWM(motordir,MAX_SPEED,index);
        __SetIn2PWM(motordir,MAX_SPEED - speed,index);
    }
}

/**
 * @brief ���Ƶ������
 * @param speed �ٶ�ֵ��0-100��
 */
void DRVL298N_Backward(u8 motordir,uint8_t speed,u8 index)
{
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    
    if (currentDecayMode == FAST_DECAY) {
        __SetIn1PWM(motordir,0,index);
        __SetIn2PWM(motordir,speed,index);
    } else {
        __SetIn1PWM(motordir,MAX_SPEED - speed,index);
        __SetIn2PWM(motordir,MAX_SPEED,index);
    }
}

/**
 * @brief ���ɲ��
 */
void DRVL298N_Brake(u8 motordir,u8 index)
{
    __SetIn1PWM(motordir,MAX_SPEED,index);
    __SetIn2PWM(motordir,MAX_SPEED,index);
}

/**
 * @brief �������
 */
void DRVL298N_Coast(u8 motordir,u8 index)
{
    __SetIn1PWM(motordir,0,index);
    __SetIn2PWM(motordir,0,index);
}
