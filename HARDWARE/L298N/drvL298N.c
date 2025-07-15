/**
 * @file drvL298N.c
 * @brief DRVL298N双H桥电机驱动器控制实现
 */

#include "drvL298N.h"
#include "timer.h"

//升降的两路电机
//第一路电机
/** @brief IN1引脚的定时器 */
#define IN1_TIM &htim2
/** @brief IN2引脚的定时器 */
#define IN2_TIM &htim2

/** @brief IN1引脚的定时器通道 */
#define IN1_CH TIM_CHANNEL_2
/** @brief IN2引脚的定时器通道 */
#define IN2_CH TIM_CHANNEL_3

//第二路电机
/** @brief IN1引脚的定时器 */
#define IN1_2_TIM &htim3
/** @brief IN2引脚的定时器 */
#define IN2_2_TIM &htim3

/** @brief IN1引脚的定时器通道 */
#define IN1_2_CH TIM_CHANNEL_1
/** @brief IN2引脚的定时器通道 */
#define IN2_2_CH TIM_CHANNEL_2

//开合的两路电机
//第一路电机
/** @brief IN1引脚的定时器 */
#define KHIN1_TIM &htim4
/** @brief IN2引脚的定时器 */
#define KHIN2_TIM &htim4

/** @brief IN1引脚的定时器通道 */
#define KHIN1_CH TIM_CHANNEL_1
/** @brief IN2引脚的定时器通道 */
#define KHIN2_CH TIM_CHANNEL_2

//第二路电机
/** @brief IN1引脚的定时器 */
#define KHIN1_2_TIM &htim4
/** @brief IN2引脚的定时器 */
#define KHIN2_2_TIM &htim4

/** @brief IN1引脚的定时器通道 */
#define KHIN1_2_CH TIM_CHANNEL_3
/** @brief IN2引脚的定时器通道 */
#define KHIN2_2_CH TIM_CHANNEL_4

//托举的一路电机
/** @brief IN1引脚的定时器 */
#define TJIN1_TIM &htim3
/** @brief IN2引脚的定时器 */
#define TJIN2_TIM &htim3

/** @brief IN1引脚的定时器通道 */
#define TJIN1_CH TIM_CHANNEL_3
/** @brief IN2引脚的定时器通道 */
#define TJIN2_CH TIM_CHANNEL_4





/** @brief 当前衰减模式 */
static DecayMode currentDecayMode = SLOW_DECAY;

/**
 * @brief 设置IN1引脚的PWM占空比
 * @param duty 占空比值
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
 * @brief 设置IN2引脚的PWM占空比
 * @param duty 占空比值
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

   __HAL_RCC_GPIOE_CLK_ENABLE();			   //开启GPIOE时钟
   
   GPIO_Initure.Pin=GPIO_PIN_13 | GPIO_PIN_14; 			   //PB5
   GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;	   //推挽输出
   GPIO_Initure.Pull=GPIO_PULLUP;			   //上拉
   GPIO_Initure.Speed=GPIO_SPEED_HIGH;		   //高速
   HAL_GPIO_Init(GPIOE,&GPIO_Initure);

   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);   //PE5置1，默认初始化后灯灭
   HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);   //PE5置1，默认初始化后灯灭

}


/**
 * @brief 初始化DRVL298N
 */
void DRVL298N_Init(void)
{

	//MX_TIM1_Init();
  	MX_TIM2_Init();

	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM6_Init();
	//MX_TIM8_Init();


	//这是升降的
	//这是第一路电机驱动
    HAL_TIM_PWM_Start(IN1_TIM, IN1_CH);
    HAL_TIM_PWM_Start(IN2_TIM, IN2_CH);


	//这是第二路电机驱动
	HAL_TIM_PWM_Start(IN1_2_TIM, IN1_2_CH);
    HAL_TIM_PWM_Start(IN2_2_TIM, IN2_2_CH);


	//这是开合的
	//这是第一路电机驱动
    HAL_TIM_PWM_Start(KHIN1_TIM, KHIN1_CH);
    HAL_TIM_PWM_Start(KHIN2_TIM, KHIN2_CH);
	//这是第二路电机驱动
	HAL_TIM_PWM_Start(KHIN1_2_TIM, KHIN1_2_CH);
    HAL_TIM_PWM_Start(KHIN2_2_TIM, KHIN2_2_CH);

	//这是托举电机的
	//这是第一路电机驱动
    HAL_TIM_PWM_Start(TJIN1_TIM, TJIN1_CH);
    HAL_TIM_PWM_Start(TJIN2_TIM, TJIN2_CH);

}

/**
 * @brief 设置衰减模式
 * @param mode 衰减模式
 */
void DRVL298N_SetDecayMode(DecayMode mode)
{
    currentDecayMode = mode;
}

/**
 * @brief 控制电机前进
 * @param speed 速度值（0-100）
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
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
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
 * @brief 电机刹车
 */
void DRVL298N_Brake(u8 motordir,u8 index)
{
    __SetIn1PWM(motordir,MAX_SPEED,index);
    __SetIn2PWM(motordir,MAX_SPEED,index);
}

/**
 * @brief 电机滑行
 */
void DRVL298N_Coast(u8 motordir,u8 index)
{
    __SetIn1PWM(motordir,0,index);
    __SetIn2PWM(motordir,0,index);
}
