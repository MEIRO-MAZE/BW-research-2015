/*==============================================================================================
								  PROJECT YUME Ver.1.0
								  GLOBAL VARIABLE LIBRARY
==============================================================================================*/


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
    const float correct[18] ={/*0,0,0,0,0,0,					0,0,0,0,0,0
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
	  	  	  					25,24,90,//MIDDLE LEFT LEG //
	  	  	  					13,29,112,//REAR LEFT LEG //

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

    		    		         27,27,117, //FRONT RIGHT LEG //
    		    		         24,20,90 , //MIDDLE RIGHT LEG //
    		    		         17,20,106  //REAR RIGHT LEG //

   							     //CALIBRATING RIGHT SIDE
     		    		         //95,21,120, //FRONT RIGHT LEG //
     		    		         //20,20,98 , //MIDDLE RIGHT LEG //
     		    		         //-50,20,106  //REAR RIGHT LEG //
     		    		         //100,20,104  //REAR RIGHT LEG //COXA REAR DITARIK KE BELAKANG

     		    		         //30,31,112, //FRONT RIGHT LEG //
     		    		         //20,24,123, //MIDDLE RIGHT LEG //
     		    		         //17,24,104  //REAR RIGHT LEG //
    						  };

    const float leg_1[6]	=	{8.2802,4.5,8.2802,8.2802,4.5,8.2802};	//L1
    const float leg_norm[6]	=	{9.5,9.5,9.5,9.5,9.5,9.5};				//L2
    const float alpha[6]	=	{30,90,150,30,90,150};					//alpha


struct {
        float teta1;
        float teta2;
        float teta3;
       }temp_angle;

float c1,c2;
float Lsem,c3,c4;
int i;
float temp_var,temp_var2;

/*
 * GLOBAL VARIABLES RECENTLY ADDED BY RYAN
 */
//Control loop for gait_mode(SEARCH_FIRE);
//float right=90,left=90;



