/*==============================================================================================
								  PROJECT YUME Ver.1.0
							YUME PING))) RANGEFINDER RECEIVER
==============================================================================================*/

#ifndef YUME_PING_RECEIVER_H
#define YUME_PING_RECEIVER_H

#include "main.h"


//USART COMM MACRO
#define ON 0x01
#define OFF 0x02
#define TO 0x04
#define USART_ACTIVE_MODE 1
#define MAX_STRLEN 10000

//USART COMM VARIABLES
extern char received_string[MAX_STRLEN];
extern char buf[16];
extern char data1_lcd[16];
extern char data2_lcd[16];
extern char tampil_adc[16];
extern unsigned int Ping[9];
extern unsigned int USART_Count;

extern unsigned int Ping[9],USART_Count;
extern unsigned int COMMAND_FLAG_A;
extern unsigned int COMMAND_FLAG_B;
extern unsigned int COMMAND_COMPLETED;
extern unsigned int COMMAND_NOT_RECEIVED;

#define PING_LEFT 0
#define PING_RIGHT 1
#define PING_FRONT 2
#define PING_REAR 3
#define PING_ASKEW_LEFT 4
#define PING_ASKEW_RIGHT 5

//USART COMM FUNCTION PROTOTYPE
void USART3_Init(uint32_t baudrate);

void usart_puts(char *data);

/*
 * USART PING
 */
void Display_Ping_Status (void);
void Send_Ping_Status (void);

#endif


