/*==============================================================================================
								 	 PROJECT YUME Ver.1.0
								  	 USER INTERFACE LIBRARY
==============================================================================================*/



#ifndef YUME_USER_INTERFACE_H
#define YUME_USER_INTERFACE_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "yume_macro_lib.h"
//#include "yume_global_var.h"
#include <yume_systick_delay.h>

/*
 * YUME START BUTTON
 */
#define RCC_AHB1Periph_BUTTON RCC_AHB1Periph_GPIOA
#define BUTTON_PORT GPIOA
#define BUTTON_PIN GPIO_Pin_1
#define YUME_BUTTON_INPUT GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)
#define YUME_BUTTON_CLICKED GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0
#define YUME_BUTTON_UNCLICKED   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)==0

void Button_Init(void);

/*
 * YUME BUZZER
 */
#define RCC_AHB1Periph_BUZZER RCC_AHB1Periph_GPIOD
#define BUZZER_PORT GPIOD
#define BUZZER_PIN GPIO_Pin_14
#define BUZZ_ON GPIO_SetBits(BUZZER_PORT, BUZZER_PIN);
#define BUZZ_OFF GPIO_ResetBits (BUZZER_PORT, BUZZER_PIN);


void Buzzer_Init(void);
void YUME_Buzz(int counter);

/*
 * YUME LED
 */
#define RCC_AHB1Periph_LED RCC_AHB1Periph_GPIOD
#define LED_PORT GPIOD
#define LED_PIN GPIO_Pin_13

#define YUME_LED_ON GPIO_SetBits(LED_PORT, LED_PIN);
#define YUME_LED_OFF GPIO_ResetBits(LED_PORT, LED_PIN);

#define YUME_LED_BLINK_ON TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
#define YUME_LED_BLINK_OFF TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);
void LED_Init(void);

extern uint16_t led_counter;
extern uint16_t led_off_counter;
extern unsigned int UV_state;;
extern unsigned int UV_Lock;


#endif
