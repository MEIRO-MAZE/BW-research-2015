/*****************************************************
Project   : YUME* Main Controller
Version   : 2.0
Year      : 2015
Date      :
Author    : Ryan
University: University of Brawijaya
Department: Electrical Engineering Department, Engineering Faculty
Division  : Legged Fire-Fighting Division (KRPAI)
Comments  : *Dreams/aspirations, in Japanese language

What's New: Optimization from Ver.1.x , YUME System using single STM32 and Rangefinder Controller
			New Pin Mapping for every actuator, sensor, and other peripheral

Board Type: STM32F4 Discovery Board
Chip  Type: STM32F407VG
*****************************************************/


#include "main.h"



void YUME_Initialization(void);
int main(void)
{
	YUME_Initialization();

	mov_static();
	delay_ms(100);

	Sendto_PC(USART1,"System Running... \r ");


	YUME_Sound_Activation();
	YUME_Buzz(4);

	RotSwitch_Sampling();

	while(rot_switch_mode==0)
	{
		RotSwitch_Sampling();
	}

	switch(rot_switch_mode)
	{
		//YUME FAILED INITIALIZATION
		case YUME_MODE_DEFAULT:
			{

			}break;

		//YUME LOCATION-ROOM MODE
		case YUME_MODE_1:
			{
				YUME_Initial_Setup(rot_switch_mode);
				Sendto_PC(USART1,"\r YUME BLUE FIGHTER! \r");

				while(1)
				{
					YUME_BlueFighter();
				}

			}break;

		//YUME RANGEFINDER MONITOR
		case YUME_MODE_2:
			{
				YUME_Initial_Setup(rot_switch_mode);
				Sendto_PC(USART1,"\r YUME RED FIGHTER! \r");
				while(1)
				{
					YUME_RedFighter();
				}
			}break;

		//YUME RUNTIME CALIBRATION
		case YUME_MODE_3:
			{
				YUME_Initial_Setup(rot_switch_mode);
				Sendto_PC(USART1,"\r YUME RUNTIME CALIBRATION! \r");
				while(1)
				{
					YUME_Calibration();
					CMD_ACTUATE();
				}
			}break;

		//YUME TRIAL MODE
		case YUME_MODE_4:
			{

				YUME_Initial_Setup(rot_switch_mode);
				Sendto_PC(USART1,"\r YUME DISPLAY PING STATUS! \r");
				while(1)
				{
					Display_Ping_Status ();
					Send_Ping_Status ();
				}
			}break;

		//YUME TESTING MODE
		case YUME_MODE_5:
			{
				YUME_Initial_Setup(rot_switch_mode);
//				Sendto_PC(USART1,"YUME PUSH-TO-SEND TCS3200 STAT \r");
//
//				while(1)
//				{
//
//					while(1)
//					{
//						PushtoSend_TCS3200_Status();
//					}
//				}

				Sendto_PC(USART1,"YUME FIRE FIGHT!!! \r");
				YUME_LOCATION[LAST_LOC]=LOCATION_CORRIDOR;
				YUME_LOCATION[PRESENT_LOC]=LOCATION_ROOM;
				INITIAL_ZONE=ZONE_BLUE;

				while(1)
				{


						while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
						{
							Sendto_PC(USART1,"YUME ROOM \r");
							YUME_ROOM();
							if(FIRE_PUTOFF==1) break;
							if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
						}

						Sendto_PC(USART1,"YUME SUCCESS! \r");
						while(1)
						{
							mov_static();
							delay_ms(100);
						}
				}

			}break;

		//YUME SEARCH FIRE TEST
		case YUME_MODE_6:
			{

				YUME_Initial_Setup(rot_switch_mode);
				Sendto_PC(USART1,"YUME SEARCH FIRE \r");

				while(1)
				{
					Get_UV();

					while(UV_Lock==0)
					{
						Get_UV();
					}
					while(UV_Lock==1)
					{
						YUME_LED_BLINK_ON;
						YUME_Search_Fire();
						Get_UV();
						if(UV_state==1)
						{
							UV_Lock=0;
							led_off_counter=1;
						}
					}
					led_off_counter=1;
					Sendto_PC(USART1,"\rUV_state: %d \r",UV_state);
					Sendto_PC(USART1,"UV_Lock: %d \r",UV_Lock);
					Sendto_PC(USART1,"led_off_counter: %d \r",led_off_counter);
				}
			}break;
	}
	while(1);

	return 0;
}

void YUME_Initialization(void)
{
	//CLOCK CONFIG
	SystemInit();

	//SYSTICK DELAY INIT
	SysTick_Init();

	//YUME HEXAPOD SERVO INIT
	YUME_Servo_Initialization();

	//LCD INIT
	delay_ms(50);
	lcd_init();
	lcd_cursor_off_blink_off();
	lcd_display_clear();

	lcd_display_clear();
	lcd_gotoxy(2,0);sprintf(lcd,"PROJECT YUME");lcd_putstr(lcd);
	lcd_gotoxy(5,1);sprintf(lcd,"SYSTEM");lcd_putstr(lcd);
	lcd_gotoxy(1,2);sprintf(lcd,"INITIALIZATION");lcd_putstr(lcd);
	lcd_gotoxy(1,3);sprintf(lcd,"BRAWIJAYA 2015");lcd_putstr(lcd);
	delay_ms(500);

	//YUME USART COMMUNICATION PROTOCOLS
	USART1_Init(9600);
	USART2_Init(9600);
	USART3_Init(9600);
	//USART4_Init(9600);


	//YUME USER INTERFACE
	Button_Init();
	Buzzer_Init();
	LED_Init();
	RotSwitch_Init();

	//YUME SOUND ACTIVATION INIT
	YUME_Sound_Activation_Init();

	//YUME UV-TRON INIT
	YUME_UV_TRON_Init();

	//YUME THERMOPILE INIT
	YUME_Thermopile_Init();

	//INFRARED PROXIMITY SENSOR
	IR_Proximity_Init();
	//IR_Interrupt_Init();

//	YUME COLOUR SENSOR
	TCS3200_Init();
//	YUME_TCS3200_Interrupt_Init();
//	YUME_Tracer_Init();
//	Tracer_Decoder_Interrupt_Init();

	//YUME BRUSHLESS DC FAN INITIALIZATION
	YUME_BLDC_Init();

	//YUME PID CONTROLLER INITIALIZATION
	YUME_PID_Init();
	//YUME_PID_Interrupt_Init();

	//IMU MPU6050
	//MPU_6050_Init();

	//YUME SENSOR STAT INTERRUPT
//	YUME_Stat_Interrupt_Init();
	YUME_LED_Interrupt_Init();

	lcd_display_clear();
	lcd_gotoxy(2,0);sprintf(lcd,"PROJECT YUME");lcd_putstr(lcd);
	lcd_gotoxy(5,1);sprintf(lcd,"SYSTEM");lcd_putstr(lcd);
	lcd_gotoxy(1,2);sprintf(lcd,"INITIALIZATION");lcd_putstr(lcd);
	lcd_gotoxy(3,3);sprintf(lcd,"COMPLETED");lcd_putstr(lcd);
	delay_ms(50);

	Sendto_PC(USART1,"PROJECT YUME 2016 \r");
	Sendto_PC(USART1,"ELECTRICAL ENGINEERING - UNIVERSITY OF BRAWIJAYA \r");
	Sendto_PC(USART1,"SYSTEM INITIALIZATION");
	Sendto_PC(USART1,".");
	Sendto_PC(USART1,".");
	Sendto_PC(USART1,". \r");
	Sendto_PC(USART1,"INITIALIZATION COMPLETED \r");

}

