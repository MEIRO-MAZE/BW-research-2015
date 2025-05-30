/*==============================================================================================
								  PROJECT YUME Ver.1.0
							THERMOPHILE ARRAY AND SERVO LIBRARY
==============================================================================================*/
#ifndef YUME_USART_COMM_H
#define YUME_USART_COMM_H

#include "main.h"

/*
 * YUME USART VARIABLES
 */
extern char command_code[];
extern unsigned int USART_Bluetooth_Count;
extern unsigned int COMMAND_FLAG_A;
extern unsigned int COMMAND_FLAG_B;
extern unsigned int COMMAND_COMPLETED;
extern unsigned int COMMAND_NOT_RECEIVED;

void USART1_Init(uint32_t baudrate);
void USART2_Init(uint32_t baudrate);
void Command_Gait(USART_TypeDef* USARTx, volatile char *s);

void Sendto_PC(USART_TypeDef* USARTx, const char *pFormat, ... );

#endif
