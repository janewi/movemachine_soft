#include "Syncmotor.h"	
#include "timer.h" 

#include "drvL298N.h"
#include <stdlib.h>

///实现通过GPIO获取编码器的转动次数
int g_encoder_count1 = 0x00;
int g_encoder_count2 = 0x00;


///实现通过GPIO获取编码器的转动次数
int g_encoder_count3 = 0x00;//SJ编码器数值
int g_encoder_count4 = 0x00;//SJ编码器数值


/** @brief 开合的编码器端口 */
#define GPIOBASE1_1	GPIOE
#define	GPIOPORT1_1	GPIO_PIN_0
/** @brief 开合的编码器端口*/
#define GPIOBASE1_2	GPIOE
#define	GPIOPORT1_2	GPIO_PIN_1


/** @brief 开合的编码器端口 ，第二路*/
#define GPIOBASE2_1	GPIOE
#define	GPIOPORT2_1	GPIO_PIN_2

#define GPIOBASE2_2	GPIOE
#define	GPIOPORT2_2	GPIO_PIN_3

/** @brief 升降的编码器端口 */
#define GPIOBASE3_1	GPIOE
#define	GPIOPORT3_1	GPIO_PIN_9
/** @brief 开合的编码器端口*/
#define GPIOBASE3_2	GPIOE
#define	GPIOPORT3_2	GPIO_PIN_11


/** @brief 开合的编码器端口 ，第二路*/
#define GPIOBASE4_1	GPIOC
#define	GPIOPORT4_1	GPIO_PIN_6

#define GPIOBASE4_2	GPIOC
#define	GPIOPORT4_2	GPIO_PIN_7


////在tim6 1ms定时器中读取编码器的值


//该函数实现双电机同步功能
//通过PID闭环控制，保持电机转动速度一致

Motor leftMotor,rightMotor;//左右开合电机

Motor SJleftMotor,SJrightMotor;//左右升降电机

u8 g_motorType = NULLMOTOR;

//初始化两个电机的所有状态变量为0，确保系统从一个已知的确定状态开始运行。
void initMotors() {
    leftMotor.position = 0;
    leftMotor.target = 0;
    leftMotor.speed = 0;
    leftMotor.integral = 0;
    leftMotor.lastError = 0;

    rightMotor = leftMotor;  // 初始化相同

	SJleftMotor.position = 0;
    SJleftMotor.target = 0;
    SJleftMotor.speed = 0;
    SJleftMotor.integral = 0;
    SJleftMotor.lastError = 0;
    
    SJleftMotor = SJleftMotor;  // 初始化相同
}
/****
PID控制器的三个核心组成部分：

比例(P)项：立即响应当前误差，误差越大输出越大

积分(I)项：累计历史误差，消除系统稳态误差

微分(D)项：预测误差变化趋势，抑制超调和振荡

. PID参数调整原则
先调P：增大Kp直到系统开始振荡

再调D：加入Kd抑制振荡

最后调I：加入Ki消除稳态误差

微调优化：根据实际响应调整

2. 典型参数范围
参数	范围	影响
Kp	0.5-5.0	响应速度
Ki	0.01-0.5	消除稳态误差
Kd	0.1-1.0	抑制振荡


****/
//根据编码器位置，计算输出PWM的增量
int calculatePID(Motor *motor, long error) {
    // 比例项(当前误差的直接影响)
	float integral = 0.0;
	float derivative = 0.0;
	int output = 0;
	
    float proportional = 0.0;
	if(	motor->direct == BACKWARD){
		proportional= KP_B * error;//error是目标值与当前值的误差
	}
	else{

		proportional= KP_F* error;//error是目标值与当前值的误差
	}
    // 积分项(累计历史误差，消除稳态误差)
    motor->integral += error;//累计误差
    // 积分限幅防止积分饱和
    if(motor->integral > 100) motor->integral = 100;
    if(motor->integral < -100) motor->integral = -100;
    integral = KI * motor->integral;
    
    // 微分项(误差变化率，抑制超调)
    derivative = KD * (error - motor->lastError);
    motor->lastError = error;  // 更新上次误差
    
    // 计算总输出
    output = (int)(proportional + integral + derivative);
    
    // 输出限幅
    if(output > MAX_SPEED) output = MAX_SPEED;
    if(output < -MAX_SPEED) output = -MAX_SPEED;

	if(output<0)
		output = -output;

	printf("outpwm:%d\r\n",output);
    return output;
}

long readEncoder(u8 isLeft,u8 index)
{

	if(isLeft == RIGHTDIR)
	{
		if(index == SJMOTOR)
		{
			return g_encoder_count3;

		}
		else
			return g_encoder_count1;//右边PE0/PE1
	}
	else if(isLeft == LEFTDIR)
	{
		if(index == SJMOTOR){
			
			return g_encoder_count4;
		}
		else
			return g_encoder_count2;//左边PE2/PE3
	}
	else
		return 0;
}
//升降电机的控制
void syncMotors(u8 motortype) {
	long avgTarget = 0;
	long leftError = 0;
	long rightError = 0;
	long syncError  = 0;

	long SJavgTarget = 0;
	long SJleftError = 0;
	long SJrightError = 0;
	long SJsyncError  = 0;

	if(motortype == KHMOTOR){
		// 1. 读取实际位置(闭环反馈)
	    leftMotor.position = readEncoder(RIGHTDIR,KHMOTOR);
	    rightMotor.position = readEncoder(RIGHTDIR,KHMOTOR);//测试

		// 2. 计算平均目标(使两个电机向中间靠拢)
    	avgTarget = (leftMotor.target + rightMotor.target) / 2;
		 // 3. 计算位置偏差
	   	leftError = avgTarget - leftMotor.position;
	    rightError = avgTarget - rightMotor.position;
		 // 4. 计算同步补偿(关键同步逻辑)
	    syncError = leftMotor.position - rightMotor.position;
	    if(abs(syncError) > SYNC_THRESHOLD) {
	        leftError -= syncError / 2;  // 左电机向右侧靠拢
	        rightError += syncError / 2; // 右电机向左侧靠拢
	    }
		 // 5. PID计算控制量
	    leftMotor.speed = calculatePID(&leftMotor, leftError);
	    rightMotor.speed = calculatePID(&rightMotor, rightError);
		  // 6. 输出控制信号
    	printf("Left: Pos=%ld, Speed=%d | Right: Pos=%ld, Speed=%d\n",
           leftMotor.position, leftMotor.speed,
           rightMotor.position, rightMotor.speed);

		 // 模拟电机运动(实际硬件中替换为PWM输出)
		if(	leftMotor.direct == BACKWARD){
	   		setMotorSpeed(BACKWARD,RIGHTDIR,leftMotor.speed,KHMOTOR); 
			leftMotor.position -= leftMotor.speed / 10;
	    	rightMotor.position -= rightMotor.speed / 10;

		}
		else{
			setMotorSpeed(FORWARD,RIGHTDIR,leftMotor.speed,KHMOTOR);
			leftMotor.position += leftMotor.speed / 10;
	    	rightMotor.position += rightMotor.speed / 10;
		}

	}
	else if(motortype == SJMOTOR){
		// 1. 读取实际位置(闭环反馈)
		SJleftMotor.position = readEncoder(RIGHTDIR,SJMOTOR);
    	SJrightMotor.position = readEncoder(RIGHTDIR,SJMOTOR);//测试
    	// 2. 计算平均目标(使两个电机向中间靠拢)
    	SJavgTarget = (SJleftMotor.target + SJrightMotor.target) / 2;
		 // 3. 计算位置偏差
		SJleftError = SJavgTarget - SJleftMotor.position;
   		SJrightError = SJavgTarget - SJrightMotor.position;
		 // 4. 计算同步补偿(关键同步逻辑)
		 SJsyncError = SJleftMotor.position - SJrightMotor.position;
	    if(abs(SJsyncError) > SYNC_THRESHOLD) {
	        SJleftError -= SJsyncError / 2;  // 左电机向右侧靠拢
	        SJrightError += SJsyncError / 2; // 右电机向左侧靠拢
	    }
		// 5. PID计算控制量
		SJleftMotor.speed = calculatePID(&SJleftMotor, SJleftError);
    	SJrightMotor.speed = calculatePID(&SJrightMotor, SJrightError);
		  // 6. 输出控制信号
    	printf("SJ Left: Pos=%ld, Speed=%d | Right: Pos=%ld, Speed=%d\n",
           SJleftMotor.position, SJleftMotor.speed,
           SJrightMotor.position, SJrightMotor.speed);

		 // 模拟电机运动(实际硬件中替换为PWM输出)
		if(	SJleftMotor.direct == BACKWARD){
	   		setMotorSpeed(BACKWARD,RIGHTDIR,SJleftMotor.speed,SJMOTOR); 
			SJleftMotor.position -= SJleftMotor.speed / 10;
	    	SJrightMotor.position -= SJrightMotor.speed / 10;

		}
		else{
			setMotorSpeed(FORWARD,RIGHTDIR,SJleftMotor.speed,SJMOTOR);
			SJleftMotor.position += SJleftMotor.speed / 10;
	    	SJrightMotor.position += SJrightMotor.speed / 10;
		}
	}

}

//调用示例
//程序说明:
//1、电机最大速度20,PID是保持速度恒定。
//2、速度恒定的条件下，根据编码器的目标位置，进行电机的转动控制
//3、根据遥控命令，如果是升，电机正转?(根据机械结构，待定),目标位置为最大值，直到遥控停下或者到达机械最大位置
/*
//开合电机测试
正转
leftMotor.direct = FORWARD;
// 设置目标位置(示例)
leftMotor.target = 5000;
rightMotor.target = 5000;

反转
leftMotor.direct = BACKWARD;
// 设置目标位置(示例)
leftMotor.target = -5000;
rightMotor.target = -5000;

//升降和开合都完成了
*/
int SYNCMotor(void) {
	//int i = 0;
    initMotors();
	
	SJleftMotor.direct = BACKWARD;
	// 设置目标位置(示例)
	SJleftMotor.target = -5000;
	SJrightMotor.target = -5000;
	    
    // 控制循环
	#if 0
	if(leftMotor.position >= leftMotor.target)
	{
		DRVL298N_Brake(LEFTDIR,KHMOTOR);
	}
	else
	{
        syncMotors();
	}
	#else
		g_motorType = SJMOTOR;
		syncMotors(g_motorType);
    #endif
	
    return 0;
}

// 替换printf为实际的PWM输出
void setMotorSpeed(u8 motoroperate,u8 motordir,int speed,u8 index) {
    if(motoroperate == BACKWARD) 
	{
        DRVL298N_Backward(motordir,speed,index);//电机逆时针转动
    } 
	else 
    {
        DRVL298N_Forward(motordir,speed,index);//电机顺时针转动
    }
}



u8 GetEncodeValue(u8 st_encoder_status,GPIO_TypeDef* GPIOx, u16 GPIO_Pin,GPIO_TypeDef* GPIOx_2, u16 GPIO_Pin_2,int* encoder_count,u8* zhengzhuan,u8* fanzhuan)
{
	//第一路编码器
	if(0 == st_encoder_status)
	{
		if(1== HAL_GPIO_ReadPin(GPIOx,GPIO_Pin))
		{
			st_encoder_status = 2;

		}
	}
	else if(2 == st_encoder_status)
	{
		if(0 == HAL_GPIO_ReadPin(GPIOx_2,GPIO_Pin_2))
		{
			*(u8*)zhengzhuan  = 1;
		}
		else
		{
			*(u8*)fanzhuan = 1; 
		}
		st_encoder_status = 3;
	}
	else if(3 == st_encoder_status)
	{
		if(0 == HAL_GPIO_ReadPin(GPIOx,GPIO_Pin))
		{
			st_encoder_status = 5;
		}
	}
	else if(5 == st_encoder_status)
	{
		if((1 == HAL_GPIO_ReadPin(GPIOx_2,GPIO_Pin_2))&&(1 == *(u8*)zhengzhuan))
		{
			*(u8*)zhengzhuan  = 0;
			*(u8*)fanzhuan = 0;

			*(int*)encoder_count = *(int*)encoder_count + 1;

		}
		else if((0 == HAL_GPIO_ReadPin(GPIOx_2,GPIO_Pin_2))&&(1 == *(u8*)fanzhuan))
		{
			*(u8*)zhengzhuan  = 0;
			*(u8*)fanzhuan = 0;
			*(int*)encoder_count = *(int*)encoder_count - 1;

			
		}
		else
		{
			*(u8*)zhengzhuan  = 0;
			*(u8*)fanzhuan = 0;

		}
		st_encoder_status = 0;

	}

	return st_encoder_status;
}


//需要调试:能否进入中断，能否获取2路编码器的值
//0.1ms执行一次
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim)
{
	//static u8 st_cnt = 0;
	static u8 st_encoder_status1=0,st_encoder_status2 =0;
	static u8 st_encoder_status3=0,st_encoder_status4 =0;
	static u8 st_zhenzhuan1=0,st_zhenzhuan2 =0;
	static u8 st_fanzhuan1=0,st_fanzhuan2 =0;

	static u8 st_zhenzhuan3=0,st_zhenzhuan4 =0;
	static u8 st_fanzhuan3=0,st_fanzhuan4 =0;
	
	
	if(htim->Instance == TIM6)
	{
		if(g_motorType == KHMOTOR){
			st_encoder_status1 = GetEncodeValue(st_encoder_status1,GPIOBASE1_1,GPIOPORT1_1,GPIOBASE1_2,GPIOPORT1_2,&g_encoder_count1,&st_zhenzhuan1,&st_fanzhuan1);
			st_encoder_status2 = GetEncodeValue(st_encoder_status2,GPIOBASE2_1,GPIOPORT2_1,GPIOBASE2_2,GPIOPORT2_2,&g_encoder_count2,&st_zhenzhuan2,&st_fanzhuan2);
		}
		else if(g_motorType == SJMOTOR){
			st_encoder_status3 = GetEncodeValue(st_encoder_status3,GPIOBASE3_1,GPIOPORT3_1,GPIOBASE3_2,GPIOPORT3_2,&g_encoder_count3,&st_zhenzhuan3,&st_fanzhuan3);
			st_encoder_status4 = GetEncodeValue(st_encoder_status4,GPIOBASE4_1,GPIOPORT4_1,GPIOBASE4_2,GPIOPORT4_2,&g_encoder_count4,&st_zhenzhuan4,&st_fanzhuan4);
		}
	}
}




