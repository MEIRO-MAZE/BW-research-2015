/*==============================================================================================
								  PROJECT YUME Ver.1.0
							  USART/UART COMMUNICATION LIBRARY
==============================================================================================*/

#ifndef YUME_USART_COMM_H
#define YUME_USART_COMM_H

//STANDARD LIBRARIES
#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"

/*
 *  YUME TYPEDEF
 */

// GPIO TYPEDEF
GPIO_InitTypeDef          	GPIO_InitStructure;

// TIMER & INTERRUPT TYPEDEF
TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
TIM_OCInitTypeDef           TIM_OCInitStructure;
NVIC_InitTypeDef 			NVIC_InitStructure;

//USART COMMNUNICATION TYPEDEF
USART_InitTypeDef 			USART_InitStructure;

#define ON 0x01
#define OFF 0x02
#define TO 0x04
#define USART_ACTIVE_MODE 1
#define MAX_STRLEN 10000

//usart
void print_toPC(USART_TypeDef* USARTx, const char *pFormat, ... );
void Sendto_MainController(USART_TypeDef* USARTx, volatile char *s);
void USART3_Init(uint32_t baudrate);
void USART1_Init(uint32_t baudrate);
void cek_ping_toPC(void);
void cek_ping_1();
void usart_puts(char *data);
void USART_Command_Troubleshoot(void);


#endif
