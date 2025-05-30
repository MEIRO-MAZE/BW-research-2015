/*==============================================================================================
								  	  PROJECT YUME Ver.1.0
								  HAMAMATSU UV-TRON SENSOR LIBRARY
==============================================================================================*/

#ifndef YUME_UV_TRON_H
#define YUME_UV_TRON_H

//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_rcc.h"
//#include "yume_macro_lib.h"
//#include "yume_global_var.h"
//#include "stm32f4xx_tim.h"
//#include "misc.h"
//#include "yume_systick_delay.h"

#include "main.h"
/*
 * YUME START BUTTON
 */
#define RCC_AHB1Periph_UV RCC_AHB1Periph_GPIOD
#define UV_PORT GPIOD
#define UV_PIN  GPIO_Pin_3

#define UV_READ  GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_3)


extern unsigned int UV_state;
extern unsigned int UV_Lock;
/*
 *  YUME HAMAMATSU UV-TRON GLOBAL VARIABLE
 */
void YUME_UV_TRON_Init(void);
void UV_GPIO_Init(void);
void YUME_Display_UV_Stat(void);

void Get_UV(void);
void IR_Proximity_Init(void);
void IR_Control(void);
void IR_Interrupt_Init(void);


#endif
