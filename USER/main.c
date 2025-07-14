#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "gpio.h"

/************************************************
 ALIENTEK ս��STM32F103������ ʵ��51
 UOSIIʵ��1-1-�������-HAL��汾
 ����֧�֣�www.openedv.com
 �Ա����̣� http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

//START ����
//�����������ȼ�
#define START_TASK_PRIO			10  ///��ʼ��������ȼ�Ϊ���
//���������ջ��С
#define START_STK_SIZE			128
//���������ջ
OS_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *pdata);

//�����������
//�����������ȼ�
#define MOTOR_TASK_PRIO			7
//���������ջ��С
#define MOTOR_STK_SIZE			512
//�����ջ
OS_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];
//������
void motordrive_task(void *pdata);

//ң�ؿ�������
//�����������ȼ�
#define REMOTE_TASK_PRIO			6
//���������ջ��С
#define REMOTE_STK_SIZE			128
//�����ջ
OS_STK REMOTE_TASK_STK[REMOTE_STK_SIZE];
//������
void remotectrl_task(void *pdata);

int main(void)
{ 
    HAL_Init();                    	 	//��ʼ��HAL��    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//����ʱ��,72M
	delay_init(72);               		//��ʼ����ʱ����
	LED_Init();							//��ʼ��LED	
	uart_init(115200);					//��ʼ������
	MX_GPIO_Init();

	printf("uart init ok\r\n");
	OSInit();                       	//UCOS��ʼ��
    OSTaskCreateExt((void(*)(void*) )start_task,                //������
                    (void*          )0,                         //���ݸ��������Ĳ���
                    (OS_STK*        )&START_TASK_STK[START_STK_SIZE-1],//�����ջջ��
                    (INT8U          )START_TASK_PRIO,           //�������ȼ�
                    (INT16U         )START_TASK_PRIO,           //����ID����������Ϊ�����ȼ�һ��
                    (OS_STK*        )&START_TASK_STK[0],        //�����ջջ��
                    (INT32U         )START_STK_SIZE,            //�����ջ��С
                    (void*          )0,                         //�û�����Ĵ洢��
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);//����ѡ��,Ϊ�˱���������������񶼱��渡��Ĵ�����ֵ
	OSStart(); //��ʼ����
}

//��ʼ����
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //����ͳ������
	
	OS_ENTER_CRITICAL();  //�����ٽ���(�ر��ж�)
    //LED0����
    OSTaskCreateExt((void(*)(void*) )motordrive_task,                 
                    (void*          )0,
                    (OS_STK*        )&MOTOR_TASK_STK[MOTOR_STK_SIZE-1],
                    (INT8U          )MOTOR_TASK_PRIO,            
                    (INT16U         )MOTOR_TASK_PRIO,            
                    (OS_STK*        )&MOTOR_TASK_STK[0],         
                    (INT32U         )MOTOR_STK_SIZE,             
                    (void*          )0,                         
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);
	//LED1����
    OSTaskCreateExt((void(*)(void*) )remotectrl_task,                 
                    (void*          )0,
                    (OS_STK*        )&REMOTE_TASK_STK[REMOTE_STK_SIZE-1],
                    (INT8U          )REMOTE_TASK_PRIO,            
                    (INT16U         )REMOTE_TASK_PRIO,            
                    (OS_STK*        )&REMOTE_TASK_STK[0],         
                    (INT32U         )REMOTE_STK_SIZE,             
                    (void*          )0,                         
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

    OS_EXIT_CRITICAL();             //�˳��ٽ���(���ж�)
	OSTaskSuspend(START_TASK_PRIO); //����ʼ����
}
 
//�����������
void motordrive_task(void *pdata)
{	 	
	motordrive_init();
	while(1)
	{
		motordrive_func();
		OSTimeDly(20);
	};
}

//ң������
void remotectrl_task(void *pdata)
{	  
	remotectrl_init();
	while(1)
	{
		//remotectrl_func();
		OSTimeDly(300);
	}
}
