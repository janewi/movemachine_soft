/**
 * @file drvL298N.c
 * @brief DRVL298N˫H�ŵ������������ʵ��
 */

#include "drvL298N.h"
#include "timer.h"

/** @brief IN1���ŵĶ�ʱ�� */
#define IN1_TIM &htim2
/** @brief IN2���ŵĶ�ʱ�� */
#define IN2_TIM &htim2

/** @brief IN1���ŵĶ�ʱ��ͨ�� */
#define IN1_CH TIM_CHANNEL_2
/** @brief IN2���ŵĶ�ʱ��ͨ�� */
#define IN2_CH TIM_CHANNEL_3

/** @brief ����ٶ�ֵ */
#define MAX_SPEED 100


/** @brief ��ǰ˥��ģʽ */
static DecayMode currentDecayMode = SLOW_DECAY;

/**
 * @brief ����IN1���ŵ�PWMռ�ձ�
 * @param duty ռ�ձ�ֵ
 */
 void __SetIn1PWM(uint8_t duty)
{
    __HAL_TIM_SET_COMPARE(IN1_TIM, IN1_CH, duty);
}

/**
 * @brief ����IN2���ŵ�PWMռ�ձ�
 * @param duty ռ�ձ�ֵ
 */
void __SetIn2PWM(uint8_t duty)
{
    __HAL_TIM_SET_COMPARE(IN2_TIM, IN2_CH, duty);
}

/**
 * @brief ��ʼ��DRVL298N
 */
void DRVL298N_Init(void)
{
    HAL_TIM_PWM_Start(IN1_TIM, IN1_CH);
    HAL_TIM_PWM_Start(IN2_TIM, IN2_CH);
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
 * @brief ���Ƶ������
 * @param speed �ٶ�ֵ��0-100��
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
 * @brief ���ɲ��
 */
void DRVL298N_Brake(void)
{
    __SetIn1PWM(MAX_SPEED);
    __SetIn2PWM(MAX_SPEED);
}

/**
 * @brief �������
 */
void DRVL298N_Coast(void)
{
    __SetIn1PWM(0);
    __SetIn2PWM(0);
}
