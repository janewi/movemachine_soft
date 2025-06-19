
#ifndef __DRVL298N_H_
#define __DRVL298N_H_

#include <stdint.h>

/**
 * @brief 衰减模式枚举
 */
typedef enum {
    SLOW_DECAY,  /**< 慢衰减模式 */
    FAST_DECAY   /**< 快衰减模式 */
} DecayMode;

/**
 * @brief 初始化DRVL298N
 */
void DRVL298N_Init(void);

/**
 * @brief 设置衰减模式
 * @param mode 衰减模式
 */
void DRVL298N_SetDecayMode(DecayMode mode);

/**
 * @brief 控制电机前进
 * @param speed 速度值（0-100）
 */
void DRVL298N_Forward(uint8_t speed);

/**
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
 */
void DRVL298N_Backward(uint8_t speed);

/**
 * @brief 电机刹车
 */
void DRVL298N_Brake(void);

/**
 * @brief 电机滑行
 */
void DRVL298N_Coast(void);

#endif /* __DRVL298N_H_ */
