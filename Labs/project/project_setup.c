/*
 * project_setup.c
 *
 * Created: 12/11/2020 1:25:48 AM
 *  Author: Alexandr Voronin, Richard Šebo
 */ 

#include "gpio.h"
#include "project_setup.h"
#include "lcd.h"
#include "lcd_definitions.h"


//initial configuration of pins
void pins_config(void)
{
	//configure LED pins as output
	GPIO_config_output(&DDRC,LED1);
	GPIO_config_output(&DDRC,LED2);
	GPIO_config_output(&DDRC,LED3);
	GPIO_config_output(&DDRC,LED4);
	GPIO_config_output(&DDRC,LED5);
	
	//configure sound pin as output
	GPIO_config_output(&DDRB, alarm); 
	
	//configure echo pins as inputs without pull up resistor
	GPIO_config_input_nopull(&DDRD, Front_Echo);
	GPIO_config_input_nopull(&DDRD, Back_Echo);
		
	//configure trigger pins as outputs and set them to low
	GPIO_config_output(&DDRB, Front_trigger);
	GPIO_config_output(&DDRB, Back_trigger);
	GPIO_write_low(&PORTB, Front_trigger);
	GPIO_write_low(&PORTB, Back_trigger);
		
	//configure alarm as output and turn it off
	GPIO_config_output(&DDRB, alarm);
	GPIO_write_low(&PORTB, alarm);
}

void lcd_config(void)
{
	//array of custom characters representing different stages of progress on one segment of load bar
	uint8_t customChar[] = 
	{
		// addr 0: .....
		0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
		// addr 1: |....
		0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
		// addr 2: ||...
		0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
		// addr 3: |||..
		0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
		// addr 4: ||||.
		0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110,
		// addr 5: |||||
		0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
	};
		
	//initialize LCD, display strings that do not change
	lcd_init(LCD_DISP_ON);
	lcd_gotoxy(1, 0);
	lcd_puts("dist_front:");
	lcd_gotoxy(1, 2);
	lcd_puts("dist_back :");
		
	//set pointer at beginning of CGRAM
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 48; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	

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

//togggles number of leds on led stripe
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

