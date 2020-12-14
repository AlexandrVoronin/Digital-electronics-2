#include "gpio.h"
#include "LED_setup.h"


//set LEDs as output
void LEDs_out(void)
{
	GPIO_config_output(&DDRC,LED1);
	GPIO_config_output(&DDRC,LED2);
	GPIO_config_output(&DDRC,LED3);
	GPIO_config_output(&DDRC,LED4);
	GPIO_config_output(&DDRC,LED5);
	GPIO_config_output(&DDRC,LED_BLINK);
}

//turn all LEDs off
void LEDs_off(void)
{
	GPIO_write_low(&PORTC, LED1);
	GPIO_write_low(&PORTC, LED2);
	GPIO_write_low(&PORTC, LED3);
	GPIO_write_low(&PORTC, LED4);
	GPIO_write_low(&PORTC, LED5);
}


void LED_toggle(int number_of_LEDs)
{
	switch(number_of_LEDs)
	{
		case 1:
			GPIO_toggle(&PORTC, LED1);
			break;
		case 2:
			GPIO_toggle(&PORTC, LED1);
			GPIO_toggle(&PORTC, LED2);
			break;
		case 3:
			GPIO_toggle(&PORTC, LED1);
			GPIO_toggle(&PORTC, LED2);
			GPIO_toggle(&PORTC, LED3);
			break;
		case 4:
			GPIO_toggle(&PORTC, LED1);
			GPIO_toggle(&PORTC, LED2);
			GPIO_toggle(&PORTC, LED3);
			GPIO_toggle(&PORTC, LED4);
			break;
		case 5:
			GPIO_toggle(&PORTC, LED1);
			GPIO_toggle(&PORTC, LED2);
			GPIO_toggle(&PORTC, LED3);
			GPIO_toggle(&PORTC, LED4);
			GPIO_toggle(&PORTC, LED5);
			break;
	}
	
}
