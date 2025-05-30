/*==============================================================================================
								 	 PROJECT YUME Ver.1.0
								YUME INFRARED PROXIMITY LIBRARY
==============================================================================================*/

#ifndef YUME_INFRARED_PROXIMITY_H
#define YUME_INFRARED_PROXIMITY_H

//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
//#include "yume_macro_lib.h"
//#include "yume_global_var.h"
//#include "stm32f4xx_tim.h"
//#include "misc.h"

#include "main.h"

/*
 * YUME START BUTTON
 */
#define RCC_AHB1Periph_IR RCC_AHB1Periph_GPIOC
#define IR_PORT GPIOC

#define IR_PIN_FRONT_LEFT  GPIO_Pin_1
#define IR_PIN_FRONT_RIGHT GPIO_Pin_2
#define IR_PIN_MID_LEFT   GPIO_Pin_4
#define IR_PIN_MID_RIGHT  GPIO_Pin_5

#define IR_FRONT_LEFT   GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define IR_FRONT_RIGHT  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)
#define IR_MID_LEFT    GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_4)
#define IR_MID_RIGHT   GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_5)

#define IR_DETECTED 0
#define IR_NOT_DETECTED 1

extern int INFRARED[];
void IR_Proximity_Init(void);
void Get_IR(void);
void IR_Interrupt_Init(void);
void Get_IR_Right(void);
void Get_IR_Left(void);


#endif

