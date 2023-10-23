#include "StdTypes.h"
#include "MemMap.h"
#include "Utils.h"

#include "JoystickApp.h"





int main(void)
{
	
	DIO_Init();
	LCD_Init();

	sei();
	

	JoystickAPP_Init();
	while (1)
	{
		JoystickAPP_StepperRunnable();
		
	}
	return 0;
}
























