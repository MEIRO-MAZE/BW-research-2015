/*==============================================================================================
								  	  PROJECT YUME Ver.1.0
								  TCS3200 COLOUR SENSOR LIBRARY
==============================================================================================*/

#ifndef YUME_TCS3200_H
#define YUME_TCS3200_H

#include "main.h"
#define  TIM2_ON             TIM_Cmd(TIM2, ENABLE)    //register kontrol untuk mengendalikan timer 1
#define  TIM2_OFF            TIM_Cmd(TIM2, DISABLE)    //register kontrol untuk mengendalikan timer 1
#define  RESET_TIM2          TIM2->CNT=0x00000000   //register pencacah


#define  TIM13_ON             TIM_Cmd(TIM13, ENABLE)    //register kontrol untuk mengendalikan timer 1
#define  TIM13_OFF            TIM_Cmd(TIM13, DISABLE)    //register kontrol untuk mengendalikan timer 1
#define  RESET_TIM13          TIM13->CNT=0x00000000   //register pencacah


#define RCC_AHB1Periph_TCS3200_FRONT_OUT RCC_AHB1Periph_GPIOB
#define RCC_AHB1Periph_TCS3200_FRONT_IN RCC_AHB1Periph_GPIOC
#define RCC_AHB1Periph_TCS3200_MIDDLE_OUT RCC_AHB1Periph_GPIOE
#define RCC_AHB1Periph_TCS3200_MIDDLE_IN RCC_AHB1Periph_GPIOE

#define TCS3200_FRONT_OUT_PORT GPIOB
#define TCS3200_FRONT_IN_PORT GPIOC
#define TCS3200_MIDDLE_OUT_PORT GPIOE
#define TCS3200_MIDDLE_IN_PORT GPIOE

/*
 * TCS3200 FRONT SENSOR
 */
#define TCS3200_F_S3 GPIOB, GPIO_Pin_5
#define TCS3200_F_S2 GPIOB, GPIO_Pin_4
#define TCS3200_F_OE GPIOB, GPIO_Pin_0
#define TCS3200_F_S1 GPIOB, GPIO_Pin_2
#define TCS3200_F_S0 GPIOB, GPIO_Pin_1
#define TCS3200_F_OUT GPIOC, GPIO_Pin_13

#define F_S3_PIN GPIO_Pin_5
#define F_S2_PIN GPIO_Pin_4
#define F_OE_PIN GPIO_Pin_0
#define F_S1_PIN GPIO_Pin_2
#define F_S0_PIN GPIO_Pin_1
#define F_OUT_PIN GPIO_Pin_13

/*
 * TCZ3200 MIDDLE SENSOR
 */

#define TCS3200_M_S3 GPIOE, GPIO_Pin_7
#define TCS3200_M_S2 GPIOE, GPIO_Pin_6
#define TCS3200_M_OE GPIOE, GPIO_Pin_3
#define TCS3200_M_S1 GPIOE, GPIO_Pin_5
#define TCS3200_M_S0 GPIOE, GPIO_Pin_4
#define TCS3200_M_OUT GPIOE, GPIO_Pin_8

#define M_S3_PIN GPIO_Pin_7
#define M_S2_PIN GPIO_Pin_6
#define M_OE_PIN GPIO_Pin_3
#define M_S1_PIN GPIO_Pin_5
#define M_S0_PIN GPIO_Pin_4
#define M_OUT_PIN GPIO_Pin_8


/*
 * TCS3200 VARIABLES
 */




extern unsigned int COLOUR_NONE;
extern unsigned int COLOUR_BLACK;
extern unsigned int COLOUR_WHITE;
extern unsigned int COLOUR_RED;
extern unsigned int COLOUR_BLUE;

extern unsigned int F_DETECTED_COLOUR;
extern unsigned int M_DETECTED_COLOUR;


extern unsigned int TRACER_STAT;
extern unsigned int COLOUR_STAT;

extern unsigned int ZONE;
extern unsigned int ZONE_RED;
extern unsigned int ZONE_BLUE;

extern float F_sum_red;
extern float F_sum_blue;
extern float F_sum_white;
extern float F_THRESHOLD;

extern float M_sum_red;
extern float M_sum_blue;
extern float M_sum_white;
extern float M_THRESHOLD;

extern uint16_t YUME_START;

#define TRACER_DETECTED 1
#define TRACER_NOT_DETECTED 0

#define COLOUR_DETECTED 1
#define COLOUR_NOT_DETECTED 0

void TIM2_Init(void);
void TCS3200_Pin_Config(void);
void TCS3200_FRONT_Mode(unsigned char mode);
float FRONT_Colour_Read(unsigned char color,  unsigned char LEDstate);
void FRONT_Detect_Colour(void);
void FRONT_Sum_Colour(void);


void TIM13_Init(void);
void TCS3200_MID_Mode(unsigned char mode);
float MID_Colour_Read(unsigned char color,  unsigned char LEDstate);
void MID_Detect_Colour(void);
void MID_Sum_Colour(void);
void TCS3200_Init(void);
void yume_display_colour(void);
void TCS3200_Init(void);


void TCS3200_Pin_Config(void);
void TCS3200_Init(void);
void yume_display_colour(void);

#endif
