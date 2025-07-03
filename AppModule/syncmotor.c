#include "Syncmotor.h"	
#include "timer.h" 

//该函数实现双电机同步功能
//通过PID闭环控制，保持电机转动速度一致

Motor leftMotor,rightMotor;//左右升降电机
//初始化两个电机的所有状态变量为0，确保系统从一个已知的确定状态开始运行。
void initMotors() {
    leftMotor.position = 0;
    leftMotor.target = 0;
    leftMotor.speed = 0;
    leftMotor.integral = 0;
    leftMotor.lastError = 0;
    
    rightMotor = leftMotor;  // 初始化相同
}
/****
PID控制器的三个核心组成部分：

比例(P)项：立即响应当前误差，误差越大输出越大

积分(I)项：累计历史误差，消除系统稳态误差

微分(D)项：预测误差变化趋势，抑制超调和振荡

****/
int calculatePID(Motor *motor, long error) {
    // 比例项(当前误差的直接影响)
    float proportional = KP * error;
    
    // 积分项(累计历史误差，消除稳态误差)
    motor->integral += error;
    // 积分限幅防止积分饱和
    if(motor->integral > 100) motor->integral = 100;
    if(motor->integral < -100) motor->integral = -100;
    float integral = KI * motor->integral;
    
    // 微分项(误差变化率，抑制超调)
    float derivative = KD * (error - motor->lastError);
    motor->lastError = error;  // 更新上次误差
    
    // 计算总输出
    int output = (int)(proportional + integral + derivative);
    
    // 输出限幅
    if(output > MAX_SPEED) output = MAX_SPEED;
    if(output < -MAX_SPEED) output = -MAX_SPEED;
    
    return output;
}

long readEncoder(u8 isLeft)
{
	if(isLeft == 0)
	{
		return __HAL_TIM_GET_COUNTER(&htim1);//接右边 PE9 PE11
	}
	else if(isLeft == 1)
	{
		return __HAL_TIM_GET_COUNTER(&htim3);//接左边 PA6 PA7
	}
	else
		return 0;
}
void syncMotors() {
    // 1. 读取实际位置(闭环反馈)
    leftMotor.position = readEncoder(1);
    rightMotor.position = readEncoder(0);
    
    // 2. 计算平均目标(使两个电机向中间靠拢)
    long avgTarget = (leftMotor.target + rightMotor.target) / 2;
    
    // 3. 计算位置偏差
    long leftError = avgTarget - leftMotor.position;
    long rightError = avgTarget - rightMotor.position;
    
    // 4. 计算同步补偿(关键同步逻辑)
    long syncError = leftMotor.position - rightMotor.position;
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

	//假设现在收到的升命令，应该反转(?)
	setMotorSpeed(BACKWARD,rightMotor.speed,SJMOTOR);
    setMotorSpeed(BACKWARD,leftMotor.speed,SJMOTOR);
    
    // 模拟电机运动(实际硬件中替换为PWM输出)
    leftMotor.position += leftMotor.speed / 10;
    rightMotor.position += rightMotor.speed / 10;
}

//调用示例
int SYNCMotor() {
    initMotors();
    
    // 设置目标位置(示例)
    leftMotor.target = 1000;
    rightMotor.target = 1000;
    
    // 控制循环
    for(int i = 0; i < 100; i++) {
        syncMotors();
    }
    
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



