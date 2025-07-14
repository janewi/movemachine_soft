
#ifndef __SYNCMOTOR_H
#define __SYNCMOTOR_H
#include "stdio.h"	
#include "sys.h" 
/****
position����¼��������ʵ��λ�ã��Ǳջ����Ƶķ����ź�

target������˶���Ŀ��λ��

speed����ǰ�����������ٶȿ�����

integral��PID�����еĻ������ۼ�ֵ

lastError���洢��һ�ε�λ�������ڼ���΢����

****/
typedef struct {
    long position;       // ������λ��(�������)
    long target;         // Ŀ��λ��
    int speed;          // ��ǰ�ٶ�(PWMֵ�򲽽�����Ƶ��)
    float integral;     // PID������(����������̬���)
    long lastError;     // ��һ�����(���ڼ���΢����)
    u8 direct;//����
} Motor;

//����PID��ֵ����Ҫ��������

#define KP_F 	1.5//˳ʱ��ת
#define KP_B	1.2//��ʱ��ת
#define KI	0.05
#define	KD	0

typedef enum  {
	FORWARD =0,  //��ת,˳ʱ��
    BACKWARD=1,  // ��ת����ʱ��
}motoroperate;


#define	SYNC_THRESHOLD	4


long readEncoder(u8 isLeft,u8 index);
void setMotorSpeed(u8 motoroperate,u8 motordir,int speed,u8 index);


#endif

