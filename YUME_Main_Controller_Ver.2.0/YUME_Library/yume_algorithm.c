/*==============================================================================================
								  	  PROJECT YUME Ver.2.0
								  YUME ALGORITHM LIBRARY
==============================================================================================*/

#include "yume_algorithm.h"

/* func      : void YUME_Dimension_Check(void)
 * brief     : Detecting dimension checking condition and configuring locomotion to fit the dimension check box
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Sunday, October 15th 2015)
 * Revised BY: N/A
 * programmer's Note: Trigger the YUME Dimension check by putting your hands <= 10 cm away from RIGHT and LEFT PING)))
 *
 */
void YUME_Dimension_Check(void)
{
	while( (Ping[PING_LEFT]<=10)&&(Ping[PING_RIGHT]<=10) )
	{
		mov_fwd_3cm(5,FAST);
	}
}

/* func      : void YUME_Home_Detection(void)
 * brief     : Detecting Home
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Sunday, October 15th 2015)
 * Revised BY: N/A
 * programmer's Note:
 *
 */
void YUME_Home_Detection(void)
{
	unsigned int HOME;

//	FRONT_Sum_Colour();
//	F_TRACER= TRACER_STAT;
//	MID_Sum_Colour();
//	M_TRACER= TRACER_STAT;
//	HOME= F_TRACER | M_TRACER;
	//while (F_DETECTED_COLOUR!=COLOUR_WHITE && M_DETECTED_COLOUR!=COLOUR_WHITE){}
//
//	HOME = (F_DETECTED_COLOUR==COLOUR_WHITE) || (M_DETECTED_COLOUR==COLOUR_WHITE)||
//		   (F_DETECTED_COLOUR==COLOUR_RED) || (M_DETECTED_COLOUR==COLOUR_RED);
	//HOME DETECTOR
//	if( (F_DETECTED_COLOUR==COLOUR_WHITE || M_DETECTED_COLOUR==COLOUR_WHITE) ||
//		   (F_DETECTED_COLOUR==COLOUR_WHITE && M_DETECTED_COLOUR==COLOUR_RED) ||
//		   (F_DETECTED_COLOUR==COLOUR_RED && M_DETECTED_COLOUR==COLOUR_WHITE)
//	      )
//
	HOME=1;
	if (HOME==1)
	{

		if (Ping[PING_FRONT]>30)
		{
			YUME_Buzz(4);
//			while(
//					( (Ping[PING_FRONT]>=30) && (Ping[PING_RIGHT]>=20) && (Ping[PING_ASKEW_RIGHT]>=15) ) ||
//					( (Ping[PING_FRONT]<=30) && (Ping[PING_RIGHT]>=20) && (Ping[PING_ASKEW_RIGHT]>=15) )
//				 )
			if( Ping[PING_RIGHT]>20 )
			{
				mov_rot_right(MED,FAR); //adjust to correct position
				mov_rot_right(MED,FAR); //adjust to correct position
				while(
						(Ping[PING_RIGHT]>=15) && (Ping[PING_ASKEW_RIGHT]>=15)
					 )
				{
					mov_rot_right(MED,FAR);
					if(Ping[PING_FRONT]<=13 )
					{
						mov_bwd(5,FAST);
					}

				}
			}

		}

		else if(Ping[PING_FRONT]<=30)
		{
			YUME_Buzz(5);

			while(
//				   Ping[PING_RIGHT]>=25
				   ( (Ping[PING_FRONT]<=30) && (Ping[PING_RIGHT]>=15) ) ||
				   ( (Ping[PING_FRONT]<=30 && Ping[PING_RIGHT]<=15)   )
				 )
			{
				if(Ping[PING_FRONT]<=13 )
				{
					mov_bwd(5,FAST);
				}
				mov_rot_left(MED,FAR);
			}
			while(Ping[PING_ASKEW_RIGHT]<=13)
			{
				mov_rot_left(MED,FAR);
			}
		}

	}
}


void Escape_Home(void)
{
	//mov_fwd_5cm(5,MED);
	MID_Sum_Colour();
	while(TRACER_STAT==1)
	{
		MID_Sum_Colour();
		mov_fwd_5cm(5,MED);
	}
	MID_Sum_Colour();
	if(ZONE==ZONE_RED)INITIAL_ZONE=ZONE_RED;
	else if(ZONE==ZONE_BLUE)INITIAL_ZONE=ZONE_BLUE;

	if(INITIAL_ZONE==ZONE_RED)
	{
		Sendto_PC(USART1,"YUME INITIAL ZONE: RED ZONE \r");
	}

	else if(INITIAL_ZONE==ZONE_BLUE)
	{
		Sendto_PC(USART1,"YUME INITIAL ZONE: BLUE ZONE \r");
	}



}

void Evade_Wall(void)
{
	Get_IR();

	if( (INFRARED[MID_RIGHT]==IR_DETECTED) || (INFRARED[MID_LEFT]==IR_DETECTED) )
	{
		while(INFRARED[MID_RIGHT]==IR_DETECTED)
		{
			Get_IR_Right();
			mov_deflect_left(MED,FAR);
		}

		while(INFRARED[MID_LEFT]==IR_DETECTED)
		{
			Get_IR_Left();
			mov_deflect_right(MED,FAR);
		}
	}

	if( (INFRARED[FRONT_RIGHT]==IR_DETECTED) && (INFRARED[FRONT_LEFT]==IR_NOT_DETECTED) )
	{
		mov_bwd(15,MED);
		while( INFRARED[FRONT_RIGHT]==IR_DETECTED )
		{
			Get_IR();
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
		}
	}
	else if( (INFRARED[FRONT_RIGHT]==IR_NOT_DETECTED) && (INFRARED[FRONT_LEFT]==IR_DETECTED) )
	{
		mov_bwd(15,MED);
		while( INFRARED[FRONT_LEFT]==IR_DETECTED )
		{
			Get_IR();
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
		}
	}


}


void YUME_TCS3200_Check(unsigned int tcs3200_mode, unsigned int cross_mode)
{
	switch(tcs3200_mode)
	{
		case FRONT_TCS3200_MODE:
		{
			FRONT_Sum_Colour();
			F_TRACER= TRACER_STAT;
			switch(cross_mode)
			{
				case CROSS_OFF:
				{
					while (ZONE!=INITIAL_ZONE)
					{
						FRONT_Sum_Colour();
						mov_rot_right(MED,FAR);
						if (Ping[PING_FRONT]>=15)
						{
							mov_fwd_5cm(5,MED);
						}
					}
				}break;

				case CROSS_ON:
				{

				}break;
			}

		}break;

		case MID_TCS3200_MODE:
		{
			MID_Sum_Colour();
			M_TRACER= TRACER_STAT;
			switch(cross_mode)
			{
				case CROSS_OFF:
				{
					while (ZONE!=INITIAL_ZONE)
					{
						MID_Sum_Colour();
						mov_rot_right(MED,FAR);
						if (Ping[PING_FRONT]>=15)
						{
							mov_fwd_5cm(5,MED);
						}
					}

				}break;

				case CROSS_ON:
				{

				}break;
			}

		}break;
		default: break;
	}
	Sendto_PC(USART1,"TRACER STAT: %d | ZONE : %d | INITIAL_ZONE %d \r",TRACER_STAT, ZONE, INITIAL_ZONE);
	TRACER_STAT= F_TRACER | M_TRACER;
}
void YUME_Tracer_Check(void)
{
	F_TRACER=0;
	M_TRACER=0;

    FRONT_Sum_Colour();
    F_TRACER= TRACER_STAT;
	MID_Sum_Colour();
    M_TRACER= TRACER_STAT;

   // Sendto_PC(USART1,"\rTRACER CHECK STATUS - F_TRACER: %d | M_TRACER %d! \r",F_TRACER,M_TRACER);

    //TRACER_STAT= F_TRACER | M_TRACER;
    CANDLE_CIRCLE_FLAG = F_TRACER && M_TRACER;
    HOME_CIRCLE_FLAG = F_TRACER && M_TRACER;


    if( (HOME_CIRCLE_FLAG==HOME_CIRCLE_DETECTED) &&
    	(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
      )
    {
    	Sendto_PC(USART1,"HOME CIRCLE DETECTED! \r");
    	if(INITIAL_ZONE==ZONE_RED)
    	{
    		follow_right_counter(7);
    	}
    	else if (INITIAL_ZONE==ZONE_BLUE)
    	{
    		follow_left_counter(7);
    	}
    }

    else if( (CANDLE_CIRCLE_FLAG==CANDLE_CIRCLE_DETECTED) &&
    	      (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) &&
    	      (YUME_FOUND_FIRE_OVERRIDE==1)
           )
    	{
    		//bypass
    	}

    else if( (CANDLE_CIRCLE_FLAG==CANDLE_CIRCLE_DETECTED) &&
    	      (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) &&
    	      (YUME_FOUND_FIRE_OVERRIDE==0)
           )
    {
    	Sendto_PC(USART1,"CANDLE CIRCLE DETECTED! on YUME_Tracer_Check \r");
    	if (UV_Lock==0)
    	{
    		Sendto_PC(USART1,"YUME AVOIDING CANDLE CIRCLE \r");

    		while(Ping[PING_FRONT]<=25)
    		{
    			if(INITIAL_ZONE==ZONE_BLUE)
    			{
        			mov_rot_right(MED,FAR);
        			mov_rot_right(MED,FAR);
    			}
    			else if(INITIAL_ZONE==ZONE_RED)
    			{
        			mov_rot_left(MED,FAR);
        			mov_rot_left(MED,FAR);
    			}

    		}
    		mov_fwd_5cm(5,FAST);
    	}

    	else if (UV_Lock==1)
    	{
    		//YUME_Found_Fire();
    		while(UV_Lock==1)
    		{
    			YUME_Search_Fire();
    			mov_rot_right(MED,FAR);
    			mov_rot_right(MED,FAR);
    		}


    	}
    	//RESET TRACER STAT
    	F_TRACER=0;
    	M_TRACER=0;

    }


    else if( (M_TRACER==1) && (YUME_FOUND_FIRE_OVERRIDE==0) )
	{
		Sendto_PC(USART1,"\r WHITE LINES DETECTED! \r");

		if ( (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) &&
		     (UV_Lock==1) &&
		     (YUME_FOUND_FIRE_OVERRIDE==0)
		   )
		{
			Get_IR();
			if (Ping[PING_FRONT]<=25 || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED) )
			{
				Sendto_PC(USART1,"YUME FOUND CANDLE CIRCLE! on YUME_Tracer_Check \r");
	    		while(UV_Lock==1)
	    		{
	    			YUME_Search_Fire();
	    			mov_rot_right(MED,FAR);
	    			mov_rot_right(MED,FAR);
	    		}
			}
			else
			{
				ReturnTo_Room();
		    	//RESET TRACER STAT
		    	F_TRACER=0;
		    	M_TRACER=0;
			}

		}


		else
		{
			Sendto_PC(USART1,"\rYUME Location BEFORE \r");
			Send_Location();
			YUME_LOCATION[LAST_LOC]= YUME_LOCATION[PRESENT_LOC];
			YUME_LOCATION[PRESENT_LOC]= LOCATION_ROOM;

			Sendto_PC(USART1,"\rYUME Location Estimator \r");
			Send_Location();

			if( (YUME_LOCATION[LAST_LOC]== LOCATION_CORRIDOR) && (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) )
			{
				YUME_LOCATION[PRESENT_LOC]=LOCATION_ROOM;
				Sendto_PC(USART1,"YUME FOUND A ROOM! \r");


				mov_fwd_5cm(10,FAST_LEVEL_3);

			}
			else if( (YUME_LOCATION[LAST_LOC]== LOCATION_ROOM) && (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) )
			{
				YUME_LOCATION[PRESENT_LOC]=LOCATION_CORRIDOR;
				Sendto_PC(USART1,"YUME FOUND A CORRIDOR! \r");
				Evade_Wall();
				mov_fwd_5cm(15,FAST_LEVEL_3);
			}


			Send_Location ();
	    	//RESET TRACER STAT
	    	F_TRACER=0;
	    	M_TRACER=0;
		}

    	//RESET TRACER STAT
    	F_TRACER=0;
    	M_TRACER=0;
	}

}

/* func      : void Zone_Check (unsigned int INITIAL_ZONE)
 * brief     : Checking current zone and correcting its deviations
 * param     : unsigned int INITIAL_ZONE (ZONE_RED=2, ZONE_BLUE=3)
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note:
 *
 */
void Zone_Check(unsigned int *INITIAL_ZONE)
{
	FRONT_Sum_Colour();
	while (ZONE!= *INITIAL_ZONE)
	{
		FRONT_Sum_Colour();
		mov_rot_right(MED,FAR);
		if (Ping[PING_FRONT]>=25)
		{
			mov_fwd_5cm(5,MED);
		}
	}
}


void YUME_Success(void)
{
	while(1)
	{
		YUME_Buzz(4);
		mov_static();
		delay_ms(200);
	}
}

void YUME_RedFighter(void)
{
	YUME_Initial_Setup(rot_switch_mode);
	Sendto_PC(USART1,"YUME READY TO FIGHT!!! \r");

	INITIAL_ZONE=ZONE_RED;
	while(1)
	{
		//PING_FRONT FAILCHECK
		while(Ping[PING_FRONT]==0)
		{
			YUME_Buzz(1);
			delay_ms(100);
		}

		YUME_Home_Detection();


		YUME_Buzz(4);
		Evade_Wall();
		mov_fwd_5cm(10,FAST);
		Escape_Home();
		Evade_Wall();

		//YUME NON ARBITRARY START (HOME == SC)
		if ( (INITIAL_ZONE==ZONE_RED) || (INITIAL_ZONE==ZONE_BLUE) )
		{
			Sendto_PC(USART1,"\r YUME NON ARBITRARY START \r");

			//YUME RED ZONE
			if(INITIAL_ZONE==ZONE_RED)
			{
				Sendto_PC(USART1,"YUME IN RED ZONE \r");
				YUME_Buzz(2);
				Sendto_PC(USART1,"YUME FOLLOW RIGHT RULE! \r");

				if(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
				{
					follow_right_new();
				}

				while(1)
				{
					Sendto_PC(USART1,"YUME SWITCH LOCATION TO: ROOM \r");
					YUME_Buzz(4);

					while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
					{
						YUME_ROOM();
						if(FIRE_PUTOFF==1) break;
						if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
					}

					Sendto_PC(USART1,"YUME SWITCH LOCATION TO: CORRIDOR \r");
					YUME_Buzz(7);

					while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
					{
						YUME_CORRIDOR();
						if(YUME_LOCATION[PRESENT_LOC]== LOCATION_ROOM) break;
					}
				}

			}

			//YUME BLUE ZONE
			else if (INITIAL_ZONE==ZONE_BLUE)
			{
				Sendto_PC(USART1,"YUME IN BLUE ZONE \r");
				YUME_Buzz(3);

				while(1)
				{
					mov_static();

				}


			}
		}

		//YUME ARBITRARY ZONE (HOME == ROOM)
		else if (M_DETECTED_COLOUR==COLOUR_BLACK)
		{
			YUME_Buzz(1);
			Sendto_PC(USART1,"\r YUME ARBITRARY START! \r");

			YUME_LOCATION[PRESENT_LOC]=LOCATION_ROOM;

			while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
			{
				FollowExit_Right();
				if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
			}


			while(1)
			{
				Sendto_PC(USART1,"YUME SWITCH LOCATION TO: CORRIDOR \r");
				Evade_Wall();
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);

				mov_fwd_5cm(15,MED);
				Send_Location();
				Send_TCS3200_Status();
				Sendto_PC(USART1,"\rYUME FOLLOW LEFT UNTIL RED CARPET DETECTED \r");
				YUME_PID_Init();
				FollowCarpet_Left(&COLOUR_RED,&INVERSE);

				Send_TCS3200_Status();


				while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
				{
					CAT_SHIFT_FOLLOW=NO_SHIFT;
					Sendto_PC(USART1,"\rYUME FOLLOW RIGHT UNTIL TRACER DETECTED\r");
					YUME_PID_Init();
					follow_right_new();
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
				}



				//YUME FOUND A CAT!
				if(CAT_SHIFT_FOLLOW==SHIFT_FOLLOW)
				{
					YUME_PID_Init();
					while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
					{
						CAT_SHIFT_FOLLOW=NO_SHIFT;
						Sendto_PC(USART1,"\rYUME SHIFT ALGORITHM TO FOLLOWTRACER LEFT\r");

						Sendto_PC(USART1,"CAT_SHIFT_FOLLOW: %d \r",CAT_SHIFT_FOLLOW);

						followtracer_left();
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

				}

				//Sendto_PC(USART1,"\r YUME SUCCESS! \r");

				Sendto_PC(USART1,"YUME SWITCH LOCATION TO: ROOM \r");
				YUME_Buzz(4);

				while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
				{
					ROOM_COUNTER++;
					Sendto_PC(USART1,"\r ===YUME ROOM COUNTER: %d ===\r",ROOM_COUNTER);
					YUME_ROOM();
					if(FIRE_PUTOFF==1) break;
					if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
				}


			}


			while(1)
			{
				mov_static();
				delay_ms(100);
				YUME_Buzz(7);
			}


		}
	}
}

void YUME_BlueFighter(void)
{
	YUME_Initial_Setup(rot_switch_mode);
	Sendto_PC(USART1,"\r YUME READY TO FIGHT!!! \r");
	Sendto_PC(USART1,"YUME BLUE FIGHTER! \r");
	INITIAL_ZONE=ZONE_BLUE;
	while(1)
	{
		//PING_FRONT FAILCHECK
		while(Ping[PING_FRONT]==0)
		{
			YUME_Buzz(1);
			delay_ms(100);
		}

		Home_Detection_Blue();

		YUME_Buzz(4);
		Evade_Wall();
		mov_fwd_5cm(10,FAST);
		Escape_Home();
		Evade_Wall();

		//YUME NON ARBITRARY START (HOME == SC)
		if (M_DETECTED_COLOUR==COLOUR_BLUE)
		{
			Sendto_PC(USART1,"\r YUME NON ARBITRARY START\r");

			Sendto_PC(USART1,"YUME IN BLUE ZONE \r");
			YUME_Buzz(2);
			Sendto_PC(USART1,"YUME FOLLOW LEFT RULE! \r");

			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
			{
				YUME_PID_Init();
				followtracer_left();
			}

			while(1)
			{
				Sendto_PC(USART1,"YUME SWITCH LOCATION TO: ROOM \r");
				YUME_Buzz(4);

				while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
				{
					ROOM_COUNTER++;
					Sendto_PC(USART1,"\r ===YUME ROOM COUNTER: %d ===\r",ROOM_COUNTER);
					YUME_ROOM();
					if(FIRE_PUTOFF==1) break;
					if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
				}

				Sendto_PC(USART1,"YUME SWITCH LOCATION TO: CORRIDOR \r");
				YUME_Buzz(7);

				Evade_Wall();
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				mov_fwd_5cm(15,MED);

				Send_Location();
				Send_TCS3200_Status();

				Sendto_PC(USART1,"\rYUME FOLLOW RIGHT UNTIL BLUE CARPET DETECTED \r");
				YUME_PID_Init();
				FollowCarpet_Right(&COLOUR_BLUE,&INVERSE);

				Send_TCS3200_Status();

				while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
				{
					CAT_SHIFT_FOLLOW=NO_SHIFT;
					Sendto_PC(USART1,"\rYUME FOLLOW LEFT UNTIL TRACER DETECTED\r");
					YUME_PID_Init();
					followtracer_left();
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
				}

				//YUME FOUND A CAT!
				if(CAT_SHIFT_FOLLOW==SHIFT_FOLLOW)
				{
					YUME_PID_Init();
					while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
					{
						CAT_SHIFT_FOLLOW=NO_SHIFT;
						Sendto_PC(USART1,"\rYUME SHIFT ALGORITHM TO FOLLOWTRACER LEFT\r");

						Sendto_PC(USART1,"CAT_SHIFT_FOLLOW: %d \r",CAT_SHIFT_FOLLOW);

						follow_right_new();
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}
				}


			}
		}

		//YUME ARBITRARY ZONE (HOME == ROOM)
		else if (M_DETECTED_COLOUR==COLOUR_BLACK)
		{
			YUME_Buzz(1);
			Sendto_PC(USART1,"\r YUME ARBITRARY START! \r");

			YUME_LOCATION[PRESENT_LOC]=LOCATION_ROOM;

			while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
			{
				YUME_PID_Init();
				FollowExit_Left();
				if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
			}


			while(1)
			{
				Sendto_PC(USART1,"\r YUME SWITCH LOCATION TO: CORRIDOR \r");
				Evade_Wall();

				//COUNTERMEASURE FOR ROOM 1
				if(Ping[PING_RIGHT]<=20)
				{
					Sendto_PC(USART1,"YUME IN FRONT OF ROOM 1 \r");
					mov_rot_left(MED,FAR);
					mov_rot_left(MED,FAR);
					mov_rot_left(MED,FAR);
					mov_rot_left(MED,FAR);
					mov_fwd_5cm(20,MED);

					Evade_Wall();

					Send_TCS3200_Status();
					Sendto_PC(USART1,"\rYUME FOLLOW LEFT UNTIL BLUE CARPET DETECTED \r");
					YUME_PID_Init();
					FollowCarpet_Left(&COLOUR_BLUE,&INVERSE);

					Send_TCS3200_Status();
					Sendto_PC(USART1,"\rYUME FOLLOW LEFT UNTIL BLACK FLOOR DETECTED \r");
					YUME_PID_Init();
					FollowCarpet_Left(&COLOUR_BLACK,&INVERSE);

					//NORMAL FOLLOW UNTIL TRACER
					while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
					{
						CAT_SHIFT_FOLLOW=NO_SHIFT;
						Sendto_PC(USART1,"\rYUME FOLLOW LEFT UNTIL TRACER DETECTED\r");
						YUME_PID_Init();
						follow_right_new();
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

					//YUME FOUND A CAT!
					if(CAT_SHIFT_FOLLOW==SHIFT_FOLLOW)
					{
						YUME_PID_Init();
						while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
						{
							CAT_SHIFT_FOLLOW=NO_SHIFT;
							Sendto_PC(USART1,"\rYUME SHIFT ALGORITHM TO FOLLOWTRACER RIGHT\r");
							Sendto_PC(USART1,"CAT_SHIFT_FOLLOW: %d \r",CAT_SHIFT_FOLLOW);
							YUME_PID_Init();
							followtracer_left();
							if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
						}

					}



				}

				//APPLIED FOR ALL ROOMS OTHER THAN ROOM 1
				else
				{
					mov_rot_right(MED,FAR);
					mov_rot_right(MED,FAR);
					mov_rot_right(MED,FAR);
					mov_rot_right(MED,FAR);

					Get_IR();
					while( (Ping[PING_FRONT]<=25) || (INFRARED[FRONT_RIGHT]==IR_DETECTED) || (INFRARED[FRONT_LEFT]==IR_DETECTED) )
					{
						mov_rot_right(MED,FAR);
						mov_rot_right(MED,FAR);
						Get_IR();
					}
					//mov_rot_right(MED,FAR);

					mov_fwd_5cm(15,MED);
					Send_Location();
					Send_TCS3200_Status();

					Sendto_PC(USART1,"\rYUME FOLLOW RIGHT UNTIL BLUE CARPET DETECTED \r");
					YUME_PID_Init();
					FollowCarpet_Right(&COLOUR_BLUE,&INVERSE);

					Send_TCS3200_Status();
					while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
					{
						CAT_SHIFT_FOLLOW=NO_SHIFT;
						Sendto_PC(USART1,"\rYUME FOLLOW LEFT UNTIL TRACER DETECTED\r");
						YUME_PID_Init();
						followtracer_left();
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

					//YUME FOUND A CAT!
					if(CAT_SHIFT_FOLLOW==SHIFT_FOLLOW)
					{
						YUME_PID_Init();
						while(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
						{
							CAT_SHIFT_FOLLOW=NO_SHIFT;
							Sendto_PC(USART1,"\rYUME SHIFT ALGORITHM TO FOLLOWTRACER RIGHT\r");
							Sendto_PC(USART1,"CAT_SHIFT_FOLLOW: %d \r",CAT_SHIFT_FOLLOW);
							YUME_PID_Init();
							follow_right_new();
							if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
						}

					}

				}


				Sendto_PC(USART1,"YUME SWITCH LOCATION TO: ROOM \r");
				YUME_Buzz(4);

				while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
				{
					ROOM_COUNTER++;
					Sendto_PC(USART1,"\r ===YUME ROOM COUNTER: %d ===\r",ROOM_COUNTER);
					YUME_ROOM();
					if(FIRE_PUTOFF==1) break;
					if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
				}
			}


		}
	}
}

/*
 * YUME FOLLOW TRACER
 */

/* func      : void follow_tracer_right(void)
 * brief     : Wall following algorithm using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note:
 *
 */
void follow_tracer_right(void)
{
	while(YUME_LOCATION[PRESENT_LOC]!=LOCATION_ROOM)
	{
//		YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//		if(TRACER_STAT==1){YUME_Success();}

		if (Ping[PING_FRONT]<=25)
		{
			if (Ping[PING_RIGHT]<=15)
			{
				Cat_Avoider();
				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
				mov_rot_left(MED, FAR);
//				YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//				if(TRACER_STAT==1){YUME_Success();}

			}

			if (Ping[PING_RIGHT]>15)
			{
				Cat_Avoider();
				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
				mov_rot_right(MED, FAR);

//				YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//				if(TRACER_STAT==1){YUME_Success();}
			}
		}
		else PID_follow_tracer_right();

		Cat_Avoider();
//		YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//		if(TRACER_STAT==1){YUME_Success();}
	}

}

/* func      : void PID_follow_tracer_right(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void PID_follow_tracer_right(void)
{
	PID_Calc();

	while(YUME_LOCATION[PRESENT_LOC]!=LOCATION_ROOM)
	{
		Cat_Avoider();
//		YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//		if(TRACER_STAT==1){YUME_Success();}

		//KONDISI ROBOT SESUAI
		if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
		//if ( (Ping[PING_RIGHT]<=set_point_upper) && (Ping[PING_RIGHT]>=set_point_lower) )
			{
				Sendto_PC(USART1,"YUME FORWARD \r");
				mov_fwd_5cm(5, FAST);

				Cat_Avoider();
//				YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//				if(TRACER_STAT==1){YUME_Success();}
			}

		//KONDISI ROBOT JAUH DARI DINDING
		else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
			{
				if (PID_F_R.pid_value[0]>3) PID_F_R.pid_value[0]=3; //WINDUP

				if ((PID_F_R.pid_value[0]<1)&&(PID_F_R.pid_value[0]>=0))
					{
						Sendto_PC(USART1,"YUME FORWARD \r");
						mov_fwd_5cm(5, FAST);

						Cat_Avoider();
//						YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//						if(TRACER_STAT==1){YUME_Success();}
					}

				folltracer_curve_right(MED, FAR, &PID_F_R.pid_value[0]);

				Cat_Avoider();
//				YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//				if(TRACER_STAT==1){YUME_Success();}
			}

		//KONDISI ROBOT DEKAT DENGAN DINDING
		else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
			{
				if (PID_F_R.pid_value[0]>3) PID_F_R.pid_value[0]=3; //WINDUP

				if ((PID_F_R.pid_value[0]<1)&&(PID_F_R.pid_value[0]>=0))
					{
						Sendto_PC(USART1,"YUME FORWARD \r");
						mov_fwd_5cm(5, FAST);

						Cat_Avoider();
//						YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//						if(TRACER_STAT==1){YUME_Success();}
					}

				folltracer_curve_left(MED, FAR, &PID_F_R.pid_value[0]);

				Cat_Avoider();
//				YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//				if(TRACER_STAT==1){YUME_Success();}
			}

		Cat_Avoider();
//		YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//		if(TRACER_STAT==1){YUME_Success();}
	}
}


/* func      : void folltracer_curve_right(int SPEED, int STEP, float *COMMAND_LOOP)
 * brief     : convert control signal to actuating signal in follow right until tracer
 * param     : int SPEED, int STEP, float *COMMAND_LOOP (pid_value[0])
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding PID wall following function
 *
 */
void folltracer_curve_right(int SPEED, int STEP, float *COMMAND_LOOP)
{
	int counter;
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	Cat_Avoider();
//	YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//	if(TRACER_STAT==1){YUME_Success();}

	for(counter=1;counter<=*COMMAND_LOOP;counter++)
	{
		while (Ping[PING_FRONT]<=20)
		{
			if (Ping[PING_RIGHT]<=15)
			{
				mov_rot_left(MED, FAR);
				mov_rot_left(MED, FAR);
				OVERRIDE=1;
			}

			if (Ping[PING_RIGHT]>15)
			{
				mov_rot_right(MED, FAR);
				mov_rot_right(MED, FAR);
				OVERRIDE=1;
			}

			Cat_Avoider();
//			YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//			if(TRACER_STAT==1){YUME_Success();}

		}
		if(OVERRIDE==1){break;}

		if (OVERRIDE==0)
		{
			Cat_Avoider();
//			YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//			if(TRACER_STAT==1){YUME_Success();}
			mov_curve_right_trace(MED,FAR);
		}

		Cat_Avoider();
//		YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//		if(TRACER_STAT==1){YUME_Success();}

	}
}

/* func      : void folltracer_curve_left(int SPEED, int STEP, float *COMMAND_LOOP)
 * brief     : convert control signal to actuating signal in follow right until tracer
 * param     : int SPEED, int STEP, float *COMMAND_LOOP (pid_value[0])
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding PID wall following function
 *
 */
void folltracer_curve_left(int SPEED, int STEP, float *COMMAND_LOOP)
{
	int counter;
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	Cat_Avoider();
//	YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//	if(TRACER_STAT==1){YUME_Success();}

	for(counter=1;counter<=*COMMAND_LOOP;counter++)
	{
		while (Ping[PING_FRONT]<=20)
		{
			if (Ping[PING_RIGHT]<=15)
			{
				mov_rot_left(MED, FAR);
				mov_rot_left(MED, FAR);
				OVERRIDE=1;
			}

			if (Ping[PING_RIGHT]>15)
			{
				mov_rot_right(MED, FAR);
				mov_rot_right(MED, FAR);
				OVERRIDE=1;
			}
			Cat_Avoider();
//			YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//			if(TRACER_STAT==1){YUME_Success();}
		}
		if(OVERRIDE==1){break;}

		if(OVERRIDE==0)
		{
			Cat_Avoider();
//			YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//			if(TRACER_STAT==1){YUME_Success();}

			mov_curve_left_trace(MED,FAR);


		}

		Cat_Avoider();
//		YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//		if(TRACER_STAT==1){YUME_Success();}

	}
}


/* func      : void YUME_Found_Fire(void)
 * brief     : YUME FOUND FIRESPOT AND EXTINGUISHING ALGORITHM
 * param     : N/A
 * retval    : N/A
 * Ver       : 2
 * 				 1. Initial Code
 * 			     2. Add anticipation for different value of UV_state and UV_Lock
 * written By: Ryan (???? Missing)
 * Revised BY: Ryan (Monday, November 16th, 2015
 * programmer's Note:
 *
 */
void YUME_Found_Fire(void)
{
	YUME_FOUND_FIRE_OVERRIDE=1;
	YUME_Buzz(3);
	Sendto_PC(USART1,"YUME has found the fire source! \r");
	YUME_Tracer_Check();

	Evade_Wall();
	while( (Ping[PING_FRONT]>=15) && (F_TRACER==TRACER_NOT_DETECTED) )
	{
		Mov_Fwd_Trace(5,FAST_LEVEL_3);
	}

	Get_UV();
	//while ( (UV_state==0) && (UV_Lock==1) )
	while (UV_Lock==1)
	{
		YUME_Search_Fire();
		Get_UV();if ( (UV_state==NO_FIRE)&& (UV_Lock==0) ) break;
		mov_rotright_fire(FAST_LEVEL_3,FAR);
		mov_rotright_fire(FAST_LEVEL_3,FAR);
		mov_rotright_fire(FAST_LEVEL_3,FAR);
	}

	Get_UV();
	UV_Lock=0; //UV_Lock FLAG RESET
	EXTINGUISHED_FIRE++;
	led_off_counter=1;

	Sendto_PC(USART1,"\r||================================================================||\r");
	Sendto_PC(USART1,"UV_state: %d \r",UV_state);
	Sendto_PC(USART1,"UV_Lock: %d \r",UV_Lock);
	Sendto_PC(USART1,"EXTINGUISHED FIRE : %d \r",EXTINGUISHED_FIRE);
	Sendto_PC(USART1,"\r||================================================================||\r");

	if (UV_Lock==0)
	{
		//Get_IR();

		Evade_Wall();
		if(INITIAL_ZONE==ZONE_RED)
		{
			while(Ping[PING_FRONT]<=25)
			//while( (IR_FRONT_RIGHT==IR_NOT_DETECTED)&& (IR_FRONT_LEFT==IR_NOT_DETECTED) )
			{
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
			}
		}
		else if(INITIAL_ZONE==ZONE_BLUE)
		{
			while(Ping[PING_FRONT]<=25)
			//while( (IR_FRONT_RIGHT==IR_NOT_DETECTED)&& (IR_FRONT_LEFT==IR_NOT_DETECTED) )
			{
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);


			}
		}




		Sendto_PC(USART1,"\r END OF YUME FOUND FIRE FUNCTION \r");
		Send_Location ();
		FIRE_PUTOFF=1;
		YUME_FOUND_FIRE_OVERRIDE=0;

	}

}

void YUME_Search_Fire(void)
{
	Sendto_PC(USART1,"YUME Hexapod Search Fire Sequences \r");

	static int right=90,left=90;

	generate_movement_data(0,0,10);
	delay_ms(50);

//	if (FIRE==0&&UV_state==0)
	if (UV_state==0)
	{
		while(right>=20&&left<=160&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Extinguisher();
//			panas = YUME_TPA_Check(1);
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(10);
			right-=1;
			left+=1;

		}
		delay_ms(50);
	}

//	if (FIRE==0&&UV_state==0)
	if (UV_state==0)
	{
		while(right<=90&&left>=90&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Extinguisher();
//			panas = YUME_TPA_Check(1);
		    inv_kine(front_left,left,7,leg);
		    inv_kine(front_right,right,7,leg);
		    inv_kine(middle_left,left,7,leg);
		    inv_kine(middle_right,right,7,leg);
		    inv_kine(rear_left,left,7,leg);
		    inv_kine(rear_right,right,7,leg);
		    delay_ms(10);
		    right+=1;
		    left-=1;
		}
		delay_ms(50);
	}

//	if (FIRE==0&&UV_state==0)
	if (UV_state==0)
	{
		while(right<=160&&left>=20&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Extinguisher();
	//		panas = YUME_TPA_Check(1);

			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(10);
			right+=1;
			left-=1;
		}
		delay_ms(50);
	}

//	if (FIRE==0&&UV_state==0)
	if (UV_state==0)
	{
		while(right>=90&&left<=90&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Extinguisher();
	//		panas = YUME_TPA_Check(1);
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(10);
			right-=1;
			left+=1;
		}
		delay_ms(50);
	}

}

void YUME_FireTracker(void)
{
	Sendto_PC(USART1,"YUME FIRE TRACKING... \r");

	static int right=90,left=90;

	generate_movement_data(0,0,10);
	delay_ms(50);

	if (UV_state==0)
	{
		while(right>=20&&left<=160&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Found_Fire();
//			panas = YUME_TPA_Check(1);
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(5);
			right-=1;
			left+=1;

		}
		delay_ms(50);
	}

	if (UV_state==0)
	{
		while(right<=90&&left>=90&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Found_Fire();
//			panas = YUME_TPA_Check(1);
		    inv_kine(front_left,left,7,leg);
		    inv_kine(front_right,right,7,leg);
		    inv_kine(middle_left,left,7,leg);
		    inv_kine(middle_right,right,7,leg);
		    inv_kine(rear_left,left,7,leg);
		    inv_kine(rear_right,right,7,leg);
		    delay_ms(5);
		    right+=1;
		    left-=1;
		}
		delay_ms(50);
	}

	if (UV_state==0)
	{
		while(right<=160&&left>=20&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Found_Fire();
	//		panas = YUME_TPA_Check(1);

			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(5);
			right+=1;
			left-=1;
		}
		delay_ms(50);
	}

	if (UV_state==0)
	{
		while(right>=90&&left<=90&&UV_state==0)
		{
			Get_TPA81();
			if (FIRE==1)YUME_Found_Fire();
	//		panas = YUME_TPA_Check(1);
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(5);
			right-=1;
			left+=1;
		}
		delay_ms(50);
	}

}


/* func      : void YUME_FireFight(void)
 * brief     : YUME FIRE FIGHTING ALGORITHM IN ENCLOSED ROOM
 * param     : N/A
 * retval    : N/A
 * Ver       : 2
 * 				 1. Initial Code
 * 			     2. Add anticipation for different value of UV_state and UV_Lock
 * written By: Ryan (???? Missing)
 * Revised BY: Ryan (Monday, November 16th, 2015
 * programmer's Note:
 *
 */
void YUME_FireFight(void)
{
	FIRE_PUTOFF==0;
	firetrack_counter=0;
	Get_UV();
	Get_IR();
	Get_TPA81();
	led_off_counter=0;

	YUME_PID_Init();
	//while( (UV_state==0) && (FIRE==0) && (UV_Lock==1) )
	while( (UV_Lock==1) )
	{
		YUME_LED_BLINK_ON;
		Get_UV();
		Get_TPA81();
		if(FIRE==1)YUME_Found_Fire();
		followfire_right_new();
		if(FIRE_PUTOFF==1) break;
		if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
	}

	Get_UV();
	if(UV_state==NO_FIRE)
	{
		UV_Lock=0;
		led_off_counter=1;
	}

	Sendto_PC(USART1,"\r\r ===YUME FOLLOW COUNTER RIGHT RULE=== \r\r");
	YUME_PID_Init();
	follow_right_counter(10);

	Sendto_PC(USART1,"\r\r ===YUME FOLLOW EXIT RIGHT RULE=== \r\r");
	YUME_PID_Init();
	FollowExit_Right();
	YUME_LED_BLINK_OFF;

}

void follow_fire_right(void)
{
	if (Ping[PING_FRONT]<=20)
	{
		while (Ping[PING_ASKEW_RIGHT]<=15)
		{
			mov_rot_left(MED, FAR);
			Get_TPA81();
			if(FIRE==1) YUME_Found_Fire();
		}

		if (Ping[PING_ASKEW_RIGHT]>15)
		{
			mov_rot_right(MED, FAR);
			Get_TPA81();
			if(FIRE==1) YUME_Found_Fire();
		}
	}
	else PID_follow_fire_right();
}

void PID_follow_fire_right(void)
{
	PID_Calc();

	//KONDISI ROBOT SESUAI
	if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
		{
			mov_fwd_5cm(5, MED);
			Get_TPA81();
			if(FIRE==1) YUME_Found_Fire();
		}

	//KONDISI ROBOT JAUH DARI DINDING
	else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
		{
			if (PID_F_R.pid_value[0]>=3) PID_F_R.pid_value[0]=3; //windup

			if ((PID_F_R.pid_value[0]<1)&&(PID_F_R.pid_value[0]>=0))
				{
					mov_fwd_5cm(5, MED);
					Get_TPA81();
					if(FIRE==1) YUME_Found_Fire();
				}
			else SearchFire_curve_right(MED, FAR, &PID_F_R.pid_value[0]);
		}

	//KONDISI ROBOT DEKAT DENGAN DINDING
	else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
		{
			if (PID_F_R.pid_value[0]>=3) PID_F_R.pid_value[0]=3; //windup
			if ((PID_F_R.pid_value[0]<1)&&(PID_F_R.pid_value[0]>=0))
				{
					mov_fwd_5cm(5, MED);
					Get_TPA81();
					if(FIRE==1) YUME_Found_Fire();
				}
			SearchFire_curve_left(MED, FAR, &PID_F_R.pid_value[0]);
		}
}


void SearchFire_curve_right(int SPEED, int STEP, float *COMMAND_LOOP)
{
	int counter;
	//int limit= *COMMAND_LOOP;

	for(counter=1;counter<=*COMMAND_LOOP;counter++)
	{
		Get_TPA81();
		if(FIRE==1) YUME_Found_Fire();
		mov_curve_right_fire(MED,FAR);
	}

}

void SearchFire_curve_left(int SPEED, int STEP, float *COMMAND_LOOP)
{
	int counter;
	//int limit= *COMMAND_LOOP;

	for(counter=1;counter<=*COMMAND_LOOP;counter++)
	{
		Get_TPA81();
		if(FIRE==1) YUME_Found_Fire();
		mov_curve_left_fire(MED,FAR);
	}

}


void mov_curve_right_fire(int SPEED, int STEP)
{
	Sendto_PC(USART1,"YUME Curve Right Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();

	//gait_mode(CURVE_RIGHT, SPEED);
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//2
	inv_kine(front_left,90+beta[0]+15,7,leg);
	inv_kine(front_right,90-beta[3]+10,10,leg-3);
	inv_kine(middle_left,90-beta[1]-15,10,leg-3);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+15,7,leg);
	inv_kine(rear_right,90-beta[5]+10,10,leg-3);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//3
	inv_kine(front_left,90+beta[0]+15,7,leg);
	inv_kine(front_right,90-beta[3]+10,7,leg);
	inv_kine(middle_left,90-beta[1]-15,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+15,7,leg);
	inv_kine(rear_right,90-beta[5]+10,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//4
	inv_kine(front_left,90+beta[0]+15,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],10.5,leg-3);
	inv_kine(rear_left,90+beta[2]+15,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//5
	inv_kine(front_left,90-beta[0]-15,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+15,7,leg);
	inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]-15,10,leg-3);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//6
	inv_kine(front_left,90-beta[0]-15,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+15,7,leg);
	inv_kine(middle_right,90-beta[4]+10,7,leg);
	inv_kine(rear_left,90-beta[2]-15,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);
}


void mov_curve_left_fire(int SPEED, int STEP)
{
	Sendto_PC(USART1,"YUME Curve Left Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//gait_mode(CURVE_LEFT, SPEED);

	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3]+15,10,leg-3);
	inv_kine(middle_left,90+beta[1]-10,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5]+15,10,leg-3);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//2
	inv_kine(front_left,90+beta[0]-10,7,leg);
	inv_kine(front_right,90-beta[3]-15,10,leg-3);
	inv_kine(middle_left,90-beta[1]+10,10,leg-3);
	inv_kine(middle_right,90+beta[4]+15,7,leg);
	inv_kine(rear_left,90+beta[2]-10,7,leg);
	inv_kine(rear_right,90-beta[5]-15,10,leg-3);
	delay_ms(SPEED);


	Get_TPA81();
	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//3
	inv_kine(front_left,90+beta[0]-10,7,leg);
	inv_kine(front_right,90-beta[3]-15,7,leg);
	inv_kine(middle_left,90-beta[1]+10,7,leg);
	inv_kine(middle_right,90+beta[4]+15,7,leg);
	inv_kine(rear_left,90+beta[2]-10,7,leg);
	inv_kine(rear_right,90-beta[5]-15,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//4
	inv_kine(front_left,90+beta[0]-10,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4]+15,10.5,leg-3);
	inv_kine(rear_left,90+beta[2]-10,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//5
	inv_kine(front_left,90-beta[0]+10,10,leg-3);
	inv_kine(front_right,90+beta[3]+15,7,leg);
	inv_kine(middle_left,90+beta[1]-10,7,leg);
	inv_kine(middle_right,90-beta[4]-15,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]+10,10,leg-3);
	inv_kine(rear_right,90+beta[5]+15,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//6
	inv_kine(front_left,90-beta[0]+10,7,leg);
	inv_kine(front_right,90+beta[3]+15,7,leg);
	inv_kine(middle_left,90+beta[1]-10,7,leg);
	inv_kine(middle_right,90-beta[4]-15,7,leg);
	inv_kine(rear_left,90-beta[2]+10,7,leg);
	inv_kine(rear_right,90+beta[5]+15,7,leg);
	delay_ms(SPEED);
}

void YUME_Extinguisher(void)
{
	Sendto_PC(USART1,"YUME Hexapod Search Fire Sequences \r");

	static int right=90,left=90;

	generate_movement_data(0,0,10);
	delay_ms(50);

	YUME_BLDC_ON;
	delay_ms(50);

		while(right>=20&&left<=160&&UV_state==0)
		{
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(10);
			right-=5;
			left+=5;
			if(UV_state==1) break;
		}
		delay_ms(100);

		while(right<=90&&left>=90&&UV_state==0)
		{
		    inv_kine(front_left,left,7,leg);
		    inv_kine(front_right,right,7,leg);
		    inv_kine(middle_left,left,7,leg);
		    inv_kine(middle_right,right,7,leg);
		    inv_kine(rear_left,left,7,leg);
		    inv_kine(rear_right,right,7,leg);
		    delay_ms(10);
		    right+=5;
		    left-=5;
		    if(UV_state==1) break;
		}
		delay_ms(100);

		while(right<=160&&left>=20&&UV_state==0)
		{
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(10);
			right+=5;
			left-=5;
			if(UV_state==1) break;
		}
		delay_ms(100);

		while(right>=90&&left<=90&&UV_state==0)
		{
			inv_kine(front_left,left,7,leg);
			inv_kine(front_right,right,7,leg);
			inv_kine(middle_left,left,7,leg);
			inv_kine(middle_right,right,7,leg);
			inv_kine(rear_left,left,7,leg);
			inv_kine(rear_right,right,7,leg);
			delay_ms(10);
			right-=5;
			left+=5;
			if(UV_state==1) break;
		}
		delay_ms(200);

	YUME_BLDC_OFF;

	Get_UV();
	if(UV_state==NO_FIRE)
	{
		UV_Lock=0;
	}

}


void Cat_Avoider(void)
{
	Get_IR();

	if(((INFRARED[IR_FRONT_LEFT]==IR_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED)) && (Ping[PING_FRONT]>=25))
	{
		CAT_FOUND[CAT_PREVIOUS] = CAT_FOUND[CAT_PRESENT];
		CAT_FOUND[CAT_PRESENT]++;
	}
	if(CAT_FOUND[CAT_PREVIOUS]!=CAT_FOUND[CAT_PRESENT]) CAT_SHIFT_FOLLOW=SHIFT_FOLLOW;
	else if(CAT_FOUND[CAT_PREVIOUS]==CAT_FOUND[CAT_PRESENT]) CAT_SHIFT_FOLLOW=NO_SHIFT;


	while ( ((INFRARED[IR_FRONT_LEFT]==IR_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED)) && (Ping[PING_FRONT]>=25) )
	{
		Sendto_PC(USART1,"\r YUME DETECTING CAT OBSTACLE! \r CAT_FOUND[CAT_PREVIOUS]: %d \r CAT_FOUND[CAT_PRESENT]: %d \r" ,CAT_FOUND[CAT_PREVIOUS],CAT_FOUND[CAT_PRESENT]);
		Sendto_PC(USART1,"CAT_SHIFT_FOLLOW: %d \r\r",CAT_SHIFT_FOLLOW);

		Get_IR();
		mov_bwd(15,MED);

		if (Ping[PING_RIGHT]<=Ping[PING_LEFT])
		{
			Get_IR();
			while ((INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED))
			{
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				Get_IR();
			}

			Get_IR();
			while ((INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
			{
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				mov_rot_right(MED,FAR);
				Get_IR();
			}
		}

		else if (Ping[PING_LEFT]<Ping[PING_RIGHT])
		{
			Get_IR();
			while ((INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED))
			{
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				Get_IR();
			}

			Get_IR();
			while ((INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
			{
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				mov_rot_left(MED,FAR);
				Get_IR();
			}
		}

		else break;
	}
}

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
void mov_fwd_trace (int distance, int SPEED)
{
	Sendto_PC(USART1,"YUME Forward Movement 5 cm \r");
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

			YUME_Tracer_Check();
//			if(TRACER_STAT==1){YUME_Success();}


			//2
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],10,leg-3);
			inv_kine(middle_left,90-beta[1],10,leg-3);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],10,leg-3);
			delay_ms(SPEED);

			YUME_Tracer_Check();
//			if(TRACER_STAT==1){YUME_Success();}

			//3
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],7,leg);
			inv_kine(middle_left,90-beta[1],7,leg);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],7,leg);
			delay_ms(SPEED);

			YUME_Tracer_Check();
//			if(TRACER_STAT==1){YUME_Success();}

			//4
			inv_kine(front_left,90+beta[0],10,leg-3);
			inv_kine(front_right,90,7,leg);
			inv_kine(middle_left,90,7,leg);
			inv_kine(middle_right,90+beta[4],10,leg-3);
			inv_kine(rear_left,90+beta[2],7,leg-2);
			inv_kine(rear_right,90,7,leg);
			delay_ms(SPEED);

			YUME_Tracer_Check();
//			if(TRACER_STAT==1){YUME_Success();}

			//5
			inv_kine(front_left,90-beta[0],10,leg-3);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],10,leg-3);
			inv_kine(rear_left,90-beta[2],7,leg-2);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);

			YUME_Tracer_Check();
//			if(TRACER_STAT==1){YUME_Success();}

			//6
			inv_kine(front_left,90-beta[0],7,leg);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],7,leg);
			inv_kine(rear_left,90-beta[2],7,leg);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);

			YUME_Tracer_Check();
//			if(TRACER_STAT==1){YUME_Success();}
		}
}

/*
 * Func : void mov_curve_right(int SPEED, int STEP)
 * brief: Robot's forward curve right locomotion, useful for wall following and solving the crossroad
 * Written By: Ryan (MON, MARCH 9th, 2015)
 * Ver  : 2 (Last Revised By: Ryan (MON, August 19th, 2015)
 *   	     sudut belok dikurangi (yang lama sering bermasalah untuk follow)
 * programmer's note  :
 */
void mov_curve_right_trace(int SPEED, int STEP)
{
	Sendto_PC(USART1,"YUME Curve Right Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}

	//gait_mode(CURVE_RIGHT, SPEED);
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//2
	inv_kine(front_left,90+beta[0]+15,7,leg);
	inv_kine(front_right,90-beta[3]+10,10,leg-3);
	inv_kine(middle_left,90-beta[1]-15,10,leg-3);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+15,7,leg);
	inv_kine(rear_right,90-beta[5]+10,10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//3
	inv_kine(front_left,90+beta[0]+15,7,leg);
	inv_kine(front_right,90-beta[3]+10,7,leg);
	inv_kine(middle_left,90-beta[1]-15,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+15,7,leg);
	inv_kine(rear_right,90-beta[5]+10,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//4
	inv_kine(front_left,90+beta[0]+15,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],10.5,leg-3);
	inv_kine(rear_left,90+beta[2]+15,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//5
	inv_kine(front_left,90-beta[0]-15,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+15,7,leg);
	inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]-15,10,leg-3);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//6
	inv_kine(front_left,90-beta[0]-15,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+15,7,leg);
	inv_kine(middle_right,90-beta[4]+10,7,leg);
	inv_kine(rear_left,90-beta[2]-15,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);
}

/*
 * Func : void mov_curve_left(int SPEED, int STEP)
 * brief: Robot's forward curve right locomotion, useful for wall following and solving the crossroad
 * Written By: Ryan (MON, MARCH 9th, 2015)
 * Ver  : 2 (Last Revised By: Ryan (MON, August 19th, 2015)
 *   	     sudut belok dikurangi (yang lama sering bermasalah untuk follow)
 * programmer's note  :
 */
void mov_curve_left_trace(int SPEED, int STEP)
{
	Sendto_PC(USART1,"YUME Curve Left Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//gait_mode(CURVE_LEFT, SPEED);
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3]+15,10,leg-3);
	inv_kine(middle_left,90+beta[1]-10,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5]+15,10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//2
	inv_kine(front_left,90+beta[0]-10,7,leg);
	inv_kine(front_right,90-beta[3]-15,10,leg-3);
	inv_kine(middle_left,90-beta[1]+10,10,leg-3);
	inv_kine(middle_right,90+beta[4]+15,7,leg);
	inv_kine(rear_left,90+beta[2]-10,7,leg);
	inv_kine(rear_right,90-beta[5]-15,10,leg-3);
	delay_ms(SPEED);


	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//3
	inv_kine(front_left,90+beta[0]-10,7,leg);
	inv_kine(front_right,90-beta[3]-15,7,leg);
	inv_kine(middle_left,90-beta[1]+10,7,leg);
	inv_kine(middle_right,90+beta[4]+15,7,leg);
	inv_kine(rear_left,90+beta[2]-10,7,leg);
	inv_kine(rear_right,90-beta[5]-15,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//4
	inv_kine(front_left,90+beta[0]-10,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4]+15,10.5,leg-3);
	inv_kine(rear_left,90+beta[2]-10,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//5
	inv_kine(front_left,90-beta[0]+10,10,leg-3);
	inv_kine(front_right,90+beta[3]+15,7,leg);
	inv_kine(middle_left,90+beta[1]-10,7,leg);
	inv_kine(middle_right,90-beta[4]-15,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]+10,10,leg-3);
	inv_kine(rear_right,90+beta[5]+15,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
//	if(TRACER_STAT==1){YUME_Success();}
	//6
	inv_kine(front_left,90-beta[0]+10,7,leg);
	inv_kine(front_right,90+beta[3]+15,7,leg);
	inv_kine(middle_left,90+beta[1]-10,7,leg);
	inv_kine(middle_right,90-beta[4]-15,7,leg);
	inv_kine(rear_left,90-beta[2]+10,7,leg);
	inv_kine(rear_right,90+beta[5]+15,7,leg);
	delay_ms(SPEED);
}

void YUME_ROOM(void)
{
	FIRE_PUTOFF=0;
	WARZONE_FLAG=0;
	UV_state=1;
	UV_Lock=0;
	firetrack_counter=0;

	if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		YUME_Buzz(5);
		Get_UV();

		YUME_Tracer_Check();
		if( (F_DETECTED_COLOUR==COLOUR_RED) || (M_DETECTED_COLOUR==COLOUR_RED) ||
		    (F_DETECTED_COLOUR==COLOUR_BLUE) || (M_DETECTED_COLOUR==COLOUR_BLUE)
		  )
		{
			WARZONE_FLAG=1;
			Sendto_PC(USART1,"\r WARZONE FLAG: %d \r",WARZONE_FLAG);
		}

		if (WARZONE_FLAG==1)
		{
			mov_fwd_5cm(15,FAST_LEVEL_3);
			Sendto_PC(USART1,"YUME ESCAPE WARZONE! \r");
			Escape_Warzone();

		}

		else if( (UV_state==FIRE_DETECTED)||(UV_Lock==1) )
		{
			Sendto_PC(USART1,"YUME ON FIRE ROOM \r");
			YUME_FireFight();
		}

		else if ( (UV_state==NO_FIRE) && (UV_Lock==0) )
		{
			mov_fwd_5cm(10,FAST_LEVEL_3);
			Sendto_PC(USART1,"YUME ON NORMAL ROOM \r");

			//Exit_No_Fire();

			if(INITIAL_ZONE==ZONE_RED)
			{
				Exit_No_Fire_Red();
			}
			else if(INITIAL_ZONE==ZONE_BLUE)
			{
				Exit_No_Fire_Blue();
			}



		}
	}
}

void Exit_No_Fire_Red(void)
{
	mov_fwd_5cm(10,FAST_LEVEL_3);
	Evade_Wall();
	Sendto_PC(USART1,"YUME EXIT NO FIRE RED ZONE \r");
	Get_IR();

	while ( (Ping[PING_FRONT]>=30) && (INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED) )
	{

		mov_rot_left(MED,FAR);
		mov_rot_left(MED,FAR);
		mov_rot_left(MED,FAR);
		if(Ping[PING_FRONT]<=10) mov_bwd(5,MED);
		Get_IR();
	}

	Get_IR();
	while ( Ping[PING_FRONT]<=30 || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
	{
		if(Ping[PING_FRONT]<=10) mov_bwd(5,MED);
		mov_rot_left(MED,FAR);
		mov_rot_left(MED,FAR);
		mov_rot_left(MED,FAR);
		mov_rot_left(MED,FAR);

		Get_IR();
	}
	FollowExit_Right();

}

void Exit_No_Fire_Blue(void)
{
	mov_fwd_5cm(5,FAST_LEVEL_3);
	Evade_Wall();
	Sendto_PC(USART1,"YUME EXIT NO FIRE BLUE ZONE \r");
	Get_IR();

	while ( (Ping[PING_FRONT]>=30) && (INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED) )
	{
		mov_rot_right(MED,FAR);
		mov_rot_right(MED,FAR);
		mov_rot_right(MED,FAR);
		if(Ping[PING_FRONT]<=10) mov_bwd(5,MED);
		Get_IR();
	}

	Get_IR();
	while ( Ping[PING_FRONT]<=30 || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
	{
		if(Ping[PING_FRONT]<=10) mov_bwd(5,MED);
		mov_rot_right(MED,FAR);
		mov_rot_right(MED,FAR);
		mov_rot_right(MED,FAR);
		mov_rot_right(MED,FAR);

		Get_IR();
	}
	FollowExit_Left();
}
void YUME_CORRIDOR(void)
{
	if( (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR) )
	{

		YUME_Buzz(7);

		if(WARZONE_FLAG==1)
		{
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);

			mov_fwd_5cm(25,FAST_LEVEL_3);
			WARZONE_FLAG==0;
		}

		else
		{
			mov_fwd_5cm(10,FAST_LEVEL_3);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
		}




		YUME_Tracer_Check();

		Send_TCS3200_Status();
		Sendto_PC(USART1,"\rYUME FOLLOW RIGHT UNTIL DETECTING CARPET \r");
		YUME_PID_Init();
		FollowCarpet_Right(&COLOUR_BLACK,&NORMAL);

		Send_TCS3200_Status();
		Sendto_PC(USART1,"\rYUME FOLLOW LEFT WHILE RED CARPED DETECTED \r");
		YUME_PID_Init();
		FollowCarpet_Left(&COLOUR_RED,&NORMAL);

		Send_TCS3200_Status();
		Sendto_PC(USART1,"\rYUME FOLLOW RIGHT UNTIL TRACER DETECTED\r");
		YUME_PID_Init();
		follow_right_new();

		while(1)
		{
			mov_static();
			delay_ms(100);
			YUME_Buzz(7);
		}

	}
}
/*
 * WALL FOLLOWING NEW ALGORITHM
 */


/* func      : void follow_tracer_right(void)
 * brief     : Wall following algorithm using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note:
 *
 */
void follow_right_new(void)
{
	while( (YUME_LOCATION[PRESENT_LOC]!=LOCATION_ROOM) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
	{
		if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
		Evade_Wall();

		YUME_Tracer_Check();
		if (Ping[PING_FRONT]<=15)
		{
			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
			if (Ping[PING_RIGHT]<=25)
			{
				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
				mov_rot_left(FAST_LEVEL_3, FAR);
				mov_rot_left(FAST_LEVEL_3, FAR);
				mov_rot_left(FAST_LEVEL_3, FAR);
				YUME_Tracer_Check();

			}

			else if (Ping[PING_RIGHT]>25)
			{
				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
				mov_rot_right(FAST_LEVEL_3, FAR);
				mov_rot_right(FAST_LEVEL_3, FAR);
				mov_rot_right(FAST_LEVEL_3, FAR);
				YUME_Tracer_Check();

			}
		}
		else
		{
			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
			Evade_Wall();
			YUME_Tracer_Check();
			PID_follow_right_new();
		}


		YUME_Tracer_Check();
		Cat_Avoider();
	}

}

/* func      : void PID_follow_tracer_right(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void PID_follow_right_new(void)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;



	YUME_Tracer_Check();
	while( (YUME_LOCATION[PRESENT_LOC]!=LOCATION_ROOM) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
	{
		if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
		OVERRIDE=0;

		Evade_Wall();
		PID_Calc();


		while(OVERRIDE==0)
		{
			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
			Evade_Wall();
			Cat_Avoider();

			YUME_Tracer_Check();
			//KONDISI ROBOT SESUAI
			//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
			if ( (Ping[PING_ASKEW_RIGHT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_RIGHT]>=PID_F_R.set_point_lower) )
				{
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					Evade_Wall();
					YUME_Tracer_Check();
					if (Ping[PING_FRONT]<=15)
					{
						if (Ping[PING_RIGHT]<=25)
						{
							YUME_Tracer_Check();
							mov_rot_left(FAST_LEVEL_3, FAR);
							mov_rot_left(FAST_LEVEL_3, FAR);
							mov_rot_left(FAST_LEVEL_3, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_RIGHT]>25)
						{
							YUME_Tracer_Check();
							mov_rot_right(FAST_LEVEL_3, FAR);
							mov_rot_right(FAST_LEVEL_3, FAR);
							mov_rot_right(FAST_LEVEL_3, FAR);
							OVERRIDE=1;
						}
						Cat_Avoider();
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
						YUME_Tracer_Check();
						Sendto_PC(USART1,"YUME FORWARD \r");
						mov_fwd_5cm(5, FAST_LEVEL_3);
						//Mov_Fwd_Trace();
					}
					Cat_Avoider();
					YUME_Tracer_Check();
				}

			//KONDISI ROBOT JAUH DARI DINDING
			else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
				{
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					Evade_Wall();
					YUME_Tracer_Check();
					if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP
					Cat_Avoider();

					if (Ping[PING_FRONT]<=15)
					{
						if (Ping[PING_RIGHT]<=25)
						{

							mov_rot_left(FAST_LEVEL_3, FAR);
							mov_rot_left(FAST_LEVEL_3, FAR);
							mov_rot_left(FAST_LEVEL_3, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_RIGHT]>25)
						{
							mov_rot_right(FAST_LEVEL_3, FAR);
							mov_rot_right(FAST_LEVEL_3, FAR);
							mov_rot_right(FAST_LEVEL_3, FAR);
							OVERRIDE=1;
						}
						Cat_Avoider();
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
						YUME_Tracer_Check();
						curve_right_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
					}

					YUME_Tracer_Check();
					Cat_Avoider();
				}

			//KONDISI ROBOT DEKAT DENGAN DINDING
			else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
				{
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					Evade_Wall();
					YUME_Tracer_Check();
					if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=15)
					{
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
						if (Ping[PING_RIGHT]<=25)
						{
							mov_rot_left(FAST_LEVEL_3, FAR);
							mov_rot_left(FAST_LEVEL_3, FAR);
							mov_rot_left(FAST_LEVEL_3, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_RIGHT]>25)
						{
							mov_rot_right(FAST_LEVEL_3, FAR);
							mov_rot_right(FAST_LEVEL_3, FAR);
							mov_rot_right(FAST_LEVEL_3, FAR);
							OVERRIDE=1;
						}
						YUME_Tracer_Check();
						Cat_Avoider();
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
						YUME_Tracer_Check();
						curve_left_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
					}

					YUME_Tracer_Check();
					Cat_Avoider();
				}

			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
			Evade_Wall();
			YUME_Tracer_Check();
			Cat_Avoider();
		}
		if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
	}

}


/* func      : void folltracer_curve_right(int SPEED, int STEP, float *COMMAND_LOOP)
 * brief     : convert control signal to actuating signal in follow right until tracer
 * param     : int SPEED, int STEP, float *COMMAND_LOOP (pid_value[0])
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding PID wall following function
 *
 */
void curve_right_new(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Right Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=15+ (1 * *COMMAND_LOOP);
	//unsigned int DELTA_MR;
	unsigned int DELTA_RL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=10+ (1 * *COMMAND_LOOP);


	Cat_Avoider();

	//gait_mode(CURVE_RIGHT, SPEED);
	YUME_Tracer_Check();
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//2
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//3
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//4
	inv_kine(front_left,90+beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],10.5,leg-3);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//5
	inv_kine(front_left,90-beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//6
	inv_kine(front_left,90-beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,7,leg);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
	{
		Sendto_PC(USART1,"YUME_LOCATION: CORRIDOR  \r");
	}

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		Sendto_PC(USART1,"YUME_LOCATION: ROOM  \r");
	}
}

/* func      : void folltracer_curve_left(int SPEED, int STEP, float *COMMAND_LOOP)
 * brief     : convert control signal to actuating signal in follow right until tracer
 * param     : int SPEED, int STEP, float *COMMAND_LOOP (pid_value[0])
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding PID wall following function
 *
 */
void curve_left_new(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Left Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_MR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=15+ (1 * *COMMAND_LOOP);

	Cat_Avoider();
//	YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//	if(TRACER_STAT==1){YUME_Success();}

	//gait_mode(CURVE_LEFT, SPEED);
	YUME_Tracer_Check();
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

    YUME_Tracer_Check();
	//2
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//3
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//4
	inv_kine(front_left,90+beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//5
	inv_kine(front_left,90-beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//6
	inv_kine(front_left,90-beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,7,leg);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
	{
		Sendto_PC(USART1,"YUME_LOCATION: CORRIDOR  \r");
	}

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		Sendto_PC(USART1,"YUME_LOCATION: ROOM  \r");
	}
}


void CandleCircle_Check(void)
{
	if ( (M_TRACER==TRACER_DETECTED) && (M_TRACER==TRACER_DETECTED) &&
	     ( (Ping[PING_FRONT]<=25) || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED) )
	   )
	{
		Sendto_PC(USART1,"CANDLE CIRCLE DETECTED on CandleCircle_Check! \r");

		while(UV_Lock==1)
		{
			YUME_Search_Fire();
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
		}

		//YUME_Found_Fire();
	}
}

void followfire_right_new(void)
{

		Sendto_PC(USART1,"UV_state: %d \r",UV_state);
		Sendto_PC(USART1,"UV_Lock: %d \r",UV_Lock);
		Sendto_PC(USART1,"EXTINGUISHED FIRE : %d \r",EXTINGUISHED_FIRE);

		Evade_Wall();
		CandleCircle_Check();
		if (Ping[PING_FRONT]<=20)
		{
			Evade_Wall();
			CandleCircle_Check();
			if (Ping[PING_RIGHT]>=Ping[PING_LEFT])
			{
				CandleCircle_Check();
				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
				mov_rotright_fire(FAST, FAR);
			}

			if (Ping[PING_LEFT]>Ping[PING_RIGHT])
			{
				CandleCircle_Check();
				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
				mov_rotleft_fire(FAST, FAR);

			}
		}
		else PID_followfire_right_new();



}

void PID_followfire_right_new(void)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;


	Evade_Wall();
	CandleCircle_Check();
	while(OVERRIDE==0)
	{
		Sendto_PC(USART1,"firetrack_counter: %d \r",firetrack_counter);
		if(firetrack_counter>=4)
		{

			YUME_FireTracker();
			Evade_Wall();
			mov_rotright_fire(MED,FAR);
			mov_rotright_fire(MED,FAR);
			YUME_FireTracker();

			firetrack_counter=0;
		}

		firetrack_counter++;

		if(FIRE_PUTOFF==1) break;
		CandleCircle_Check();
		Evade_Wall();
		PID_Calc();
		//KONDISI ROBOT SESUAI
		//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
		if ( (Ping[PING_ASKEW_RIGHT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_RIGHT]>=PID_F_R.set_point_lower) )
			{
//				Evade_Wall();
				if(FIRE_PUTOFF==1) break;

				if (Ping[PING_FRONT]<=20)
				{
					CandleCircle_Check();
					if (Ping[PING_RIGHT]>=Ping[PING_LEFT])
					{
						CandleCircle_Check();
						Sendto_PC(USART1,"YUME ROTATE LEFT \r");
						mov_rotright_fire(FAST, FAR);
						mov_rotright_fire(FAST, FAR);
						OVERRIDE=1;
					}

					if (Ping[PING_LEFT]>Ping[PING_RIGHT])
					{
						CandleCircle_Check();
						Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
						mov_rotleft_fire(FAST, FAR);
						mov_rotleft_fire(FAST, FAR);
						OVERRIDE=1;
					}
				}

				if(OVERRIDE==1){break;}

				if (OVERRIDE==0)
				{
					CandleCircle_Check();
					Sendto_PC(USART1,"YUME FORWARD \r");
					Mov_Fwd_Trace(5, FAST_LEVEL_3);
				}
			}

		//KONDISI ROBOT JAUH DARI DINDING
		else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
			{
				if(FIRE_PUTOFF==1) break;
				CandleCircle_Check();
				Evade_Wall();
				if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

				if ((PID_F_R.pid_value[0]<1)&&(PID_F_R.pid_value[0]>=0))
				{
					Sendto_PC(USART1,"PID VALUE FOLLOW FIRE CURVE RIGHT ERROR \r");
					Mov_Fwd_Trace(5,FAST_LEVEL_3);
				}

				else
				{
					CandleCircle_Check();

					if (Ping[PING_FRONT]<=20)
					{
						CandleCircle_Check();
						if (Ping[PING_RIGHT]>=Ping[PING_LEFT])
						{
							CandleCircle_Check();
							Sendto_PC(USART1,"YUME ROTATE LEFT \r");
							mov_rotright_fire(FAST, FAR);
							mov_rotright_fire(FAST, FAR);
							OVERRIDE=1;
						}

						if (Ping[PING_LEFT]>Ping[PING_RIGHT])
						{
							CandleCircle_Check();
							Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
							mov_rotleft_fire(FAST, FAR);
							mov_rotleft_fire(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{

						CandleCircle_Check();
						Evade_Wall();
						curve_right_fire_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
					}

				}


			}

		//KONDISI ROBOT DEKAT DENGAN DINDING
		else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
			{
				if(FIRE_PUTOFF==1) break;
				CandleCircle_Check();
				Evade_Wall();
				if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

				if ((PID_F_R.pid_value[0]<1)&&(PID_F_R.pid_value[0]>=0))
				{
					Sendto_PC(USART1,"PID VALUE FOLLOW FIRE CURVE LEFT ERROR \r");
					Mov_Fwd_Trace(5,FAST_LEVEL_3);
				}

				else
				{
					CandleCircle_Check();
					Evade_Wall();
					if (Ping[PING_FRONT]<=20)
					{
						CandleCircle_Check();
						if (Ping[PING_RIGHT]>=Ping[PING_LEFT])
						{
							CandleCircle_Check();
							Sendto_PC(USART1,"YUME ROTATE LEFT \r");
							mov_rotright_fire(FAST, FAR);
							mov_rotright_fire(FAST, FAR);
							OVERRIDE=1;
						}

						if (Ping[PING_LEFT]>Ping[PING_RIGHT])
						{
							CandleCircle_Check();
							Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
							mov_rotleft_fire(FAST, FAR);
							mov_rotleft_fire(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						CandleCircle_Check();
						Evade_Wall();
						curve_left_fire_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
					}
				}

			}


		CandleCircle_Check();
		if(FIRE_PUTOFF==1) break;
		if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
	}
}

void curve_right_fire_new(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Right Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=15+ (1 * *COMMAND_LOOP);
	//unsigned int DELTA_MR;
	unsigned int DELTA_RL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=10+ (1 * *COMMAND_LOOP);


	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();

	//gait_mode(CURVE_RIGHT, SPEED);
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//2
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//3
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//4
	inv_kine(front_left,90+beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],10.5,leg-3);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//5
	inv_kine(front_left,90-beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//6
	inv_kine(front_left,90-beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,7,leg);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
	{
		Sendto_PC(USART1,"YUME_LOCATION: CORRIDOR  \r");
	}

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		Sendto_PC(USART1,"YUME_LOCATION: ROOM  \r");
	}
}


void curve_left_fire_new(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Left Movement \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_MR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=15+ (1 * *COMMAND_LOOP);


	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//gait_mode(CURVE_LEFT, SPEED);
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//2
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,10,leg-3);
	delay_ms(SPEED);


	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//3
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//4
	inv_kine(front_left,90+beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//5
	inv_kine(front_left,90-beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	Get_TPA81();
	YUME_Tracer_Check();
	if(FIRE==1) YUME_Found_Fire();
	//6
	inv_kine(front_left,90-beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,7,leg);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
	{
		Sendto_PC(USART1,"YUME_LOCATION: CORRIDOR  \r");
	}

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		Sendto_PC(USART1,"YUME_LOCATION: ROOM  \r");
	}

}




/*
 *  YUME WALL FOLLOWING LEFT RULE NEW ALGORITHM
 */

/* func      : void followtracer_left(void)
 * brief     : Wall following left rule algorithm using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 28th 2015)
 * Revised BY: N/A
 * programmer's Note:
 *
 */
void followtracer_left(void)
{
	Evade_Wall();
	while( (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)  && (CAT_SHIFT_FOLLOW==NO_SHIFT) )
	{
		Evade_Wall();
		YUME_Tracer_Check();
		if (Ping[PING_FRONT]<=20)
		{
//			if (Ping[PING_LEFT]>=20)
//			{
//				Cat_Avoider();
//				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
//				mov_rot_left(MED, FAR);
//				YUME_Tracer_Check();
//				if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
//
//			}

//			if (Ping[PING_LEFT]<20)
//			{
//				Cat_Avoider();
//				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
//				mov_rot_right(MED, FAR);
//				YUME_Tracer_Check();
//				if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
//
//			}
			Evade_Wall();
			Cat_Avoider();
			Sendto_PC(USART1,"YUME FACING WALL - ROTATE RIGHT \r");
			mov_rot_right(MED, FAR);
			mov_rot_right(MED, FAR);
			mov_rot_right(MED, FAR);
			YUME_Tracer_Check();
			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
		}
		else
		{
			Evade_Wall();
			YUME_Tracer_Check();
			PID_followtracer_left();
			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
		}

		YUME_Tracer_Check();
		Evade_Wall();
		Cat_Avoider();
		if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
	}

}

/* func      : void PID_followtracer_left(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void PID_followtracer_left(void)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	Evade_Wall();
	YUME_Tracer_Check();
	while( (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)  && (CAT_SHIFT_FOLLOW==NO_SHIFT) )
	{
		OVERRIDE=0;
		while(OVERRIDE==0)
		{
			Evade_Wall();
			PID_Calc_LeftRule();

			Evade_Wall();
			Cat_Avoider();

			YUME_Tracer_Check();
			//KONDISI ROBOT SESUAI
			//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
			if ( (Ping[PING_ASKEW_LEFT]<=PID_F_L.set_point_upper) && (Ping[PING_ASKEW_LEFT]>=PID_F_L.set_point_lower) )
				{
					Evade_Wall();
					YUME_Tracer_Check();
					if (Ping[PING_FRONT]<=20)
					{
//						if (Ping[PING_LEFT]>=15)
//						{
//							YUME_Tracer_Check();
//							mov_rot_left(MED, FAR);
//							mov_rot_left(MED, FAR);
//							OVERRIDE=1;
//						}
//						if (Ping[PING_LEFT]<15)
//						{
//							YUME_Tracer_Check();
//							mov_rot_right(MED, FAR);
//							mov_rot_right(MED, FAR);
//							OVERRIDE=1;
//						}

						YUME_Tracer_Check();
						Sendto_PC(USART1,"YUME FACING WALL - ROTATE RIGHT OVERRIDE \r");
						mov_rot_right(MED, FAR);
						mov_rot_right(MED, FAR);
						mov_rot_right(MED, FAR);
						OVERRIDE=1;

						Evade_Wall();
						Cat_Avoider();
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						YUME_Tracer_Check();
						Sendto_PC(USART1,"YUME FORWARD \r");
						//mov_fwd_5cm(5, MED);
						mov_fwd_trace(5,FAST_LEVEL_3);
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}
					Evade_Wall();
					Cat_Avoider();
					YUME_Tracer_Check();
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
				}

			//KONDISI ROBOT JAUH DARI DINDING
			else if ( Ping[PING_ASKEW_LEFT]<PID_F_L.set_point )
				{
					Evade_Wall();
					YUME_Tracer_Check();
					if (PID_F_L.pid_value[0]>5) PID_F_L.pid_value[0]=5; //WINDUP
					Cat_Avoider();

					if (Ping[PING_FRONT]<=20)
					{
//						if (Ping[PING_LEFT]>=15)
//						{
//							YUME_Tracer_Check();
//							mov_rot_left(MED, FAR);
//							mov_rot_left(MED, FAR);
//							OVERRIDE=1;
//						}
//						if (Ping[PING_LEFT]<15)
//						{
//							YUME_Tracer_Check();
//							mov_rot_right(MED, FAR);
//							mov_rot_right(MED, FAR);
//							OVERRIDE=1;
//						}
						YUME_Tracer_Check();
						Sendto_PC(USART1,"YUME FACING WALL - ROTATE RIGHT OVERRIDE \r");
						mov_rot_right(MED, FAR);
						mov_rot_right(MED, FAR);
						mov_rot_right(MED, FAR);
						OVERRIDE=1;

						Evade_Wall();
						Cat_Avoider();
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						Evade_Wall();
						YUME_Tracer_Check();
						curve_right_new(FAST_LEVEL_3, FAR, &PID_F_L.pid_value[0]);
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

					YUME_Tracer_Check();
					Evade_Wall();
					Cat_Avoider();
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
				}

			//KONDISI ROBOT DEKAT DENGAN DINDING
			else if ( Ping[PING_ASKEW_LEFT]>PID_F_L.set_point )
				{
					YUME_Tracer_Check();
					if (PID_F_L.pid_value[0]>5) PID_F_L.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
//						if (Ping[PING_LEFT]>=15)
//						{
//							YUME_Tracer_Check();
//							mov_rot_left(MED, FAR);
//							mov_rot_left(MED, FAR);
//							OVERRIDE=1;
//						}
//						if (Ping[PING_LEFT]<15)
//						{
//							YUME_Tracer_Check();
//							mov_rot_right(MED, FAR);
//							mov_rot_right(MED, FAR);
//							OVERRIDE=1;
//						}
						YUME_Tracer_Check();
						Sendto_PC(USART1,"YUME FACING WALL - ROTATE RIGHT OVERRIDE \r");
						mov_rot_right(MED, FAR);
						mov_rot_right(MED, FAR);
						mov_rot_right(MED, FAR);
						OVERRIDE=1;

						Cat_Avoider();
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						Evade_Wall();
						YUME_Tracer_Check();
						curve_left_new(FAST_LEVEL_3, FAR, &PID_F_L.pid_value[0]);
						if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
					}

					YUME_Tracer_Check();
					Cat_Avoider();
					if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
				}

			YUME_Tracer_Check();
			Cat_Avoider();
			if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) break;
		}
	}
}


/* func      : void FollowExit_Right(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       :  1. Initial Code
 * 				2. Add anticipation for exit room in blue zone
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: Ryan (Monday, November 16th 2015)
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void FollowExit_Right(void)
{
	Evade_Wall();
	Sendto_PC(USART1,"\rYUME FOLLOW EXIT \r");
	while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
//		Evade_Wall();
		//if(FIRE_PUTOFF==1) break;
		if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;

		if (Ping[PING_FRONT]<=20)
		{
//			Evade_Wall();
//			if (Ping[PING_RIGHT]<=20)
//			{
//				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
//				mov_rot_left(FAST_LEVEL_3, FAR);
//			}
//
//			if (Ping[PING_RIGHT]>20)
//			{
//				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
//				mov_rot_right(FAST_LEVEL_3, FAR);
//			}
			Evade_Wall();
			Sendto_PC(USART1,"YUME FACING WALL - ROTATE LEFT \r");
			mov_rot_left(MED, FAR);
			mov_rot_left(MED, FAR);
			mov_rot_left(MED, FAR);
			YUME_Tracer_Check();

		}
		else
		{
//			Evade_Wall();
			//if(FIRE_PUTOFF==1) break;
			if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
			PID_FollowExit_Right();
		}

//		Evade_Wall();
		//if(FIRE_PUTOFF==1) break;
		if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
	}

}

/* func      : void PID_FollowExit_Right(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void PID_FollowExit_Right(void)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	Send_Location ();

//	while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
//	{
//		Evade_Wall();
		if (OVERRIDE==0)
		{
//			Evade_Wall();
			PID_Calc();
			//KONDISI ROBOT SESUAI
			//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
			if ( (Ping[PING_ASKEW_RIGHT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_RIGHT]>=PID_F_R.set_point_lower) )
				{
					if (Ping[PING_FRONT]<=20)
					{
//						if (Ping[PING_RIGHT]<=15)
//						{
//							mov_rot_left(FAST_LEVEL_3, FAR);
//							mov_rot_left(FAST_LEVEL_3, FAR);
//							OVERRIDE=1;
//						}
//						if (Ping[PING_RIGHT]>15)
//						{
//							mov_rot_right(FAST_LEVEL_3, FAR);
//							mov_rot_right(FAST_LEVEL_3, FAR);
//							OVERRIDE=1;
//						}
						Evade_Wall();
						Sendto_PC(USART1,"YUME FACING WALL - ROTATE LEFT OVERRIDE \r");
						mov_rot_left(MED, FAR);
						mov_rot_left(MED, FAR);
						mov_rot_left(MED, FAR);
						YUME_Tracer_Check();
						OVERRIDE=1;
					}

					//if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
//						Evade_Wall();
						Sendto_PC(USART1,"YUME FORWARD \r");
						Mov_Fwd_Trace(5, FAST_LEVEL_3);
					}
				}

			//KONDISI ROBOT JAUH DARI DINDING
			else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
				{
					if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
//						if (Ping[PING_RIGHT]<=15)
//						{
//							mov_rot_left(FAST_LEVEL_3, FAR);
//							mov_rot_left(FAST_LEVEL_3, FAR);
//							OVERRIDE=1;
//						}
//						if (Ping[PING_RIGHT]>15)
//						{
//							mov_rot_right(FAST_LEVEL_3, FAR);
//							mov_rot_right(FAST_LEVEL_3, FAR);
//							OVERRIDE=1;
//						}
						Evade_Wall();
						Sendto_PC(USART1,"YUME FACING WALL - ROTATE LEFT OVERRIDE \r");
						mov_rot_left(MED, FAR);
						mov_rot_left(MED, FAR);
						mov_rot_left(MED, FAR);
						YUME_Tracer_Check();
						OVERRIDE=1;
					}

					//if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
//						Evade_Wall();
						curve_right_room(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
					}

				}

			//KONDISI ROBOT DEKAT DENGAN DINDING
			else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
				{
					if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
//						if (Ping[PING_RIGHT]<=15)
//						{
//							mov_rot_left(FAST_LEVEL_3, FAR);
//							mov_rot_left(FAST_LEVEL_3, FAR);
//							OVERRIDE=1;
//						}
//						if (Ping[PING_RIGHT]>15)
//						{
//							mov_rot_right(FAST_LEVEL_3, FAR);
//							mov_rot_right(FAST_LEVEL_3, FAR);
//							OVERRIDE=1;
//						}
						Evade_Wall();
						Sendto_PC(USART1,"YUME FACING WALL - ROTATE LEFT OVERRIDE \r");
						mov_rot_left(MED, FAR);
						mov_rot_left(MED, FAR);
						mov_rot_left(MED, FAR);
						YUME_Tracer_Check();
						OVERRIDE=1;
					}

					//if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
//						Evade_Wall();
						curve_left_room(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
					}

				}

		}
//	}

}

/* func      : void FollowExit_Left(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Sunday, November 8th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void FollowExit_Left(void)
{
	Evade_Wall();
	Sendto_PC(USART1,"\rYUME FOLLOW EXIT LEFT RULE\r");
	while(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		Evade_Wall();
		if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;

		if (Ping[PING_FRONT]<=20)
		{
			Evade_Wall();
			if (Ping[PING_LEFT]>=20)
			{
				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
				mov_rot_left(MED, FAR);
			}

			if (Ping[PING_LEFT]<20)
			{
				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
				mov_rot_right(MED, FAR);
			}
		}
		else
		{
			Evade_Wall();
			if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
			PID_FollowExit_Left();
		}

		Evade_Wall();
		if(YUME_LOCATION[PRESENT_LOC]== LOCATION_CORRIDOR) break;
	}

}

/* func      : void PID_FollowExit_Left(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Sunday, November 8th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void PID_FollowExit_Left(void)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	Send_Location ();


//		Evade_Wall();
		if (OVERRIDE==0)
		{
//			Evade_Wall();
			PID_Calc_LeftRule();
			//KONDISI ROBOT SESUAI
			//if ( Ping[PING_ASKEW_RIGHT]==PID_F_L.set_point )
			if ( (Ping[PING_ASKEW_LEFT]<=PID_F_L.set_point_upper) && (Ping[PING_ASKEW_LEFT]>=PID_F_L.set_point_lower) )
				{
					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_LEFT]>=15)
						{
							mov_rot_left(MED, FAR);
							mov_rot_left(MED, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_LEFT]<15)
						{
							mov_rot_right(MED, FAR);
							mov_rot_right(MED, FAR);
							OVERRIDE=1;
						}
					}

					//if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
//						Evade_Wall();
						Sendto_PC(USART1,"YUME FORWARD \r");
						Mov_Fwd_Trace(5, FAST_LEVEL_3);
					}
				}

			//KONDISI ROBOT JAUH DARI DINDING
			else if ( Ping[PING_ASKEW_LEFT]>PID_F_L.set_point )
				{
					if (PID_F_L.pid_value[0]>6) PID_F_L.pid_value[0]=6; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_LEFT]>=15)
						{
							mov_rot_left(MED, FAR);
							mov_rot_left(MED, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_LEFT]<15)
						{
							mov_rot_right(MED, FAR);
							mov_rot_right(MED, FAR);
							OVERRIDE=1;
						}
					}

					//if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
//						Evade_Wall();
						curve_left_room(FAST_LEVEL_3, FAR, &PID_F_L.pid_value[0]);
					}

				}

			//KONDISI ROBOT DEKAT DENGAN DINDING
			else if ( Ping[PING_ASKEW_LEFT]<PID_F_L.set_point )
				{
					if (PID_F_L.pid_value[0]>6) PID_F_L.pid_value[0]=6; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_LEFT]>=15)
						{
							mov_rot_left(MED, FAR);
							mov_rot_left(MED, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_LEFT]<15)
						{
							mov_rot_right(MED, FAR);
							mov_rot_right(MED, FAR);
							OVERRIDE=1;
						}
					}

					//if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
//						Evade_Wall();
						curve_right_room(FAST_LEVEL_3, FAR, &PID_F_L.pid_value[0]);
					}
				}
		}
//	}

}


void Mov_Fwd_Trace (int distance, int SPEED)
{
	Sendto_PC(USART1,"YUME Forward Tracing Movement\r");
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
			Get_IR();

			YUME_Tracer_Check();
			//1
			inv_kine(front_left,90,7,leg);
			inv_kine(front_right,90+beta[3],10,leg-3);
			inv_kine(middle_left,90+beta[1],10,leg-3);
			inv_kine(middle_right,90,7,leg);
			inv_kine(rear_left,90,7,leg);
			inv_kine(rear_right,90+beta[5],10,leg-3);
			delay_ms(SPEED);

			YUME_Tracer_Check();
			//2
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],10,leg-3);
			inv_kine(middle_left,90-beta[1],10,leg-3);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],10,leg-3);
			delay_ms(SPEED);

			YUME_Tracer_Check();
			//3
			inv_kine(front_left,90+beta[0],7,leg);
			inv_kine(front_right,90-beta[3],7,leg);
			inv_kine(middle_left,90-beta[1],7,leg);
			inv_kine(middle_right,90+beta[4],7,leg);
			inv_kine(rear_left,90+beta[2],7,leg);
			inv_kine(rear_right,90-beta[5],7,leg);
			delay_ms(SPEED);

			YUME_Tracer_Check();
			//4
			inv_kine(front_left,90+beta[0],10,leg-3);
			inv_kine(front_right,90,7,leg);
			inv_kine(middle_left,90,7,leg);
			inv_kine(middle_right,90+beta[4],10,leg-3);
			inv_kine(rear_left,90+beta[2],7,leg-2);
			inv_kine(rear_right,90,7,leg);
			delay_ms(SPEED);

			YUME_Tracer_Check();
			//5
			inv_kine(front_left,90-beta[0],10,leg-3);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],10,leg-3);
			inv_kine(rear_left,90-beta[2],7,leg-2);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);

			YUME_Tracer_Check();
			//6
			inv_kine(front_left,90-beta[0],7,leg);
			inv_kine(front_right,90+beta[3],7,leg);
			inv_kine(middle_left,90+beta[1],7,leg);
			inv_kine(middle_right,90-beta[4],7,leg);
			inv_kine(rear_left,90-beta[2],7,leg);
			inv_kine(rear_right,90+beta[5],7,leg);
			delay_ms(SPEED);


		}
}

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
void mov_rotright_fire(int SPEED, int STEP)
{
	Sendto_PC(USART1,"YUME Rotate Right Movement \r");
	switch(STEP)
	{
		case CLOSE:
			{
				generate_movement_data(0,STEP,10);
				delay_ms(50);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//gait_mode(ROTATE_RIGHT,SPEED);
				//1
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],10,leg-3); //mundur
				inv_kine(middle_left,90-beta[1],10,leg-3); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],10,leg-3); //mundur
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//2
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],7,leg); //mundur
				inv_kine(middle_left,90-beta[1],7,leg); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],7,leg); //mundur
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//3
				inv_kine(front_left,90-beta[0],10,leg-3); //maju
				inv_kine(front_right,90-beta[3],7,leg); //maju
				inv_kine(middle_left,90+beta[1],7,leg); //mundur
				inv_kine(middle_right,90+beta[4],10.5,leg-3); //mundur
				inv_kine(rear_left,90-beta[2],10,leg-3); //maju
				inv_kine(rear_right,90-beta[5],7,leg); //maju
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
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

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//gait_mode(ROTATE_RIGHT,SPEED);
				//1
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],10,leg-3); //mundur
				inv_kine(middle_left,90-beta[1],10,leg-3); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],10,leg-3); //mundur
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//2
				inv_kine(front_left,90+beta[0],7,leg); //mundur
				inv_kine(front_right,90+beta[3],7,leg); //mundur
				inv_kine(middle_left,90-beta[1],7,leg); //maju
				inv_kine(middle_right,90-beta[4],7,leg); //maju
				inv_kine(rear_left,90+beta[2],7,leg); //mundur
				inv_kine(rear_right,90+beta[5],7,leg); //mundur
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//3
				inv_kine(front_left,90-beta[0],10,leg-3); //maju
				inv_kine(front_right,90-beta[3],7,leg); //maju
				inv_kine(middle_left,90+beta[1],7,leg); //mundur
				inv_kine(middle_right,90+beta[4],10.5,leg-3); //mundur
				inv_kine(rear_left,90-beta[2],10,leg-3); //maju
				inv_kine(rear_right,90-beta[5],7,leg); //maju
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
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


void mov_rotleft_fire(int SPEED, int STEP)
{
	Sendto_PC(USART1,"YUME Rotate Left SEARCH FIRE Movement \r");
	switch(STEP)
	{
		case CLOSE:
			{
				generate_movement_data(0,STEP,10);
				delay_ms(50);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//gait_mode(ROTATE_LEFT, SPEED);
				//1
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],10,leg-3); //maju
				inv_kine(middle_left,90+beta[1],10,leg-3); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],10,leg-3); //maju /
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//2
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],7,leg); //maju /
				inv_kine(middle_left,90+beta[1],7,leg); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],7,leg); //maju /
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//3
				inv_kine(front_left,90+beta[0],10,leg-3); //mundur /
				inv_kine(front_right,90+beta[3],7,leg); //mundur /
				inv_kine(middle_left,90-beta[1],7,leg); //maju /
				inv_kine(middle_right,90-beta[4],10.5,leg-3); //maju /
				inv_kine(rear_left,90+beta[2],10,leg-3); //mundur /
				inv_kine(rear_right,90+beta[5],7,leg); //mundur /
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
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

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//gait_mode(ROTATE_LEFT, SPEED);
				//1
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],10,leg-3); //maju
				inv_kine(middle_left,90+beta[1],10,leg-3); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],10,leg-3); //maju /
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//2
				inv_kine(front_left,90-beta[0],7,leg); //maju /
				inv_kine(front_right,90-beta[3],7,leg); //maju /
				inv_kine(middle_left,90+beta[1],7,leg); //mundur /
				inv_kine(middle_right,90+beta[4],7,leg); //mundur /
				inv_kine(rear_left,90-beta[2],7,leg); //maju /
				inv_kine(rear_right,90-beta[5],7,leg); //maju /
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
				//3
				inv_kine(front_left,90+beta[0],10,leg-3); //mundur /
				inv_kine(front_right,90+beta[3],7,leg); //mundur /
				inv_kine(middle_left,90-beta[1],7,leg); //maju /
				inv_kine(middle_right,90-beta[4],10.5,leg-3); //maju /
				inv_kine(rear_left,90+beta[2],10,leg-3); //mundur /
				inv_kine(rear_right,90+beta[5],7,leg); //mundur /
				delay_ms(SPEED);

				Get_TPA81();
				if(FIRE==1) YUME_Found_Fire();
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


void Send_Location (void)
{

	Sendto_PC(USART1,"\r\r||===========YUME LOCATION STATUS===========||\r");
	if(YUME_LOCATION[LAST_LOC]==LOCATION_ROOM) Sendto_PC(USART1,"LAST LOCATION: ROOM \r");
	else if(YUME_LOCATION[LAST_LOC]==LOCATION_CORRIDOR) Sendto_PC(USART1,"LAST LOCATION: CORRIDOR \r");

	if(YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM) Sendto_PC(USART1,"PRESENT LOCATION: ROOM \r");
	else if(YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR) Sendto_PC(USART1,"PRESENT LOCATION: CORRIDOR \r \r");
	Sendto_PC(USART1,"\r ||==========================================||\r\r");
}


/*
 *  FOLLOW RIGHT COUNTER
 *
 */

void follow_right_counter(int limit)
{
	int counter;
	//int limit=15;
	for(counter=1;counter<=limit;counter++)
	{
		if (Ping[PING_FRONT]<=20)
		{
			if (Ping[PING_RIGHT]<=20)
			{
				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
				mov_rot_left(FAST, FAR);
			}

			if (Ping[PING_RIGHT]>20)
			{
				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
				mov_rot_right(FAST, FAR);
			}
		}
		else
		{
			PID_follow_right_counter();
		}
	}



}

/* func      : void PID_follow_tracer_right(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void PID_follow_right_counter(void)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	PID_Calc();

	while(OVERRIDE==0)
	{
		if(OVERRIDE==0)
		{
			//KONDISI ROBOT SESUAI
			//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
			if ( (Ping[PING_ASKEW_RIGHT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_RIGHT]>=PID_F_R.set_point_lower) )
				{
					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_RIGHT]<=15)
						{

							mov_rot_left(FAST, FAR);
							mov_rot_left(FAST, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_RIGHT]>15)
						{
							mov_rot_right(FAST, FAR);
							mov_rot_right(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						Sendto_PC(USART1,"YUME FORWARD \r");
						mov_fwd_5cm(5, FAST_LEVEL_3);
						OVERRIDE=1;
					}
				}

			//KONDISI ROBOT JAUH DARI DINDING
			else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
				{
					if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_RIGHT]<=15)
						{
							mov_rot_left(FAST, FAR);
							mov_rot_left(FAST, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_RIGHT]>15)
						{
							mov_rot_right(FAST, FAR);
							mov_rot_right(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						curve_right_counter(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						OVERRIDE=1;
					}

				}

			//KONDISI ROBOT DEKAT DENGAN DINDING
			else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
				{
					if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_RIGHT]<=15)
						{
							mov_rot_left(FAST, FAR);
							mov_rot_left(FAST, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_RIGHT]>15)
						{
							mov_rot_right(FAST, FAR);
							mov_rot_right(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						curve_left_counter(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						OVERRIDE=1;
					}

				}
		}
	}

}

void follow_left_counter(int limit)
{
	int counter;
	//int limit=15;
	for(counter=1;counter<=limit;counter++)
	{
		if (Ping[PING_FRONT]<=20)
		{
			if (Ping[PING_LEFT]>=20)
			{
				Sendto_PC(USART1,"YUME ROTATE LEFT \r");
				mov_rot_left(FAST, FAR);
			}

			if (Ping[PING_LEFT]<20)
			{
				Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
				mov_rot_right(FAST, FAR);
			}
		}
		else
		{
			PID_follow_left_counter();
		}
	}



}

/* func      : void PID_follow_tracer_right(void)
 * brief     : Wall following using PID controller until YUME detects TRACER LINE
 * param     : N/A
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding wall following algorithm
 *
 */
void PID_follow_left_counter(void)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	PID_Calc_LeftRule();

	while(OVERRIDE==0)
	{
		if(OVERRIDE==0)
		{
			//KONDISI ROBOT SESUAI
			//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
			if ( (Ping[PING_ASKEW_LEFT]<=PID_F_L.set_point_upper) && (Ping[PING_ASKEW_LEFT]>=PID_F_L.set_point_lower) )
				{
					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_LEFT]>=15)
						{

							mov_rot_left(FAST, FAR);
							mov_rot_left(FAST, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_LEFT]<15)
						{
							mov_rot_right(FAST, FAR);
							mov_rot_right(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						Sendto_PC(USART1,"YUME FORWARD \r");
						mov_fwd_5cm(5, FAST_LEVEL_3);
						OVERRIDE=1;
					}
				}

			//KONDISI ROBOT JAUH DARI DINDING
			else if ( Ping[PING_ASKEW_LEFT]<PID_F_L.set_point )
				{
					if (PID_F_L.pid_value[0]>5) PID_F_L.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_LEFT]>=15)
						{

							mov_rot_left(FAST, FAR);
							mov_rot_left(FAST, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_LEFT]<15)
						{
							mov_rot_right(FAST, FAR);
							mov_rot_right(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						curve_right_counter(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						OVERRIDE=1;
					}

				}

			//KONDISI ROBOT DEKAT DENGAN DINDING
			else if ( Ping[PING_ASKEW_LEFT]>PID_F_L.set_point )
				{
					if (PID_F_L.pid_value[0]>5) PID_F_L.pid_value[0]=5; //WINDUP

					if (Ping[PING_FRONT]<=20)
					{
						if (Ping[PING_LEFT]>=15)
						{

							mov_rot_left(FAST, FAR);
							mov_rot_left(FAST, FAR);
							OVERRIDE=1;
						}
						if (Ping[PING_LEFT]<15)
						{
							mov_rot_right(FAST, FAR);
							mov_rot_right(FAST, FAR);
							OVERRIDE=1;
						}
					}

					if(OVERRIDE==1){break;}

					if (OVERRIDE==0)
					{
						curve_left_counter(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						OVERRIDE=1;
					}

				}
		}
	}

}

void curve_right_counter(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Right Counter \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=15+ (1 * *COMMAND_LOOP);
	//unsigned int DELTA_MR;
	unsigned int DELTA_RL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=10+ (1 * *COMMAND_LOOP);

	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3);
	delay_ms(SPEED);

	//2
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	//3
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	//4
	inv_kine(front_left,90+beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],10.5,leg-3);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	//5
	inv_kine(front_left,90-beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	//6
	inv_kine(front_left,90-beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,7,leg);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);
}


void curve_left_counter(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Left Counter \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_MR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=15+ (1 * *COMMAND_LOOP);

	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	//2
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	//3
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,7,leg);
	delay_ms(SPEED);

	//4
	inv_kine(front_left,90+beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	//5
	inv_kine(front_left,90-beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	//6
	inv_kine(front_left,90-beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,7,leg);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);
}



/* func      : void folltracer_curve_right(int SPEED, int STEP, float *COMMAND_LOOP)
 * brief     : convert control signal to actuating signal in follow right until tracer
 * param     : int SPEED, int STEP, float *COMMAND_LOOP (pid_value[0])
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding PID wall following function
 *
 */
void curve_right_room(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Right in ROOM \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=15+ (1 * *COMMAND_LOOP);
	//unsigned int DELTA_MR;
	unsigned int DELTA_RL=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=10+ (1 * *COMMAND_LOOP);

	//gait_mode(CURVE_RIGHT, SPEED);
	YUME_Tracer_Check();
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3],10,leg-3);
	inv_kine(middle_left,90+beta[1],10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5],10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//2
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//3
	inv_kine(front_left,90+beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4],7,leg);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//4
	inv_kine(front_left,90+beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4],10.5,leg-3);
	inv_kine(rear_left,90+beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//5
	inv_kine(front_left,90-beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//6
	inv_kine(front_left,90-beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3],7,leg);
	inv_kine(middle_left,90+beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]+10,7,leg);
	inv_kine(rear_left,90-beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5],7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
	{
		Sendto_PC(USART1,"YUME_LOCATION: CORRIDOR  \r");
	}

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		Sendto_PC(USART1,"YUME_LOCATION: ROOM  \r");
	}
}

/* func      : void folltracer_curve_left(int SPEED, int STEP, float *COMMAND_LOOP)
 * brief     : convert control signal to actuating signal in follow right until tracer
 * param     : int SPEED, int STEP, float *COMMAND_LOOP (pid_value[0])
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Monday, October 19th 2015)
 * Revised BY: N/A
 * programmer's Note: This function MUST be called int the corresponding PID wall following function
 *
 */
void curve_left_room(int SPEED, int STEP, float *COMMAND_LOOP)
{
	Sendto_PC(USART1,"YUME Curve Left in ROOM \r");
	generate_movement_data(0,STEP,10);
	delay_ms(50);

	unsigned int DELTA_FL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_FR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_ML=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_MR=15+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RL=10+ (1 * *COMMAND_LOOP);
	unsigned int DELTA_RR=15+ (1 * *COMMAND_LOOP);

//	YUME_TCS3200_Check(MID_TCS3200, YUME_CROSS_MODE);
//	if(TRACER_STAT==1){YUME_Success();}

	//gait_mode(CURVE_LEFT, SPEED);
	YUME_Tracer_Check();
	//1
	inv_kine(front_left,90,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,10,leg-3);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,10,leg-3);
	inv_kine(middle_right,90,7,leg);
	inv_kine(rear_left,90,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,10,leg-3);
	delay_ms(SPEED);

    YUME_Tracer_Check();
	//2
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,10,leg-3);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,10,leg-3);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,10,leg-3);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//3
	inv_kine(front_left,90+beta[0]-DELTA_FL,7,leg);
	inv_kine(front_right,90-beta[3]-DELTA_FR,7,leg);
	inv_kine(middle_left,90-beta[1]+DELTA_ML,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,7,leg);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,7,leg);
	inv_kine(rear_right,90-beta[5]-DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//4
	inv_kine(front_left,90+beta[0]-DELTA_FL,10,leg-3);
	inv_kine(front_right,90,7,leg);
	inv_kine(middle_left,90,7,leg);
	inv_kine(middle_right,90+beta[4]+DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90+beta[2]-DELTA_RL,10,leg-3);
	inv_kine(rear_right,90,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//5
	inv_kine(front_left,90-beta[0]+DELTA_FL,10,leg-3);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,10.5,leg-3);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,10,leg-3);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();
	//6
	inv_kine(front_left,90-beta[0]+DELTA_FL,7,leg);
	inv_kine(front_right,90+beta[3]+DELTA_FR,7,leg);
	inv_kine(middle_left,90+beta[1]-DELTA_ML,7,leg);
	inv_kine(middle_right,90-beta[4]-DELTA_MR,7,leg);
	inv_kine(rear_left,90-beta[2]+DELTA_RL,7,leg);
	inv_kine(rear_right,90+beta[5]+DELTA_RR,7,leg);
	delay_ms(SPEED);

	YUME_Tracer_Check();

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_CORRIDOR)
	{
		Sendto_PC(USART1,"YUME_LOCATION: CORRIDOR  \r");
	}

	if (YUME_LOCATION[PRESENT_LOC]==LOCATION_ROOM)
	{
		Sendto_PC(USART1,"YUME_LOCATION: ROOM  \r");
	}
}



/*
 *  YUME FOLLOW RIGHT UNTIL CARPET FUNCTION
 */

/* func      : void FollowCarpet_Right(int CARPET, int INV_STAT)
 * brief     : Wall following algorithm using PID controller until YUME Detecting designated carpet
 * param     : int CARPET ( COLOUR_BLACK, COLOUR_WHITE, COLOUR_BLUE, COLOUR_RED )
 *                  -> function active as long as designated carpet detected
 * 			   int INV_STAT ( NORMAL, INVERSE )
 * 			        -> switching between function active while detect certain carpet or active as long as certain carpet not detected
 * 			        i.e. NORMAL  -> while(COLOUR_STAT==CARPET)
 * 			             INVERSE -> while(COLOUR_STAT!=CARPET)
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Tuesday, November 3rd 2015)
 * Revised BY: N/A
 * programmer's Note:
 *
 */
void FollowCarpet_Right(int *CARPET, int *INV_STAT)
{
	Sendto_PC(USART1,"\r YUME FOLLOW CARPET RIGHT RULE \r");

	if(*INV_STAT==NORMAL)
	{
		while( (M_DETECTED_COLOUR==*CARPET)  && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			Cat_Avoider();
			Evade_Wall();
			YUME_Tracer_Check();

			if (Ping[PING_FRONT]<=20)
			{
				if (Ping[PING_RIGHT]<=20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE LEFT \r");
					mov_rot_left(FAST_LEVEL_3, FAR);
					YUME_Tracer_Check();
				}
				if (Ping[PING_RIGHT]>20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
					mov_rot_right(FAST_LEVEL_3, FAR);
					YUME_Tracer_Check();
				}
			}
			else
			{
				Cat_Avoider();
				Evade_Wall();
				YUME_Tracer_Check();
				PID_FollowCarpet_Right(& *CARPET,& *INV_STAT);
			}
		}
	}

	else if(*INV_STAT==INVERSE)
	{
		while( (M_DETECTED_COLOUR!= *CARPET) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			Cat_Avoider();
			Evade_Wall();
			YUME_Tracer_Check();

			if (Ping[PING_FRONT]<=20)
			{
				if (Ping[PING_RIGHT]<=20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE LEFT \r");
					mov_rot_left(FAST_LEVEL_3, FAR);
					YUME_Tracer_Check();
				}
				if (Ping[PING_RIGHT]>20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
					mov_rot_right(FAST_LEVEL_3, FAR);
					YUME_Tracer_Check();
				}
			}
			else
			{
				Cat_Avoider();
				Evade_Wall();
				YUME_Tracer_Check();
				PID_FollowCarpet_Right(& *CARPET,& *INV_STAT);
			}
		}
	}
}

/* func      : void PID_FollowCarpet_Right(int *CARPET, int *INV_STAT)
 * brief     : Wall following algorithm using PID controller until YUME Detecting designated carpet
 * param     : int CARPET ( COLOUR_BLACK, COLOUR_WHITE, COLOUR_BLUE, COLOUR_RED )
 *                  -> function active as long as designated carpet detected
 * 			   int INV_STAT ( NORMAL, INVERSE )
 * 			        -> switching between function active while detect certain carpet or active as long as certain carpet not detected
 * 			        i.e. NORMAL  -> while(COLOUR_STAT==CARPET)
 * 			             INVERSE -> while(COLOUR_STAT!=CARPET)
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Tuesday, November 3rd 2015)
 * Revised BY: N/A
 * programmer's Note: THIS FUNCTION MUST BE CALLED IN THE FollowCarpet_Right(int CARPET, int INV_STAT) function
 *
 */
void PID_FollowCarpet_Right(int *CARPET, int *INV_STAT)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	Evade_Wall();
	YUME_Tracer_Check();

	PID_Calc();

	if(*INV_STAT==NORMAL)
	{
		while( (M_DETECTED_COLOUR==*CARPET) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			if(OVERRIDE==0)
			{
				Evade_Wall();
				Cat_Avoider();
				YUME_Tracer_Check();

				//KONDISI ROBOT SESUAI
				//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
				if ( (Ping[PING_ASKEW_RIGHT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_RIGHT]>=PID_F_R.set_point_lower) )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_RIGHT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_RIGHT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							Sendto_PC(USART1,"YUME FORWARD \r");
							mov_fwd_5cm(5, FAST_LEVEL_3);
							//Mov_Fwd_Trace();
						}
						Cat_Avoider();
						YUME_Tracer_Check();
					}

				//KONDISI ROBOT JAUH DARI DINDING
				else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP
						Cat_Avoider();

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_RIGHT]<=15)
							{

								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_RIGHT]>15)
							{
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_right_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				//KONDISI ROBOT DEKAT DENGAN DINDING
				else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_RIGHT]<=15)
							{
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_RIGHT]>15)
							{
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							YUME_Tracer_Check();
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_left_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				Evade_Wall();
				YUME_Tracer_Check();
				Cat_Avoider();
			}
		}
	}

	else if(*INV_STAT==INVERSE)
	{
		while( (M_DETECTED_COLOUR!=*CARPET) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			if(OVERRIDE==0)
			{
				Evade_Wall();
				Cat_Avoider();
				YUME_Tracer_Check();

				//KONDISI ROBOT SESUAI
				//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
				if ( (Ping[PING_ASKEW_RIGHT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_RIGHT]>=PID_F_R.set_point_lower) )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_RIGHT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_RIGHT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							Sendto_PC(USART1,"YUME FORWARD \r");
							mov_fwd_5cm(5, FAST_LEVEL_3);
							//Mov_Fwd_Trace();
						}
						Cat_Avoider();
						YUME_Tracer_Check();
					}

				//KONDISI ROBOT JAUH DARI DINDING
				else if ( Ping[PING_ASKEW_RIGHT]>PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP
						Cat_Avoider();

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_RIGHT]<=15)
							{

								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_RIGHT]>15)
							{
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_right_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				//KONDISI ROBOT DEKAT DENGAN DINDING
				else if ( Ping[PING_ASKEW_RIGHT]<PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_RIGHT]<=15)
							{
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_RIGHT]>15)
							{
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							YUME_Tracer_Check();
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_left_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				Evade_Wall();
				YUME_Tracer_Check();
				Cat_Avoider();
			}
		}
	}

}



/*
 *  YUME FOLLOW LEFT UNTIL CARPET FUNCTION
 */

/* func      :void FollowCarpet_Left(int CARPET, int INV_STAT)
 * brief     : Wall following algorithm using PID controller until YUME Detecting designated carpet
 * param     : int CARPET ( COLOUR_BLACK, COLOUR_WHITE, COLOUR_BLUE, COLOUR_RED )
 *                  -> function active as long as designated carpet detected
 * 			   int INV_STAT ( NORMAL, INVERSE )
 * 			        -> switching between function active while detect certain carpet or active as long as certain carpet not detected
 * 			        i.e. NORMAL  -> while(COLOUR_STAT==CARPET)
 * 			             INVERSE -> while(COLOUR_STAT!=CARPET)
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Tuesday, November 3rd 2015)
 * Revised BY: N/A
 * programmer's Note:
 *
 */
void FollowCarpet_Left(int *CARPET, int *INV_STAT)
{
	Sendto_PC(USART1,"\r YUME FOLLOW CARPET LEFT RULE \r");

	YUME_Tracer_Check();
	if(*INV_STAT==NORMAL)
	{
		while( (M_DETECTED_COLOUR==*CARPET) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			Cat_Avoider();
			Evade_Wall();
			YUME_Tracer_Check();

			if (Ping[PING_FRONT]<=20)
			{
				if (Ping[PING_LEFT]>=20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE LEFT \r");
					mov_rot_left(FAST_LEVEL_3, FAR);
					YUME_Tracer_Check();
				}
				if (Ping[PING_LEFT]<20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
					mov_rot_right(FAST_LEVEL_3, FAR);
					YUME_Tracer_Check();
				}
			}
			else
			{
				Cat_Avoider();
				Evade_Wall();
				YUME_Tracer_Check();
				PID_FollowCarpet_Left(& *CARPET,& *INV_STAT);
			}
		}
	}

	else if(*INV_STAT==INVERSE)
	{
		while( (M_DETECTED_COLOUR!=*CARPET) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			Cat_Avoider();
			Evade_Wall();
			YUME_Tracer_Check();

			if (Ping[PING_FRONT]<=20)
			{
				if (Ping[PING_LEFT]>=20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE LEFT \r");
					mov_rot_left(MED, FAR);
					YUME_Tracer_Check();
				}
				if (Ping[PING_LEFT]<20)
				{
					Evade_Wall();
					Sendto_PC(USART1,"YUME ROTATE RIGHT \r");
					mov_rot_right(MED, FAR);
					YUME_Tracer_Check();
				}
			}
			else
			{
				Cat_Avoider();
				Evade_Wall();
				YUME_Tracer_Check();
				PID_FollowCarpet_Left(& *CARPET,& *INV_STAT);
			}
		}
	}
}

/* func      : void PID_FollowCarpet_Left(int *CARPET, int *INV_STAT)
 * brief     : Wall following algorithm using PID controller until YUME Detecting designated carpet
 * param     : int CARPET ( COLOUR_BLACK, COLOUR_WHITE, COLOUR_BLUE, COLOUR_RED )
 *                  -> function active as long as designated carpet detected
 * 			   int INV_STAT ( NORMAL, INVERSE )
 * 			        -> switching between function active while detect certain carpet or active as long as certain carpet not detected
 * 			        i.e. NORMAL  -> while(COLOUR_STAT==CARPET)
 * 			             INVERSE -> while(COLOUR_STAT!=CARPET)
 * retval    : N/A
 * Ver       : 1
 * written By: Ryan (Tuesday, November 3rd 2015)
 * Revised BY: N/A
 * programmer's Note: THIS FUNCTION MUST BE CALLED IN THE FollowCarpet_Left(int CARPET, int INV_STAT) function
 *
 */
void PID_FollowCarpet_Left(int *CARPET, int *INV_STAT)
{
	//int limit= *COMMAND_LOOP;
	int OVERRIDE=0;

	Evade_Wall();
	YUME_Tracer_Check();

	PID_Calc_LeftRule();

	if(*INV_STAT==NORMAL)
	{
		while( (M_DETECTED_COLOUR==*CARPET) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			if(OVERRIDE==0)
			{
				Evade_Wall();
				Cat_Avoider();
				YUME_Tracer_Check();

				//KONDISI ROBOT SESUAI
				//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
				if ( (Ping[PING_ASKEW_LEFT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_LEFT]>=PID_F_R.set_point_lower) )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_LEFT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_LEFT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							Sendto_PC(USART1,"YUME FORWARD \r");
							mov_fwd_5cm(5, FAST_LEVEL_3);
							//Mov_Fwd_Trace();
						}
						Cat_Avoider();
						YUME_Tracer_Check();
					}

				//KONDISI ROBOT JAUH DARI DINDING
				else if ( Ping[PING_ASKEW_LEFT]>PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP
						Cat_Avoider();

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_LEFT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_LEFT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_left_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				//KONDISI ROBOT DEKAT DENGAN DINDING
				else if ( Ping[PING_ASKEW_LEFT]<PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_LEFT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_LEFT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_right_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				Evade_Wall();
				YUME_Tracer_Check();
				Cat_Avoider();
			}
		}
	}

	else if(*INV_STAT==INVERSE)
	{
		while( (M_DETECTED_COLOUR!=*CARPET) && (CAT_SHIFT_FOLLOW!=SHIFT_FOLLOW) )
		{
			if(OVERRIDE==0)
			{
				Evade_Wall();
				Cat_Avoider();
				YUME_Tracer_Check();

				//KONDISI ROBOT SESUAI
				//if ( Ping[PING_ASKEW_RIGHT]==PID_F_R.set_point )
				if ( (Ping[PING_ASKEW_LEFT]<=PID_F_R.set_point_upper) && (Ping[PING_ASKEW_LEFT]>=PID_F_R.set_point_lower) )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_LEFT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_LEFT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							Sendto_PC(USART1,"YUME FORWARD \r");
							mov_fwd_5cm(5, FAST_LEVEL_3);
							//Mov_Fwd_Trace();
						}
						Cat_Avoider();
						YUME_Tracer_Check();
					}

				//KONDISI ROBOT JAUH DARI DINDING
				else if ( Ping[PING_ASKEW_LEFT]>PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP
						Cat_Avoider();

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_LEFT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_LEFT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_left_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				//KONDISI ROBOT DEKAT DENGAN DINDING
				else if ( Ping[PING_ASKEW_LEFT]<PID_F_R.set_point )
					{
						Evade_Wall();
						YUME_Tracer_Check();
						if (PID_F_R.pid_value[0]>5) PID_F_R.pid_value[0]=5; //WINDUP

						if (Ping[PING_FRONT]<=20)
						{
							if (Ping[PING_LEFT]>15)
							{
								YUME_Tracer_Check();
								mov_rot_left(FAST_LEVEL_3, FAR);
								mov_rot_left(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							if (Ping[PING_LEFT]<=15)
							{
								YUME_Tracer_Check();
								mov_rot_right(FAST_LEVEL_3, FAR);
								mov_rot_right(FAST_LEVEL_3, FAR);
								OVERRIDE=1;
							}
							Cat_Avoider();
						}

						if(OVERRIDE==1){break;}

						if (OVERRIDE==0)
						{
							YUME_Tracer_Check();
							curve_right_new(FAST_LEVEL_3, FAR, &PID_F_R.pid_value[0]);
						}

						YUME_Tracer_Check();
						Cat_Avoider();
					}

				Evade_Wall();
				YUME_Tracer_Check();
				Cat_Avoider();
			}
		}
	}

}


void Escape_Warzone(void)
{
	UV_state=1;
	UV_Lock=0;
	//mov_fwd_5cm(15,FAST_LEVEL_3);
	Get_IR();

	YUME_Tracer_Check();

	if(F_DETECTED_COLOUR==COLOUR_RED)
	{
		Sendto_PC(USART1,"YUME ESCAPING RED WARZONE \r");
		while ( (Ping[PING_FRONT]>=30) && (INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED) )
		{

			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);

			mov_rot_left(MED,FAR);
			Get_IR();
		}

		Get_IR();
		while ( Ping[PING_FRONT]<=30 || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
		{
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			Get_IR();
		}
		FollowExit_Right();
	}

	else if(F_DETECTED_COLOUR==COLOUR_BLUE)
	{
		Sendto_PC(USART1,"YUME ESCAPING BLUE WARZONE \r");
		while ( (Ping[PING_FRONT]>=30) && (INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED) )
		{
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			Get_IR();
		}

		Get_IR();
		while ( Ping[PING_FRONT]<=30 || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
		{
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			Get_IR();
		}
		FollowExit_Left();
	}


}

void ReturnTo_Room(void)
{
	if(INITIAL_ZONE==ZONE_BLUE)
	{
		Sendto_PC(USART1,"YUME Returnto_Room BLUE ZONE \r");
		Get_IR();
		while ( (Ping[PING_FRONT]>=30) && (INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED) )
		{
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			Get_IR();
		}
		Get_IR();
		while ( Ping[PING_FRONT]<=30 || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
		{
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			mov_rot_left(MED,FAR);
			Get_IR();
		}
		mov_fwd_5cm(10,FAST);
	}

	else if(INITIAL_ZONE==ZONE_RED)
	{
		Sendto_PC(USART1,"YUME Returnto_Room RED ZONE \r");
		Get_IR();
		while ( (Ping[PING_FRONT]>=30) && (INFRARED[IR_FRONT_LEFT]==IR_NOT_DETECTED) && (INFRARED[IR_FRONT_RIGHT]==IR_NOT_DETECTED) )
		{
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			Get_IR();
		}

		Get_IR();
		while ( Ping[PING_FRONT]<=30 || (INFRARED[IR_FRONT_LEFT]==IR_DETECTED) || (INFRARED[IR_FRONT_RIGHT]==IR_DETECTED))
		{
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			mov_rot_right(MED,FAR);
			Get_IR();
		}
		mov_fwd_5cm(10,FAST);
	}

}

void Home_Detection_Blue(void)
{
	unsigned int HOME;

	HOME=1;
	if (HOME==1)
	{

		if (Ping[PING_FRONT]>30)
		{
			YUME_Buzz(4);
			if( Ping[PING_LEFT]>20 )
			{
				mov_rot_left(MED,FAR); //adjust to correct position
				mov_rot_left(MED,FAR); //adjust to correct position
				while(
						(Ping[PING_LEFT]>=15) && (Ping[PING_ASKEW_LEFT]>=15)
					 )
				{
					mov_rot_left(MED,FAR);
					if(Ping[PING_FRONT]<=13 )
					{
						mov_bwd(5,FAST);
					}

				}
			}

		}

		else if(Ping[PING_FRONT]<=30)
		{
			YUME_Buzz(5);

			while(
				   ( (Ping[PING_FRONT]<=30) && (Ping[PING_LEFT]>=15) ) ||
				   ( (Ping[PING_FRONT]<=30 && Ping[PING_LEFT]<=15)   )
				 )
			{
				if(Ping[PING_FRONT]<=13 )
				{
					mov_bwd(5,FAST);
				}
				mov_rot_right(MED,FAR);
			}
			while(Ping[PING_ASKEW_LEFT]<=13)
			{
				mov_rot_right(MED,FAR);
			}
		}

	}

}

