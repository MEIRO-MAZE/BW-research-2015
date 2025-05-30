/*==============================================================================================
								  	  PROJECT YUME Ver.2.0
								  	 YUME SOUND ACTIVATION LIBRARY
==============================================================================================*/

#ifndef YUME_SOUND_ACTIVATION_H
#define YUME_SOUND_ACTIVATION_H

#include "main.h"

/*
 * SOUND ACTIVATION PINOUT
 */

#define RCC_AHB1Periph_SOUND_ACT RCC_AHB1Periph_GPIOA
#define SOUND_ACT_PORT GPIOA
#define SOUND_ACT_PIN GPIO_Pin_8

extern uint16_t YUME_Sound_Activate;
extern uint16_t YUME_START;

void YUME_Sound_Activation_Init(void);
void YUME_Sound_Activation(void);

#define NO_SIGNAL 0
#define SIGNAL_DETECTED 1
#define SOUND_SCAN   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)

void YUME_Sound_Activation(void);
void YUME_Sound_Activation_Init(void);
#endif
