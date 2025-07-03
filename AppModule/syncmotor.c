#include "Syncmotor.h"	
#include "timer.h" 

//�ú���ʵ��˫���ͬ������
//ͨ��PID�ջ����ƣ����ֵ��ת���ٶ�һ��

Motor leftMotor,rightMotor;//�����������
//��ʼ���������������״̬����Ϊ0��ȷ��ϵͳ��һ����֪��ȷ��״̬��ʼ���С�
void initMotors() {
    leftMotor.position = 0;
    leftMotor.target = 0;
    leftMotor.speed = 0;
    leftMotor.integral = 0;
    leftMotor.lastError = 0;
    
    rightMotor = leftMotor;  // ��ʼ����ͬ
}
/****
PID������������������ɲ��֣�

����(P)�������Ӧ��ǰ�����Խ�����Խ��

����(I)��ۼ���ʷ������ϵͳ��̬���

΢��(D)�Ԥ�����仯���ƣ����Ƴ�������

****/
int calculatePID(Motor *motor, long error) {
    // ������(��ǰ����ֱ��Ӱ��)
    float proportional = KP * error;
    
    // ������(�ۼ���ʷ��������̬���)
    motor->integral += error;
    // �����޷���ֹ���ֱ���
    if(motor->integral > 100) motor->integral = 100;
    if(motor->integral < -100) motor->integral = -100;
    float integral = KI * motor->integral;
    
    // ΢����(���仯�ʣ����Ƴ���)
    float derivative = KD * (error - motor->lastError);
    motor->lastError = error;  // �����ϴ����
    
    // ���������
    int output = (int)(proportional + integral + derivative);
    
    // ����޷�
    if(output > MAX_SPEED) output = MAX_SPEED;
    if(output < -MAX_SPEED) output = -MAX_SPEED;
    
    return output;
}

long readEncoder(u8 isLeft)
{
	if(isLeft == 0)
	{
		return __HAL_TIM_GET_COUNTER(&htim1);//���ұ� PE9 PE11
	}
	else if(isLeft == 1)
	{
		return __HAL_TIM_GET_COUNTER(&htim3);//����� PA6 PA7
	}
	else
		return 0;
}
void syncMotors() {
    // 1. ��ȡʵ��λ��(�ջ�����)
    leftMotor.position = readEncoder(1);
    rightMotor.position = readEncoder(0);
    
    // 2. ����ƽ��Ŀ��(ʹ����������м俿£)
    long avgTarget = (leftMotor.target + rightMotor.target) / 2;
    
    // 3. ����λ��ƫ��
    long leftError = avgTarget - leftMotor.position;
    long rightError = avgTarget - rightMotor.position;
    
    // 4. ����ͬ������(�ؼ�ͬ���߼�)
    long syncError = leftMotor.position - rightMotor.position;
    if(abs(syncError) > SYNC_THRESHOLD) {
        leftError -= syncError / 2;  // �������Ҳ࿿£
        rightError += syncError / 2; // �ҵ������࿿£
    }
    
    // 5. PID���������
    leftMotor.speed = calculatePID(&leftMotor, leftError);
    rightMotor.speed = calculatePID(&rightMotor, rightError);
    
    // 6. ��������ź�
    printf("Left: Pos=%ld, Speed=%d | Right: Pos=%ld, Speed=%d\n",
           leftMotor.position, leftMotor.speed,
           rightMotor.position, rightMotor.speed);

	//���������յ��������Ӧ�÷�ת(?)
	setMotorSpeed(BACKWARD,rightMotor.speed,SJMOTOR);
    setMotorSpeed(BACKWARD,leftMotor.speed,SJMOTOR);
    
    // ģ�����˶�(ʵ��Ӳ�����滻ΪPWM���)
    leftMotor.position += leftMotor.speed / 10;
    rightMotor.position += rightMotor.speed / 10;
}

//����ʾ��
int SYNCMotor() {
    initMotors();
    
    // ����Ŀ��λ��(ʾ��)
    leftMotor.target = 1000;
    rightMotor.target = 1000;
    
    // ����ѭ��
    for(int i = 0; i < 100; i++) {
        syncMotors();
    }
    
    return 0;
}

// �滻printfΪʵ�ʵ�PWM���
void setMotorSpeed(u8 motoroperate,u8 motordir,int speed,u8 index) {
    if(motoroperate == BACKWARD) 
	{
        DRVL298N_Backward(motordir,speed,index);//�����ʱ��ת��
    } 
	else 
    {
        DRVL298N_Forward(motordir,speed,index);//���˳ʱ��ת��
    }
}



