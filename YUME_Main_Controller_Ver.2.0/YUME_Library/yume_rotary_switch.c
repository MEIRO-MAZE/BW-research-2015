/*==============================================================================================
								 	 PROJECT YUME Ver.1.0
								  	 ROTARY SWITCH LIBRARY
==============================================================================================*/

#include "yume_rotary_switch.h"

//uint16_t ADCResult;

void RotSwitch_Init(void)
{
	//Enable Port C Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	//-----------------Pin ADC3 Channel1 pin as analog input--------
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //gak perlu
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/*Enable ADC3 clock*********************************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);

	/* ADC Common Init **********************************************************/
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC3 Init ****************************************************************/
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;

	ADC_Init(ADC3, &ADC_InitStructure);

	/* Enable ADC3 */
	ADC_Cmd(ADC3, ENABLE);
	ADC_ContinuousModeCmd(ADC3, ENABLE);
	ADC_RegularChannelConfig(ADC3,ADC_Channel_13,1,ADC_SampleTime_3Cycles); //channel 13 = IN1= datasheet PA.1(ADC323,IN1)
}

void RotSwitch_Sampling(void)
{
	uint16_t ADCResult;
	delay_ms(50);
	ADC_SoftwareStartConv(ADC3); //harus masuk while(1)
	ADC_GetConversionValue(ADC3);
	ADCResult= ADC_GetConversionValue(ADC3);
	//rot_switch_mode=ADCResult/50;
	//printf("Rotary Switch ADC: %d | \n",ADCResult);

	Sendto_PC(USART1,"YUME Rotary Switch Initializing... \r");
//
	if (ADCResult<=15)
		{
			rot_switch_mode=1;//{printf("YUME Mode: 1| ");}
		}
	else if (((ADCResult>=50)&&(ADCResult<=60)))
		{
			rot_switch_mode=2;//{printf("YUME Mode: 1| ");}
		}
	else if (((ADCResult>=100)&&(ADCResult<=120)))
		{
			rot_switch_mode=3;//{printf("YUME Mode: 1| ");}
		}
	else if (((ADCResult>=150)&&(ADCResult<=170)))
		{
			rot_switch_mode=4;//{printf("YUME Mode: 1| ");}
		}
	else if (((ADCResult>=210)&&(ADCResult<=230)))
		{
			rot_switch_mode=5;//{printf("YUME Mode: 1| ");}
		}
	else if (ADCResult>=250)
		{
			rot_switch_mode=6;//{printf("YUME Mode: 1| ");}
		}
	else rot_switch_mode=0;

	if (rot_switch_mode==0) Sendto_PC(USART1,"YUME Sampling Error \r",rot_switch_mode);
	else Sendto_PC(USART1,"YUME Mode: %d \r",rot_switch_mode);

	lcd_display_clear();
	lcd_gotoxy(0,0);sprintf(lcd,"ADC_Result: %d",(int)ADCResult);lcd_putstr(lcd);
	lcd_gotoxy(0,1);sprintf(lcd,"YUME Mode: %d",rot_switch_mode);lcd_putstr(lcd);

}
