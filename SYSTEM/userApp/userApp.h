
#ifndef __USERAPP_H
#define __USERAPP_H
#include "stdio.h"	
#include "sys.h" 

#define KEYNUM 6
#define	KEYFILTER	3

typedef enum  {
	KEY1 =0,  //
    KEY2,  // 
    KEY3,  //
    KEY4,  // 
    KEY5,  //
    KEY6,  // 
    VTKEY,
}KEYCODE;


//获取遥控器按键Q1到Q6，Q7的电平
#define	KEYQ1BASE	GPIOC
#define	KEYQ1PORT	GPIO_PIN_0

#define	KEYQ2BASE	GPIOC
#define	KEYQ2PORT	GPIO_PIN_1

#define	KEYQ3BASE	GPIOC
#define	KEYQ3PORT	GPIO_PIN_2

#define	KEYQ4BASE	GPIOC
#define	KEYQ4PORT	GPIO_PIN_3

#define	KEYQ5BASE	GPIOD
#define	KEYQ5PORT	GPIO_PIN_0

#define	KEYQ6BASE	GPIOD
#define	KEYQ6PORT	GPIO_PIN_1

#define	KEYVTBASE	GPIOD
#define	KEYVTPORT	GPIO_PIN_3


#define GetKEYQ1() 	(HAL_GPIO_ReadPin(KEYQ1BASE, KEYQ1PORT) == GPIO_PIN_SET?TRUE:FALSE)
#define GetKEYQ2() 	(HAL_GPIO_ReadPin(KEYQ2BASE, KEYQ2PORT) == GPIO_PIN_SET?TRUE:FALSE)

#define GetKEYQ3() 	(HAL_GPIO_ReadPin(KEYQ3BASE, KEYQ3PORT) == GPIO_PIN_SET?TRUE:FALSE)
#define GetKEYQ4() 	(HAL_GPIO_ReadPin(KEYQ4BASE, KEYQ4PORT) == GPIO_PIN_SET?TRUE:FALSE)

#define GetKEYQ5() 	(HAL_GPIO_ReadPin(KEYQ5BASE, KEYQ5PORT) == GPIO_PIN_SET?TRUE:FALSE)
#define GetKEYQ6() 	(HAL_GPIO_ReadPin(KEYQ6BASE, KEYQ6PORT) == GPIO_PIN_SET?TRUE:FALSE)

#define GetKEYVT() 	(HAL_GPIO_ReadPin(KEYVTBASE, KEYVTPORT) == GPIO_PIN_SET?TRUE:FALSE)



void motordrive_init(void);
void motordrive_func(void);

void remotectrl_init(void);

void remotectrl_func(void);

#endif


