/*==============================================================================================
								 	 PROJECT YUME Ver.2.0
								  	 YUME UTILITIES LIBRARY
==============================================================================================*/

#include "yume_utilities.h"

void YUME_Initial_Setup(int YUME_MODE)
{
	delay_ms(50);
	YUME_Buzz(YUME_MODE);

	lcd_display_clear();
	lcd_gotoxy(0,0);sprintf(lcd,"YUME Mode %d", YUME_MODE);lcd_putstr(lcd);delay_ms(50);
}
