
#ifndef __DRVL298N_H_
#define __DRVL298N_H_

#include <stdint.h>
#include "Sys.h"

/** @brief ����ٶ�ֵ*/
#define MAX_SPEED 100
/**
 * @brief ˥��ģʽö��
 */
typedef enum {
    SLOW_DECAY,  /**< ��˥��ģʽ */
    FAST_DECAY   /**< ��˥��ģʽ */
} DecayMode;


typedef enum {
	SJMOTOR=0,
	KHMOTOR,
	TJMOTOR,
}MOTORIndex;

typedef enum  {
	RIGHTDIR =0,  //�ұ�
    LEFTDIR=1,  // ���
}motordirect;


/**
 * @brief ��ʼ��DRVL298N
 */
void DRVL298N_Init(void);

/**
 * @brief ����˥��ģʽ
 * @param mode ˥��ģʽ
 */
void DRVL298N_SetDecayMode(DecayMode mode);

/**
 * @brief ���Ƶ��ǰ��
 * @param speed �ٶ�ֵ��0-100��
 */
void DRVL298N_Forward(u8 motordir,uint8_t speed,u8 index);

/**
 * @brief ���Ƶ������
 * @param speed �ٶ�ֵ��0-100��
 */
void DRVL298N_Backward(u8 motordir,uint8_t speed,u8 index);

/**
 * @brief ���ɲ��
 */
void DRVL298N_Brake(u8 motordir,u8 index);

/**
 * @brief �������
 */
void DRVL298N_Coast(u8 motordir,u8 index);

#endif /* __DRVL298N_H_ */
