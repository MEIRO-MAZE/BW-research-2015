/*==============================================================================================
								 	 PROJECT YUME Ver.1.0
								 YUME INFRARED PROXIMITY LIBRARY
==============================================================================================*/

#include <yume_infrared_proximity.h>

void IR_Proximity_Init(void)
{
	//Enable Port C Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_IR, ENABLE);
	//-----------------Pin ADC3 Channel1 pin as analog input--------
	GPIO_InitStructure.GPIO_Pin   = IR_PIN_FRONT_LEFT | IR_PIN_FRONT_RIGHT | IR_PIN_MID_LEFT | IR_PIN_MID_RIGHT ;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //kecepatan clock(2, 25, 50 or 100 MHz)
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_Init(IR_PORT, &GPIO_InitStructure);
}

void Get_IR(void)
{
	INFRARED[0]= IR_FRONT_LEFT;
	INFRARED[1]= IR_FRONT_RIGHT;
	INFRARED[2]= IR_MID_LEFT;
	INFRARED[3]= IR_MID_RIGHT;
//	if    (IR_FRONT_LEFT==0) {INFRARED[0]=1;}
//	else   INFRARED[0]=0;
//
//	if    (IR_FRONT_RIGHT==0) {INFRARED[1]=1;}
//	else   INFRARED[1]=0;
//
//	if    (IR_REAR_LEFT==0) {INFRARED[2]=1;}
//	else   INFRARED[2]=0;
//
//	if    (IR_REAR_RIGHT==0) {INFRARED[3]=1;}
//	else   INFRARED[3]=0;
}

void Get_IR_Right(void)
{
	INFRARED[3]= IR_MID_RIGHT;
}
void Get_IR_Left(void)
{
	INFRARED[2]= IR_MID_LEFT;
}
void Display_IR_Stat(void)
{
	Get_IR();
	lcd_display_clear();
	lcd_gotoxy(0,0);sprintf(lcd," IR_FL: %d",INFRARED[0]);lcd_putstr(lcd);
	lcd_gotoxy(0,1);sprintf(lcd," IR_FR: %d",INFRARED[1]);lcd_putstr(lcd);
	lcd_gotoxy(0,2);sprintf(lcd," IR_RL: %d",INFRARED[2]);lcd_putstr(lcd);
	lcd_gotoxy(0,3);sprintf(lcd," IR_RR: %d",INFRARED[3]);lcd_putstr(lcd);
}

/*
void IR_Interrupt_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Prescaler = 335;  //f per count= 84000000/(335+1)=250000Hz = 0.004ms
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = 5000; //0.004*5000=50ms jadi sampling adc setiap 20ms
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM3, ENABLE); //default di off dulu

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    	{
    		IR_Control();
    		//YUME_RGB_Tracer_Encoder();
    		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    	}
}
*/
