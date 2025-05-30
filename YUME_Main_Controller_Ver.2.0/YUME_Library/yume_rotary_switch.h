/*==============================================================================================
								  	  PROJECT YUME Ver.1.0
								  	 YUME ROTARY SWITCH ADC
==============================================================================================*/

#ifndef YUME_ROTARY_SWITCH_H
#define YUME_ROTARY_SWITCH_H

//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_tim.h"
//#include "stm32f4xx_rcc.h"
//#include "stm32f4xx_adc.h"
////#include <stdio.h>
//
//#include "yume_systick_delay.h"
//#include "yume_rotary_switch.h"
//#include "yume_macro_lib.h"
//#include "yume_global_var.h"

#include "main.h"
/*
 * ROTARY SWITCH PINOUT
 */

#define RCC_AHB1Periph_ROT_SWITCH RCC_AHB1Periph_GPIOC
#define ROT_SWITCH_PORT GPIOC
#define ROT_SWITCH_PIN GPIO_Pin_3

extern uint16_t rot_switch_mode;

void RotSwitch_Init(void);
void RotSwitch_Sampling(void);


#endif
