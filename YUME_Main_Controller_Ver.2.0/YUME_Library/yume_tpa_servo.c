/*==============================================================================================
								  PROJECT YUME Ver.1.0
							THERMOPHILE ARRAY AND SERVO LIBRARY
==============================================================================================*/



//USER DEFINED LIBRARIES
#include "yume_tpa_servo.h"
#include "yume_hexapod_cmd.h"

//MIN    -> 1850
//CENTER -> 3000
//MAX    -> 4250

const int SERVO_MIN    = 1850;
const int SERVO_CENTER = 3000;
const int SERVO_MAX    = 4250;




//void TIM12_PWM_Configuration(void)
//{
//	  // Clock enable
//	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);       // Clocking GPIOC (AHB1/APB1 = 42MHz)
//	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);        // Clocking TIM12  (APB1 = 42x2PLL=84MHz)
//
//	  // GPIO__TIM_init
//	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15;
//	  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;                           // PWM is an alternative function
//	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                      // GPIO_HIGH_Speed
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                          // Push-pull
//	  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//	  GPIO_Init(GPIOB, &GPIO_InitStructure);                                  // Initializing GPIOC structure
//	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_TIM12);     // Routing TIM3 output to PC6
//
//	  // Timer init
//	 TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 // => 0 = Not dividing
//	 TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;           // Upcounting configuration
//	 TIM_TimeBaseStructure.TIM_Period        = 39999;            			 // 39999=TCNT ==>pwm_freq=2000000/39999+1=50Hz
//	 TIM_TimeBaseStructure.TIM_Prescaler     = 41;            				 // Tim_tick => 84000000Hz/41+1=2000000Hz
//	 TIM_TimeBaseInit(TIM12, &TIM_TimeBaseStructure);                         // Initializing Time Base structure
//
//	 // Channel 1, Ch.1 (PC6)
//	 TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;                  // PWM mode 1 = Set on compare match, PWM mode 2 = Clear on compare match pasangan OCPolarity_LOW
//	 TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;           // Enabling the Output Compare state
//	 TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;                // Regular polarity (low will inverse it)
//	 TIM_OCInitStructure.TIM_Pulse       = PWM_servo;                      // Output Compare 1 reg value
//	 TIM_OC2Init(TIM12, &TIM_OCInitStructure);                                // Initializing Output Compare 1 structure
//	 TIM_OC2PreloadConfig(TIM12, TIM_OCPreload_Enable);                       // Enable Ch.1 Output Compare preload
//
//	 // Timer enable
//	  TIM_Cmd(TIM12, ENABLE);                                      // Ready, Set, Go!
//
//}
//
//void Set_Servo(int sudut)
//{
//    //     servo=2000;      (min)
//    //     delay_ms(2000);
//    //     servo=3200;      (center)
//    //     delay_ms(2000);
//    //     servo=4400;      (max)
//	//(4400-2000)/180 deg = 13.334
//    //float servo_pulse;
//    //sudut+=90;
//    //servo_pulse = sudut*13.333;
//    //servo_pulse = (float) sudut;
//    //servo_pulse *= 13.333;        //(4250-1850)/180
//    //servo_pulse += SERVO_MIN;       //min_ser = 2000
//    //PWM_servo = (int) servo_pulse;
//
//	//MIN    -> 1850
//	//CENTER -> 3000
//	//MAX    -> 4250
//
//    float servo_pulse;
//    servo_pulse = sudut*13.333*1000;
//
//    servo_pulse += SERVO_MIN;       //min_ser = 2000
//    PWM_servo = (int) servo_pulse;
//}

void YUME_Thermopile_Init(void)
{
	//TIM12_PWM_Configuration();
	Setup_I2C();
}


void Setup_I2C()
{
    //TPA 81
	/*  GPIO Setup  */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //I2C_DeInit(I2C1);
    /*  I2C Setup  */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_ClockSpeed = 100000; //100kHz
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Cmd(I2C1, ENABLE);
    I2C_Init(I2C1, &I2C_InitStructure);

    /* Connect PXx to I2C_SCL*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_I2C1);

    /* Connect PXx to I2C_SDA*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_I2C1);
//
//    //==========================================================
//    //COMPASS
//    /*  GPIO Setup  */
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);
//
//    //I2C_DeInit(I2C1);
//    /*  I2C Setup  */
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
//    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
//    I2C_InitStructure.I2C_ClockSpeed = 100000; //100kHz
//    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
//    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
//    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
//    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
//
//    I2C_Cmd(I2C2, ENABLE);
//    I2C_Init(I2C2, &I2C_InitStructure);
//
//    /* Connect PXx to I2C_SCL*/
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
//
//    /* Connect PXx to I2C_SDA*/
//    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);
}

void Write_I2C(int Address, int Register, int Data)
{
	if((MPU_ACTIVE_MODE&&1)==1)
	{
		while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		{
			;
		}

		I2C_GenerateSTART(I2C1, ENABLE);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			;
		}
		I2C_Send7bitAddress(I2C1,Address<<1,I2C_Direction_Transmitter);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			;
		}

		I2C_SendData(I2C1,Register);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			;
		}

		I2C_SendData(I2C1,Data);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			;
		}

		I2C_GenerateSTOP(I2C1, ENABLE);
	}
}



void Read_I2C(int Address, int Register)
{
	if((MPU_ACTIVE_MODE&&1)==1)
	{
		while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY))
		{
			;
		}

		I2C_GenerateSTART(I2C1, ENABLE);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			;
		}

		I2C_Send7bitAddress(I2C1, Address<<1,I2C_Direction_Transmitter);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			;
		}

		I2C_Cmd(I2C1, ENABLE);

		I2C_SendData(I2C1,Register);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			;
		}

		I2C_GenerateSTART(I2C1, ENABLE);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
		{
			;
		}

		I2C_Send7bitAddress(I2C1,Address<<1,I2C_Direction_Receiver);
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			;
		}

		I2C_AcknowledgeConfig(I2C1, DISABLE);
		I2C_GenerateSTOP(I2C1, ENABLE);

		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			;
		}

		DATA=I2C_ReceiveData(I2C1);
		I2C_AcknowledgeConfig(I2C1, ENABLE);
	}
}




//==============
//TPA
unsigned char YUME_TPA_Check(char val_or_flag)
{
	//unsigned char tpa_fight=0;
	i=0, temp=0,suhu_ruang=0;
	regs=0x00;

	//int shifted_address = 0xD0>>1;


	for (i=0;i<=9;i++)
	{
		//Read_I2C(shifted_address,regs);
		Read_I2C(0x68, regs); //Aslinya 0xD0, tapi hasilnya digeser satu bit ke kanan
		regs=regs+1; // register=0x02,0x03,...,0x09
		pixel[i]=DATA;
	}

	suhu_ruang = pixel[1];
	suhu_pixel[0] = pixel[3];
	suhu_pixel[1] = pixel[4];
	suhu_pixel[2] = pixel[5];
	suhu_pixel[3] = pixel[6];
	suhu_pixel[4] = pixel[7];
	suhu_pixel[5] = pixel[8];
	suhu_pixel[6] = pixel[2];
	suhu_pixel[7] = pixel[9];

	temp = fmax(temp,suhu_pixel[0]);
	temp = fmax(temp,suhu_pixel[1]);
	temp = fmax(temp,suhu_pixel[2]);
	temp = fmax(temp,suhu_pixel[3]);
	temp = fmax(temp,suhu_pixel[4]);
	temp = fmax(temp,suhu_pixel[5]);
	temp = fmax(temp,suhu_pixel[6]);
	temp = fmax(temp,suhu_pixel[7]);

	if(!val_or_flag)
	{return (unsigned char)temp;}

	temp -= suhu_ruang;


	if(temp>14 && val_or_flag) {/*tpa_fight=1;*/ return 1;}   //dulu pakai tresh hold 15 kurang respon jika di ruang 3

	else {return 0;}


//    if(((pixel[2]+pixel[3]+pixel[4]+pixel[5]+pixel[6]+pixel[7]+pixel[8]+pixel[9])/8) >= 55) //pixel[0]&&pixel[1] tidak dipakai karena nilai sensornya tetap/tdk berubah
//    //if(pixel[2]&&pixel[3]&&pixel[4]&&pixel[5]&&pixel[6]&&pixel[7]&&pixel[8]&&pixel[9] >= 54) //pake and sering salah baca
//    {return 1;}
//    else
//    {return 0;}
}


////==============
////TPA SERVO
int tpa_servo()
{
	panas=0;
    int sudut_sv=-90;
    Set_Servo(-90);
    delay_ms(1000);

    while(panas==0 && sudut_sv<=90)
    {
        //Set_Servo((signed char)sudut_sv);
        Set_Servo(sudut_sv);
        delay_ms(20);
        panas = YUME_TPA_Check(1);
        sudut_sv += 2;
    }
//    buzz(4);
    if(panas==0) //penting kalo if ini dihilangkan servo gak mau berhenti/ langsung reset ke -90
    {
//    	buzz(1);
        //PWM_servo = center_ser;    //mempercepat reset ke titik awal
        sudut_sv = 120; //angka gw bole suka"
    }
    else if(panas==1) //ada api
    {
//    	buzz(2);
    	sudut_sv = (sudut_sv-2);
    }
    delay_ms(1000);
    return sudut_sv;
}

//unsigned int YUME_Search_Fire(void)
//{
//	panas=0;
//	static int counter=1;
//
//	//mov_search_fire(SRC_FR_A);
//
//	while (panas==0)
//	{
//		panas = YUME_TPA_Check(1);
//		switch(counter)
//		{
//			case 1:  {mov_search_fire(SRC_FR_A); counter++;delay_ms(100);}break;
//			case 2:  {mov_search_fire(SRC_FR_B); counter++;delay_ms(100);}break;
//			case 3:  {mov_search_fire(SRC_FR_C); counter++;delay_ms(100);}break;
//			case 4:  {mov_search_fire(SRC_FR_D); counter++;delay_ms(100);}break;
//			case 5:  {mov_search_fire(SRC_FR_E); counter++;delay_ms(100);}break;
//			case 6:  {mov_search_fire(SRC_FR_F); counter++;delay_ms(100);}break;
//			case 7:  {mov_search_fire(SRC_FR_G); counter++;delay_ms(100);}break;
//			case 8:  {mov_search_fire(SRC_FR_H); counter++;delay_ms(100);}break;
//			case 9:  {mov_search_fire(SRC_FR_I); counter++;delay_ms(100);}break;
//			case 10:  {mov_search_fire(SRC_FR_J); counter++;delay_ms(100);}break;
//			case 11:  {mov_search_fire(SRC_FR_K); counter++;delay_ms(100);}break;
//			case 12:  {mov_search_fire(SRC_FR_L); counter++;delay_ms(100);}break;
//			case 13:  {mov_search_fire(SRC_FR_M); counter++;delay_ms(100);}break;
//			case 14:  {mov_search_fire(SRC_FR_L); counter++;delay_ms(100);}break;
//			case 15:  {mov_search_fire(SRC_FR_K); counter++;delay_ms(100);}break;
//			case 16:  {mov_search_fire(SRC_FR_J); counter++;delay_ms(100);}break;
//			case 17:  {mov_search_fire(SRC_FR_I); counter++;delay_ms(100);}break;
//			case 18:  {mov_search_fire(SRC_FR_H); counter++;delay_ms(100);}break;
//			case 19:  {mov_search_fire(SRC_FR_G); counter++;delay_ms(100);}break;
//			case 20:  {mov_search_fire(SRC_FR_F); counter++;delay_ms(100);}break;
//			case 21:  {mov_search_fire(SRC_FR_E); counter++;delay_ms(100);}break;
//			case 22:  {mov_search_fire(SRC_FR_D); counter++;delay_ms(100);}break;
//			case 23:  {mov_search_fire(SRC_FR_C); counter++;delay_ms(100);}break;
//			case 24:  {mov_search_fire(SRC_FR_B); counter++;delay_ms(100);}break;
//			default: counter=1;break;
//		}
//	}
//
//	if (panas==1)
//	{
//		//panas=0;		Mov_fwd_5cm(5,MED);
//
//		//mov_search_fire(SEARCH_FIRE_RESET);
//	}
//	return 1;
//}

void Get_TPA81(void)
{
	FIRE= YUME_TPA_Check(1);
}
