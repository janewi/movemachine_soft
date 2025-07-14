
#ifndef __DRVL298N_H_
#define __DRVL298N_H_

#include <stdint.h>
#include "Sys.h"

/** @brief 最大速度值*/
#define MAX_SPEED 100
/**
 * @brief 衰减模式枚举
 */
typedef enum {
    SLOW_DECAY,  /**< 慢衰减模式 */
    FAST_DECAY   /**< 快衰减模式 */
} DecayMode;


typedef enum {
	SJMOTOR=0,
	KHMOTOR,
	TJMOTOR,
}MOTORIndex;

typedef enum  {
	RIGHTDIR =0,  //右边
    LEFTDIR=1,  // 左边
}motordirect;


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
void DRVL298N_Forward(u8 motordir,uint8_t speed,u8 index);

/**
 * @brief 控制电机后退
 * @param speed 速度值（0-100）
 */
void DRVL298N_Backward(u8 motordir,uint8_t speed,u8 index);

/**
 * @brief 电机刹车
 */
void DRVL298N_Brake(u8 motordir,u8 index);

/**
 * @brief 电机滑行
 */
void DRVL298N_Coast(u8 motordir,u8 index);

#endif /* __DRVL298N_H_ */
