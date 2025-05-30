/*==============================================================================================
								  PROJECT YUME Ver.1.0
							YUME PING))) RANGEFINDER RECEIVER
==============================================================================================*/


#include "yume_ping_receiver.h"

/*=============================== YUME USART COMMUNICATION =====================================*/

void USART3_Init(uint32_t baudrate) //TO RANGEFINDER uCU
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; // Pins 8 (TX) and 9 (RX) are used
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
	GPIO_Init(GPIOD, &GPIO_InitStructure);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_USART3); //
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_USART3);

	USART_InitStructure.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStructure.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART3, &USART_InitStructure);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // enable the USART3 receive interrupt

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		 // we want to configure the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;// this sets the priority group of the USART3 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART3 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff

	USART_Cmd(USART3, ENABLE);
}
void USART3_PutChar(char c)
{
    uint8_t ch;
    ch = c;
    USART_SendData(USART3, (uint8_t) ch);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET)
    {}
}

void usart_puts(char *data)
{
	if(USART_ACTIVE_MODE&&1==1)
	{
		int i=0;
		int n = strlen(data);
		for(i=0;i<n;i++)
		{
			USART3_PutChar(data[i]);
		}
	}
}



/*
void USART3_IRQHandler(void){

	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
	{
		static uint8_t cnt = 0;
		char t = USART3->DR;

		if(strcmp(received_string,"oyi")==0)
		{
			//cetak(USART3,"#");
			switch(angka)
			{
				case 0 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 1 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 2 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 3 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 4 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 5 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 6 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 7 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 8 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 9 : terima[angka+1]=USART3->DR;angka+=1; break;
				case 10 : terima[0]=USART3->DR;angka=0; break;
			}
		}

		else
		{
			if((USART3->DR !=0x0A))
			{
				if(USART3->DR !=0x0D)
				{
				received_string[cnt] = t;
				cnt++;
				}
			}
			else
			{
				received_string[cnt]=0;
				cnt = 0;
			}
		}
	}
}
*/

void USART3_IRQHandler(void)
{
	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
	{
		char t = USART3->DR; // the character from the USART3 data register is saved in t
		char static last_data;

		switch(USART_Count)
		{
			case  1 : Ping[0]=USART3->DR; USART_Count+=1; break;
			case  2 : Ping[1]=USART3->DR; USART_Count+=1; break;
			case  3 : Ping[2]=USART3->DR; USART_Count+=1; break;
			case  4 : Ping[3]=USART3->DR; USART_Count+=1; break;
			case  5 : Ping[4]=USART3->DR; USART_Count+=1; break;
			case  6 : Ping[5]=USART3->DR; USART_Count=811; break;
		}
		if(t==COMMAND_FLAG_B && last_data==COMMAND_FLAG_A) USART_Count=1;
		last_data=t;
	}
}


/*
 * Func  : void Display_Ping_Status (void)
 * Brief : Displaying Ultrasonic rangefinder value sent by PING))) Controller
 * Param : N/A
 * Written By: Ryan (Friday, August 28th, 2015
 * Ver   : 1
 */
void Display_Ping_Status (void)
{
	lcd_display_clear();
	lcd_gotoxy(0,0);sprintf(lcd,"L: %d",Ping[0]);lcd_putstr(lcd);
	lcd_gotoxy(8,0);sprintf(lcd,"R: %d",Ping[1]);lcd_putstr(lcd);
	lcd_gotoxy(0,1);sprintf(lcd,"F: %d",Ping[2]);lcd_putstr(lcd);
	lcd_gotoxy(8,1);sprintf(lcd,"R: %d",Ping[3]);lcd_putstr(lcd);
	lcd_gotoxy(0,2);sprintf(lcd,"AL: %d",Ping[4]);lcd_putstr(lcd);
	lcd_gotoxy(8,2);sprintf(lcd,"AR: %d",Ping[5]);lcd_putstr(lcd);
	//lcd_gotoxy(3,3);sprintf(lcd,"YUME PING");lcd_putstr(lcd);

	delay_ms(50);
}

/*
 * Func  : void Display_Ping_Status (void)
 * Brief : Displaying Ultrasonic rangefinder value sent by PING))) Controller
 * Param : N/A
 * Written By: Ryan (Friday, August 28th, 2015
 * Ver   : 1
 */
void Send_Ping_Status (void)
{
	Sendto_PC(USART1,"YUME Ping Status \r");
	Sendto_PC(USART1,"                     Ping Front: %d \r",Ping[PING_FRONT]);
	Sendto_PC(USART1,"   Ping Askew Left: %d          Ping Askew Right: %d \r",Ping[PING_ASKEW_LEFT],Ping[PING_ASKEW_RIGHT]);
	Sendto_PC(USART1,"Ping Left: %d                          Ping Right: %d \r",Ping[PING_LEFT],Ping[PING_RIGHT]);
	Sendto_PC(USART1,"                     Ping Rear: %d \r",Ping[PING_REAR]);
	delay_ms(60);
}
