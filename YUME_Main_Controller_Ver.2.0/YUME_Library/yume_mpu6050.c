/*==============================================================================================
								  PROJECT YUME Ver.1.0
								  YUME MPU6050 LIBRARY
==============================================================================================*/

#include "yume_mpu6050.h"


/*
 * MPU 6050 6 DOF Gyro Accelerometer
 */

char IMU_Stat[100];

void Write_I2C2(int Address, int Register, int Data)
{
	if((MPU_ACTIVE_MODE&&1)==1)
	{
		while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
		{
			;
		}

		I2C_GenerateSTART(I2C2, ENABLE);

		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		{
			;
		}
		I2C_Send7bitAddress(I2C2,Address<<1,I2C_Direction_Transmitter);

		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			;
		}

		I2C_SendData(I2C2,Register);

		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			;
		}

		I2C_SendData(I2C2,Data);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			;
		}

		I2C_GenerateSTOP(I2C2, ENABLE);
	}
}

void Read_I2C2(int Address, int Register)
{
	if((MPU_ACTIVE_MODE&&1)==1)
	{
		while (I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
		{
			;
		}

		I2C_GenerateSTART(I2C2, ENABLE);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		{
			;
		}

		I2C_Send7bitAddress(I2C2, Address<<1,I2C_Direction_Transmitter);

		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
		{
			;
		}

		I2C_Cmd(I2C2, ENABLE);

		I2C_SendData(I2C2,Register);

		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
		{
			;
		}

		I2C_GenerateSTART(I2C2, ENABLE);

		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))
		{
			;
		}

		I2C_Send7bitAddress(I2C2,Address<<1,I2C_Direction_Receiver);
		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			;
		}

		I2C_AcknowledgeConfig(I2C2, DISABLE);
		I2C_GenerateSTOP(I2C2, ENABLE);

		while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			;
		}

		DATA=I2C_ReceiveData(I2C2);
		I2C_AcknowledgeConfig(I2C2, ENABLE);
	}
}

void MPU_6050_I2C_Init (void)
{
    //TPA 81
	/*  GPIO Setup  */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //I2C_DeInit(I2C2);
    /*  I2C Setup  */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_ClockSpeed = 400000; //400kHz
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;

    I2C_Cmd(I2C2, ENABLE);
    I2C_Init(I2C2, &I2C_InitStructure);

    /* Connect PXx to I2C_SCL*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);

    /* Connect PXx to I2C_SDA*/
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);
}
void MPU_6050_Init(void)
{

		//Format I2C Address
		//YUME_MPU6050.Address = MPU6050_I2C_ADDR | 1;

		YUME_MPU6050.Address=0x68;

		//Setup I2C
		//Setup_I2C();
		MPU_6050_I2C_Init ();


		Write_I2C2(0x68, MPU6050_PWR_MGMT_1, 0x6B);

		Write_I2C2(0x68, MPU6050_PWR_MGMT_1, 0x00);

		/* Config gyroscope */
		//	temp = TM_I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_CONFIG);
		Read_I2C2(0x68, MPU6050_GYRO_CONFIG); //store data in 'DATA'

		//	temp = (temp & 0xE7) | (uint8_t)GyroscopeSensitivity << 3;
		DATA = (DATA & 0xE7) | Gyro_Sensitivity_250s << 3;

		//write shifted DATA to GYRO_CONFIG register
		//	TM_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_GYRO_CONFIG, temp);
		Write_I2C2(0x68, 0x1B, DATA);


		//DataStruct->Gyro_Mult = (float)1 / MPU6050_GYRO_SENS_250;
		Gyro_Mult= (float) 1/MPU6050_GYRO_SENS_250;



//		//ACCELEROMETER INIT
		/* Config accelerometer */
		//temp = TM_I2C_Read(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_CONFIG);
		Read_I2C2(0x68,MPU6050_ACCEL_CONFIG);

		//temp = (temp & 0xE7) | (uint8_t)AccelerometerSensitivity << 3;
		DATA= (DATA & 0xE7)|(uint8_t)Accelerometer_Range <<3;

		//TM_I2C_Write(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_CONFIG, temp);
		Write_I2C2(0X68,MPU6050_ACCEL_CONFIG,DATA);

		Acce_Mult = (float) 1/ MPU6050_ACCE_SENS_2;

}


void Read_Gyro(void)
{
	int count;
	uint8_t data[6];

	for (count=0;count<=5;count++)
	{
		//Read_I2C(shifted_address,regs);
		Read_I2C2(0x68, MPU6050_GYRO_XOUT_H); //Aslinya 0xD0, tapi hasilnya digeser satu bit ke kanan
		data[count]=DATA;
	}

	/* Format */
	YUME_MPU6050.Gyroscope_X = (int16_t)(data[0] << 8 | data[1]);
	YUME_MPU6050.Gyroscope_Y = (int16_t)(data[2] << 8 | data[3]);
	YUME_MPU6050.Gyroscope_Z = (int16_t)(data[4] << 8 | data[5]);



	//Read_Gyro();
	//sprintf(lcd,"X: %d Y: %d Z: %d",YUME_MPU6050.Gyroscope_X,YUME_MPU6050.Gyroscope_Y,YUME_MPU6050.Gyroscope_Z);
	//Command_Gait(USART1,lcd);

	//printf(lcd);
}


void Read_Accelerometer(void)
{
	int count;
	uint8_t data[6];

	/* Read accelerometer data */
	//TM_I2C_ReadMulti(MPU6050_I2C, DataStruct->Address, MPU6050_ACCEL_XOUT_H, data, 6);


	for (count=0;count<=5;count++)
	{
		//Read_I2C(shifted_address,regs);
		Read_I2C2(0x68, MPU6050_ACCEL_XOUT_H); //Aslinya 0xD0, tapi hasilnya digeser satu bit ke kanan
		data[count]=DATA;
	}


	/* Format */
	YUME_MPU6050.Accelerometer_X = (int16_t)(data[0] << 8 | data[1]);
	YUME_MPU6050.Accelerometer_Y = (int16_t)(data[2] << 8 | data[3]);
	YUME_MPU6050.Accelerometer_Z = (int16_t)(data[4] << 8 | data[5]);
}



//var distance_X = 0
//var velocity_X = 0
//
//function update_acceleration_X (acceleration_X) {
//    velocity_X = velocity_X + acceleration_X
//    distance_X = distance_X + velocity_X
//}
//
//// To use the distance value just read the distance_X variable:
//function get_distance_X_and_reset () {
//    x = distance_X
//    distance_X = 0
//    return x
//}
int distance_x = 0;
int velocity_x = 0;

int yume_get_distance_x(void)
{
	int ret_val=0;
	velocity_x = velocity_x + YUME_MPU6050.Accelerometer_X;
	distance_x = distance_x + velocity_x;
	ret_val = distance_x;
	distance_x=0;

	return ret_val;
}

double Accel2mms(double accel, double freq)
{
    double result = 0;
    result = (GRAVITY*accel)/(2*PI*freq);
    return result;
}



void ComplementaryFilter(float *pitch, float *roll)
{
    float pitchAcc, rollAcc;

    // Integrate the gyroscope data -> int(angularSpeed) = angle
    *pitch += ((float)YUME_MPU6050.Gyroscope_X / GYROSCOPE_SENSITIVITY) * dt; // Angle around the X-axis
    *roll -= ((float)YUME_MPU6050.Gyroscope_Y / GYROSCOPE_SENSITIVITY) * dt;    // Angle around the Y-axis

    // Compensate for drift with accelerometer data if !bullshit
    // Sensitivity = -2 to 2 G at 16Bit -> 2G = 32768 && 0.5G = 8192
    int forceMagnitudeApprox = abs(YUME_MPU6050.Accelerometer_X) + abs(YUME_MPU6050.Accelerometer_Y) + abs(YUME_MPU6050.Accelerometer_Z);
    if (forceMagnitudeApprox > 8192 && forceMagnitudeApprox < 32768)
    {
	// Turning around the X axis results in a vector on the Y-axis
        pitchAcc = atan2f((float)YUME_MPU6050.Accelerometer_Y, (float)YUME_MPU6050.Accelerometer_Z) * 180 / M_PI;
        *pitch = *pitch * 0.98 + pitchAcc * 0.02;

	// Turning around the Y axis results in a vector on the X-axis
        rollAcc = atan2f((float)YUME_MPU6050.Accelerometer_X, (float)YUME_MPU6050.Accelerometer_Z) * 180 / M_PI;
        *roll = *roll * 0.98 + rollAcc * 0.02;
    }
}
