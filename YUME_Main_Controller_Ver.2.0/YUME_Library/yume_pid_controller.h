/*==============================================================================================
								  	 PROJECT YUME Ver.1.0
								  	 YUME PID CONTROLLER
==============================================================================================*/

#ifndef YUME_PID_CONTROLLER_H
#define YUME_PID_CONTROLLER_H

#include "main.h"

/*
 *  YUME PID CONTROLLER TYPEDEF
 */
GPIO_InitTypeDef      	   GPIO_InitStructure;
TIM_OCInitTypeDef          TIM_OCInitStructure;
TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
NVIC_InitTypeDef           NVIC_InitStructure;

/*
 * YUME PID CONTROLLER GLOBAL VARIABLES
 */
struct PID_t
{
	float P[3];
	float I[3];
	float D[3];
	float Kp;
	float Ki;
	float Kd;
	float Ts;
	float error[3];
	float pid_value[3];
	float set_point;
	float set_point_upper;
	float set_point_lower;
}PID_F_R,PID_F_L,PID_DFL_R,PID_DFL_L;


void YUME_PID_Interrupt_Init(void);
void YUME_PID_Init(void);
void PID_Calc(void);
void PID_Calc_LeftRule(void);

//void TIM14_IRQHandler(void);
extern unsigned int Ping[9];
extern unsigned int usart_count;

extern unsigned int FIRE;

#endif
