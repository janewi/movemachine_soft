#include "Syncmotor.h"	
#include "timer.h" 

#include "drvL298N.h"
#include <stdlib.h>

///ʵ��ͨ��GPIO��ȡ��������ת������
int g_encoder_count1 = 0x00;
int g_encoder_count2 = 0x00;


///ʵ��ͨ��GPIO��ȡ��������ת������
int g_encoder_count3 = 0x00;//SJ��������ֵ
int g_encoder_count4 = 0x00;//SJ��������ֵ


/** @brief ���ϵı������˿� */
#define GPIOBASE1_1	GPIOE
#define	GPIOPORT1_1	GPIO_PIN_0
/** @brief ���ϵı������˿�*/
#define GPIOBASE1_2	GPIOE
#define	GPIOPORT1_2	GPIO_PIN_1


/** @brief ���ϵı������˿� ���ڶ�·*/
#define GPIOBASE2_1	GPIOE
#define	GPIOPORT2_1	GPIO_PIN_2

#define GPIOBASE2_2	GPIOE
#define	GPIOPORT2_2	GPIO_PIN_3

/** @brief �����ı������˿� */
#define GPIOBASE3_1	GPIOE
#define	GPIOPORT3_1	GPIO_PIN_9
/** @brief ���ϵı������˿�*/
#define GPIOBASE3_2	GPIOE
#define	GPIOPORT3_2	GPIO_PIN_11


/** @brief ���ϵı������˿� ���ڶ�·*/
#define GPIOBASE4_1	GPIOC
#define	GPIOPORT4_1	GPIO_PIN_6

#define GPIOBASE4_2	GPIOC
#define	GPIOPORT4_2	GPIO_PIN_7


////��tim6 1ms��ʱ���ж�ȡ��������ֵ


//�ú���ʵ��˫���ͬ������
//ͨ��PID�ջ����ƣ����ֵ��ת���ٶ�һ��

Motor leftMotor,rightMotor;//���ҿ��ϵ��

Motor SJleftMotor,SJrightMotor;//�����������

u8 g_motorType = NULLMOTOR;

//��ʼ���������������״̬����Ϊ0��ȷ��ϵͳ��һ����֪��ȷ��״̬��ʼ���С�
void initMotors() {
    leftMotor.position = 0;
    leftMotor.target = 0;
    leftMotor.speed = 0;
    leftMotor.integral = 0;
    leftMotor.lastError = 0;

    rightMotor = leftMotor;  // ��ʼ����ͬ

	SJleftMotor.position = 0;
    SJleftMotor.target = 0;
    SJleftMotor.speed = 0;
    SJleftMotor.integral = 0;
    SJleftMotor.lastError = 0;
    
    SJleftMotor = SJleftMotor;  // ��ʼ����ͬ
}
/****
PID������������������ɲ��֣�

����(P)�������Ӧ��ǰ�����Խ�����Խ��

����(I)��ۼ���ʷ������ϵͳ��̬���

΢��(D)�Ԥ�����仯���ƣ����Ƴ�������

. PID��������ԭ��
�ȵ�P������Kpֱ��ϵͳ��ʼ��

�ٵ�D������Kd������

����I������Ki������̬���

΢���Ż�������ʵ����Ӧ����

2. ���Ͳ�����Χ
����	��Χ	Ӱ��
Kp	0.5-5.0	��Ӧ�ٶ�
Ki	0.01-0.5	������̬���
Kd	0.1-1.0	������


****/
//���ݱ�����λ�ã��������PWM������
int calculatePID(Motor *motor, long error) {
    // ������(��ǰ����ֱ��Ӱ��)
	float integral = 0.0;
	float derivative = 0.0;
	int output = 0;
	
    float proportional = 0.0;
	if(	motor->direct == BACKWARD){
		proportional= KP_B * error;//error��Ŀ��ֵ�뵱ǰֵ�����
	}
	else{

		proportional= KP_F* error;//error��Ŀ��ֵ�뵱ǰֵ�����
	}
    // ������(�ۼ���ʷ��������̬���)
    motor->integral += error;//�ۼ����
    // �����޷���ֹ���ֱ���
    if(motor->integral > 100) motor->integral = 100;
    if(motor->integral < -100) motor->integral = -100;
    integral = KI * motor->integral;
    
    // ΢����(���仯�ʣ����Ƴ���)
    derivative = KD * (error - motor->lastError);
    motor->lastError = error;  // �����ϴ����
    
    // ���������
    output = (int)(proportional + integral + derivative);
    
    // ����޷�
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
			return g_encoder_count1;//�ұ�PE0/PE1
	}
	else if(isLeft == LEFTDIR)
	{
		if(index == SJMOTOR){
			
			return g_encoder_count4;
		}
		else
			return g_encoder_count2;//���PE2/PE3
	}
	else
		return 0;
}
//��������Ŀ���
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
		// 1. ��ȡʵ��λ��(�ջ�����)
	    leftMotor.position = readEncoder(RIGHTDIR,KHMOTOR);
	    rightMotor.position = readEncoder(RIGHTDIR,KHMOTOR);//����

		// 2. ����ƽ��Ŀ��(ʹ����������м俿£)
    	avgTarget = (leftMotor.target + rightMotor.target) / 2;
		 // 3. ����λ��ƫ��
	   	leftError = avgTarget - leftMotor.position;
	    rightError = avgTarget - rightMotor.position;
		 // 4. ����ͬ������(�ؼ�ͬ���߼�)
	    syncError = leftMotor.position - rightMotor.position;
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

		 // ģ�����˶�(ʵ��Ӳ�����滻ΪPWM���)
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
		// 1. ��ȡʵ��λ��(�ջ�����)
		SJleftMotor.position = readEncoder(RIGHTDIR,SJMOTOR);
    	SJrightMotor.position = readEncoder(RIGHTDIR,SJMOTOR);//����
    	// 2. ����ƽ��Ŀ��(ʹ����������м俿£)
    	SJavgTarget = (SJleftMotor.target + SJrightMotor.target) / 2;
		 // 3. ����λ��ƫ��
		SJleftError = SJavgTarget - SJleftMotor.position;
   		SJrightError = SJavgTarget - SJrightMotor.position;
		 // 4. ����ͬ������(�ؼ�ͬ���߼�)
		 SJsyncError = SJleftMotor.position - SJrightMotor.position;
	    if(abs(SJsyncError) > SYNC_THRESHOLD) {
	        SJleftError -= SJsyncError / 2;  // �������Ҳ࿿£
	        SJrightError += SJsyncError / 2; // �ҵ������࿿£
	    }
		// 5. PID���������
		SJleftMotor.speed = calculatePID(&SJleftMotor, SJleftError);
    	SJrightMotor.speed = calculatePID(&SJrightMotor, SJrightError);
		  // 6. ��������ź�
    	printf("SJ Left: Pos=%ld, Speed=%d | Right: Pos=%ld, Speed=%d\n",
           SJleftMotor.position, SJleftMotor.speed,
           SJrightMotor.position, SJrightMotor.speed);

		 // ģ�����˶�(ʵ��Ӳ�����滻ΪPWM���)
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

//����ʾ��
//����˵��:
//1���������ٶ�20,PID�Ǳ����ٶȺ㶨��
//2���ٶȺ㶨�������£����ݱ�������Ŀ��λ�ã����е����ת������
//3������ң�������������������ת?(���ݻ�е�ṹ������),Ŀ��λ��Ϊ���ֵ��ֱ��ң��ͣ�»��ߵ����е���λ��
/*
//���ϵ������
��ת
leftMotor.direct = FORWARD;
// ����Ŀ��λ��(ʾ��)
leftMotor.target = 5000;
rightMotor.target = 5000;

��ת
leftMotor.direct = BACKWARD;
// ����Ŀ��λ��(ʾ��)
leftMotor.target = -5000;
rightMotor.target = -5000;

//�����Ϳ��϶������
*/
int SYNCMotor(void) {
	//int i = 0;
    initMotors();
	
	SJleftMotor.direct = BACKWARD;
	// ����Ŀ��λ��(ʾ��)
	SJleftMotor.target = -5000;
	SJrightMotor.target = -5000;
	    
    // ����ѭ��
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



u8 GetEncodeValue(u8 st_encoder_status,GPIO_TypeDef* GPIOx, u16 GPIO_Pin,GPIO_TypeDef* GPIOx_2, u16 GPIO_Pin_2,int* encoder_count,u8* zhengzhuan,u8* fanzhuan)
{
	//��һ·������
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


//��Ҫ����:�ܷ�����жϣ��ܷ��ȡ2·��������ֵ
//0.1msִ��һ��
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




