
#ifndef __SYNCMOTOR_H
#define __SYNCMOTOR_H
#include "stdio.h"	
#include "sys.h" 
/****
position：记录编码器的实际位置，是闭环控制的反馈信号

target：电机运动的目标位置

speed：当前输出给电机的速度控制量

integral：PID控制中的积分项累计值

lastError：存储上一次的位置误差，用于计算微分项

****/
typedef struct {
    long position;       // 编码器位置(脉冲计数)
    long target;         // 目标位置
    int speed;          // 当前速度(PWM值或步进脉冲频率)
    float integral;     // PID积分项(用于消除稳态误差)
    long lastError;     // 上一次误差(用于计算微分项)
} Motor;

//定义PID的值，需要调试配置

#define KP 1
#define KI	1
#define	KD	1

typedef enum  {
	FORWARD =0,  //正转,顺时针
    BACKWARD=1,  // 反转，逆时针
}motoroperate;


#define	SYNC_THRESHOLD	5

#endif

