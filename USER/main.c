#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
#include "gpio.h"

/************************************************
 ALIENTEK 战舰STM32F103开发板 实验51
 UOSII实验1-1-任务调度-HAL库版本
 技术支持：www.openedv.com
 淘宝店铺： http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

//START 任务
//设置任务优先级
#define START_TASK_PRIO			10  ///开始任务的优先级为最低
//设置任务堆栈大小
#define START_STK_SIZE			128
//任务任务堆栈
OS_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *pdata);

//电机驱动任务
//设置任务优先级
#define MOTOR_TASK_PRIO			7
//设置任务堆栈大小
#define MOTOR_STK_SIZE			512
//任务堆栈
OS_STK MOTOR_TASK_STK[MOTOR_STK_SIZE];
//任务函数
void motordrive_task(void *pdata);

//遥控开关任务
//设置任务优先级
#define REMOTE_TASK_PRIO			6
//设置任务堆栈大小
#define REMOTE_STK_SIZE			128
//任务堆栈
OS_STK REMOTE_TASK_STK[REMOTE_STK_SIZE];
//任务函数
void remotectrl_task(void *pdata);

int main(void)
{ 
    HAL_Init();                    	 	//初始化HAL库    
    Stm32_Clock_Init(RCC_PLL_MUL9);   	//设置时钟,72M
	delay_init(72);               		//初始化延时函数
	LED_Init();							//初始化LED	
	uart_init(115200);					//初始化串口
	MX_GPIO_Init();

	printf("uart init ok\r\n");
	OSInit();                       	//UCOS初始化
    OSTaskCreateExt((void(*)(void*) )start_task,                //任务函数
                    (void*          )0,                         //传递给任务函数的参数
                    (OS_STK*        )&START_TASK_STK[START_STK_SIZE-1],//任务堆栈栈顶
                    (INT8U          )START_TASK_PRIO,           //任务优先级
                    (INT16U         )START_TASK_PRIO,           //任务ID，这里设置为和优先级一样
                    (OS_STK*        )&START_TASK_STK[0],        //任务堆栈栈底
                    (INT32U         )START_STK_SIZE,            //任务堆栈大小
                    (void*          )0,                         //用户补充的存储区
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);//任务选项,为了保险起见，所有任务都保存浮点寄存器的值
	OSStart(); //开始任务
}

//开始任务
void start_task(void *pdata)
{
	OS_CPU_SR cpu_sr=0;
	pdata=pdata;
	OSStatInit();  //开启统计任务
	
	OS_ENTER_CRITICAL();  //进入临界区(关闭中断)
    //LED0任务
    OSTaskCreateExt((void(*)(void*) )motordrive_task,                 
                    (void*          )0,
                    (OS_STK*        )&MOTOR_TASK_STK[MOTOR_STK_SIZE-1],
                    (INT8U          )MOTOR_TASK_PRIO,            
                    (INT16U         )MOTOR_TASK_PRIO,            
                    (OS_STK*        )&MOTOR_TASK_STK[0],         
                    (INT32U         )MOTOR_STK_SIZE,             
                    (void*          )0,                         
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);
	//LED1任务
    OSTaskCreateExt((void(*)(void*) )remotectrl_task,                 
                    (void*          )0,
                    (OS_STK*        )&REMOTE_TASK_STK[REMOTE_STK_SIZE-1],
                    (INT8U          )REMOTE_TASK_PRIO,            
                    (INT16U         )REMOTE_TASK_PRIO,            
                    (OS_STK*        )&REMOTE_TASK_STK[0],         
                    (INT32U         )REMOTE_STK_SIZE,             
                    (void*          )0,                         
                    (INT16U         )OS_TASK_OPT_STK_CHK|OS_TASK_OPT_STK_CLR|OS_TASK_OPT_SAVE_FP);

    OS_EXIT_CRITICAL();             //退出临界区(开中断)
	OSTaskSuspend(START_TASK_PRIO); //挂起开始任务
}
 
//电机驱动任务
void motordrive_task(void *pdata)
{	 	
	motordrive_init();
	while(1)
	{
		motordrive_func();
		OSTimeDly(20);
	};
}

//遥控任务
void remotectrl_task(void *pdata)
{	  
	remotectrl_init();
	while(1)
	{
		//remotectrl_func();
		OSTimeDly(300);
	}
}
