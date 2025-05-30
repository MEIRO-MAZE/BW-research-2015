/*****************************************************
Project   : YUME* Servo Controller / Hexapod Gait Generator
Version   : 2.0
Year      : 2015
Date      :
Author    : Ryan
University: University of Brawijaya
Department: Electrical Engineering Department, Engineering Faculty
Division  : Legged Fire-Fighting Division (KRPAI)
Comments  : *Dreams/aspirations, in Japanese language

Board Type: STM32F4 Discovery Board
Chip      : STM32F407VG
*****************************************************/


#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "math.h"
#include "stm32f4xx_usart.h"

//YUME PROJECT USER DEFINED LIBRARY
#include "yume_macro_lib.h"
#include "yume_global_var.h"
#include "yume_hexapod_anatomy.h"
#include "yume_systick_delay.h"


//YUME USART COMMUNICATION PROTOCOLS
#include "yume_usart_comm.h"
#include "yume_usart_var.h"

//YUME LCD
#include "yume_lcd.h"

void YUME_Initialization(void);

//INVERSE KINEMATICS PROTOTYPE
void TIM_Config(void);
void Interrupt_Config(void);
void Servo_GPIO_Init(void);
void generate_movement_data(int deflection_angle, int translation, float clearance);
void inv_kine(char leg_pos,float x,float w,float z);
void servo_lin_func(int ch_num,float value);
void gait_mode(int menu, int SPEED);
void awal();
void tahap();
void tahap1();
void tahap2();
void target();

void mov_static(void);
void mov_search_fire(int SEARCH_FIRE_CMD);
void mov_fwd_5cm (int distance, int SPEED);
void mov_fwd_3cm (int distance, int SPEED); //NEW FUNCTION
void mov_bwd(int distance, int SPEED);
//void mov_rot_right_90_dgr(int angle, int SPEED, int STEP);
//void mov_rot_left_90_dgr(int angle, int SPEED, int STEP);
void mov_rot_right(int SPEED, int STEP);
void mov_rot_left(int SPEED, int STEP);
void mov_curve_right(int SPEED, int STEP);
void mov_curve_left(int SPEED, int STEP);
void mov_askew_right(int SPEED, int STEP);
void mov_askew_right_transition(void);
void mov_askew_left(int SPEED, int STEP);
void mov_askew_left_transition(void);
void mov_celebrate(void);
void mov_fwd_ripple_gait(int SPEED);
void mov_bwd_ripple_gait(int SPEED);
void mov_fwd_wave_gait(int SPEED);
void mov_toggle_clearance(void);

void mov_deflect_right(int SPEED, int STEP);
void mov_deflect_left(int SPEED, int STEP);

void yume_demo(void);

//YUME USART VARIABLES
char received_string[MAX_STRLEN],buf[16],data1_lcd[16],data2_lcd[16],tampil_adc[16];
int usart_count=0,DATA;
int terima[9],angka=0,DATA,sudut_servo=0;
char USART_send[16];
char command_code[3];
char data_get;

unsigned int COMMAND_FLAG_A = 0x3C;
unsigned int COMMAND_FLAG_B = 0x3E;
unsigned int COMMAND_COMPLETED = 0x7C;
unsigned int COMMAND_NOT_RECEIVED = 0x3F;

//SERVO CONTROLLER COMMAND CODE
#define MOV_STATIC      			0x41
#define MOV_SEARCH_FIRE 			0x42
#define MOV_FWD         			0x43
#define MOV_BWD         			0x44
#define MOV_ROT_RIGHT   			0x45
#define MOV_ROT_LEFT    			0x46
#define MOV_CURVE_RIGHT 			0x47
#define MOV_CURVE_LEFT  			0x48
#define MOV_ASKEW_RIGHT_TRANSITION  0x49
#define MOV_ASKEW_RIGHT 			0x4A
#define MOV_ASKEW_LEFT_TRANSITION   0x4B
#define MOV_ASKEW_LEFT  			0x4C
#define MOV_DEFLECT_RIGHT           0x4D
#define MOV_DEFLECT_LEFT            0x4E
#define MOV_CELEBRATE   			0x4F


//SERVO CONTROLLER SPEED PARAMETER
#define SPEED_SLOW  0x31
#define SPEED_MED   0x32
#define SPEED_FAST  0x33

//SERVO CONTROLLER STEP PARAMETER
#define STEP_NARROW  0x31
#define STEP_FAR     0x32

////SEARCH FIRE COMMAND
//#define SEARCH_FIRE_RESET 0x31
//#define SEARCH_FIRE_GO    0x32
//SEARCH FIRE COMMAND
#define SEARCH_FIRE_RESET = 0x31;
#define SEARCH_FIRE_GO    = 0x32;
#define SRC_FR_A 0x41 //1
#define SRC_FR_B 0x42 //2
#define SRC_FR_C 0x43 //3
#define SRC_FR_D 0x44 //4
#define SRC_FR_E 0x45 //5
#define SRC_FR_F 0x46 //6
#define SRC_FR_G 0x47 //7
#define SRC_FR_H 0x48 //8
#define SRC_FR_I 0x49 //9
#define SRC_FR_J 0x4A //10
#define SRC_FR_K 0x4B //11
#define SRC_FR_L 0x4C //12
#define SRC_FR_M 0x4D //13
#define SRC_FR_N 0x4E //14
#define SRC_FR_O 0x4F //15
#define SRC_FR_P 0x50 //16
//#define SRC_FR_Q 0x51 //17



int main(void)
{
//	// Disable read out protection
//	FLASH_OB_Unlock();
//	FLASH_OB_RDPConfig(OB_RDP_Level_0);
//	FLASH_OB_Launch();
//	FLASH_OB_Lock();
//
//	// Restart platform
//	NVIC_SystemReset();


	YUME_Initialization();
	//USART3_Init(9600); // initialize USART3 @ 9600 baud
	//USART1_Init(9600);

	//init_IO(RCC_AHB1Periph_GPIOD, Pin_12 | Pin_13 | Pin_14 | Pin_15, GPIO_Mode_OUT , GPIO_OType_PP , GPIO_PuPd_NOPULL );
	//init_IO(RCC_AHB1Periph_GPIOA, Pin_0, GPIO_Mode_IN , GPIO_OType_PP , GPIO_PuPd_NOPULL );

	//int temporary;
	//mov_static();
	mov_static();
	delay_ms(1000);
    while(1)
    {
    	//mov_deflect_right(MED, STEP_FAR);
    	//mov_deflect_left(MED, STEP_FAR);
    	//mov_static();
    	//mov_fwd_5cm (5, MED);



    	//mov_deflect_right(SLOW, STEP_FAR);
    	//USART_Command_Troubleshoot();
    	Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);

    	switch(command_code[0])
    	{
    		case MOV_STATIC:
    			{
    				mov_static();delay_ms(1000);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    			}break;

    		case MOV_SEARCH_FIRE:
    			{
    				switch(command_code[1])
    				{
    					case SRC_FR_A : mov_search_fire(SRC_FR_A);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_B : mov_search_fire(SRC_FR_B);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_C : mov_search_fire(SRC_FR_C);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_D : mov_search_fire(SRC_FR_D);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_E : mov_search_fire(SRC_FR_E);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_F : mov_search_fire(SRC_FR_F);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_G : mov_search_fire(SRC_FR_G);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_H : mov_search_fire(SRC_FR_H);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_I : mov_search_fire(SRC_FR_I);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_J : mov_search_fire(SRC_FR_J);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_K : mov_search_fire(SRC_FR_K);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_L : mov_search_fire(SRC_FR_L);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					case SRC_FR_M : mov_search_fire(SRC_FR_M);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					//case SRC_FR_N : mov_search_fire(SRC_FR_N);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					///case SRC_FR_O : mov_search_fire(SRC_FR_O);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					//case SRC_FR_P : mov_search_fire(SRC_FR_P);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    					//case SRC_FR_Q : mov_search_fire(SRC_FR_Q);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    				}
    			}break;
    		case MOV_FWD:
    		    {
    		    	switch(command_code[1])
    		    	{
    		    		case SPEED_SLOW  :
    		    			{
    		    				switch (command_code[2])
    		    				{
    		    					case STEP_NARROW: break;
    		    					case STEP_FAR   : mov_fwd_5cm(5, SLOW); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    		    				}
    		    			}break;

    		    		case SPEED_MED   :
    		    			{
    		    				switch (command_code[2])
    		    				{
    								case STEP_NARROW: break;
    								case STEP_FAR   : mov_fwd_5cm(5, MED); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    		    				}
    		    			}break;
    		    		case SPEED_FAST  :
    		    			{
    		    				switch (command_code[2])
    		    				{
    								case STEP_NARROW: break;
    								case STEP_FAR   : mov_fwd_5cm(5, FAST); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    		    				}
    		    			}break;
    		    	}break;
    		    }break;
    		case MOV_BWD:
    		    {
    		    	switch(command_code[1])
    		    	{
    		    		case SPEED_SLOW  :
    		    			{
    		    				switch (command_code[2])
    		    				{
    		    					case STEP_NARROW: break;
    		    					case STEP_FAR   : mov_bwd(5, SLOW); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    		    				}
    		    			}break;

    		    		case SPEED_MED   :
    		    			{
    		    				switch (command_code[2])
    		    				{
    								case STEP_NARROW: break;
    								case STEP_FAR   : mov_bwd(5, MED); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    		    				}
    		    			}break;
    		    		case SPEED_FAST  :
    		    			{
    		    				switch (command_code[2])
    		    				{
    								case STEP_NARROW: break;
    								case STEP_FAR   : mov_bwd(5, FAST); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    		    				}
    		    			}break;
    		    	}break;
    		    }break;
    		case MOV_ROT_RIGHT:
		    	{
		    		switch(command_code[1])
		    		{
		    			case SPEED_SLOW  :
		    				{
		    					switch (command_code[2])
		    					{
		    						case STEP_NARROW: mov_rot_right(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    						case STEP_FAR   : mov_rot_right(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;

		    			case SPEED_MED   :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_rot_right(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_rot_right(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    			case SPEED_FAST  :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_rot_right(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_rot_right(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    		}break;
		    	}break;

    		case MOV_ROT_LEFT:
		    	{
		    		switch(command_code[1])
		    		{
		    			case SPEED_SLOW  :
		    				{
		    					switch (command_code[2])
		    					{
		    						case STEP_NARROW: mov_rot_left(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    						case STEP_FAR   : mov_rot_left(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;

		    			case SPEED_MED   :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_rot_left(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_rot_left(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    			case SPEED_FAST  :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_rot_left(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_rot_left(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    		}break;
		    	}break;

    		case MOV_CURVE_RIGHT:
		    	{
		    		switch(command_code[1])
		    		{
		    			case SPEED_SLOW  :
		    				{
		    					switch (command_code[2])
		    					{
		    						case STEP_NARROW: mov_curve_right(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    						case STEP_FAR   : mov_curve_right(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;

		    			case SPEED_MED   :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_curve_right(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_curve_right(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    			case SPEED_FAST  :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_curve_right(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_curve_right(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    		}break;
		    	}break;

    		case MOV_CURVE_LEFT:
		    	{
		    		switch(command_code[1])
		    		{
		    			case SPEED_SLOW  :
		    				{
		    					switch (command_code[2])
		    					{
		    						case STEP_NARROW: mov_curve_left(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    						case STEP_FAR   : mov_curve_left(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;

		    			case SPEED_MED   :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_curve_left(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_curve_left(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    			case SPEED_FAST  :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_curve_left(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_curve_left(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    		}break;
		    	}break;

    		case MOV_ASKEW_RIGHT_TRANSITION:
		    	{
					mov_askew_right_transition();command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    	}break;

    		case MOV_ASKEW_RIGHT:
		    	{
		    		switch(command_code[1])
		    		{
		    			case SPEED_SLOW  :
		    				{
		    					switch (command_code[2])
		    					{
		    						case STEP_NARROW: mov_askew_right(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    						case STEP_FAR   : mov_askew_right(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;

		    			case SPEED_MED   :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_askew_right(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_askew_right(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    			case SPEED_FAST  :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_askew_right(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_askew_right(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    		}break;
		    	}break;

    		case MOV_ASKEW_LEFT_TRANSITION:
		    	{
					mov_askew_left_transition();command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    	}break;

    		case MOV_ASKEW_LEFT:
		    	{
		    		switch(command_code[1])
		    		{
		    			case SPEED_SLOW  :
		    				{
		    					switch (command_code[2])
		    					{
		    						case STEP_NARROW: mov_askew_left(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    						case STEP_FAR   : mov_askew_left(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;

		    			case SPEED_MED   :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_askew_left(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_askew_left(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    			case SPEED_FAST  :
		    				{
		    					switch (command_code[2])
		    					{
									case STEP_NARROW: mov_askew_left(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_askew_left(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
		    					}
		    				}break;
		    		}break;
		    	}break;

    		case MOV_DEFLECT_RIGHT:
	    		{
	    			switch(command_code[1])
	    			{
	    				case SPEED_SLOW  :
	    					{
	    						switch (command_code[2])
	    						{
	    							case STEP_NARROW: mov_deflect_right(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    							case STEP_FAR   : mov_deflect_right(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    						}
	    					}break;

	    				case SPEED_MED   :
	    					{
	    						switch (command_code[2])
	    						{
									case STEP_NARROW: mov_deflect_right(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_deflect_right(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    						}
	    					}break;
	    				case SPEED_FAST  :
	    					{
	    						switch (command_code[2])
	    						{
									case STEP_NARROW: mov_deflect_right(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_deflect_right(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    						}
	    					}break;
	    			}break;
	    		}break;
    		case MOV_DEFLECT_LEFT:
	    		{
	    			switch(command_code[1])
	    			{
	    				case SPEED_SLOW  :
	    					{
	    						switch (command_code[2])
	    						{
	    							case STEP_NARROW: mov_deflect_left(SLOW,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    							case STEP_FAR   : mov_deflect_left(SLOW,FAR); command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    						}
	    					}break;

	    				case SPEED_MED   :
	    					{
	    						switch (command_code[2])
	    						{
									case STEP_NARROW: mov_deflect_left(MED,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_deflect_left(MED,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    						}
	    					}break;
	    				case SPEED_FAST  :
	    					{
	    						switch (command_code[2])
	    						{
									case STEP_NARROW: mov_deflect_left(FAST,CLOSE);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
									case STEP_FAR   : mov_deflect_left(FAST,FAR);command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
	    						}
	    					}break;
	    			}break;
	    		}break;
    		case MOV_CELEBRATE:
    			{
    				mov_celebrate();command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);delay_ms(50);break;
    			}break;

    		default: Sendto_MainController(USART3,&COMMAND_NOT_RECEIVED);delay_ms(50);break;
    	}


    	//delay_ms(300);
    	//mov_static();
    	//delay_ms(1000);

    	//mov_fwd_5cm(20, FAST);
    	//mov_search_fire();
    	//mov_celebrate(3);
    	//mov_bwd(20, MED);
    	//mov_rot_right_90_dgr(90,MED,FAR);
    	//mov_rot_left_90_dgr(90,MED,FAR);

    	//mov_askew_left(20,MED,FAR);
    	//mov_askew_right(20,MED,FAR);


    	//temporary=terima[7];
    	//GPIO_ResetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);
    	//mov_static();
    	//delay_ms(300);
    	//cek_ping();
    	//if(terima[7]>=20) GPIO_SetBits(GPIOD,GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15);
    	//mov_search_fire();
    	//mov_fwd_5cm(30, SLOW);
    	//mov_fwd_5cm(30, MED);
    	//mov_fwd_5cm(30, FAST); //home lorong sampai tengah crossroad 15.03 second
    	//mov_fwd_5cm(30, FAST_LEVEL_2); //home lorong sampai tengah crossroad 11.08 second
    	//mov_fwd_5cm(30, FAST_LEVEL_3); //home lorong sampai tengah crossroad 10 second

    	//cek_ping();
    	//delay_ms(100);
    	///if(terima[7]>=50) mov_fwd_5cm(20, SLOW);




    	//mov_search_fire();
    	//mov_fwd_5cm(20, SLOW);

    	//mov_toggle_clearance();
    	//mov_celebrate(2);
    	//mov_search_fire();



/*
	    inv_kine(front_left,110,11,leg-5);
	    inv_kine(front_right,30,12,leg-6);
	    inv_kine(middle_left,100,7,leg);
	    inv_kine(middle_right,20,7,leg);
	    inv_kine(rear_left,160,10,leg-3);
	    inv_kine(rear_right,80,10,leg-3);
	    delay_ms(400);

    	inv_kine(front_left,25,12,leg-5);
	    inv_kine(front_right,110,11,leg-6);
	    inv_kine(middle_left,100,7,leg);
	    inv_kine(middle_right,20,7,leg);
	    inv_kine(rear_left,160,10,leg-3);
	    inv_kine(rear_right,80,10,leg-3);
	    delay_ms(400);

	    //STATIC
	    inv_kine(front_left,110,11,leg-5);
	    inv_kine(front_right,30,12,leg-6);
	    inv_kine(middle_left,60,7,leg);
	    inv_kine(middle_right,60,7,leg);
	    inv_kine(rear_left,120,10,leg-3);
	    inv_kine(rear_right,120,10,leg-3);
	    delay_ms(400);

    	inv_kine(front_left,25,12,leg-5);
	    inv_kine(front_right,110,11,leg-6);
	    inv_kine(middle_left,60,7,leg);
	    inv_kine(middle_right,60,7,leg);
	    inv_kine(rear_left,120,10,leg-3);
	    inv_kine(rear_right,120,10,leg-3);
	    delay_ms(400);

	    //RIGHT
	    inv_kine(front_left,110,11,leg-5);
	    inv_kine(front_right,30,12,leg-6);
	    inv_kine(middle_left,20,7,leg);
	    inv_kine(middle_right,100,7,leg);
	    inv_kine(rear_left,80,10,leg-3);
	    inv_kine(rear_right,160,10,leg-3);
	    delay_ms(400);

    	inv_kine(front_left,25,12,leg-5);
	    inv_kine(front_right,110,11,leg-6);
	    inv_kine(middle_left,20,7,leg);
	    inv_kine(middle_right,100,7,leg);
	    inv_kine(rear_left,80,10,leg-3);
	    inv_kine(rear_right,160,10,leg-3);
	    delay_ms(400);



    	mov_fwd_5cm(80, FAST_LEVEL_2);
    	mov_turn_right(MED, FAR);
    	mov_fwd_5cm(20, FAST_LEVEL_2);
    	mov_turn_right(MED, FAR);
    	mov_fwd_5cm(30, FAST_LEVEL_2);
    	gait_mode(SEARCH_FIRE, FAST);
    	gait_mode(SEARCH_FIRE, FAST);
    	mov_rot_right_90_dgr(180, MED, FAR);
    	mov_fwd_5cm(30, FAST_LEVEL_2);
    	while(1) {
    				mov_static();
    				delay_ms(5000);
    			 }
    	//break;

    	//mov_fwd_wave_gait(30, MED);
    	//mov_fwd_5cm(30, SLOW);
    	//mov_fwd_5cm(30, MED);
    	//mov_fwd_5cm(30, FAST); //home lorong sampai tengah crossroad 15.03 second
    	//mov_fwd_5cm(30, FAST_LEVEL_2); //home lorong sampai tengah crossroad 11.08 second
    	//mov_fwd_5cm(30, FAST_LEVEL_3); //home lorong sampai tengah crossroad 10 second
    	//FUNGSI PERGERAKAN YANG SUDAH BISA DIPAKAI
    	//gait_mode(SEARCH_FIRE, FAST);
    	//mov_fwd_5cm(20, FAST);
    	//mov_fwd_3cm(20, FAST);
    	//mov_rot_left_90_dgr(90, MED, CLOSE);
    	//mov_rot_left_90_dgr(90, MED, FAR);
    	//mov_rot_right_90_dgr(90, MED, CLOSE);
    	//mov_rot_right_90_dgr(90, MED, FAR);
    	//mov_turn_left(MED, FAR);
    	//mov_turn_left(MED, CLOSE);
    	//mov_turn_right(MED, FAR);
    	//mov_turn_right(MED, CLOSE);

    	//mov_askew_right(10, MED, FAR);
    	//mov_askew_left(10, MED, FAR);

    	//mov_bwd(40, SLOW);
	*/
    }
}

void YUME_Initialization(void)
{
	SystemInit();
	Servo_GPIO_Init();
	Interrupt_Config();
	TIM_Config();
	SysTick_Init();

	USART3_Init(9600);
	USART1_Init(9600);

	//USER BUTTON INITIALIZATION
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*
 //SIMPLE BUT NOT ACCURATE DELAY
void delay_ms(__IO uint32_t nCount)
	{Count = nCount * 15272;
	  while(Count--)
	  {
	  }
	}

*/

void TIM_Config()
{
	 //PrescalerValue = (uint32_t) ((SystemCoreClock / 2) / 9830250) - 1;

	 /* Time base configuration */
	 TIM_TimeBaseStructure.TIM_Period = 65535;//10000;//65535; /*nilai mak utk timer=16bit*/
	 TIM_TimeBaseStructure.TIM_Prescaler = 21;
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	 /* Prescaler configuration */
	 //TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
}

void Interrupt_Config()
{
	 NVIC_InitTypeDef NVIC_InitStructure;

	 /* TIM3 clock enable */
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	 /* Enable the TIM3 gloabal Interrupt */
	 NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);

	 /* Output Compare Timing Mode configuration: Channel1 */
	 TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
	 TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Disable);

	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
	 TIM_OC2Init(TIM3, &TIM_OCInitStructure);

	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Disable);

	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
	 TIM_OC3Init(TIM3, &TIM_OCInitStructure);

	 TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Disable);

	 //TIM_ClearOC1Ref(TIM3, TIM_OCClear_Enable);
	 //TIM_ClearOC2Ref(TIM3, TIM_OCClear_Enable);
	 //TIM_ForcedOC1Config(TIM3, TIM_ForcedAction_Active);
	 /* TIM Interrupts enable */
	 TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	 TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	 TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
	 TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);

	 /* TIM3 enable counter */
	 TIM_Cmd(TIM3, ENABLE);
}

void Servo_GPIO_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

}

void TIM3_IRQHandler(void)
{

	 if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	  {
		 if(++index_servo>=8) {index_servo=0;}

		 TIM_ClearITPendingBit(TIM3, TIM_IT_Update);

		 switch (index_servo)
		 {

		 	 	 case 0:
		 		        GPIO_SetBits(ch_0);
		 		 	    GPIO_SetBits(ch_6);
		 		 	    GPIO_SetBits(ch_12);
		 		 	    break;
		 		 case 1:
		 	 	    	GPIO_SetBits(ch_1);
		 	 	    	GPIO_SetBits(ch_7);
		 	 	    	GPIO_SetBits(ch_13);
		 	 	    	break;
		 		 case 2:
		 	 	    	GPIO_SetBits(ch_2);
		 	 	    	GPIO_SetBits(ch_8);
		 	 	    	GPIO_SetBits(ch_14);
		 	 	    	break;
		 		 case 3:
		 	 	    	GPIO_SetBits(ch_3);
		 	 	    	GPIO_SetBits(ch_9);
		 	 	    	GPIO_SetBits(ch_15);
		 	 	    	break;
		 		 case 4:
		 	 	    	GPIO_SetBits(ch_4);
		 	 	    	GPIO_SetBits(ch_10);
		 	 	    	GPIO_SetBits(ch_16);
		 	 	    	break;
		 		 case 5:
		 	    		GPIO_SetBits(ch_5);
		 	    		GPIO_SetBits(ch_11);
		 	    		GPIO_SetBits(ch_17);
		 	    		break;
		 }
		 if(index_servo>5)
		 {index_bantu=0;}
		 else if(index_servo<=5)
		 {index_bantu=index_servo;}
		 TIM3 -> CCR1=sudut[index_bantu];
		 TIM3 -> CCR2=sudut[index_bantu+6];
		 TIM3 -> CCR3=sudut[index_bantu+12];

		 //index_servo++;
		 //if(index_servo>=5) {index_servo=0;}
	  }
	 else if (TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
							{  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		 	 	 	 	 	 	 switch (index_servo)
		 	 	 	 	 	 	 {
		 	 	 	 	 	 case 0: GPIO_ResetBits(ch_0);break;
		 	 	 	 	 	 case 1: GPIO_ResetBits(ch_1);break;
		 	 	 	 	 	 case 2: GPIO_ResetBits(ch_2);break;
		 	 	 	 	 	 case 3: GPIO_ResetBits(ch_3);break;
		 	 	 	 	 	 case 4: GPIO_ResetBits(ch_4);break;
		 	 	 	 	 	 case 5: GPIO_ResetBits(ch_5);break;
		 	 	 	 	 	 	 }
							}
	 else if (TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
							{  TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);
			 	 	 	 	 	 switch (index_servo)
			 	 	 	 	 	 {
			 	 	 	 	 case 0: GPIO_ResetBits(ch_6);break;
			 	 	 	 	 case 1: GPIO_ResetBits(ch_7);break;
			 	 	 	 	 case 2: GPIO_ResetBits(ch_8);break;
			 	 	 	 	 case 3: GPIO_ResetBits(ch_9);break;
			 	 	 	 	 case 4: GPIO_ResetBits(ch_10);break;
			 	 	 	 	 case 5: GPIO_ResetBits(ch_11);break;
			 	 	 	 	 	 }
							}
	 else if (TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
	 						{  TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);
	 			 	 	 	 	 switch (index_servo)
	 			 	 	 	 	 {
	 			 	 	 	 case 0:  GPIO_ResetBits(ch_12);break;
	 			 	 	 	 case 1:  GPIO_ResetBits(ch_13);break;
	 			 	 	 	 case 2:  GPIO_ResetBits(ch_14);break;
	 			 	 	 	 case 3:  GPIO_ResetBits(ch_15);break;
	 			 	 	 	 case 4:  GPIO_ResetBits(ch_16);break;
	 			 	 	 	 case 5:  GPIO_ResetBits(ch_17);break;
	 			 	 	 	 	 }
	 						}
}


void generate_movement_data(int deflection_angle, int translation, float clearance)
{
	leg=clearance;
	translation -=1;
    A_body = (float) translation;
    for(i=0;i<6;i++)
	{
	  ww[i] = (int)(alpha[i] - deflection_angle);			//hitung gamma = alpha - psi
	  //printf("%d \n",param->gamma);
	  // hitung beta tiap kaki
	  temp_var  = (float) (A_body * cos(ww[i] / rad2deg));	//temp_var = Ab*cos(gamma)
	  temp_var  = (float) (leg_norm[i] - temp_var);				//temp_var = L2 - temp_var
	  temp_var2 = (float) (A_body * sin(ww[i] / rad2deg));	//temp_var2 = Ab*sin(gamma)
	  temp_var  = (float) (temp_var2 / temp_var);				//var = temp_var2 / temp_var
	  temp_var  = (float) atan(temp_var);						//temp_var = arctan (temp_var)
	  beta[i] = (temp_var*rad2deg);							//beta[i] = temp_var
	  // hitung panjang kaki (L2')
	  temp_var  = (float) sin(ww[i] / rad2deg);				//temp_var = sin(gamma);
	  temp_var  = (float) (A_body * temp_var);					//temp_var = Ab * temp_var;
	  temp_var2 = (float) sin(beta[i] / rad2deg);				//temp_var2 = sin(alpha);
	  temp_var  = (float) (temp_var / temp_var2);				//temp_var  = temp_var/temp_var2
	  leg_next[i] = temp_var-7;									// leg_next[i] = temp_var

	}
}

void inv_kine(char leg_pos,float x,float w, float z)
{
	temp_angle.teta1  = x;

	Lsem = w;//(cos(temp_angle.teta1));
	Lsem -= lj;
	c1 = (la*la) + (lb*lb);
	c2 = 2*la*lb;
	temp_angle.teta3 = acos(((((Lsem*Lsem)+(z*z))-c1)/c2));

	c3=la+(lb*cos(temp_angle.teta3));
	c4=lb*sin(temp_angle.teta3);
	temp_angle.teta2 = atan((((z*c3)-(Lsem*c4))/((Lsem*c3)+(z*c4))));

	temp_angle.teta3 *= rad2deg;
	temp_angle.teta2 *= rad2deg;

	switch(leg_pos)
	{
		case front_left      :   servo_lin_func(0, temp_angle.teta1);
                            	 servo_lin_func(1, temp_angle.teta2);
                            	 servo_lin_func(2, temp_angle.teta3);
                            	 break;
		case front_right     :   servo_lin_func(9,temp_angle.teta1);
                            	 servo_lin_func(10,temp_angle.teta2);
                            	 servo_lin_func(11,temp_angle.teta3);
                            	 break;
		case middle_left     :   servo_lin_func(3,temp_angle.teta1);
                            	 servo_lin_func(4,temp_angle.teta2);
                            	 servo_lin_func(5,temp_angle.teta3);
                            	 break;
		case middle_right    :   servo_lin_func(12,temp_angle.teta1);
								 servo_lin_func(13,temp_angle.teta2);
								 servo_lin_func(14,temp_angle.teta3);
								 break;
		case rear_left       :   servo_lin_func(6,temp_angle.teta1);
                            	 servo_lin_func(7,temp_angle.teta2);
                            	 servo_lin_func(8,temp_angle.teta3);
                            	 break;
		case rear_right      :   servo_lin_func(15,temp_angle.teta1);
                            	 servo_lin_func(16,temp_angle.teta2);
                            	 servo_lin_func(17,temp_angle.teta3);
                            	 break;
	}
}

void servo_lin_func(int ch_num,float value)
{
	sudut[ch_num] =(long int)((A_const[ch_num]*(value+correct[ch_num]))+ B_const[ch_num]);
	//sudut[ch_num] = ((A_const[ch_num]*value)+B_const[ch_num]);
}


/* func : void gait_mode(int menu, int *SPEED)
 * brief: toggle the hexapod gait's mode
 * param: int menu (desc. below),
 * int *SPEED (SLOW=250, MED_SLOW=200, MED=150, FAST=130, ULTRA_FAST=50}
 * HEXAPOD LOCOMOTION'S MODE
 * gait_mode(0/STATIC_LOW_CLEARANCE)    = Static Gait, LOW Clearance /UNUSED
 * gait_mode(1/STATIC_MED_CLEARANCE)    = Static Gait, MED Clearance /UNUSED
 * gait_mode(2/STATIC_HIGH_CLEARANCE)   = Static Gait, HIGH Clearance
 * gait_mode(3/SEARCH_FIRE)             = Search Fire
 * gait_mode(4/FORWARD)                 = Forward Kinematics, Tripod Gait
 * gait_mode(5/BACKWARD) 			    = Backward Kinematics, Tripod Gait
 * gait_mode(6)/ROTATE_RIGHT            = Rotate Right Kinematics
 * gait_mode(7)/ROTATE_LEFT             = Rotate Left Kinemtics
 * gait_mode(8)/TURN_RIGHT				= Turn Right Kinematics
 * gai_mode(9)/TURN_LEFT				= Turn Left Kinematics
 *
 */
//void gait_mode(int menu, int SPEED)
//{
//	switch (menu)
//	{
//		case 0:
//				generate_movement_data(0,0,8);
//				delay_ms(50);
//				inv_kine(front_left,90,10,leg-2);
//				inv_kine(front_right,90,10,leg-2);
//				inv_kine(middle_left,90,10,leg-1);
//				inv_kine(middle_right,90,7,leg-1);
//				inv_kine(rear_left,90,9.5,leg-1.5);
//				inv_kine(rear_right,90,11,leg-2);
//				break;
//
//		case 1:
//			    generate_movement_data(0,0,9);
//			    delay_ms(50);
//			    inv_kine(front_left,90,10,leg-1);
//			    inv_kine(front_right,90,9,leg-1);
//			    inv_kine(middle_left,90,9,leg);
//			    inv_kine(middle_right,90,7,leg);
//			    inv_kine(rear_left,90,8.5,leg-0.5);
//			    inv_kine(rear_right,90,10,leg-1);
//			    break;
//
//		case 2: //STATIC GAIT HIGH CLEARANCE (DEFAULT)
//
//				mov_static();
//			    break;
//
//		case 3: //SEARCH FIRE
//
//				mov_search_fire();
//			    break;
//
//		case 4: //FORWARD LOCOMOTION
//				//Configured for the new YUME's mechanical
//				//Last Revised By: Ryan (Tuesday, March 31st, 2015
//
//				mov_fwd_5cm(10, SPEED);
//				break;
//
//		case 5: //BACKWARD LOCOMOTION
//				//LAST REVISED BY: RYAN (TUE, MARCH 3, 2015)
//
//				mov_bwd(10, SPEED);
//				break;
//
//		case 6: //ROTATE RIGHT LOCOMOTION. LAST REVISED BY: RYAN (THU, APRIL 2nd, 2015)
//
//				mov_rot_right_90_dgr(90, MED, FAR);
//	    		break;
//
//		case 7: //ROTATE LEFT LOCOMOTION. LAST REVISED BY: RYAN (THU, MARCH 5th, 2015)
//
//				mov_rot_left_90_dgr(90, MED, FAR);
//				break;
//
//		case 8: //CURVE RIGHT
//
//				mov_curve_right(MED, FAR);
//				break;
//
//		case 9: //CURVE LEFT
//
//				mov_curve_left(MED, FAR);
//				break;
//
//		case 10: //ASKEW RIGHT
//
//				mov_askew_right(10, MED, FAR);
//		    	break;
//
//		case 11: //ASKEW LEFT
//
//				mov_askew_left(10, MED, FAR);
//				break;
//
//		case 12: //FORWARD KINEMATICS USING RIPPLE GAIT
//
//				mov_fwd_ripple_gait(10, MED);
//    			break;
//
//		case 13: //BACKWARD KINEMATICS USING RIPPLE GAIT
//
//				mov_bwd_ripple_gait(10, MED);
//				break;
//
//		case 14: //FOWARD KINEMATICS USING WAVE GAIT
//
//				mov_fwd_wave_gait(10, MED);
//	    		break;
//
//		default: break;
//
//	}
//}

void awal()
{
	inv_kine(front_left,60,8,12);

	for(b=0;b<18;b++)
	{
		sudut_awal[b]=sudut[b];
	}
}

void tahap1()
{
	inv_kine(front_left,90+beta[0],7+(leg_next[0]),11);

	for(b=0;b<18;b++)
	{
		sudut_tahap1[b]=sudut[b];
	}
}

void tahap2()
{
	inv_kine(front_left,60,8,10);

	for(b=0;b<18;b++)
	{
		sudut_tahap2[b]=sudut[b];
	}
}

void target()
{
	inv_kine(front_left,90+beta[0],7+(leg_next[0]),13);

	for(b=0;b<18;b++)
	{
		sudut_target[b]=sudut[b];
	}
}

void tahap()
{
	v=0;
	//target();
	for(a=0;a<18;a++)
		{
			y_A[a]=sudut_target[a]-sudut_awal[a];
			z_A[a]=y_A[a]/pembagi;
		}

    for(v=0;v<=pembagi;v++)
        {
    		for(c=0;c<18;c++)
    			{
    				sudut[c]=sudut_awal[c] + (z_A[c]*v);
    				// TIM3->CNT=0;
    			}
    		delay_ms(10);
        }
    for(d=0;d<18;d++)
    	{
    		//sudut_awal[d]=sudut_target[d];
    		//sudut_target[d]=sudut[d];
    	}

    //tahap1();
    delay_ms(10);
    for(a=0;a<18;a++)
    	{
    		y_A[a]=sudut_tahap1[a]-sudut_target[a];
    		z_A[a]=y_A[a]/pembagi;
    	}

    for(v=0;v<=pembagi;v++)
        {
    		for(c=0;c<18;c++)
    		{
    			sudut[c]=sudut_target[c] + (z_A[c]*v);
    			// TIM3->CNT=0;
    		}
    		delay_ms(10);
        }
    for(d=0;d<18;d++)
    	{
    		//sudut_target[d]=sudut_tahap1[d];
    		//sudut_tahap1[d]=sudut[d];
    	}

    //tahap2();
    delay_ms(10);
    for(a=0;a<18;a++)
    	{
    		y_A[a]=sudut_tahap2[a]-sudut_tahap1[a];
    		z_A[a]=y_A[a]/pembagi;
    	}

    for(v=0;v<=pembagi;v++)
        {
    		for(c=0;c<18;c++)
    		{
    			sudut[c]=sudut_tahap1[c] + (z_A[c]*v);
    			// TIM3->CNT=0;
    		}
    		delay_ms(10);
        }
    for(d=0;d<18;d++)
    	{
    		//sudut_tahap1[d]=sudut_tahap2[d];
    		//sudut_tahap2[d]=sudut[d];
    	}

    //awal();
    delay_ms(10);
    for(a=0;a<18;a++)
    	{
    		y_A[a]=sudut_awal[a]-sudut_tahap2[a];
    		z_A[a]=y_A[a]/pembagi;
    	}

    for(v=0;v<=pembagi;v++)
        {
    		for(c=0;c<18;c++)
    		{
    			sudut[c]=sudut_tahap2[c] + (z_A[c]*v);
            // TIM3->CNT=0;
    		}
    		delay_ms(10);
        }
    for(d=0;d<18;d++)
    	{
    		//sudut_tahap2[d]=sudut_awal[d];
    	}
}

void mov_static(void)
{
    generate_movement_data(0,0,10);
    delay_ms(50);
    inv_kine(front_left,90,7,leg);
    inv_kine(front_right,90,7,leg);
    inv_kine(middle_left,90,7,leg);
    inv_kine(middle_right,90,7,leg);
    inv_kine(rear_left,90,7,leg);
    inv_kine(rear_right,90,7,leg);
    delay_ms(100);
}


void mov_search_fire(int SEARCH_FIRE_CMD)
{
	generate_movement_data(0,0,10);
	delay_ms(10);
	static int right=90,left=90;

	switch(SEARCH_FIRE_CMD)
	{
		case SRC_FR_A:
		{
			right=150;
			left= 30;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_B:
		{
			right=140;
			left= 40;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_C:
		{
			right=130;
			left= 50;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_D:
		{
			right=120;
			left= 60;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_E:
		{
			right=110;
			left= 70;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_F:
		{
			right=100;
			left= 80;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_G:
		{
			right=90;
			left= 90;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_H:
		{
			right=80;
			left= 100;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_I:
		{
			right=70;
			left= 110;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_J:
		{
			right=60;
			left= 120;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_K:
		{
			right=50;
			left= 130;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_L:
		{
			right=40;
			left= 140;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_M:
		{
			right=30;
			left= 150;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;
/*
		case SRC_FR_N:
		{
			right=40;
			left= 140;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_O:
		{
			right=30;
			left= 150;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;

		case SRC_FR_P:
		{
			right=20;
			left= 160;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;
/*
		case SRC_FR_Q:
		{
			right=10;
			left= 170;

			//INITIAL POSITION
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(20);
			//COMPLETE FLAG
			command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
			delay_ms(20);
		}break;
		*/
		default: break;
	}
}
//
//void mov_search_fire(int SEARCH_FIRE_CMD)
//{
//	static int right=170;
//	static int left= 10;
//
//	//sweep variables, 1 means sweep from left to right, while 2 means sweep from right to left
//	static int sweep=1;
//
//
//	generate_movement_data(0,0,10);
//	delay_ms(10);
//
//
//	switch(SEARCH_FIRE_CMD)
//	{
//		case SEARCH_FIRE_RESET:
//			{
//				right=170;
//				left= 10;
//
//				//INITIAL POSITION
//				inv_kine(front_left,left,7,leg);
//				inv_kine(front_right,right,7,leg);
//				inv_kine(middle_left,left,7,leg);
//				inv_kine(middle_right,right,7,leg);
//				inv_kine(rear_left,left,7,leg);
//				inv_kine(rear_right,right,7,leg);
//				delay_ms(2);
//
//				//COMPLETE FLAG
//				command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
//				//delay_ms(50);
//			}break;
//
//		case SEARCH_FIRE_GO:
//			{
//				//SEARCH FIRE SWEEP FROM LEFT TO RIGHT
//				if (sweep==1)
//				{
//						right-=5;
//						left+=5;
//
//						//COMPLETE POSITION
//						inv_kine(front_left,left,7,leg);
//						inv_kine(front_right,right,7,leg);
//						inv_kine(middle_left,left,7,leg);
//						inv_kine(middle_right,right,7,leg);
//						inv_kine(rear_left,left,7,leg);
//						inv_kine(rear_right,right,7,leg);
//						delay_ms(2);
//
//						if (right<=10&&left>=170) sweep=2;
//						if (right<=10&&left>=170) sweep=2;
//
//						//COMPLETE FLAG
//						command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
//						//delay_ms(50);
//
//				}
//
//				//SEARCHING FIRE SWEEP FROM RIGHT TO LEFT
//				if (sweep==2)
//				{
//					right+=5;
//					left-=5;
//
//					//COMPLETE POSITION
//					inv_kine(front_left,left,7,leg);
//					inv_kine(front_right,right,7,leg);
//					inv_kine(middle_left,left,7,leg);
//					inv_kine(middle_right,right,7,leg);
//					inv_kine(rear_left,left,7,leg);
//					inv_kine(rear_right,right,7,leg);
//					delay_ms(2);
//
//					if (right>=170&&left<=10) sweep=1;
//
//					//COMPLETE FLAG
//					command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
//					//delay_ms(50);
//				}
//			}break;
//		default:
//			{
//				//COMPLETE FLAG
//				command_code[0]=0x0; command_code[1]=0x0;command_code[2]=0x0;Sendto_MainController(USART3,&COMMAND_COMPLETED);
//				//delay_ms(50);
//			}break;
//	}


/*
	while(right!=170&&left!=10)
	{

		inv_kine(front_left,left,7,leg);
		inv_kine(front_right,right,7,leg);
		inv_kine(middle_left,left,7,leg);
		inv_kine(middle_right,right,7,leg);
		inv_kine(rear_left,left,7,leg);
		inv_kine(rear_right,right,7,leg);
		delay_ms(2);
		right+=0.5;
		left-=0.5;

	}
	delay_ms(300);

	while(right!=90&&left!=90)
	{

	    inv_kine(front_left,left,7,leg);
	    inv_kine(front_right,right,7,leg);
	    inv_kine(middle_left,left,7,leg);
	    inv_kine(middle_right,right,7,leg);
	    inv_kine(rear_left,left,7,leg);
	    inv_kine(rear_right,right,7,leg);
	    delay_ms(2);
	    right-=0.5;
	    left+=0.5;

	}
	delay_ms(300);

	while(right!=10&&left!=170)
	{

		inv_kine(front_left,left,7,leg);
		inv_kine(front_right,right,7,leg);
		inv_kine(middle_left,left,7,leg);
		inv_kine(middle_right,right,7,leg);
		inv_kine(rear_left,left,7,leg);
		inv_kine(rear_right,right,7,leg);
		delay_ms(2);
		right-=0.5;
		left+=0.5;

	}
	delay_ms(300);

	while(right!=90&&left!=90)
	{

	    inv_kine(front_left,left,7,leg);
	    inv_kine(front_right,right,7,leg);
	    inv_kine(middle_left,left,7,leg);
	    inv_kine(middle_right,right,7,leg);
	    inv_kine(rear_left,left,7,leg);
	    inv_kine(rear_right,right,7,leg);
	    delay_ms(2);
	    right+=0.5;
	    left-=0.5;

	}
	delay_ms(50);

}
*/


/*
 * func  : void mov_fwd_5cm (int distance, int SPEED)
 * brief : convert (int) distance into forward locomotion
 * param : (int) distance is prescaled by 5
 * Ver   : 4
 * 		   Configured for the new YUME's mechanical
 *		   Last Revised By: Ryan (Thursday, August 28, 2015
 * programmer's note  : theoretically, robot should move 5 cm in each iteration,
 * 					    but in the actual case, it just move for about 4.75 cm
 * 					    (case: mov_fwd_5cm(20,FAST) -> YUME moves forward only 18 cm
 *
 */
void mov_fwd_5cm (int distance, int SPEED)
{
	int limit= distance/5;
	int loop;
	//if (STEP==FAR)       { limit= distance/5; generate_movement_data(0,5,10);}
	//else if (STEP==HALF) { limit= distance/3; generate_movement_data(0,3,10);}
	//else if (STEP=CLOSE) { limit= distance/2; generate_movement_data(0,2,10);}

	generate_movement_data(0,5,10);
	delay_ms(50);
	for (loop=1;loop<=limit;loop++)
		{
			//gait_mode(FORWARD, &SPEED);
			//1
			inv_kine(front_left,90,7,leg);
			inv_kine(front_right,90+beta[3],10,leg-3);
			inv_kine(middle_left,90+beta[1],10,leg-3);
			inv_kine(middle_right,90,7,leg);
			inv_kine(rear_left,90,7,leg);
			inv_kine(rear_right,90+beta[5],10,leg-3);
			delay_ms(SPEED);

			//2
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],10,leg-3);
			inv_kine(middle_left,90-beta[1],10,leg-3);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],10,leg-3);
			delay_ms(SPEED);

			//3
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],7,leg);
			inv_kine(middle_left,90-beta[1],7,leg);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],7,leg);
			delay_ms(SPEED);

			//4
			inv_kine(front_left,90+beta[0],10,leg-3);
			inv_kine(front_right,90,7,leg);
			inv_kine(middle_left,90,7,leg);
			inv_kine(middle_right,90+beta[4],10,leg-3);
			inv_kine(rear_left,90+beta[2],7,leg-2);
			inv_kine(rear_right,90,7,leg);
			delay_ms(SPEED);

			//5
			inv_kine(front_left,90-beta[0]-5,10,leg-3);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],10,leg-3);
			inv_kine(rear_left,90-beta[2],7,leg-2);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);

			//6
			inv_kine(front_left,90-beta[0],7,leg);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],7,leg);
			inv_kine(rear_left,90-beta[2],7,leg);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);
		}
	//mov_static();
	//delay_ms(150);
}

void mov_fwd_3cm (int distance, int SPEED)
{
	int limit= distance/3;
	int loop;
	generate_movement_data(0,3,10);
	delay_ms(50);
	for (loop=1;loop<=limit;loop++)
		{
			//1
			inv_kine(front_left,90,7,leg);
			inv_kine(front_right,90+beta[3],10,leg-3);
			inv_kine(middle_left,90+beta[1],10,leg-3);
			inv_kine(middle_right,90,7,leg);
			inv_kine(rear_left,90,7,leg);
			inv_kine(rear_right,90+beta[5],10,leg-3);
			delay_ms(SPEED);

			//2
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],10,leg-3);
			inv_kine(middle_left,90-beta[1],10,leg-3);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],10,leg-3);
			delay_ms(SPEED);

			//3
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],7,leg);
			inv_kine(middle_left,90-beta[1],7,leg);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],7,leg);
			delay_ms(SPEED);

			//4
			inv_kine(front_left,90+beta[0],10,leg-3);
			inv_kine(front_right,90,7,leg);
			inv_kine(middle_left,90,7,leg);
			inv_kine(middle_right,90+beta[4],10,leg-3);
			inv_kine(rear_left,90+beta[2],10,leg-3);
			inv_kine(rear_right,90,7,leg);
			delay_ms(SPEED);

			//5
			inv_kine(front_left,90-beta[0]-5,10,leg-3);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],10,leg-3);
			inv_kine(rear_left,90-beta[2],10,leg-3);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);

			//6
			inv_kine(front_left,90-beta[0],7,leg);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],7,leg);
			inv_kine(rear_left,90-beta[2],7,leg);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);
		}
	//mov_static();
	//delay_ms(150);
}

/*
 * func  : void mov_bwd (int distance)
 * brief : convert (int) distance into backward locomotion
 * param : (int) distance is prescaled by 5
 * Ver   : 2
 * 		   Configured for the new YUME's mechanical
 *		   Last Revised By: Ryan (Friday, April 3rd, 2015)
 * programmer's note  :
 *
 */
void mov_bwd(int distance, int SPEED)
{
	int limit= distance/5;
	int loop;
	generate_movement_data(0,-5,10);
	delay_ms(50);
	for (loop=1;loop<=limit;loop++)
		{
			//1
			inv_kine(front_left,90,7,leg);
			inv_kine(front_right,90+beta[3]-7,10,leg-3); //MAJU
			inv_kine(middle_left,90+beta[4]-5,10,leg-3); //MAJU
			inv_kine(middle_right,90,7,leg);
			inv_kine(rear_left,90,7,leg);
			inv_kine(rear_right,90+beta[5]+12,10,leg-3); //MAJU
			delay_ms(SPEED);

			//2
			inv_kine(front_left,90+beta[0]-2,7,leg); //MAJU
			inv_kine(front_right,90-beta[3]+1,10,leg-3); //MUNDUR
			inv_kine(middle_left,90-beta[1]-7,10,leg-3); //MAJU
			inv_kine(middle_right,90+beta[4]-5,7,leg); //MUNDUR
			inv_kine(rear_left,90+beta[2]+8,7,leg); //MAJU
			inv_kine(rear_right,90-beta[5]+11,10,leg-3); //MUNDUR
			delay_ms(SPEED);

			//3
			inv_kine(front_left,90+beta[0]-2,7,leg); //MAJU
			inv_kine(front_right,90-beta[3]+1,7,leg); //MUNDUR
			inv_kine(middle_left,90-beta[1]-7,7,leg); //MUNDUR
			inv_kine(middle_right,90+beta[4]-5,7,leg); //MAJU
			inv_kine(rear_left,90+beta[2]+8,7,leg); //MAJU
			inv_kine(rear_right,90-beta[5]+11,7,leg); //MUNDUR
			delay_ms(SPEED);

			//4
			inv_kine(front_left,90+beta[0]-2,10,leg-3); //MAJU
			inv_kine(front_right,90,7,leg);
			inv_kine(middle_left,90,7,leg);
			inv_kine(middle_right,90+beta[4]-5,10.5,leg-3); //MAJU
			inv_kine(rear_left,90+beta[2]+8,10,leg-3); //MAJU
			inv_kine(rear_right,90,7,leg);
			delay_ms(SPEED);

			//5
			inv_kine(front_left,90-beta[0]+8,10,leg-3); //MUNDUR
			inv_kine(front_right,90+beta[3]-7,7,leg); //MAJU
			inv_kine(middle_left,90+beta[1]-10,7,leg); //MAJU
			inv_kine(middle_right,90-beta[4]-5,10.5,leg-3); //MUNDUR
			inv_kine(rear_left,90-beta[2]+5,10,leg-3); //MUNDUR
			inv_kine(rear_right,90+beta[5]+12,7,leg); //MAJU
			delay_ms(SPEED);

			//6
			inv_kine(front_left,90-beta[0]+8,7,leg); //MUNDUR
			inv_kine(front_right,90+beta[3]-7,7,leg); //MAJU
			inv_kine(middle_left,90+beta[1]-10,7,leg); //MAJU
			inv_kine(middle_right,90-beta[4]-5,7,leg); //MUNDUR
			inv_kine(rear_left,90-beta[2]+5,7,leg); //MUNDUR
			inv_kine(rear_right,90+beta[5]+12,7,leg); //MAJU
			delay_ms(SPEED);
		}
}

///*
// *  Func  : void mov_rot_right_90_dgr(int angle, int SPEED, int STEP)
// *  Brief : rotate hexapod body 90 degrees clockwise per parameter with desired speed and step length
// *  Param : angle -> multiplication of 90 degrees
// *  		SPEED -> default= MED_SLOW(200), MED(150), FAST(130), ULTRA_FAST(50);
// *  		STEP  -> default= FAR(5), CLOSE (2);
// *  ver	  : 3
// *  Last revised by: Ryan (THU, APRIL 2nd, 2015)
// *     	             Configured for the new YUME's mechanical
// * programmer's note  :
// */
//void mov_rot_right_90_dgr(int angle, int SPEED, int STEP)
//{
//	int limit = angle/90;
//	int loop;
//	switch(STEP)
//	{
//		case CLOSE:
//					{
//						for (loop=1;loop<=limit;loop++)
//						{
//							generate_movement_data(0,STEP,10);
//							delay_ms(50);
//							for (x=1;x<=20;x++)
//							{
//
//								//gait_mode(ROTATE_RIGHT,SPEED);
//					    		//1
//					    		inv_kine(front_left,90+beta[0],7,leg); //mundur
//					    		inv_kine(front_right,90+beta[3],10,leg-3); //mundur
//					    		inv_kine(middle_left,90-beta[1],10,leg-3); //maju
//					    		inv_kine(middle_right,90-beta[4],7,leg); //maju
//					    		inv_kine(rear_left,90+beta[2],7,leg); //mundur
//					    		inv_kine(rear_right,90+beta[5],10,leg-3); //mundur
//					    		delay_ms(SPEED);
//
//					    		//2
//					    		inv_kine(front_left,90+beta[0],7,leg); //mundur
//					    		inv_kine(front_right,90+beta[3],7,leg); //mundur
//					    		inv_kine(middle_left,90-beta[1],7,leg); //maju
//					    		inv_kine(middle_right,90-beta[4],7,leg); //maju
//					    		inv_kine(rear_left,90+beta[2],7,leg); //mundur
//					    		inv_kine(rear_right,90+beta[5],7,leg); //mundur
//					    		delay_ms(SPEED);
//
//					    		//3
//					    		inv_kine(front_left,90-beta[0],10,leg-3); //maju
//					    		inv_kine(front_right,90-beta[3],7,leg); //maju
//					    		inv_kine(middle_left,90+beta[1],7,leg); //mundur
//					    		inv_kine(middle_right,90+beta[4],10.5,leg-3); //mundur
//					    		inv_kine(rear_left,90-beta[2],10,leg-3); //maju
//					    		inv_kine(rear_right,90-beta[5],7,leg); //maju
//					    		delay_ms(SPEED);
//
//					    		//4
//					    		inv_kine(front_left,90-beta[0],7,leg); //maju
//					    		inv_kine(front_right,90-beta[3],7,leg); //maju
//					    		inv_kine(middle_left,90+beta[1],7,leg); //mundur
//					    		inv_kine(middle_right,90+beta[4],7,leg); //mundur
//					    		inv_kine(rear_left,90-beta[2],7,leg); //maju
//					    		inv_kine(rear_right,90-beta[5],7,leg); //maju
//					    		delay_ms(SPEED);
//
//							}
//						}
//					}
//					break;
//		case FAR:
//					{
//						for (loop=1;loop<=limit;loop++)
//						{
//							generate_movement_data(0,STEP,10);
//							delay_ms(50);
//							for (x=1;x<=7;x++)
//							{
//								//gait_mode(ROTATE_RIGHT,SPEED);
//					    		//1
//					    		inv_kine(front_left,90+beta[0],7,leg); //mundur
//					    		inv_kine(front_right,90+beta[3],10,leg-3); //mundur
//					    		inv_kine(middle_left,90-beta[1],10,leg-3); //maju
//					    		inv_kine(middle_right,90-beta[4],7,leg); //maju
//					    		inv_kine(rear_left,90+beta[2],7,leg); //mundur
//					    		inv_kine(rear_right,90+beta[5],10,leg-3); //mundur
//					    		delay_ms(SPEED);
//
//					    		//2
//					    		inv_kine(front_left,90+beta[0],7,leg); //mundur
//					    		inv_kine(front_right,90+beta[3],7,leg); //mundur
//					    		inv_kine(middle_left,90-beta[1],7,leg); //maju
//					    		inv_kine(middle_right,90-beta[4],7,leg); //maju
//					    		inv_kine(rear_left,90+beta[2],7,leg); //mundur
//					    		inv_kine(rear_right,90+beta[5],7,leg); //mundur
//					    		delay_ms(SPEED);
//
//					    		//3
//					    		inv_kine(front_left,90-beta[0],10,leg-3); //maju
//					    		inv_kine(front_right,90-beta[3],7,leg); //maju
//					    		inv_kine(middle_left,90+beta[1],7,leg); //mundur
//					    		inv_kine(middle_right,90+beta[4],10.5,leg-3); //mundur
//					    		inv_kine(rear_left,90-beta[2],10,leg-3); //maju
//					    		inv_kine(rear_right,90-beta[5],7,leg); //maju
//					    		delay_ms(SPEED);
//
//					    		//4
//					    		inv_kine(front_left,90-beta[0],7,leg); //maju
//					    		inv_kine(front_right,90-beta[3],7,leg); //maju
//					    		inv_kine(middle_left,90+beta[1],7,leg); //mundur
//					    		inv_kine(middle_right,90+beta[4],7,leg); //mundur
//					    		inv_kine(rear_left,90-beta[2],7,leg); //maju
//					    		inv_kine(rear_right,90-beta[5],7,leg); //maju
//					    		delay_ms(SPEED);
//							}
//						}
//					}
//					break;
//		default: break;
//	}
//
//}

/*
 *  Func  : void mov_rot_right(int SPEED, int STEP)
 *  Brief : rotate hexapod body clockwise per parameter with desired speed and step length
 *  Param : SPEED -> default= MED_SLOW(200), MED(150), FAST(130), ULTRA_FAST(50);
 *  		STEP  -> default= FAR(5), CLOSE (2);
 *  ver	  : 1
 *  Last revised by: Ryan (MON, August 3rd 2015)
 *     	             Configured for the new YUME's mechanical
 * programmer's note  :
 */
void mov_rot_right(int SPEED, int STEP)
{
	switch(STEP)
	{
		case CLOSE:
			{
				generate_movement_data(0,STEP,10);
				delay_ms(50);

				//gait_mode(ROTATE_RIGHT,SPEED);
				//1
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],10,leg-3); //mundur
				inv_kine(middle_left,90-beta[1],10,leg-3); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],10,leg-3); //mundur
				delay_ms(SPEED);

				//2
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],7,leg); //mundur
				inv_kine(middle_left,90-beta[1],7,leg); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],7,leg); //mundur
				delay_ms(SPEED);

				//3
				inv_kine(front_left,90-beta[0],10,leg-3); //maju
				inv_kine(front_right,90-beta[3],7,leg); //maju
				inv_kine(middle_left,90+beta[1],7,leg); //mundur
				inv_kine(middle_right,90+beta[4],10.5,leg-3); //mundur
				inv_kine(rear_left,90-beta[2],10,leg-3); //maju
				inv_kine(rear_right,90-beta[5],7,leg); //maju
				delay_ms(SPEED);

				//4
				inv_kine(front_left,90-beta[0],7,leg); //maju
				inv_kine(front_right,90-beta[3],7,leg); //maju
				inv_kine(middle_left,90+beta[1],7,leg); //mundur
				inv_kine(middle_right,90+beta[4],7,leg); //mundur
				inv_kine(rear_left,90-beta[2],7,leg); //maju
				inv_kine(rear_right,90-beta[5],7,leg); //maju
				delay_ms(SPEED);

			}break;

		case FAR:
			{
				generate_movement_data(0,STEP,10);
				delay_ms(50);

				//gait_mode(ROTATE_RIGHT,SPEED);
				//1
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],10,leg-3); //mundur
				inv_kine(middle_left,90-beta[1],10,leg-3); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],10,leg-3); //mundur
				delay_ms(SPEED);

				//2
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],7,leg); //mundur
				inv_kine(middle_left,90-beta[1],7,leg); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],7,leg); //mundur
				delay_ms(SPEED);

				//3
				inv_kine(front_left,90-beta[0],10,leg-3); //maju
				inv_kine(front_right,90-beta[3],7,leg); //maju
				inv_kine(middle_left,90+beta[1],7,leg); //mundur
				inv_kine(middle_right,90+beta[4],10.5,leg-3); //mundur
				inv_kine(rear_left,90-beta[2],10,leg-3); //maju
				inv_kine(rear_right,90-beta[5],7,leg); //maju
				delay_ms(SPEED);

				//4
				inv_kine(front_left,90-beta[0],7,leg); //maju
				inv_kine(front_right,90-beta[3],7,leg); //maju
				inv_kine(middle_left,90+beta[1],7,leg); //mundur
				inv_kine(middle_right,90+beta[4],7,leg); //mundur
				inv_kine(rear_left,90-beta[2],7,leg); //maju
				inv_kine(rear_right,90-beta[5],7,leg); //maju
				delay_ms(SPEED);

			}break;
		default: break;
	}
}


///*
// *  Func  : void mov_rot_left_90_dgr(int angle, int SPEED, int STEP)
// *  Brief : rotate hexapod body 90 degrees counter clockwise per parameter with desired speed and step length
// *  Param : angle -> multiplication of 90 degrees
// *  		SPEED -> default= MED_SLOW(200), MED(150), FAST(130), ULTRA_FAST(50);
// *  		STEP  -> default= FAR(5), CLOSE (2);
// *  Ver   : 2
// *  Last revised by: Ryan (THU, APRIL 2nd, 2015)
// *     	             Configured for the new YUME's mechanical
// * programmer's note  :
// */
//void mov_rot_left_90_dgr(int angle, int SPEED, int STEP)
//{
//	int limit = angle/90;
//	int loop;
//	switch(STEP)
//	{
//		case CLOSE:
//					{
//						for (loop=1;loop<=limit;loop++)
//						{
//							generate_movement_data(0,STEP,10);
//							delay_ms(50);
//							for (x=1;x<=26;x++)
//							{
//								//gait_mode(ROTATE_LEFT, SPEED);
//								//1
//								inv_kine(front_left,90-beta[0],7,leg); //maju /
//								inv_kine(front_right,90-beta[3],10,leg-3); //maju
//								inv_kine(middle_left,90+beta[1],10,leg-3); //mundur /
//								inv_kine(middle_right,90+beta[4],7,leg); //mundur /
//								inv_kine(rear_left,90-beta[2],7,leg); //maju /
//								inv_kine(rear_right,90-beta[5],10,leg-3); //maju /
//								delay_ms(SPEED);
//
//								//2
//								inv_kine(front_left,90-beta[0],7,leg); //maju /
//								inv_kine(front_right,90-beta[3],7,leg); //maju /
//								inv_kine(middle_left,90+beta[1],7,leg); //mundur /
//								inv_kine(middle_right,90+beta[4],7,leg); //mundur /
//								inv_kine(rear_left,90-beta[2],7,leg); //maju /
//								inv_kine(rear_right,90-beta[5],7,leg); //maju /
//								delay_ms(SPEED);
//
//								//3
//								inv_kine(front_left,90+beta[0],10,leg-3); //mundur /
//								inv_kine(front_right,90+beta[3],7,leg); //mundur /
//								inv_kine(middle_left,90-beta[1],7,leg); //maju /
//								inv_kine(middle_right,90-beta[4],10.5,leg-3); //maju /
//								inv_kine(rear_left,90+beta[2],10,leg-3); //mundur /
//								inv_kine(rear_right,90+beta[5],7,leg); //mundur /
//								delay_ms(SPEED);
//
//								//4
//								inv_kine(front_left,90+beta[0],7,leg); //mundur /
//								inv_kine(front_right,90+beta[3],7,leg); //mundur /
//								inv_kine(middle_left,90-beta[1],7,leg); //maju /
//								inv_kine(middle_right,90-beta[4],7,leg); //maju /
//								inv_kine(rear_left,90+beta[2],7,leg); //mundur /
//								inv_kine(rear_right,90+beta[5],7,leg); //mundur /
//								delay_ms(SPEED);
//
//							}
//						}
//
//					}
//					break;
//
//		case FAR:
//					{
//						for (loop=1;loop<=limit;loop++)
//						{
//							generate_movement_data(0,STEP,10);
//							delay_ms(50);
//							for (x=1;x<=6;x++)
//							{
//								//gait_mode(ROTATE_LEFT, SPEED);
//								//1
//								inv_kine(front_left,90-beta[0],7,leg); //maju /
//								inv_kine(front_right,90-beta[3],10,leg-3); //maju
//								inv_kine(middle_left,90+beta[1],10,leg-3); //mundur /
//								inv_kine(middle_right,90+beta[4],7,leg); //mundur /
//								inv_kine(rear_left,90-beta[2],7,leg); //maju /
//								inv_kine(rear_right,90-beta[5],10,leg-3); //maju /
//								delay_ms(SPEED);
//
//								//2
//								inv_kine(front_left,90-beta[0],7,leg); //maju /
//								inv_kine(front_right,90-beta[3],7,leg); //maju /
//								inv_kine(middle_left,90+beta[1],7,leg); //mundur /
//								inv_kine(middle_right,90+beta[4],7,leg); //mundur /
//								inv_kine(rear_left,90-beta[2],7,leg); //maju /
//								inv_kine(rear_right,90-beta[5],7,leg); //maju /
//								delay_ms(SPEED);
//
//								//3
//								inv_kine(front_left,90+beta[0],10,leg-3); //mundur /
//								inv_kine(front_right,90+beta[3],7,leg); //mundur /
//								inv_kine(middle_left,90-beta[1],7,leg); //maju /
//								inv_kine(middle_right,90-beta[4],10.5,leg-3); //maju /
//								inv_kine(rear_left,90+beta[2],10,leg-3); //mundur /
//								inv_kine(rear_right,90+beta[5],7,leg); //mundur /
//								delay_ms(SPEED);
//
//								//4
//								inv_kine(front_left,90+beta[0],7,leg); //mundur /
//								inv_kine(front_right,90+beta[3],7,leg); //mundur /
//								inv_kine(middle_left,90-beta[1],7,leg); //maju /
//								inv_kine(middle_right,90-beta[4],7,leg); //maju /
//								inv_kine(rear_left,90+beta[2],7,leg); //mundur /
//								inv_kine(rear_right,90+beta[5],7,leg); //mundur /
//								delay_ms(SPEED);
//
//							}
//						}
//					}
//					break;
//
//		default: break;
//
//	}
//
//}

/*
 *  Func  : void mov_rot_left(int SPEED, int STEP)
 *  Brief : rotate hexapod body clockwise per parameter with desired speed and step length
 *  Param : SPEED -> default= MED_SLOW(200), MED(150), FAST(130), ULTRA_FAST(50);
 *  		STEP  -> default= FAR(5), CLOSE (2);
 *  ver	  : 1
 *  Last revised by: Ryan (MON, August 3rd 2015)
 *     	             Configured for the new YUME's mechanical
 * programmer's note  :
 */
void mov_rot_left(int SPEED, int STEP)
{
	switch(STEP)
	{
		case CLOSE:
			{
				generate_movement_data(0,STEP,10);
				delay_ms(50);

				//gait_mode(ROTATE_LEFT, SPEED);
				//1
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],10,leg-3); //maju
				inv_kine(middle_left,90+beta[1],10,leg-3); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],10,leg-3); //maju /
				delay_ms(SPEED);

				//2
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],7,leg); //maju /
				inv_kine(middle_left,90+beta[1],7,leg); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],7,leg); //maju /
				delay_ms(SPEED);

				//3
				inv_kine(front_left,90+beta[0],10,leg-3); //mundur /
				inv_kine(front_right,90+beta[3],7,leg); //mundur /
				inv_kine(middle_left,90-beta[1],7,leg); //maju /
				inv_kine(middle_right,90-beta[4],10.5,leg-3); //maju /
				inv_kine(rear_left,90+beta[2],10,leg-3); //mundur /
				inv_kine(rear_right,90+beta[5],7,leg); //mundur /
				delay_ms(SPEED);

				//4
				inv_kine(front_left,90+beta[0],7,leg); //mundur /
				inv_kine(front_right,90+beta[3],7,leg); //mundur /
				inv_kine(middle_left,90-beta[1],7,leg); //maju /
				inv_kine(middle_right,90-beta[4],7,leg); //maju /
				inv_kine(rear_left,90+beta[2],7,leg); //mundur /
				inv_kine(rear_right,90+beta[5],7,leg); //mundur /
				delay_ms(SPEED);

			}break;

		case FAR:
			{
				generate_movement_data(0,STEP,10);
				delay_ms(50);

				//gait_mode(ROTATE_LEFT, SPEED);
				//1
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],10,leg-3); //maju
				inv_kine(middle_left,90+beta[1],10,leg-3); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],10,leg-3); //maju /
				delay_ms(SPEED);

				//2
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],7,leg); //maju /
				inv_kine(middle_left,90+beta[1],7,leg); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],7,leg); //maju /
				delay_ms(SPEED);

				//3
				inv_kine(front_left,90+beta[0],10,leg-3); //mundur /
				inv_kine(front_right,90+beta[3],7,leg); //mundur /
				inv_kine(middle_left,90-beta[1],7,leg); //maju /
				inv_kine(middle_right,90-beta[4],10.5,leg-3); //maju /
				inv_kine(rear_left,90+beta[2],10,leg-3); //mundur /
				inv_kine(rear_right,90+beta[5],7,leg); //mundur /
				delay_ms(SPEED);

				//4
				inv_kine(front_left,90+beta[0],7,leg); //mundur /
				inv_kine(front_right,90+beta[3],7,leg); //mundur /
				inv_kine(middle_left,90-beta[1],7,leg); //maju /
				inv_kine(middle_right,90-beta[4],7,leg); //maju /
				inv_kine(rear_left,90+beta[2],7,leg); //mundur /
				inv_kine(rear_right,90+beta[5],7,leg); //mundur /
				delay_ms(SPEED);

			}break;
		default: break;
	}
}


///*
// * Func : void mov_turn_right(int SPEED, int STEP)
// * brief: Robot's forward curve right locomotion, useful for wall following and solving the crossroad
// * Written By: Ryan (MON, MARCH 9th, 2015)
// * Ver  : 2 (Last Revised By: Ryan (Friday, April 2nd, 2015)
// *   	     Configured for the new YUME's mechanical
// * programmer's note  :
// */
//void mov_curve_right(int SPEED, int STEP)
//{
//	int loop;
//	generate_movement_data(0,STEP,10);
//	delay_ms(50);
//
//	switch(STEP)
//	{
//		case CLOSE:
//					{
//						for (loop=1;loop<=5;loop++)
//							{
//
//								//gait_mode(CURVE_RIGHT, SPEED);
//							//1
//							inv_kine(front_left,90,7,leg);
//							inv_kine(front_right,90+beta[3],10,leg-3);
//							inv_kine(middle_left,90+beta[1],10,leg-3);
//							inv_kine(middle_right,90,7,leg);
//							inv_kine(rear_left,90,7,leg);
//							inv_kine(rear_right,90+beta[5],10,leg-3);
//							delay_ms(SPEED);
//
//							//2
//							inv_kine(front_left,90+beta[0]+25,7,leg);
//							inv_kine(front_right,90-beta[3]+10,10,leg-3);
//							inv_kine(middle_left,90-beta[1]-25,10,leg-3);
//							inv_kine(middle_right,90+beta[4],7,leg);
//							inv_kine(rear_left,90+beta[2]+25,7,leg);
//							inv_kine(rear_right,90-beta[5]+10,10,leg-3);
//							delay_ms(SPEED);
//
//							//3
//							inv_kine(front_left,90+beta[0]+25,7,leg);
//							inv_kine(front_right,90-beta[3]+10,7,leg);
//							inv_kine(middle_left,90-beta[1]-25,7,leg);
//							inv_kine(middle_right,90+beta[4],7,leg);
//							inv_kine(rear_left,90+beta[2]+25,7,leg);
//							inv_kine(rear_right,90-beta[5]+10,7,leg);
//							delay_ms(SPEED);
//
//							//4
//							inv_kine(front_left,90+beta[0]+25,10,leg-3);
//							inv_kine(front_right,90,7,leg);
//							inv_kine(middle_left,90,7,leg);
//							inv_kine(middle_right,90+beta[4],10.5,leg-3);
//							inv_kine(rear_left,90+beta[2]+25,10,leg-3);
//							inv_kine(rear_right,90,7,leg);
//							delay_ms(SPEED);
//
//							//5
//							inv_kine(front_left,90-beta[0]-25,10,leg-3);
//							inv_kine(front_right,90+beta[3],7,leg);
//							inv_kine(middle_left,90+beta[1]+25,7,leg);
//							inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
//							inv_kine(rear_left,90-beta[2]-25,10,leg-3);
//							inv_kine(rear_right,90+beta[5],7,leg);
//							delay_ms(SPEED);
//
//							//6
//							inv_kine(front_left,90-beta[0]-25,7,leg);
//							inv_kine(front_right,90+beta[3],7,leg);
//							inv_kine(middle_left,90+beta[1]+25,7,leg);
//							inv_kine(middle_right,90-beta[4]+10,7,leg);
//							inv_kine(rear_left,90-beta[2]-25,7,leg);
//							inv_kine(rear_right,90+beta[5],7,leg);
//							delay_ms(SPEED);
//							}
//					}
//					break;
//
//		case FAR:
//					{
//						for (loop=1;loop<=5;loop++)
//							{
//								//gait_mode(CURVE_RIGHT, SPEED);
//								//1
//								inv_kine(front_left,90,7,leg);
//								inv_kine(front_right,90+beta[3],10,leg-3);
//								inv_kine(middle_left,90+beta[1],10,leg-3);
//								inv_kine(middle_right,90,7,leg);
//								inv_kine(rear_left,90,7,leg);
//								inv_kine(rear_right,90+beta[5],10,leg-3);
//								delay_ms(SPEED);
//
//								//2
//								inv_kine(front_left,90+beta[0]+25,7,leg);
//								inv_kine(front_right,90-beta[3]+10,10,leg-3);
//								inv_kine(middle_left,90-beta[1]-25,10,leg-3);
//								inv_kine(middle_right,90+beta[4],7,leg);
//								inv_kine(rear_left,90+beta[2]+25,7,leg);
//								inv_kine(rear_right,90-beta[5]+10,10,leg-3);
//								delay_ms(SPEED);
//
//								//3
//								inv_kine(front_left,90+beta[0]+25,7,leg);
//								inv_kine(front_right,90-beta[3]+10,7,leg);
//								inv_kine(middle_left,90-beta[1]-25,7,leg);
//								inv_kine(middle_right,90+beta[4],7,leg);
//								inv_kine(rear_left,90+beta[2]+25,7,leg);
//								inv_kine(rear_right,90-beta[5]+10,7,leg);
//								delay_ms(SPEED);
//
//								//4
//								inv_kine(front_left,90+beta[0]+25,10,leg-3);
//								inv_kine(front_right,90,7,leg);
//								inv_kine(middle_left,90,7,leg);
//								inv_kine(middle_right,90+beta[4],10.5,leg-3);
//								inv_kine(rear_left,90+beta[2]+25,10,leg-3);
//								inv_kine(rear_right,90,7,leg);
//								delay_ms(SPEED);
//
//								//5
//								inv_kine(front_left,90-beta[0]-25,10,leg-3);
//								inv_kine(front_right,90+beta[3],7,leg);
//								inv_kine(middle_left,90+beta[1]+25,7,leg);
//								inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
//								inv_kine(rear_left,90-beta[2]-25,10,leg-3);
//								inv_kine(rear_right,90+beta[5],7,leg);
//								delay_ms(SPEED);
//
//								//6
//								inv_kine(front_left,90-beta[0]-25,7,leg);
//								inv_kine(front_right,90+beta[3],7,leg);
//								inv_kine(middle_left,90+beta[1]+25,7,leg);
//								inv_kine(middle_right,90-beta[4]+10,7,leg);
//								inv_kine(rear_left,90-beta[2]-25,7,leg);
//								inv_kine(rear_right,90+beta[5],7,leg);
//								delay_ms(SPEED);
//							}
//					}
//					break;
//
//		default: break;
//	}
//
//}

/*
 * Func : void mov_curve_right(int SPEED, int STEP)
 * brief: Robot's forward curve right locomotion, useful for wall following and solving the crossroad
 * Written By: Ryan (MON, MARCH 9th, 2015)
 * Ver  : 2 (Last Revised By: Ryan (MON, August 19th, 2015)
 *   	     sudut belok dikurangi (yang lama sering bermasalah untuk follow)
 * programmer's note  :
 */
void mov_curve_right(int SPEED, int STEP)
{
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	//gait_mode(CURVE_RIGHT, SPEED);
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3);
	delay_ms(SPEED);

	//2
	inv_kine(front_left,90+beta[0]+15,7,leg);
	inv_kine(front_right,90-beta[3]+10,10,leg-3);
	inv_kine(middle_left,90-beta[1]-15,10,leg-3);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+15,7,leg);
	inv_kine(rear_right,90-beta[5]+10,10,leg-3);
	delay_ms(SPEED);

	//3
	inv_kine(front_left,90+beta[0]+15,7,leg);
	inv_kine(front_right,90-beta[3]+10,7,leg);
	inv_kine(middle_left,90-beta[1]-15,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+15,7,leg);
	inv_kine(rear_right,90-beta[5]+10,7,leg);
	delay_ms(SPEED);

	//4
	inv_kine(front_left,90+beta[0]+15,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],10.5,leg-3);
	inv_kine(rear_left,90+beta[2]+15,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	//5
	inv_kine(front_left,90-beta[0]-15,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+15,7,leg);
	inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]-15,10,leg-3);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	//6
	inv_kine(front_left,90-beta[0]-15,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+15,7,leg);
	inv_kine(middle_right,90-beta[4]+10,7,leg);
	inv_kine(rear_left,90-beta[2]-15,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);
}

///*
// * Func : void mov_turn_left(int SPEED, int STEP)
// * brief: Robot's forward curve left locomotion, useful for wall following and solving the crossroad
// * Written By: Ryan (MON, MARCH 9th, 2015)
// * Ver  : 2 (Last Revised By: Ryan (Friday, April 3rd, 2015)
// *   	     Configured for the new YUME's mechanical
// * programmer's note  :
// */
//void mov_curve_left(int SPEED, int STEP)
//{
//	int loop;
//	generate_movement_data(0,STEP,10);
//	delay_ms(50);
//	for (loop=1;loop<=4;loop++)
//		{
//			//gait_mode(CURVE_LEFT, SPEED);
//			//1
//			inv_kine(front_left,90,7,leg);
//			inv_kine(front_right,90+beta[3]+25,10,leg-3);
//			inv_kine(middle_left,90+beta[1]-10,10,leg-3);
//			inv_kine(middle_right,90,7,leg);
//			inv_kine(rear_left,90,7,leg);
//			inv_kine(rear_right,90+beta[5]+25,10,leg-3);
//			delay_ms(SPEED);
//
//			//2
//			inv_kine(front_left,90+beta[0]-10,7,leg);
//			inv_kine(front_right,90-beta[3]-25,10,leg-3);
//			inv_kine(middle_left,90-beta[1]+10,10,leg-3);
//			inv_kine(middle_right,90+beta[4]+25,7,leg);
//			inv_kine(rear_left,90+beta[2]-10,7,leg);
//			inv_kine(rear_right,90-beta[5]-25,10,leg-3);
//			delay_ms(SPEED);
//
//			//3
//			inv_kine(front_left,90+beta[0]-10,7,leg);
//			inv_kine(front_right,90-beta[3]-25,7,leg);
//			inv_kine(middle_left,90-beta[1]+10,7,leg);
//			inv_kine(middle_right,90+beta[4]+25,7,leg);
//			inv_kine(rear_left,90+beta[2]-10,7,leg);
//			inv_kine(rear_right,90-beta[5]-25,7,leg);
//			delay_ms(SPEED);
//
//			//4
//			inv_kine(front_left,90+beta[0]-10,10,leg-3);
//			inv_kine(front_right,90,7,leg);
//			inv_kine(middle_left,90,7,leg);
//			inv_kine(middle_right,90+beta[4]+25,10.5,leg-3);
//			inv_kine(rear_left,90+beta[2]-10,10,leg-3);
//			inv_kine(rear_right,90,7,leg);
//			delay_ms(SPEED);
//
//			//5
//			inv_kine(front_left,90-beta[0]+10,10,leg-3);
//			inv_kine(front_right,90+beta[3]+25,7,leg);
//			inv_kine(middle_left,90+beta[1]-10,7,leg);
//			inv_kine(middle_right,90-beta[4]-25,10.5,leg-3);
//			inv_kine(rear_left,90-beta[2]+10,10,leg-3);
//			inv_kine(rear_right,90+beta[5]+25,7,leg);
//			delay_ms(SPEED);
//
//			//6
//			inv_kine(front_left,90-beta[0]+10,7,leg);
//			inv_kine(front_right,90+beta[3]+25,7,leg);
//			inv_kine(middle_left,90+beta[1]-10,7,leg);
//			inv_kine(middle_right,90-beta[4]-25,7,leg);
//			inv_kine(rear_left,90-beta[2]+10,7,leg);
//			inv_kine(rear_right,90+beta[5]+25,7,leg);
//			delay_ms(SPEED);
//		}
//}

/*
 * Func : void mov_curve_left(int SPEED, int STEP)
 * brief: Robot's forward curve right locomotion, useful for wall following and solving the crossroad
 * Written By: Ryan (MON, MARCH 9th, 2015)
 * Ver  : 2 (Last Revised By: Ryan (MON, August 19th, 2015)
 *   	     sudut belok dikurangi (yang lama sering bermasalah untuk follow)
 * programmer's note  :
 */
void mov_curve_left(int SPEED, int STEP)
{
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	//gait_mode(CURVE_LEFT, SPEED);
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3]+15,10,leg-3);
	inv_kine(middle_left,90+beta[1]-10,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5]+15,10,leg-3);
	delay_ms(SPEED);

	//2
	inv_kine(front_left,90+beta[0]-10,7,leg);
	inv_kine(front_right,90-beta[3]-15,10,leg-3);
	inv_kine(middle_left,90-beta[1]+10,10,leg-3);
	inv_kine(middle_right,90+beta[4]+15,7,leg);
	inv_kine(rear_left,90+beta[2]-10,7,leg);
	inv_kine(rear_right,90-beta[5]-15,10,leg-3);
	delay_ms(SPEED);

	//3
	inv_kine(front_left,90+beta[0]-10,7,leg);
	inv_kine(front_right,90-beta[3]-15,7,leg);
	inv_kine(middle_left,90-beta[1]+10,7,leg);
	inv_kine(middle_right,90+beta[4]+15,7,leg);
	inv_kine(rear_left,90+beta[2]-10,7,leg);
	inv_kine(rear_right,90-beta[5]-15,7,leg);
	delay_ms(SPEED);

	//4
	inv_kine(front_left,90+beta[0]-10,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4]+15,10.5,leg-3);
	inv_kine(rear_left,90+beta[2]-10,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	//5
	inv_kine(front_left,90-beta[0]+10,10,leg-3);
	inv_kine(front_right,90+beta[3]+15,7,leg);
	inv_kine(middle_left,90+beta[1]-10,7,leg);
	inv_kine(middle_right,90-beta[4]-15,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]+10,10,leg-3);
	inv_kine(rear_right,90+beta[5]+15,7,leg);
	delay_ms(SPEED);

	//6
	inv_kine(front_left,90-beta[0]+10,7,leg);
	inv_kine(front_right,90+beta[3]+15,7,leg);
	inv_kine(middle_left,90+beta[1]-10,7,leg);
	inv_kine(middle_right,90-beta[4]-15,7,leg);
	inv_kine(rear_left,90-beta[2]+10,7,leg);
	inv_kine(rear_right,90+beta[5]+15,7,leg);
	delay_ms(SPEED);
}

///*
// * Func : void mov_askew_right(int distance, int SPEED, int STEP)
// * brief: move robot's body in askew motion to the right
// * 		  Every 10 cm translation to x coordinates, robot moves 1 cm to the right (y positive)
// * Written By: Ryan (MON, MARCH 9th, 2015)
// * Ver  : 2 (Last Revised By: Ryan (Friday, April 3rd, 2015)
// *   	     Configured for the new YUME's mechanical
// * programmer's note  :
// */
//void mov_askew_right(int distance, int SPEED, int STEP)
//{
//	int loop;
//	int limit= distance/5;
//	mov_static();
//	delay_ms(100);
//
//	/*
//	 * pengurangan/penambahan 20: tiap sequence satu langkah defleksi 0.5 cm
//	 * pengurangan/penambahan 40: terlalu extreme, body tengah justru yang berubah arah, mengganggu sensor ultrasonic
//	 * transitions dengan tripod gait tidak stabil
//	 * transition dengan tetrapo gait right handed, robot stabil
//	 *
//	 */
//
//	//TRANSITION FROM STATIC GAIT HIGH CLEARANCE
//	//USED GAIT: TETRAPOD/RIPPLE GAIT RIGHT HANDED
//	generate_movement_data(0,0,10);
//	delay_ms(150);
//
//	//1
//	inv_kine(front_left,60,10,leg-3);
//	inv_kine(front_right,90,7,leg);
//	inv_kine(middle_left,90,7,leg);
//	inv_kine(middle_right,90,7,leg);
//	inv_kine(rear_left,90,7,leg);
//	inv_kine(rear_right,120,10,leg-3);
//	delay_ms(150);
//
//	//2
//	inv_kine(front_left,60,7,leg);
//	inv_kine(front_right,90,7,leg);
//	inv_kine(middle_left,90,7,leg);
//	inv_kine(middle_right,90,7,leg);
//	inv_kine(rear_left,90,7,leg);
//	inv_kine(rear_right,120,7,leg);
//	delay_ms(150);
//
//	//3
//	inv_kine(front_left,60,7,leg);
//	inv_kine(front_right,90,7,leg);
//	inv_kine(middle_left,90,7,leg);
//	inv_kine(middle_right,120,10,leg-3);
//	inv_kine(rear_left,60,10,leg-3);
//	inv_kine(rear_right,120,7,leg);
//	delay_ms(150);
//
//	//4
//	inv_kine(front_left,60,7,leg);
//	inv_kine(front_right,90,7,leg);
//	inv_kine(middle_left,90,7,leg);
//	inv_kine(middle_right,120,7,leg);
//	inv_kine(rear_left,60,7,leg);
//	inv_kine(rear_right,120,7,leg);
//	delay_ms(150);
//
//	//5
//	inv_kine(front_left,60,7,leg);
//	inv_kine(front_right,120,10,leg-3);
//	inv_kine(middle_left,60,10,leg-3);
//	inv_kine(middle_right,120,7,leg);
//	inv_kine(rear_left,60,7,leg);
//	inv_kine(rear_right,120,7,leg);
//	delay_ms(150);
//
//	//FINAL FORM
//	inv_kine(front_left,60,7,leg);
//	inv_kine(front_right,120,7,leg);
//	inv_kine(middle_left,60,7,leg);
//	inv_kine(middle_right,120,7,leg);
//	inv_kine(rear_left,60,7,leg);
//	inv_kine(rear_right,120,7,leg);
//	delay_ms(150);
//
//	generate_movement_data(0,5,10);
//	delay_ms(150);
//	for(loop=1;loop<=limit;loop++)
//	{
//		//gait_mode(ASKEW_RIGHT, SPEED);
//		//1
//		inv_kine(front_left,60,7,leg);
//		inv_kine(front_right,120+beta[3],10,leg-3);
//		inv_kine(middle_left,60+beta[1],10,leg-3);
//		inv_kine(middle_right,120,7,leg);
//		inv_kine(rear_left,60,7,leg);
//		inv_kine(rear_right,120+beta[5],10,leg-3);
//		delay_ms(SPEED);
//
//		//2
//		inv_kine(front_left,60+beta[0],7,leg);
//		inv_kine(front_right,120-beta[3],10,leg-3);
//		inv_kine(middle_left,60-beta[1],10,leg-3);
//		inv_kine(middle_right,120+beta[4],7,leg);
//		inv_kine(rear_left,60+beta[2],7,leg);
//		inv_kine(rear_right,120-beta[5],10,leg-3);
//		delay_ms(SPEED);
//
//		//3
//		inv_kine(front_left,60+beta[0],7,leg);
//		inv_kine(front_right,120-beta[3],7,leg);
//		inv_kine(middle_left,60-beta[1],7,leg);
//		inv_kine(middle_right,120+beta[4],7,leg);
//		inv_kine(rear_left,60+beta[2],7,leg);
//		inv_kine(rear_right,120-beta[5],7,leg);
//		delay_ms(SPEED);
//
//		//4
//		inv_kine(front_left,60+beta[0],10,leg-3);
//		inv_kine(front_right,120,7,leg);
//		inv_kine(middle_left,60,7,leg);
//		inv_kine(middle_right,120+beta[4],10,leg-3);
//		inv_kine(rear_left,60+beta[2],10,leg-3);
//		inv_kine(rear_right,120,7,leg);
//		delay_ms(SPEED);
//
//		//5
//		inv_kine(front_left,60-beta[0],10,leg-3);
//		inv_kine(front_right,120+beta[3],7,leg);
//		inv_kine(middle_left,60+beta[1],7,leg);
//		inv_kine(middle_right,120-beta[4],10,leg-3);
//		inv_kine(rear_left,60-beta[2],10,leg-3);
//		inv_kine(rear_right,120+beta[5],7,leg);
//		delay_ms(SPEED);
//
//		//6
//		inv_kine(front_left,60-beta[0],7,leg);
//		inv_kine(front_right,120+beta[3],7,leg);
//		inv_kine(middle_left,60+beta[1],7,leg);
//		inv_kine(middle_right,120-beta[4],7,leg);
//		inv_kine(rear_left,60-beta[2],7,leg);
//		inv_kine(rear_right,120+beta[5],7,leg);
//		delay_ms(SPEED);
//
//	}
//
//}

/*
 * Func : void mov_askew_right_transition(void)
 * Brief: Transition gait as preparatory stage for askew right locomotion
 * 		  transition using quadrupedal gait
 * Param: N/A
 * Written By: Ryan (Thursday, August 6th, 2015
 * Ver  : 1
 * Programmer's Note: This function MUST be called in mov_askew_right function
 */
void mov_askew_right_transition(void)
{
	mov_static();
	delay_ms(100);

	/*
	 * pengurangan/penambahan 20: tiap sequence satu langkah defleksi 0.5 cm
	 * pengurangan/penambahan 40: terlalu extreme, body tengah justru yang berubah arah, mengganggu sensor ultrasonic
	 * transitions dengan tripod gait tidak stabil
	 * transition dengan tetrapo gait right handed, robot stabil
	 *
	 */

	//TRANSITION FROM STATIC GAIT HIGH CLEARANCE
	//USED GAIT: TETRAPOD/RIPPLE GAIT RIGHT HANDED
	generate_movement_data(0,0,10);
	delay_ms(150);

	//1
	inv_kine(front_left,60,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,120,10,leg-3);
	delay_ms(150);

	//2
	inv_kine(front_left,60,7,leg);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,120,7,leg);
	delay_ms(150);

	//3
	inv_kine(front_left,60,7,leg);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,120,10,leg-3);
	inv_kine(rear_left,60,10,leg-3);
	inv_kine(rear_right,120,7,leg);
	delay_ms(150);

	//4
	inv_kine(front_left,60,7,leg);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,120,7,leg);
	inv_kine(rear_left,60,7,leg);
	inv_kine(rear_right,120,7,leg);
	delay_ms(150);

	//5
	inv_kine(front_left,60,7,leg);
	inv_kine(front_right,120,10,leg-3);
	inv_kine(middle_left,60,10,leg-3);
	inv_kine(middle_right,120,7,leg);
	inv_kine(rear_left,60,7,leg);
	inv_kine(rear_right,120,7,leg);
	delay_ms(150);

	//FINAL FORM
	inv_kine(front_left,60,7,leg);
	inv_kine(front_right,120,7,leg);
	inv_kine(middle_left,60,7,leg);
	inv_kine(middle_right,120,7,leg);
	inv_kine(rear_left,60,7,leg);
	inv_kine(rear_right,120,7,leg);
	delay_ms(150);


}

/*
 * Func : void mov_askew_right(int SPEED, int STEP)
 * brief: move robot's body in askew motion to the right
 * 		  Every 10 cm translation to x coordinates, robot moves 1 cm to the right (y positive)
 * Written By: Ryan (MON, August 3rd, 2015)
 * Ver  : 2
 * Revised By: Ryan (Thursday, August 7th, 2015)
 * programmer's note  :
 */
void mov_askew_right(int SPEED, int STEP)
{

	generate_movement_data(0,5,10);
	delay_ms(150);
	//gait_mode(ASKEW_RIGHT, SPEED);
	//1
	inv_kine(front_left,60,7,leg);
	inv_kine(front_right,120+beta[3],10,leg-3);
	inv_kine(middle_left,60+beta[1],10,leg-3);
	inv_kine(middle_right,120,7,leg);
	inv_kine(rear_left,60,7,leg);
	inv_kine(rear_right,120+beta[5],10,leg-3);
	delay_ms(SPEED);

	//2
	inv_kine(front_left,60+beta[0],7,leg);
	inv_kine(front_right,120-beta[3],10,leg-3);
	inv_kine(middle_left,60-beta[1],10,leg-3);
	inv_kine(middle_right,120+beta[4],7,leg);
	inv_kine(rear_left,60+beta[2],7,leg);
	inv_kine(rear_right,120-beta[5],10,leg-3);
	delay_ms(SPEED);

	//3
	inv_kine(front_left,60+beta[0],7,leg);
	inv_kine(front_right,120-beta[3],7,leg);
	inv_kine(middle_left,60-beta[1],7,leg);
	inv_kine(middle_right,120+beta[4],7,leg);
	inv_kine(rear_left,60+beta[2],7,leg);
	inv_kine(rear_right,120-beta[5],7,leg);
	delay_ms(SPEED);

	//4
	inv_kine(front_left,60+beta[0],10,leg-3);
	inv_kine(front_right,120,7,leg);
	inv_kine(middle_left,60,7,leg);
	inv_kine(middle_right,120+beta[4],10,leg-3);
	inv_kine(rear_left,60+beta[2],10,leg-3);
	inv_kine(rear_right,120,7,leg);
	delay_ms(SPEED);

	//5
	inv_kine(front_left,60-beta[0],10,leg-3);
	inv_kine(front_right,120+beta[3],7,leg);
	inv_kine(middle_left,60+beta[1],7,leg);
	inv_kine(middle_right,120-beta[4],10,leg-3);
	inv_kine(rear_left,60-beta[2],10,leg-3);
	inv_kine(rear_right,120+beta[5],7,leg);
	delay_ms(SPEED);

	//6
	inv_kine(front_left,60-beta[0],7,leg);
	inv_kine(front_right,120+beta[3],7,leg);
	inv_kine(middle_left,60+beta[1],7,leg);
	inv_kine(middle_right,120-beta[4],7,leg);
	inv_kine(rear_left,60-beta[2],7,leg);
	inv_kine(rear_right,120+beta[5],7,leg);
	delay_ms(SPEED);

}

///*
// * Func: void mov_askew_left(int distance, int SPEED, int STEP)
// * brief: move robot's body in askew motion to the left
// * Every 10 cm translation to x coordinates, robot moves 1 cm to the left (y negative)
// * Written By: Ryan (MON, MARCH 9th, 2015)
// * Ver  : 2 (Last Revised By: Ryan (Friday, April 3rd, 2015)
// *   	     Configured for the new YUME's mechanical
// * programmer's note  :
// */
//void mov_askew_left(int distance, int SPEED, int STEP)
//{
//	int loop;
//	int limit= distance/5;
//	mov_static();
//	delay_ms(100);
//
//	/*
//	 * pengurangan/penambahan 20: tiap sequence satu langkah defleksi 0.5 cm
//	 * pengurangan/penambahan 40: terlalu extreme, body tengah justru yang berubah arah, mengganggu sensor ultrasonic
//	 * transitions dengan tripod gait tidak stabil
//	 * transition dengan tetrapo gait right handed, robot stabil
//	 *
//	 */
//
//	//TRANSITION FROM STATIC GAIT HIGH CLEARANCE
//	//USED GAIT: TETRAPOD/RIPPLE GAIT LEFT HANDED
//	generate_movement_data(0,0,10);
//	delay_ms(150);
//
//	//1
//	inv_kine(front_left,90,7,leg);
//	inv_kine(front_right,90,7,leg);
//	inv_kine(middle_left,90,7,leg);
//	inv_kine(middle_right,90,7,leg);
//	inv_kine(rear_left,90,7,leg);
//	inv_kine(rear_right,90,7,leg);
//	delay_ms(150);
//
//	//PHASE 1 STARTED
//	//2
//	inv_kine(front_left,90,7,leg);
//	inv_kine(front_right,60,10,leg-3);
//	inv_kine(middle_left,90,7,leg);
//	inv_kine(middle_right,90,7,leg);
//	inv_kine(rear_left,120,10,leg-3);
//	inv_kine(rear_right,90,7,leg);
//	delay_ms(150);
//
//	//PHASE 1 COMPLETED
//	//3
//	inv_kine(front_left,90,7,leg);
//	inv_kine(front_right,60,7,leg);
//	inv_kine(middle_left,90,7,leg);
//	inv_kine(middle_right,90,7,leg);
//	inv_kine(rear_left,120,7,leg);
//	inv_kine(rear_right,90,7,leg);
//	delay_ms(150);
//
//	//PHASE 2 STARTED
//	//4
//	inv_kine(front_left,90,7,leg);
//	inv_kine(front_right,60,7,leg);
//	inv_kine(middle_left,120,10,leg-3);
//	inv_kine(middle_right,90,7,leg);
//	inv_kine(rear_left,120,7,leg);
//	inv_kine(rear_right,60,10,leg-3);
//	delay_ms(150);
//
//	//PHASE 2 COMPLETED
//	//5
//	inv_kine(front_left,90,7,leg);
//	inv_kine(front_right,60,7,leg);
//	inv_kine(middle_left,120,7,leg);
//	inv_kine(middle_right,90,7,leg);
//	inv_kine(rear_left,120,7,leg);
//	inv_kine(rear_right,60,10,leg);
//	delay_ms(150);
//
//	//PHASE 3 STARTED
//	//5
//	inv_kine(front_left,120,10,leg-3);
//	inv_kine(front_right,60,7,leg);
//	inv_kine(middle_left,120,7,leg);
//	inv_kine(middle_right,60,10,leg-3);
//	inv_kine(rear_left,120,7,leg);
//	inv_kine(rear_right,60,7,leg);
//	delay_ms(150);
//
//	//FINAL FORM
//	inv_kine(front_left,120,7,leg);
//	inv_kine(front_right,60,7,leg);
//	inv_kine(middle_left,120,7,leg);
//	inv_kine(middle_right,60,7,leg);
//	inv_kine(rear_left,120,7,leg);
//	inv_kine(rear_right,60,7,leg);
//	delay_ms(150);
//
//	generate_movement_data(0,5,10);
//	delay_ms(150);
//	for(loop=1;loop<=limit;loop++)
//	{
//		//gait_mode(ASKEW_LEFT, &SPEED);
//		//1
//		inv_kine(front_left,120,7,leg);
//		inv_kine(front_right,60+beta[3],10,leg-3);
//		inv_kine(middle_left,120+beta[1],10,leg-3);
//		inv_kine(middle_right,60,7,leg);
//		inv_kine(rear_left,120,7,leg);
//		inv_kine(rear_right,60+beta[5],10,leg-3);
//		delay_ms(SPEED);
//
//		//2
//		inv_kine(front_left,120+beta[0],7,leg);
//		inv_kine(front_right,60-beta[3],10,leg-3);
//		inv_kine(middle_left,120-beta[1],10,leg-3);
//		inv_kine(middle_right,60+beta[4],7,leg);
//		inv_kine(rear_left,120+beta[2],7,leg);
//		inv_kine(rear_right,60-beta[5],10,leg-3);
//		delay_ms(SPEED);
//
//		//3
//		inv_kine(front_left,120+beta[0],7,leg);
//		inv_kine(front_right,60-beta[3],7,leg);
//		inv_kine(middle_left,120-beta[1],7,leg);
//		inv_kine(middle_right,60+beta[4],7,leg);
//		inv_kine(rear_left,120+beta[2],7,leg);
//		inv_kine(rear_right,60-beta[5],7,leg);
//		delay_ms(SPEED);
//
//		//4
//		inv_kine(front_left,120+beta[0],10,leg-3);
//		inv_kine(front_right,60,7,leg);
//		inv_kine(middle_left,120,7,leg);
//		inv_kine(middle_right,60+beta[4],10,leg-3);
//		inv_kine(rear_left,120+beta[2],10,leg-3);
//		inv_kine(rear_right,60,7,leg);
//		delay_ms(SPEED);
//
//		//5
//		inv_kine(front_left,120-beta[0],10,leg-3);
//		inv_kine(front_right,60+beta[3],7,leg);
//		inv_kine(middle_left,120+beta[1],7,leg);
//		inv_kine(middle_right,60-beta[4],10,leg-3);
//		inv_kine(rear_left,120-beta[2],10,leg-3);
//		inv_kine(rear_right,60+beta[5],7,leg);
//		delay_ms(SPEED);
//
//		//6
//		inv_kine(front_left,120-beta[0],7,leg);
//		inv_kine(front_right,60+beta[3],7,leg);
//		inv_kine(middle_left,120+beta[1],7,leg);
//		inv_kine(middle_right,60-beta[4],7,leg);
//		inv_kine(rear_left,120-beta[2],7,leg);
//		inv_kine(rear_right,60+beta[5],7,leg);
//		delay_ms(SPEED);
//	}
//}


/*
 * Func : void mov_askew_left_transition(void)
 * Brief: Transition gait as preparatory stage for askew left locomotion
 * 		  transition using quadrupedal gait
 * Param: N/A
 * Written By: Ryan (Thursday, August 6th, 2015
 * Ver  : 1
 * Programmer's Note: This function MUST be called in mov_askew_left function
 */
void mov_askew_left_transition(void)
{
	mov_static();
	delay_ms(100);

	/*
	 * pengurangan/penambahan 20: tiap sequence satu langkah defleksi 0.5 cm
	 * pengurangan/penambahan 40: terlalu extreme, body tengah justru yang berubah arah, mengganggu sensor ultrasonic
	 * transitions dengan tripod gait tidak stabil
	 * transition dengan tetrapo gait right handed, robot stabil
	 *
	 */
	//TRANSITION FROM STATIC GAIT HIGH CLEARANCE
	//USED GAIT: TETRAPOD/RIPPLE GAIT LEFT HANDED
	generate_movement_data(0,0,10);
	delay_ms(150);

	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90,7,leg);
	delay_ms(150);

	//PHASE 1 STARTED
	//2
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,60,10,leg-3);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,120,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(150);

	//PHASE 1 COMPLETED
	//3
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,60,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,120,7,leg);
	inv_kine(rear_right,90,7,leg);
	delay_ms(150);

	//PHASE 2 STARTED
	//4
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,60,7,leg);
	inv_kine(middle_left,120,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,120,7,leg);
	inv_kine(rear_right,60,10,leg-3);
	delay_ms(150);

	//PHASE 2 COMPLETED
	//5
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,60,7,leg);
	inv_kine(middle_left,120,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,120,7,leg);
	inv_kine(rear_right,60,10,leg);
	delay_ms(150);

	//PHASE 3 STARTED
	//5
	inv_kine(front_left,120,10,leg-3);
	inv_kine(front_right,60,7,leg);
	inv_kine(middle_left,120,7,leg);
	inv_kine(middle_right,60,10,leg-3);
	inv_kine(rear_left,120,7,leg);
	inv_kine(rear_right,60,7,leg);
	delay_ms(150);

	//FINAL FORM
	inv_kine(front_left,120,7,leg);
	inv_kine(front_right,60,7,leg);
	inv_kine(middle_left,120,7,leg);
	inv_kine(middle_right,60,7,leg);
	inv_kine(rear_left,120,7,leg);
	inv_kine(rear_right,60,7,leg);
	delay_ms(150);
}


/*
 * Func: void mov_askew_left(int SPEED, int STEP)
 * brief: move robot's body in askew motion to the left
 * Every 10 cm translation to x coordinates, robot moves 1 cm to the left (y negative)
 * Written By: Ryan (MON, MARCH 9th, 2015)
 * Ver  : 2
 * Revised By: Ryan (Thursday, August 6th, 2015)
 * programmer's note  :
 */
void mov_askew_left(int SPEED, int STEP)
{
	generate_movement_data(0,5,10);
	delay_ms(150);

	//gait_mode(ASKEW_LEFT, &SPEED);
	//1
	inv_kine(front_left,120,7,leg);
	inv_kine(front_right,60+beta[3],10,leg-3);
	inv_kine(middle_left,120+beta[1],10,leg-3);
	inv_kine(middle_right,60,7,leg);
	inv_kine(rear_left,120,7,leg);
	inv_kine(rear_right,60+beta[5],10,leg-3);
	delay_ms(SPEED);

	//2
	inv_kine(front_left,120+beta[0],7,leg);
	inv_kine(front_right,60-beta[3],10,leg-3);
	inv_kine(middle_left,120-beta[1],10,leg-3);
	inv_kine(middle_right,60+beta[4],7,leg);
	inv_kine(rear_left,120+beta[2],7,leg);
	inv_kine(rear_right,60-beta[5],10,leg-3);
	delay_ms(SPEED);

	//3
	inv_kine(front_left,120+beta[0],7,leg);
	inv_kine(front_right,60-beta[3],7,leg);
	inv_kine(middle_left,120-beta[1],7,leg);
	inv_kine(middle_right,60+beta[4],7,leg);
	inv_kine(rear_left,120+beta[2],7,leg);
	inv_kine(rear_right,60-beta[5],7,leg);
	delay_ms(SPEED);

	//4
	inv_kine(front_left,120+beta[0],10,leg-3);
	inv_kine(front_right,60,7,leg);
	inv_kine(middle_left,120,7,leg);
	inv_kine(middle_right,60+beta[4],10,leg-3);
	inv_kine(rear_left,120+beta[2],10,leg-3);
	inv_kine(rear_right,60,7,leg);
	delay_ms(SPEED);

	//5
	inv_kine(front_left,120-beta[0],10,leg-3);
	inv_kine(front_right,60+beta[3],7,leg);
	inv_kine(middle_left,120+beta[1],7,leg);
	inv_kine(middle_right,60-beta[4],10,leg-3);
	inv_kine(rear_left,120-beta[2],10,leg-3);
	inv_kine(rear_right,60+beta[5],7,leg);
	delay_ms(SPEED);

	//6
	inv_kine(front_left,120-beta[0],7,leg);
	inv_kine(front_right,60+beta[3],7,leg);
	inv_kine(middle_left,120+beta[1],7,leg);
	inv_kine(middle_right,60-beta[4],7,leg);
	inv_kine(rear_left,120-beta[2],7,leg);
	inv_kine(rear_right,60+beta[5],7,leg);
	delay_ms(SPEED);
}

///*
// * Func: void mov_fwd_ripple_gait(int distance, int SPEED)
// * brief: hexapod forward locomotion using ripple gait/tetrapod gait
// * Written By: Ryan (TUE, MARCH 10th, 2015)
// */
//void mov_fwd_ripple_gait(int distance, int SPEED)
//{
//	int limit= distance/5;
//	int loop;
//	generate_movement_data(0,5,10);
//	delay_ms(50);
//
//	for (loop=1;loop<=limit;loop++)
//		{
//			//gait_mode(FWD_RIPPLE, &SPEED);
//			//PHASE 1 STARTED
//			inv_kine(front_left,90-beta[0],10,leg-3); //MAJU ANGKAT
//			inv_kine(front_right,90,7,leg);
//			inv_kine(middle_left,90,7,leg);
//			inv_kine(middle_right,90+beta[4],7,leg); //REAR
//			inv_kine(rear_left,90+beta[2],7,leg); //REAR
//			inv_kine(rear_right,90-beta[5],10,leg-3); //MAJU ANGKAT
//			delay_ms(SPEED);
//
//			//PHASE 1
//			inv_kine(front_left,90-beta[0],7,leg); //MAJU MENAPAK
//			inv_kine(front_right,90,7,leg);
//			inv_kine(middle_left,90,7,leg);
//			inv_kine(middle_right,90+beta[4],7,leg); //REAR
//			inv_kine(rear_left,90+beta[2],7,leg); //REAR
//			inv_kine(rear_right,90-beta[5],7,leg); //MAJU MENAPAK
//			delay_ms(SPEED);
//
//			//PHASE 2 STARTED
//			inv_kine(front_left,90,7,leg);
//			inv_kine(front_right,90+beta[3],7,leg);//REAR
//			inv_kine(middle_left,90+beta[1],7,leg);//REAR
//			inv_kine(middle_right,90-beta[4],10,leg-3); //MAJU ANGKAT
//			inv_kine(rear_left,90-beta[2],10,leg-3); //MAJU ANGKAT
//			inv_kine(rear_right,90,7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 2
//			inv_kine(front_left,90,7,leg);
//			inv_kine(front_right,90+beta[3],7,leg);//REAR
//			inv_kine(middle_left,90+beta[1],7,leg);//REAR
//			inv_kine(middle_right,90-beta[4],7,leg); //MAJU MENAPAK
//			inv_kine(rear_left,90-beta[2],7,leg); //MAJU MENAPAK
//			inv_kine(rear_right,90,7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 3 STARTED
//			inv_kine(front_left,90+beta[0],8,leg);//REAR
//			inv_kine(front_right,90-beta[3],10,leg-3); //MAJU ANGKAT
//			inv_kine(middle_left,90-beta[1],10,leg-3); //MAJU ANGKAT
//			inv_kine(middle_right,90,7,leg);
//			inv_kine(rear_left,90,7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg); //REAR
//			delay_ms(SPEED);
//
//			//PHASE 3 STARTED
//			inv_kine(front_left,90+beta[0],7,leg);//REAR
//			inv_kine(front_right,90-beta[3],7,leg); //MAJU MENAPAK
//			inv_kine(middle_left,90-beta[1],7,leg); //MAJU MENAPAK
//			inv_kine(middle_right,90,7,leg);
//			inv_kine(rear_left,90,7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg); //REAR
//			delay_ms(SPEED);
//
//		}
//}

/*
 * Func: void mov_fwd_ripple_gait(int distance, int SPEED)
 * brief: hexapod forward locomotion using ripple gait/tetrapod gait
 * Written By: Ryan (TUE, MARCH 10th, 2015)
 * Revised By: Ryan (Tuesday, AUgust 4th, 2015)
 *
 */
void mov_fwd_ripple_gait(int SPEED)
{
	generate_movement_data(0,5,10);
	delay_ms(50);

	//gait_mode(FWD_RIPPLE, &SPEED);
	//PHASE 1 STARTED
	inv_kine(front_left,90-beta[0],10,leg-3); //MAJU ANGKAT
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg); //REAR
	inv_kine(rear_left,90+beta[2],7,leg); //REAR
	inv_kine(rear_right,90-beta[5],10,leg-3); //MAJU ANGKAT
	delay_ms(SPEED);

	//PHASE 1
	inv_kine(front_left,90-beta[0],7,leg); //MAJU MENAPAK
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg); //REAR
	inv_kine(rear_left,90+beta[2],7,leg); //REAR
	inv_kine(rear_right,90-beta[5],7,leg); //MAJU MENAPAK
	delay_ms(SPEED);

	//PHASE 2 STARTED
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);//REAR
	inv_kine(middle_left,90+beta[1],7,leg);//REAR
	inv_kine(middle_right,90-beta[4],10,leg-3); //MAJU ANGKAT
	inv_kine(rear_left,90-beta[2],10,leg-3); //MAJU ANGKAT
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	//PHASE 2
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);//REAR
	inv_kine(middle_left,90+beta[1],7,leg);//REAR
	inv_kine(middle_right,90-beta[4],7,leg); //MAJU MENAPAK
	inv_kine(rear_left,90-beta[2],7,leg); //MAJU MENAPAK
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	//PHASE 3 STARTED
	inv_kine(front_left,90+beta[0],8,leg);//REAR
	inv_kine(front_right,90-beta[3],10,leg-3); //MAJU ANGKAT
	inv_kine(middle_left,90-beta[1],10,leg-3); //MAJU ANGKAT
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg); //REAR
	delay_ms(SPEED);

	//PHASE 3 STARTED
	inv_kine(front_left,90+beta[0],7,leg);//REAR
	inv_kine(front_right,90-beta[3],7,leg); //MAJU MENAPAK
	inv_kine(middle_left,90-beta[1],7,leg); //MAJU MENAPAK
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg); //REAR
	delay_ms(SPEED);

}


//void mov_bwd_ripple_gait( int distance, int SPEED)
//{
//	int limit= distance/5;
//	int loop;
//	generate_movement_data(0,5,10);
//	delay_ms(50);
//
//	for (loop=1;loop<=limit;loop++)
//		{
//
//		//PHASE 1
//		inv_kine(front_left,90+beta[0]+5,7.5,leg); //MAJU MENAPAK
//		inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
//		inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
//		inv_kine(middle_right,90,7,leg);
//		inv_kine(rear_left,90,7,leg);
//		inv_kine(rear_right,90+beta[5],7,leg); //MAJU MENAPAK
//		delay_ms(250);
//
//		//PHASE 2 STARTED
//		inv_kine(front_left,90,7.5,leg);
//		inv_kine(front_right,90+beta[3],7,leg-3); //MAJU-ANGKAT
//		inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
//		inv_kine(middle_right,90-beta[4],7,leg); //REAR
//		inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
//		inv_kine(rear_right,90,7,leg);
//		delay_ms(250);
//
//		//PHASE 2 MID
//		inv_kine(front_left,90,7.5,leg);
//		inv_kine(front_right,90+beta[3],7,leg); //MAJU-MENAPAK
//		inv_kine(middle_left,90+beta[1]+5,8,leg-3); //MAJU-ANGKAT
//		inv_kine(middle_right,90-beta[4],7,leg); //REAR
//		inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
//		inv_kine(rear_right,90,7,leg);
//		delay_ms(250);
//
//		//PHASE 2 END
//		inv_kine(front_left,90,7.5,leg);
//		inv_kine(front_right,90+beta[3],7,leg); //MAJU-MENAPAK
//		inv_kine(middle_left,90+beta[1]+5,8,leg); //MAJU-MENAPAK
//		inv_kine(middle_right,90-beta[4],7,leg); //REAR
//		inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
//		inv_kine(rear_right,90,7,leg);
//		delay_ms(250);
//
//
//		//PHASE 3 STARTED
//		inv_kine(front_left,90-beta[0],7.5,leg); //REAR
//		inv_kine(front_right,90,7,leg);
//		inv_kine(middle_left,90,8,leg);
//		inv_kine(middle_right,90+beta[4],7,leg-3); //MAJU-ANGKAT
//		inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
//		inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
//		delay_ms(250);
//
//		//PHASE 3 MID
//		inv_kine(front_left,90-beta[0],7.5,leg); //REAR
//		inv_kine(front_right,90,7,leg);
//		inv_kine(middle_left,90,8,leg);
//		inv_kine(middle_right,90+beta[4],7,leg); //MAJU-MENAPAK
//		inv_kine(rear_left,90+beta[2]+10,7,leg-3); //MAJU-ANGKAT
//		inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
//		delay_ms(250);
//
//		//PHASE 3 END
//		inv_kine(front_left,90-beta[0],7.5,leg); //REAR
//		inv_kine(front_right,90,7,leg);
//		inv_kine(middle_left,90,8,leg);
//		inv_kine(middle_right,90+beta[4],7,leg); //MAJU-MENAPAK
//		inv_kine(rear_left,90+beta[2]+10,7,leg); //MAJU-MENAPAK
//		inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
//		delay_ms(250);
//
//		//PHASE 1 STARTED
//		inv_kine(front_left,90-beta[0],7.5,leg); //REAR
//		inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
//		inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
//		inv_kine(middle_right,90,7,leg);
//		inv_kine(rear_left,90,7,leg);
//		inv_kine(rear_right,90+beta[5],10,leg-3); //MAJU ANGKAT
//		delay_ms(250);
//
//		//PHASE 1 MID
//		inv_kine(front_left,90+beta[0]+5,7.5,leg-3); //MAJU ANGKAT
//		inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
//		inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
//		inv_kine(middle_right,90,7,leg);
//		inv_kine(rear_left,90,7,leg);
//		inv_kine(rear_right,90+beta[5],10,leg); //MAJU-MENAPAK
//		delay_ms(250);
//
//
//		/*
//			//gait_mode(BWD_RIPPLE, &SPEED);
//			//PHASE 1 STARTED
//			inv_kine(front_left,90+beta[0]+5,7.5,leg-3); //MAJU ANGKAT
//			inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
//			inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
//			inv_kine(middle_right,90,7,leg);
//			inv_kine(rear_left,90,7,leg);
//			inv_kine(rear_right,90+beta[5],10,leg-3); //MAJU ANGKAT
//			delay_ms(250);
//
//			//PHASE 1
//			inv_kine(front_left,90+beta[0]+5,7.5,leg); //MAJU MENAPAK
//			inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
//			inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
//			inv_kine(middle_right,90,7,leg);
//			inv_kine(rear_left,90,7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg); //MAJU MENAPAK
//			delay_ms(250);
//
//			//PHASE 2 STARTED
//			inv_kine(front_left,90,7.5,leg);
//			inv_kine(front_right,90+beta[3],7,leg-3); //MAJU-ANGKAT
//			inv_kine(middle_left,90+beta[1]+5,8,leg-3); //MAJU-ANGKAT
//			inv_kine(middle_right,90-beta[4],7,leg); //REAR
//			inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
//			inv_kine(rear_right,90,7,leg);
//			delay_ms(250);
//
//			//PHASE 2
//			inv_kine(front_left,90,7.5,leg);
//			inv_kine(front_right,90+beta[3],7,leg); //MAJU-MENAPAK
//			inv_kine(middle_left,90+beta[1]+5,8,leg); //MAJU-MENAPAK
//			inv_kine(middle_right,90-beta[4],7,leg); //REAR
//			inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
//			inv_kine(rear_right,90,7,leg);
//			delay_ms(250);
//
//			//PHASE 3 STARTED
//			inv_kine(front_left,90-beta[0],7.5,leg); //REAR
//			inv_kine(front_right,90,7,leg);
//			inv_kine(middle_left,90,8,leg);
//			inv_kine(middle_right,90+beta[4],7,leg-3); //MAJU-ANGKAT
//			inv_kine(rear_left,90+beta[2]+10,7,leg-3); //MAJU-ANGKAT
//			inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
//			delay_ms(250);
//
//			//PHASE 3
//			inv_kine(front_left,90-beta[0],7.5,leg); //REAR
//			inv_kine(front_right,90,7,leg);
//			inv_kine(middle_left,90,8,leg);
//			inv_kine(middle_right,90+beta[4],7,leg); //MAJU-MENAPAK
//			inv_kine(rear_left,90+beta[2]+10,7,leg); //MAJU-MENAPAK
//			inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
//*/
//		}
//}

/*
 * Func: void mov_bwd_ripple_gait(int SPEED)
 * brief: hexapod forward locomotion using ripple gait/tetrapod gait
 * Written By: Ryan (TUE, MARCH 10th, 2015)
 * Revised By: Ryan (Tuesday, AUgust 4th, 2015)
 *
 */
void mov_bwd_ripple_gait(int SPEED)
{

	generate_movement_data(0,5,10);
	delay_ms(50);

	//PHASE 1
	inv_kine(front_left,90+beta[0]+5,7.5,leg); //MAJU MENAPAK
	inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
	inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg); //MAJU MENAPAK
	delay_ms(250);

	//PHASE 2 STARTED
	inv_kine(front_left,90,7.5,leg);
	inv_kine(front_right,90+beta[3],7,leg-3); //MAJU-ANGKAT
	inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
	inv_kine(middle_right,90-beta[4],7,leg); //REAR
	inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
	inv_kine(rear_right,90,7,leg);
	delay_ms(250);

	//PHASE 2 MID
	inv_kine(front_left,90,7.5,leg);
	inv_kine(front_right,90+beta[3],7,leg); //MAJU-MENAPAK
	inv_kine(middle_left,90+beta[1]+5,8,leg-3); //MAJU-ANGKAT
	inv_kine(middle_right,90-beta[4],7,leg); //REAR
	inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
	inv_kine(rear_right,90,7,leg);
	delay_ms(250);

	//PHASE 2 END
	inv_kine(front_left,90,7.5,leg);
	inv_kine(front_right,90+beta[3],7,leg); //MAJU-MENAPAK
	inv_kine(middle_left,90+beta[1]+5,8,leg); //MAJU-MENAPAK
	inv_kine(middle_right,90-beta[4],7,leg); //REAR
	inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
	inv_kine(rear_right,90,7,leg);
	delay_ms(250);

	//PHASE 3 STARTED
	inv_kine(front_left,90-beta[0],7.5,leg); //REAR
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,8,leg);
	inv_kine(middle_right,90+beta[4],7,leg-3); //MAJU-ANGKAT
	inv_kine(rear_left,90-beta[2]-10,7,leg); //REAR
	inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
	delay_ms(250);

	//PHASE 3 MID
	inv_kine(front_left,90-beta[0],7.5,leg); //REAR
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,8,leg);
	inv_kine(middle_right,90+beta[4],7,leg); //MAJU-MENAPAK
	inv_kine(rear_left,90+beta[2]+10,7,leg-3); //MAJU-ANGKAT
	inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
	delay_ms(250);

	//PHASE 3 END
	inv_kine(front_left,90-beta[0],7.5,leg); //REAR
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,8,leg);
	inv_kine(middle_right,90+beta[4],7,leg); //MAJU-MENAPAK
	inv_kine(rear_left,90+beta[2]+10,7,leg); //MAJU-MENAPAK
	inv_kine(rear_right,90-beta[5]-5,7.5,leg); //REAR
	delay_ms(250);

	//PHASE 1 STARTED
	inv_kine(front_left,90-beta[0],7.5,leg); //REAR
	inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
	inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3); //MAJU ANGKAT
	delay_ms(250);

	//PHASE 1 MID
	inv_kine(front_left,90+beta[0]+5,7.5,leg-3); //MAJU ANGKAT
	inv_kine(front_right,90-beta[3]-5,7,leg); //REAR
	inv_kine(middle_left,90-beta[1]-5,8,leg); //REAR
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg); //MAJU-MENAPAK
	delay_ms(250);

}

///*
// * Func: void mov_fwd_wave_gait(int distance, int SPEED)
// * brief: hexapod forward locomotion using wave gait (one leg at a time)
// * Written By: Ryan (TUE, MARCH 10th, 2015)
// */
//void mov_fwd_wave_gait(int distance, int SPEED)
//{
//	int limit= distance/5;
//	int loop;
//	generate_movement_data(0,10,10);
//	delay_ms(50);
//
//	for (loop=1;loop<=limit;loop++)
//		{
//			//gait_mode(FWD_WAVE, &SPEED);
//			//PHASE 1 STARTED
//			inv_kine(front_left,90+beta[0],7,leg);
//			inv_kine(front_right,90+beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90+beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 1
//			inv_kine(front_left,90+beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],10,leg-3);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90+beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 1
//			inv_kine(front_left,90+beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1]+5,7,leg);
//			inv_kine(middle_right,90+beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 2 STARTED
//			inv_kine(front_left,90+beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],10,leg-3);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 2
//			inv_kine(front_left,90+beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90+beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 3 STARTED
//			inv_kine(front_left,90+beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2]+5,7,leg);
//			inv_kine(rear_right,90-beta[5],10,leg-3);
//			delay_ms(SPEED);
//
//			//PHASE 3
//			inv_kine(front_left,90+beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90-beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 4 STARTED
//			inv_kine(front_left,90-beta[0],10,leg-3);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90-beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 4
//			inv_kine(front_left,90-beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90+beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90-beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 5 STARTED
//			inv_kine(front_left,90-beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90-beta[1],10,leg-3);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90-beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 5
//			inv_kine(front_left,90-beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90-beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90+beta[2],7,leg);
//			inv_kine(rear_right,90-beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 6 STARTED
//			inv_kine(front_left,90-beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90-beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90-beta[2],10,leg-3);
//			inv_kine(rear_right,90-beta[5],7,leg);
//			delay_ms(SPEED);
//
//			//PHASE 6 STARTED
//			inv_kine(front_left,90-beta[0],7,leg);
//			inv_kine(front_right,90-beta[3],7,leg);
//			inv_kine(middle_left,90-beta[1],7,leg);
//			inv_kine(middle_right,90-beta[4],7,leg);
//			inv_kine(rear_left,90-beta[2],7,leg);
//			inv_kine(rear_right,90-beta[5],7,leg);
//			delay_ms(SPEED);
//		}
//
//}

/*
 * Func: void mov_fwd_wave_gait(int SPEED)
 * brief: hexapod forward locomotion using wave gait (one leg at a time)
 * Written By: Ryan (TUE, August 4th, 2015)
 */
void mov_fwd_wave_gait(int SPEED)
{

	generate_movement_data(0,10,10);
	delay_ms(50);

	//gait_mode(FWD_WAVE, &SPEED);
	//PHASE 1 STARTED
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 1
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90-beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 1
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+5,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 2 STARTED
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],10,leg-3);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 2
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 3 STARTED
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+5,7,leg);
	inv_kine(rear_right,90-beta[5],10,leg-3);
	delay_ms(SPEED);

	//PHASE 3
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 4 STARTED
	inv_kine(front_left,90-beta[0],10,leg-3);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 4
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90+beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 5 STARTED
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90-beta[1],10,leg-3);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 5
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90-beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 6 STARTED
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90-beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90-beta[2],10,leg-3);
	inv_kine(rear_right,90-beta[5],7,leg);
	delay_ms(SPEED);

	//PHASE 6 STARTED
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg);
	inv_kine(middle_left,90-beta[1],7,leg);
	inv_kine(middle_right,90-beta[4],7,leg);
	inv_kine(rear_left,90-beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg);
	delay_ms(SPEED);

}

//void mov_celebrate(int loop)
//{
//	int step;
//	int ML=60,MR=60,RL=120,RR=120;
//	mov_static();
//	delay_ms(1000);
//
//	for(step=0;step<=loop;step++)
//	{
//		//STATIC
//		inv_kine(front_left,110,11,leg-5);
//		inv_kine(front_right,30,12,leg-6);
//		inv_kine(middle_left,60,7,leg);
//		inv_kine(middle_right,60,7,leg);
//		inv_kine(rear_left,120,10,leg-3);
//		inv_kine(rear_right,120,10,leg-3);
//		delay_ms(300);
//
//		inv_kine(front_left,25,12,leg-5);
//		inv_kine(front_right,110,11,leg-6);
//		inv_kine(middle_left,60,7,leg);
//		inv_kine(middle_right,60,7,leg);
//		inv_kine(rear_left,120,10,leg-3);
//		inv_kine(rear_right,120,10,leg-3);
//		delay_ms(300);
//
//		//LEFT
//		while((ML<=100)&&(MR>=20)&&(RL<=160)&&(RR>=80))
//		{
//			inv_kine(front_left,25,12,leg-5);
//			inv_kine(front_right,30,12,leg-6);
//
//			inv_kine(middle_left,ML,7,leg);
//			inv_kine(middle_right,MR,7,leg);
//			inv_kine(rear_left,RL,10,leg-3);
//	    	inv_kine(rear_right,RR,10,leg-3);
//	    	delay_ms(2);
//	    	ML+=1;
//	    	MR-=1;
//	    	RL+=1;
//	    	RR-=1;
//		}
//		inv_kine(front_left,110,11,leg-5);
//		inv_kine(front_right,30,12,leg-6);
//		delay_ms(300);
//		inv_kine(front_left,25,12,leg-5);
//		inv_kine(front_right,110,11,leg-6);
//		delay_ms(300);
//
//		while((ML>=60)&&(MR<=60)&&(RL>=120)&&(RR<=120))
//		{
//			inv_kine(front_left,25,12,leg-5);
//			inv_kine(front_right,30,12,leg-6);
//
//			inv_kine(middle_left,ML,7,leg);
//			inv_kine(middle_right,MR,7,leg);
//			inv_kine(rear_left,RL,10,leg-3);
//			inv_kine(rear_right,RR,10,leg-3);
//			delay_ms(2);
//			ML-=1;
//			MR+=1;
//			RL-=1;
//			RR+=1;
//		}
//		inv_kine(front_left,110,11,leg-5);
//		inv_kine(front_right,30,12,leg-6);
//		delay_ms(300);
//		inv_kine(front_left,25,12,leg-5);
//		inv_kine(front_right,110,11,leg-6);
//		delay_ms(300);
//
//		while((ML>=20)&&(MR<=100)&&(RL>=80)&&(RR<=160))
//		{
//			inv_kine(front_left,25,12,leg-5);
//			inv_kine(front_right,30,12,leg-6);
//
//			inv_kine(middle_left,ML,7,leg);
//			inv_kine(middle_right,MR,7,leg);
//			inv_kine(rear_left,RL,10,leg-3);
//			inv_kine(rear_right,RR,10,leg-3);
//	    	delay_ms(2);
//	    	ML-=1;
//	    	MR+=1;
//	    	RL-=1;
//	    	RR+=1;
//		}
//
//		inv_kine(front_left,110,11,leg-5);
//		inv_kine(front_right,30,12,leg-6);
//    	delay_ms(300);
//    	inv_kine(front_left,25,12,leg-5);
//    	inv_kine(front_right,110,11,leg-6);
//    	delay_ms(300);
//
//    	while((ML<=60)&&(MR>=60)&&(RL<=120)&&(RR>=120))
//    	{
//    		inv_kine(front_left,25,12,leg-5);
//    		inv_kine(front_right,30,12,leg-6);
//
//    		inv_kine(middle_left,ML,7,leg);
//    		inv_kine(middle_right,MR,7,leg);
//    		inv_kine(rear_left,RL,10,leg-3);
//    		inv_kine(rear_right,RR,10,leg-3);
//    		delay_ms(2);
//    		ML+=1;
//    		MR-=1;
//    		RL+=1;
//    		RR-=1;
//    	}
//
//    	inv_kine(front_left,110,11,leg-5);
//    	inv_kine(front_right,30,12,leg-6);
//    	delay_ms(300);
//    	inv_kine(front_left,25,12,leg-5);
//    	inv_kine(front_right,110,11,leg-6);
//    	delay_ms(300);
//	}
//}

void mov_celebrate(void)
{
	int ML=60,MR=60,RL=120,RR=120;
	mov_static();
	delay_ms(1000);

	//STATIC
	inv_kine(front_left,110,11,leg-5);
	inv_kine(front_right,30,12,leg-6);
	inv_kine(middle_left,60,7,leg);
	inv_kine(middle_right,60,7,leg);
	inv_kine(rear_left,120,10,leg-3);
	inv_kine(rear_right,120,10,leg-3);
	delay_ms(300);

	inv_kine(front_left,25,12,leg-5);
	inv_kine(front_right,110,11,leg-6);
	inv_kine(middle_left,60,7,leg);
	inv_kine(middle_right,60,7,leg);
	inv_kine(rear_left,120,10,leg-3);
	inv_kine(rear_right,120,10,leg-3);
	delay_ms(300);

	//LEFT
	while((ML<=100)&&(MR>=20)&&(RL<=160)&&(RR>=80))
	{
		inv_kine(front_left,25,12,leg-5);
		inv_kine(front_right,30,12,leg-6);

		inv_kine(middle_left,ML,7,leg);
		inv_kine(middle_right,MR,7,leg);
		inv_kine(rear_left,RL,10,leg-3);
	    inv_kine(rear_right,RR,10,leg-3);
	    delay_ms(2);
	    ML+=1;
	    MR-=1;
	    RL+=1;
	    RR-=1;
	}
	inv_kine(front_left,110,11,leg-5);
	inv_kine(front_right,30,12,leg-6);
	delay_ms(300);
	inv_kine(front_left,25,12,leg-5);
	inv_kine(front_right,110,11,leg-6);
	delay_ms(300);

	while((ML>=60)&&(MR<=60)&&(RL>=120)&&(RR<=120))
	{
		inv_kine(front_left,25,12,leg-5);
		inv_kine(front_right,30,12,leg-6);

		inv_kine(middle_left,ML,7,leg);
		inv_kine(middle_right,MR,7,leg);
		inv_kine(rear_left,RL,10,leg-3);
		inv_kine(rear_right,RR,10,leg-3);
		delay_ms(2);
		ML-=1;
		MR+=1;
		RL-=1;
		RR+=1;
	}

	inv_kine(front_left,110,11,leg-5);
	inv_kine(front_right,30,12,leg-6);
	delay_ms(300);
	inv_kine(front_left,25,12,leg-5);
	inv_kine(front_right,110,11,leg-6);
	delay_ms(300);

	while((ML>=20)&&(MR<=100)&&(RL>=80)&&(RR<=160))
	{
		inv_kine(front_left,25,12,leg-5);
		inv_kine(front_right,30,12,leg-6);

		inv_kine(middle_left,ML,7,leg);
		inv_kine(middle_right,MR,7,leg);
		inv_kine(rear_left,RL,10,leg-3);
		inv_kine(rear_right,RR,10,leg-3);
	    delay_ms(2);
	    ML-=1;
	    MR+=1;
	    RL-=1;
	    RR+=1;
	}

	inv_kine(front_left,110,11,leg-5);
	inv_kine(front_right,30,12,leg-6);
    delay_ms(300);
    inv_kine(front_left,25,12,leg-5);
    inv_kine(front_right,110,11,leg-6);
    delay_ms(300);

    while((ML<=60)&&(MR>=60)&&(RL<=120)&&(RR>=120))
    {
    	inv_kine(front_left,25,12,leg-5);
    	inv_kine(front_right,30,12,leg-6);

    	inv_kine(middle_left,ML,7,leg);
    	inv_kine(middle_right,MR,7,leg);
    	inv_kine(rear_left,RL,10,leg-3);
    	inv_kine(rear_right,RR,10,leg-3);
    	delay_ms(2);
    	ML+=1;
    	MR-=1;
    	RL+=1;
    	RR-=1;
    }

    inv_kine(front_left,110,11,leg-5);
    inv_kine(front_right,30,12,leg-6);
    delay_ms(300);
    inv_kine(front_left,25,12,leg-5);
    inv_kine(front_right,110,11,leg-6);
    delay_ms(300);
}

void mov_toggle_clearance(void)
{
    generate_movement_data(0,0,13);
    delay_ms(50);
    inv_kine(front_left,90,7,leg);
    inv_kine(front_right,90,7,leg);
    inv_kine(middle_left,90,7,leg);
    inv_kine(middle_right,90,7,leg);
    inv_kine(rear_left,90,7,leg);
    inv_kine(rear_right,90,7,leg);
    delay_ms(300);

}

/*
 * Func : void mov_deflect_right(int SPEED, int STEP)
 * Brief: Deflect to the right hexapod gait, moving 5 cm every step
 * Param: int SPEED (speed parameter), STEP (step parameter)
 * Written By: Ryan (Wednesday, August 26th, 2015
 * Ver  : 1
 * Programmer's Note:
 */
void mov_deflect_right(int SPEED, int STEP)
{
    generate_movement_data(0,5,10);
    delay_ms(50);

	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90-beta[3],10,leg-3); //
	inv_kine(middle_left,90,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3); //
	delay_ms(SPEED);

	//2
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3); //
	inv_kine(middle_left,90,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90-beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],10,leg-3); //
	delay_ms(SPEED);

	//3
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90+beta[3],7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90-beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg); //
	delay_ms(SPEED);

	//4
	inv_kine(front_left,90+beta[0],10,leg-3);
	inv_kine(front_right,90,7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,10,leg-3);
	inv_kine(rear_left,90-beta[2],7,leg-2);
	inv_kine(rear_right,90,7,leg); //
	delay_ms(SPEED);

	//5
	inv_kine(front_left,90-beta[0]-5,10,leg-3);
	inv_kine(front_right,90-beta[3],7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,10,leg-3);
	inv_kine(rear_left,90+beta[2],7,leg-2);
	inv_kine(rear_right,90+beta[5],7,leg); //
	delay_ms(SPEED);

	//6
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],7,leg); //
	delay_ms(SPEED);


}

/*
 * Func : void mov_deflect_left(int SPEED, int STEP)
 * Brief: Deflect to the left hexapod gait, moving 5 cm every step
 * Param: int SPEED (speed parameter), STEP (step parameter)
 * Written By: Ryan (Wednesday, August 26th, 2015
 * Ver  : 1
 * Programmer's Note:
 */
void mov_deflect_left(int SPEED, int STEP)
{
    generate_movement_data(0,5,10);
    delay_ms(50);

	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3); //
	inv_kine(middle_left,90,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90-beta[5],10,leg-3); //
	delay_ms(SPEED);

	//2
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],10,leg-3); //
	inv_kine(middle_left,90,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3); //
	delay_ms(SPEED);

	//3
	inv_kine(front_left,90-beta[0],7,leg);
	inv_kine(front_right,90-beta[3],7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90+beta[2],7,leg);
	inv_kine(rear_right,90+beta[5],7,leg); //
	delay_ms(SPEED);

	//4
	inv_kine(front_left,90-beta[0],10,leg-3);
	inv_kine(front_right,90,7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,10,leg-3);
	inv_kine(rear_left,90+beta[2],7,leg-2);
	inv_kine(rear_right,90,7,leg); //
	delay_ms(SPEED);

	//5
	inv_kine(front_left,90+beta[0]-5,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,10,leg-3);
	inv_kine(rear_left,90-beta[2],7,leg-2);
	inv_kine(rear_right,90-beta[5],7,leg); //
	delay_ms(SPEED);

	//6
	inv_kine(front_left,90+beta[0],7,leg);
	inv_kine(front_right,90+beta[3],7,leg); //
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90-beta[2],7,leg);
	inv_kine(rear_right,90-beta[5],7,leg); //
	delay_ms(SPEED);


}



/*
void yume_demo(void)
{
	mov_fwd_5cm(20, SLOW);
	mov_fwd_5cm(20, MED);
	mov_fwd_5cm(35, FAST_LEVEL_2);
	mov_curve_right(MED, FAR);
	mov_fwd_5cm(30, MED);
	mov_curve_right(MED, FAR);
	mov_fwd_5cm(20, MED);
	mov_search_fire();
	mov_search_fire();
	//mov_bwd(40, MED);
	//mov_rot_right_90_dgr(90,MED,FAR);
	//mov_fwd_5cm(30, MED);
	//mov_rot_left_90_dgr(90,MED,FAR);
	//mov_askew_left(20,MED,FAR);
	//mov_askew_right(20,MED,FAR);
	mov_celebrate(3);

	while(1)
	{
		mov_static();
		delay_ms(3000);
	}

}
*/


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

void Sendto_MainController(USART_TypeDef* USARTx, volatile char *s){

	while(*s){
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) );
		USART_SendData(USARTx,(int)*s);
		*s++;
	}
}

/*
 *  func : void USART3_IRQHandler(void)
 *  brief: USART3 RX Interupt handler from Main Controller to Servo Controller
 *  param: command_code[0] -> gait_mode code selection
 *  	   command_code[1] -> speed paameter for each gait_mode function
 */
void USART3_IRQHandler(void)
{
	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
	{
		char t = USART3->DR; // the character from the USART3 data register is saved in t
		char static last_data;


		switch(usart_count)
		{
			case  1 : command_code[0] =USART3->DR; usart_count+=1; break;
			case  2 : command_code[1] =USART3->DR; usart_count+=1; break;
			case  3 : command_code[2] =USART3->DR; usart_count=811; break;
			default : break;
		}

		if(t==COMMAND_FLAG_B && last_data==COMMAND_FLAG_A) usart_count=1;
		last_data=t;
	}
}


void USART1_Init(uint32_t baudrate){ //TO SERVO CONTROLLER (DEFAULT B6 TX & B7 RX
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

	// finally this enables the complete USART3 peripheral
	USART_Cmd(USART1, ENABLE);
}


/*
void USART1_Init(uint32_t baudrate){ //TO SERVO CONTROLLER (DEFAULT A9 TX & A10 RX
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;  // Pins 9 (TX) and 10 (RX) are used
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 			// the pins are configured as alternate function so the USART peripheral has access to them
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// this defines the output type as push pull mode (as opposed to open drain)
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// this activates the pullup resistors on the IO pins
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); //
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

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

	// finally this enables the complete USART3 peripheral
	USART_Cmd(USART1, ENABLE);
}
*/
void USART1_IRQHandler(void){

	// usart1 terima data dan di interrupt
		if( USART_GetITStatus(USART1, USART_IT_RXNE) ){

			static uint8_t cnt = 0; // counter untuk data string
			char t = USART1->DR; // data register dari usart1 didefinisikan sebagai t

			/* data register akan di tampung sampai ketemu karakter s
			 * dan data yang di tampung tidak melebihi maksimal indek data string yang di definisikan
			 */
			if( (t != 's') && (cnt < MAX_STRLEN) ){
				received_string[cnt] = t; // data register di tampung dalam variabel string
				cnt++; // counter up
			}
			else{ // setelah data selesai di tampung maka data dapat di tampilkan
				cnt = 0;//reset counter
				Sendto_MainController(USART1, received_string);
			}
		}
}

void print_toPC(USART_TypeDef* USARTx, const char *pFormat, ... )
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

void cek_ping_toPC(void)
{
	print_toPC(USART3,"A=%d\t",terima[8]);
	print_toPC(USART3,"B=%d\t",terima[4]);
	print_toPC(USART3,"C=%d\t",terima[6]);
	print_toPC(USART3,"D=%d\t",terima[2]);
	print_toPC(USART3,"E=%d\t",terima[7]);
	print_toPC(USART3,"F=%d\t",terima[3]);
	print_toPC(USART3,"G=%d\t",terima[5]);
	print_toPC(USART3,"H=%d\t",terima[1]);
	print_toPC(USART3,"O=%d\t",terima[0]);
	print_toPC(USART3,"Y=%d\t",terima[9]);
	print_toPC(USART3,"I=%d\r",terima[10]);
}

void cek_ping_1()
{
	lcd_gotoxy(0,0);
	sprintf(data1_lcd,"A=%d  B=%d",terima[1],terima[3]);
	lcd_putstr(data1_lcd);
	lcd_gotoxy(0,1);
	sprintf(data2_lcd,"C=%d  D=%d",terima[5],terima[7]);
	lcd_putstr(data2_lcd);
	delay_ms(50);
	lcd_display_clear();
}

void USART_Command_Troubleshoot(void)
{
	sprintf(USART_send,"command_code[0]: %c \r",command_code[0]);
	Sendto_MainController(USART3,USART_send);
	delay_ms(50);
	sprintf(USART_send,"command_code[1]: %c \r",command_code[1]);
	Sendto_MainController(USART3,USART_send);
	delay_ms(50);
	sprintf(USART_send,"command_code[2]: %c \r",command_code[2]);
	Sendto_MainController(USART3,USART_send);
	delay_ms(50);
}

