
#ifndef __DRVL298N_H_
#define __DRVL298N_H_

#include <stdint.h>

/**
 * @brief ˥��ģʽö��
 */
typedef enum {
    SLOW_DECAY,  /**< ��˥��ģʽ */
    FAST_DECAY   /**< ��˥��ģʽ */
} DecayMode;

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
void DRVL298N_Forward(uint8_t speed);

/**
 * @brief ���Ƶ������
 * @param speed �ٶ�ֵ��0-100��
 */
void DRVL298N_Backward(uint8_t speed);

/**
 * @brief ���ɲ��
 */
void DRVL298N_Brake(void);

/**
 * @brief �������
 */
void DRVL298N_Coast(void);

#endif /* __DRVL298N_H_ */
