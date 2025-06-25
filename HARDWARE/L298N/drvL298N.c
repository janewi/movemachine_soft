/**
 * @file drvL298N.c
 * @brief DRVL298N双H桥电机驱动器控制实现
 */

#include "drvL298N.h"
#include "timer.h"

/** @brief IN1引脚的定时器 */
#define IN1_TIM &htim2
/** @brief IN2引脚的定时器 */
#define IN2_TIM &htim2

/** @brief IN1引脚的定时器通道 */
#define IN1_CH TIM_CHANNEL_2
/** @brief IN2引脚的定时器通道 */
#define IN2_CH TIM_CHANNEL_3

/** @brief 最大速度值 */
#define MAX_SPEED 100


/** @brief 当前衰减模式 */
static DecayMode currentDecayMode = SLOW_DECAY;

/**
 * @brief 设置IN1引脚的PWM占空比
 * @param duty 占空比值
 */
 void __SetIn1PWM(uint8_t duty)
{
    __HAL_TIM_SET_COMPARE(IN1_TIM, IN1_CH, duty);
}

/**
 * @brief 设置IN2引脚的PWM占空比
 * @param duty 占空比值
 */
void __SetIn2PWM(uint8_t duty)
{
    __HAL_TIM_SET_COMPARE(IN2_TIM, IN2_CH, duty);
}

/**
 * @brief 初始化DRVL298N
 */
void DRVL298N_Init(void)
{
    HAL_TIM_PWM_Start(IN1_TIM, IN1_CH);
    HAL_TIM_PWM_Start(IN2_TIM, IN2_CH);
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
void DRVL298N_Forward(uint8_t speed)
{
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    
    if (currentDecayMode == FAST_DECAY) {
        __SetIn1PWM(speed);
        __SetIn2PWM(0);
    } else {
        __SetIn1PWM(MAX_SPEED);
        __SetIn2PWM(MAX_SPEED - speed);
    }
}

/**
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
 */
void DRVL298N_Backward(uint8_t speed)
{
    if (speed > MAX_SPEED)
        speed = MAX_SPEED;
    
    if (currentDecayMode == FAST_DECAY) {
        __SetIn1PWM(0);
        __SetIn2PWM(speed);
    } else {
        __SetIn1PWM(MAX_SPEED - speed);
        __SetIn2PWM(MAX_SPEED);
    }
}

/**
 * @brief 电机刹车
 */
void DRVL298N_Brake(void)
{
    __SetIn1PWM(MAX_SPEED);
    __SetIn2PWM(MAX_SPEED);
}

/**
 * @brief 电机滑行
 */
void DRVL298N_Coast(void)
{
    __SetIn1PWM(0);
    __SetIn2PWM(0);
}
