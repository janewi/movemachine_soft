
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
} Motor;

//����PID��ֵ����Ҫ��������

#define KP 1
#define KI	1
#define	KD	1

typedef enum  {
	FORWARD =0,  //��ת,˳ʱ��
    BACKWARD=1,  // ��ת����ʱ��
}motoroperate;


#define	SYNC_THRESHOLD	5

#endif

