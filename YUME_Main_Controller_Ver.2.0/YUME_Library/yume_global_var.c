
#include "yume_global_var.h"

/*
 * YUME TCS3200 GLOBAL VARIABLES
 */

char tcs3200;

unsigned int COLOUR_NONE  = 0;
unsigned int COLOUR_BLACK = 1;
unsigned int COLOUR_WHITE = 2;
unsigned int COLOUR_RED   = 3;
unsigned int COLOUR_BLUE  = 4;

unsigned int F_DETECTED_COLOUR=0;
unsigned int M_DETECTED_COLOUR=0;

float F_sum_red=0, F_sum_blue=0, F_sum_white=0,F_THRESHOLD=0;
float M_sum_red=0, M_sum_blue=0, M_sum_white=0,M_THRESHOLD=0;

//TCS3200 MODE
unsigned int FRONT_TCS3200 = 1;
unsigned int MID_TCS3200   = 2;

//TRACER FLAG
unsigned int TRACER_STAT=0;
unsigned int COLOUR_STAT;
unsigned int CANDLE_CIRCLE_FLAG = 0;
unsigned int CANDLE_CIRCLE_DETECTED =1;

int F_TRACER,M_TRACER,TRACER_FLAG;

/*
 * YUME FOLLOW CARPET MACROS
 */
unsigned int NORMAL  = 1;
unsigned int INVERSE = 2;

/*
 *  YUME HOME AND ZONE FLAG
 */


unsigned int INITIAL_ZONE=0;
unsigned int ZONE=0;

unsigned int ZONE_BLACK = 1;
unsigned int ZONE_RED   = 2;
unsigned int ZONE_BLUE  = 3;
unsigned int TRACER     = 4;

unsigned int YUME_CROSS_MODE = 0;

//YUME START MODE
unsigned int START_MODE=0;
enum YUME_START_MODE {NON_ARBITRARY_START = 1, ARBITRARY_START = 2};


/*
 * YUME ROOM AND CORRIDOR VARIABLES
 */
unsigned int YUME_LOCATION[2]={0,0};
unsigned int LOCATION_CORRIDOR=0;
unsigned int LOCATION_ROOM=1;

/*
 * YUME FIRE FIGHTING VARIABLES
 */
unsigned int EXTINGUISHED_FIRE= 0;
unsigned int FIRE_PUTOFF=0;



/*
 * YUME ROTARY SWITCH GLOBAL VARIABLES
 */
uint16_t rot_switch_mode;

/*
 * YUME CAT OBSTACLE VARIABLES
 */
unsigned int CAT_FOUND[2]={0,0};
unsigned int CAT_SHIFT_FOLLOW=0;
unsigned int SHIFT_FOLLOW=1;
unsigned int NO_SHIFT=0;
/*
 * YUME INFRARED PROXIMITY SENSOR
 */

int INFRARED[6];

/*
 * YUME SOUND ACTIVATION VARIABLES
 */
uint16_t YUME_Sound_Activate=0;

/*
 * YUME REGULAR ACTIVATION VARIABLES
 */
uint16_t YUME_START=0;
/*
 * YUME HAMAMATSU UV-TRON GLOBAL VARIABLES
 */
unsigned int UV_state;

unsigned int UV_Lock=0;
/*
 * YUME ROTARY SWITCH
 */
unsigned int ADCResult;

/*
 * YUME LED BLINK INTERRUPT
 */
uint16_t led_counter=1;
uint16_t led_off_counter=0;

//--------Variable TPA n Servo---------//
int DATA;int sudut_servo=0;

//--------Variable UV tpa---------//
unsigned char sensor_UV_front=1,sensor_UV_back=1;
unsigned char sensor_UV_lock=0;
unsigned char fight=0;
unsigned char hitung=0;
unsigned int panas=0;
int regs;
int i=0, temp=0;
unsigned char suhu_ruang=0, suhu_pixel[7], pixel[10];
int DATA;
unsigned int FIRE=0;

//USART COMM VARIABLES
char received_string[MAX_STRLEN],buf[16],data1_lcd[16],data2_lcd[16],tampil_adc[16];
unsigned int Ping[9],USART_Count=0;
unsigned int USART_Bluetooth_Count=0;
unsigned int COMMAND_FLAG_A = 0x3C;
unsigned int COMMAND_FLAG_B = 0x3E;
unsigned int COMMAND_COMPLETED = 0x7C;
unsigned int COMMAND_NOT_RECEIVED = 0x3F;
char command_code[5];

/*
 * YUME HEXAPOD VARIABLES
 */

uint32_t CCR1_Val, CCR2_Val, CCR3_Val;
uint32_t Count;
uint32_t nCount;

int sudut_target[18],sudut_awal[18],x=0,z_A[18],pembagi,y_A[18],sudut_tahap1[18],sudut_tahap2[18];

//float gamma[6];
int leg;
int ww[6];
float beta[6];
float leg_next[6];
float A_body;			// translasi pusat badan
float A_leg[6];			// translasi setiap pangkal kaki
float P_body;			// jarak titik pusat robot ke titik imajiner putar
int epsilon_body;		// besar sudut vektor hadap thdp vektor poros-imajiner
float phi[6];
float lambpsi[6];		//lambda / psi temporal
float P_leg[6];			// jarak titik pusat kaki ke titik imajiner putar
float epsilon[6];		// epsilon tiap N

unsigned int sudut[18]= {4915,4915,4915,4915,4915,4915,    	//3615 == 1 ms   4915 == 1,5 ms
						 4915,4915,4915,4915,4915,4915,			//6550 == 2 ms
						 4915,4915,4915,4915,4915,4915};

unsigned char y,a,b,c,d,v,index_bantu,index_servo;
uint32_t PrescalerValue;

    const float A_const[18] ={26.68,26.68,26.68,26.68,26.68,26.68,
    		 	 	 	 	  26.68,26.68,26.68,26.68,26.68,26.68,
    		 	 	 	 	  26.68,26.68,26.68,26.68,26.68,26.68
    						  /*13.33,50,13.33,13.33,50,13.33,
                              18.18,18.18,18.18,18.18,18.18,18.18,
                              18.18,18.18,18.18,18.18,18.18,18.18*/ };

    const int B_const[18]   ={2681.14,2681.14,2681.14,2681.14,2681.14,2681.14,
    						  2681.14,2681.14,2681.14,2681.14,2681.14,2681.14,
    						  2681.14,2681.14,2681.14,2681.14,2681.14,2681.14
			  	  	  	  	  /*1800,-1500,1800
							  ,1800,-1500,1800,
                              1364,1364,1364,1364,1364,1364,
                              1364,1364,1364,1364,1364,1364*/};

    //LAST REVISED BY RYAN: MON, APRIL 2nd, 2015
     float correct[18]        ={/*0,0,0,0,0,0,					0,0,0,0,0,0
			  	  	  	  	  0,0,0,0,0,0,						100,35,93,43,105,37
			  	  	  	  	  0,0,0,0,0,0						121,41,103,37,102,30*/
    							/*
    		    				* LEFT SECTION
    		    				* Coxa,Femur,Tibia
    		    				* Coxa ditambah makin ke belakang
    		    				* Femur ditambah makin turun
    		    				* Tibia ditambah makin masuk ke dalam
    		    				*
    		    				*/

								27,11,107,  //FRONT LEFT LEG //
								25,24,93,//MIDDLE LEFT LEG //
								13,35,109,//REAR LEFT LEG //

//	  	  	  					27,11,110,  //FRONT LEFT LEG //
//	  	  	  					25,22,92,//MIDDLE LEFT LEG //
//	  	  	  					13,35,112,//REAR LEFT LEG //

//			  	  	  			27,11,107,  //FRONT LEFT LEG //
//			  	  	  			25,22,90,//MIDDLE LEFT LEG //
//			  	  	  			95,35,112,//REAR LEFT LEG //

								//CALIBRATING LEFT SIDE
//								90,11,107,  //FRONT LEFT LEG //
//								25,22,90,//MIDDLE LEFT LEG //
//								-55,35,112,//REAR LEFT LEG //

            					//CALIBRATING LEFT SIDE
			  	  	  	  	  	//90,11,107,  //FRONT LEFT LEG //FIX
			  	  	  	  	  	//25,22,118,//MIDDLE LEFT LEG //FIX
			  	  	  	  	  	//-55,29,114,//REAR LEFT LEG //FIX
			  	  	  	        //95,29,114,//REAR LEFT LEG //REAR LEFT DITARIK KE BELAKANG

    							  //27,8,107,  //FRONT LEFT LEG //
    							  //25,19.5,118,//MIDDLE LEFT LEG //
    							  //15,29,114,//REAR LEFT LEG //


    		    		 	 	 /* RIGHT SECTION
    		    		 	 	 * Coxa,Femur,Tibia
    		    		 	 	 * Coxa ditambah makin ke belakang
    		    		 	 	 * Femur ditambah makin turun
    		    		 	 	 * Tibia ditambah makin masuk ke dalam
    		    		 	 	 */

								 27,33,105, //FRONT RIGHT LEG //
								 24,37,78, //MIDDLE RIGHT LEG //
								 17,48,30  //REAR RIGHT LEG //
//								 17,48,68  //REAR RIGHT LEG //

//    		    		         27,27,117, //FRONT RIGHT LEG //
//    		    		         24,20,88, //MIDDLE RIGHT LEG //
//    		    		         12,49,65  //REAR RIGHT LEG //
//    		    		         100,20,110  //REAR RIGHT LEG //

   							     //CALIBRATING RIGHT SIDE
//     		    		         95,27,117, //FRONT RIGHT LEG //
//     		    		         20,20,90 , //MIDDLE RIGHT LEG //
//     		    		         -50,20,110  //REAR RIGHT LEG //
     		    		         //100,20,104  //REAR RIGHT LEG //COXA REAR DITARIK KE BELAKANG

     		    		         //30,31,112, //FRONT RIGHT LEG //
     		    		         //20,24,123, //MIDDLE RIGHT LEG //
     		    		         //17,24,104  //REAR RIGHT LEG //
    						  };

    const float leg_1[6]	=	{8.2802,4.5,8.2802,8.2802,4.5,8.2802};	//L1
    const float leg_norm[6]	=	{9.5,9.5,9.5,9.5,9.5,9.5};				//L2
    const float alpha[6]	=	{30,90,150,30,90,150};					//alpha



float c1,c2;
float Lsem,c3,c4;
int i;
float temp_var,temp_var2;


/*
 * MPU6050 GYRO ACCELEROMETER VARIABLES
 */

unsigned int MPU6050_PWR_MGMT_1  = 0x6B;
unsigned int MPU6050_GYRO_CONFIG = 0x1B;
unsigned int MPU6050_ACCEL_CONFIG = 0x1C;
float Gyro_Mult;
//GYROSCOPE SENSITIVITY
unsigned int Gyro_Sensitivity_250s = 0x00;  /*!< Range is +- 250 degrees/s */
unsigned int Gyro_Sensitivity_500s = 0x01;  /*!< Range is +- 500 degrees/s */
const unsigned int Gyro_Sensitivity_1000s = 0x02; /*!< Range is +- 1000 degrees/s */
const unsigned int Gyro_Sensitivity_2000s = 0x03;  /*!< Range is +- 2000 degrees/s */

//ACCELEROMETER RANGE
unsigned int Accelerometer_Range = 0x00;
float Acce_Mult;

float ACCEL_DEG;
float GYRO_DEG;

/*
 * CALCULATING DISTANCE
 */
double PI = 3.1415926535897932384626433832795;
double GRAVITY = 9806.65;

float dummy_correct[18];
unsigned int TEST = 0x5A;

/*
 * YUME RUNTIME CALIBRATION VARIABLES
 */
int CALIBRATION_STAT=1;

/*
 *  YUME ROOM COUNTER
 */

unsigned int ROOM_COUNTER=0;

/*
 * YUME WARZONE FLAG
 */
unsigned int WARZONE_FLAG=0;

/*
 * YUME HOME CIRCLE
 */
unsigned int HOME_CIRCLE_FLAG = 0;
unsigned int HOME_CIRCLE_DETECTED = 1;

/*
 * YUME FIRE TRACKER COUNTER
 */
unsigned int firetrack_counter=0;


/*
 * YUME FOUND FIRE OVERRIDE
 */
unsigned int YUME_FOUND_FIRE_OVERRIDE=0;
