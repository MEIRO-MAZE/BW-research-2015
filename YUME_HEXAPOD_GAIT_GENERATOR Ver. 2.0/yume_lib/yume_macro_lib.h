/*==============================================================================================
								  PROJECT YUME Ver.2.0
								    MACRO LIBRARY
==============================================================================================*/

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include "math.h"
#include "stm32f4xx_usart.h"

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

#define la  5            // besar La
#define lb  6.6            // besar Lb
#define lj  2             // besar L_junction
#define rad2deg  57.29
#define angle_correction    45      //koreksi sudut 0 di teta3, karena sudut 0 sbenarnya adalah sejajar teta2 shg jangkauan servo kurang

#define front_left      1
#define front_right     2
#define middle_left     3
#define middle_right    4
#define rear_left       5
#define rear_right      6

//FRONT LEFT
#define ch_0 	GPIOB,GPIO_Pin_0 //COXA
#define ch_1 	GPIOB,GPIO_Pin_1 //FEMUR
#define ch_2 	GPIOB,GPIO_Pin_2 //TIBIA

//MIDDLE LEFT
#define ch_3 	GPIOC,GPIO_Pin_0 //COXA
#define ch_4 	GPIOC,GPIO_Pin_1 //FEMUR
#define ch_5 	GPIOC,GPIO_Pin_2 //TIBIA

//REAR LEFT
#define ch_6 	GPIOD,GPIO_Pin_0 //COXA
#define ch_7 	GPIOD,GPIO_Pin_1 //FEMUR
#define ch_8 	GPIOD,GPIO_Pin_2 //TIBIA

//FRONT RIGHT
#define ch_9 	GPIOB,GPIO_Pin_3 //COXA
#define ch_10 	GPIOB,GPIO_Pin_4 //FEMUR
#define ch_11 	GPIOB,GPIO_Pin_5 //TIBIA

//MIDDLE RIGHT
#define ch_12 	GPIOC,GPIO_Pin_3 //COXA
#define ch_13 	GPIOC,GPIO_Pin_4 //FEMUR
#define ch_14 	GPIOC,GPIO_Pin_5 //TIBIA

//REAR RIGHT
#define ch_15 	GPIOD,GPIO_Pin_3 //COXA
#define ch_16 	GPIOD,GPIO_Pin_4 //FEMUR
#define ch_17 	GPIOD,GPIO_Pin_5 //TIBIA
    // right ccw ccw cw
    // left cw cw ccw

//HEXAPOD GAIT MODE PARAMETER
const int STATIC_LOW_CLEARANCE  = 0;
const int STATIC_MED_CLEARANCE  = 1;
const int STATIC_HIGH_CLEARANCE = 2;
const int SEARCH_FIRE           = 3;
const int FORWARD 				= 4;
const int BACKWARD 				= 5;
const int ROTATE_RIGHT 			= 6;
const int ROTATE_LEFT 			= 7;
const int CURVE_RIGHT 			= 8;
const int CURVE_LEFT 			= 9;
const int ASKEW_RIGHT 			= 10;
const int ASKEW_LEFT 			= 11;
const int FWD_RIPPLE			= 12;
const int FWD_WAVE				= 13;



//SPEED PARAMETER
enum SPEED {ULTRA_SLOW=300, SLOW=250, MED_SLOW=200, MED=150, FAST=130, FAST_LEVEL_2=100, FAST_LEVEL_3=80, ULTRA_FAST=50};
enum STEP {CLOSE=2, HALF=3, FAR=5};

//==================================== USART MACRO =========================================//
#define Pin_0  GPIO_Pin_0
#define Pin_1  GPIO_Pin_1
#define Pin_2  GPIO_Pin_2
#define Pin_3  GPIO_Pin_3
#define Pin_4  GPIO_Pin_4
#define Pin_5  GPIO_Pin_5
#define Pin_6  GPIO_Pin_6
#define Pin_7  GPIO_Pin_7
#define Pin_8  GPIO_Pin_8
#define Pin_9  GPIO_Pin_9
#define Pin_10  GPIO_Pin_10
#define Pin_11  GPIO_Pin_11
#define Pin_12  GPIO_Pin_12
#define Pin_13  GPIO_Pin_13
#define Pin_14  GPIO_Pin_14
#define Pin_15  GPIO_Pin_15

#define ON 0x01
#define OFF 0x02
#define TO 0x04
