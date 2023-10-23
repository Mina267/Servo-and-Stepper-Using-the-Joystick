#include "StdTypes.h"
#include "ADC_interface.h"
#include "Joystick.h"
#define _TAN_ARRAYS
#include "Joystick_Private.h"
# define F_CPU 8000000
#include <util/delay.h>

#include "LCD_interface.h"

static s16 X_axis;
static s16 Y_axis;
static s16 X_axis_Prev;
static s16 Y_axis_Prev;
static u16 Vx;
static u16 Vy;
static u16 Ceta;
static u16 Angle;
static u32 TanInv;
static u8 Index ;


void Joystick_Init(void)
{
	/* Do nothing */
}




void Joystick_Runnable(void)
{
	/* Take read from x-axis channel */
	Vx = ADC_ReadVolt(VX_CHANNEL);
	/* Take read from y-axis channel */
	Vy = ADC_ReadVolt(VY_CHANNEL);
	/* Adjust read of voltage to origin */
	X_axis = Vx - 2495 - X_AXIS_CORRECTION;
	Y_axis = Vy - 2495 - Y_AXIS_CORRECTION;
	

	
	bool_t AngleFlag = TRUE;
	
	
	/* Dead zone for false reads for x-axis*/
	if	(X_axis > -DEAD_ZONE_OFFSET && X_axis < DEAD_ZONE_OFFSET)
	{
		X_axis = 0;
		if ((Y_axis > -DEAD_ZONE_OFFSET && Y_axis < DEAD_ZONE_OFFSET))
		{
			// Do Nothing
			AngleFlag = FALSE;
			
		}
	}
	/* Dead zone for false reads for y-axis*/
	else if ((Y_axis > -DEAD_ZONE_OFFSET && Y_axis < DEAD_ZONE_OFFSET))
	{
		Y_axis = 0;
	}
	
	
	/* Check if new read not equal to previous to avoid false read */
	if ((X_axis != X_axis_Prev) && (Y_axis != Y_axis_Prev))
	{
		AngleFlag = FALSE;
	}
	
	/* If all check ok get angle */
	if (AngleFlag)
	{
		Calc_Angle();
	}
	
	Y_axis_Prev = Y_axis;
	X_axis_Prev = X_axis;
	_delay_ms(DELAY_ANGLE);
	
}



/* Calculation of angle */
void Calc_Angle(void)
{
	
	/* check if x-axis equal zero and then option is in y-axis */
	if (X_axis == 0)
	{
		if (Y_axis > 0)
		{
			Angle = 900;
			Ceta = 0;
		}
		else if (Y_axis < 0)
		{
			Angle = 2700;
			Ceta = 0;
		}
	}
	/* else calculate angle */
	else
	{
		/* By calculate tan inverse of angle tan-1(y/x) */
		/* Multiplied by amplification factor to avoid use float number */
		TanInv = abs_num(((s32)Y_axis * AMP_FACTOR) / (s32)X_axis);
		/* Search in look up table for value near to that to get required angle */
		Search_ceta(TanInv);
		
		/* determine where i am in x-axis and y-axis quadrant */
		if (X_axis > 0)
		{
			if (Y_axis >= 0)
			{
				Angle = Ceta;
				
			}
			else if (Y_axis < 0)
			{
				Angle = 3600 - Ceta;
			}
		}
		
		else if (X_axis < 0)
		{
			if (Y_axis > 0)
			{
				Angle = 1800 - Ceta;
				
			}
			else if (Y_axis < 0)
			{
				Angle = 2700 - Ceta;
			}
			else if (Y_axis == 0)
			{
				Angle = 1800;
			}
			
		}
	}	
}

/* Get angle */
u16 Joystick_GetAngle(void)
{
	return Angle;
}

u32 abs_num(s32 num)
{
	if (num < 0)
	{
		num = num * (-1);
	}
	return num;
}




void Search_ceta(u32 num)
{
	u16 cmp[3];
	u8 smallest;
	for (u8 i = 0; i < 180; i++)
	{
		/* If found value directly */
		if (TanArray[i] == num)
		{
			Ceta = AngleArray[i];
			break;
		}
		/* search until found number greater than angle */
		else if ((TanArray[i] > num) && i != 0)
		{
			/* Take three value 1- Found  2- prevFound, 3- average between them
			 * and decided which is closest between them to angle needed*/ 
			cmp[0] = TanArray[i] - num;
			cmp[1] = num - TanArray[i - 1];
			cmp[2] = abs_num(AVR2NUM(TanArray[i], TanArray[i - 1]) - num);
			smallest = SmallestOfThree(cmp[0], cmp[1], cmp[2]);
			Index = smallest;
			if (smallest == 2)
			{
				Ceta = AVR2NUM(AngleArray[i] , AngleArray[i - 1]);
			}
			else if (smallest == 1)
			{
				Ceta = AngleArray[i];
			}
			else
			{
				Ceta = AngleArray[i];
			}
			break;
			
		}
	}
}




u8 SmallestOfThree(u16 n1, u16 n2, u16 n3)
{
	if (n1 <= n2)
	{
		if(n1 <= n3)
		{
			return 0;
		}
	}
	else if (n2 <= n1)
	{
		if(n2 <= n3)
		{
			return 1;
		}
	}
	
	return 2;
	
}



/* use for debugging */
void Joystick_Display(void)
{
	LCD_ClearCell(1,3,3);
	LCD_ClearCell(1,10,3);
	
	
	LCD_SetCursor(1,0);
	LCD_WriteNumber(X_axis);
	
	LCD_SetCursor(1,7);
	LCD_WriteNumber(Y_axis);
	
	LCD_SetCursor(0,6);
	LCD_WriteNumber_4D(Index);
	LCD_SetCursor(0,11);
	LCD_WriteNumber_4D(Ceta);
	

	LCD_SetCursor(0,0);
	LCD_WriteNumber_4D(Angle);
}
