

#include "yume_usart_comm.h"

void USART1_Init(uint32_t baudrate)
{ //TO SERVO CONTROLLER
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  // Pins 6 (TX) and 7 (RX) are used
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
	GPIO_Init(GPIOB, &GPIO_InitStructure);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_USART1); //
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_USART1);

	USART_InitStructure.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStructure.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART1, &USART_InitStructure);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART3 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	// finally this enables the complete USART1 peripheral
	USART_Cmd(USART1, ENABLE);
}

void USART2_Init(uint32_t baudrate)
{ //TO SERVO CONTROLLER
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  // Pins 6 (TX) and 7 (RX) are used
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);

	USART_InitStructure.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStructure.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART2, &USART_InitStructure);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 // we want to configure the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART3 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	// finally this enables the complete USART3 peripheral
	USART_Cmd(USART2, ENABLE);
}

/*
 *  func : void USART1_IRQHandler(void)
 *  brief: USART1 RX Interupt handler from Main Controller to Servo Controller
 *  param: command_code[0] -> gait_mode code selection
 *  	   command_code[1] -> speed paameter for each gait_mode function
 */
void USART1_IRQHandler(void)
{
	if( USART_GetITStatus(USART1, USART_IT_RXNE) )
	{
		char t = USART1->DR; // the character from the USART3 data register is saved in t
		char static last_data;


		switch(USART_Bluetooth_Count)
		{
			//case  0 : USART_Bluetooth_Count+=1; break;
			case  1 : command_code[0] =USART1->DR; USART_Bluetooth_Count+=1; break;
			case  2 : command_code[1] =USART1->DR; USART_Bluetooth_Count+=1; break;
			case  3 : command_code[2] =USART1->DR; USART_Bluetooth_Count+=1; break;
			case  4 : command_code[3] =USART1->DR; USART_Bluetooth_Count+=1; break;
			case  5 : command_code[4] =USART1->DR; USART_Bluetooth_Count=811; break;
			default : break;
		}

		if(t==COMMAND_FLAG_B && last_data==COMMAND_FLAG_A) USART_Bluetooth_Count=1; //  TERMINATING COMMAND <>
		last_data=t;
	}
}

void Sendto_PC(USART_TypeDef* USARTx, const char *pFormat, ... )
{
    va_list ap;
    char pStr[100];

    va_start(ap, pFormat);
    vsprintf(pStr, pFormat, ap);
    va_end(ap);

    int i=0;
    int n = strlen(pStr);
    for(i=0;i<n;i++)
    {
        USART_SendData(USARTx, (uint8_t)pStr[i]);
        while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}


//
//void USART4_Init(uint32_t baudrate)
//{ //TO SERVO CONTROLLER
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6,ENABLE);
//
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;  // Pins 2 (TX) and 3 (RX) are used
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
//	GPIO_Init(GPIOC, &GPIO_InitStructure);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers
//
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
//	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
//
//	USART_InitStructure.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
//	USART_InitStructure.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
//	USART_Init(USART6, &USART_InitStructure);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting
//
//	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
//
//	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;		 // we want to configure the USART3 interrupts
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART3 interrupts
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		 // this sets the subpriority inside the group
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART3 interrupts are globally enabled
//	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff
//
//	// finally this enables the complete USART3 peripheral
//	USART_Cmd(USART6, ENABLE);
//}

//
//void USART2_Init(uint32_t baudrate)
//{ //TO SERVO CONTROLLER
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
//
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;  // Pins 2 (TX) and 3 (RX) are used
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
//	GPIO_Init(GPIOA, &GPIO_InitStructure);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers
//
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
//
//	USART_InitStructure.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
//	USART_InitStructure.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
//	USART_Init(USART2, &USART_InitStructure);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting
//
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt
//
//	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		 // we want to configure the USART3 interrupts
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART3 interrupts
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		 // this sets the subpriority inside the group
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART3 interrupts are globally enabled
//	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff
//
//	// finally this enables the complete USART3 peripheral
//	USART_Cmd(USART2, ENABLE);
//}

void Command_Gait(USART_TypeDef* USARTx, volatile char *s)
{
	while(*s)
	{
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx,(int)*s);
		*s++;
	}
}

