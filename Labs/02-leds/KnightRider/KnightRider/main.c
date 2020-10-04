/*
 * KnightRider.c
 *
 * Created: 30.09.2020 12:34:06
 * Author : Vori
 */ 



/* Defines -----------------------------------------------------------*/
#define LED_5   PB5     
#define LED_4   PB4  
#define LED_3   PB3 
#define LED_2   PB2  
#define LED_1   PB1  
#define PUSH_BUTTON PD0 
#define BLINK_DELAY 200
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle two LEDs 
 * when a push button is pressed.
 */
int main(void)
{
    /* LEDS */
	
    	// Set pin as output in Data Direction Register...
    	DDRB = DDRB | (1<<LED_1);
    	// ...and turn LED off in Data Register
    	PORTB = PORTB & ~(1<<LED_1);
    	// Set pin as output in Data Direction Register...
    	DDRB = DDRB | (1<<LED_2);
    	// ...and turn LED off in Data Register
    	PORTB = PORTB & ~(1<<LED_2);	
    	// Set pin as output in Data Direction Register...
    	DDRB = DDRB | (1<<LED_3);
    	// ...and turn LED off in Data Register
    	PORTB = PORTB & ~(1<<LED_3);	
    	// Set pin as output in Data Direction Register...
	DDRB = DDRB | (1<<LED_4);
    	// ...and turn LED off in Data Register
    	PORTB = PORTB & ~(1<<LED_4);
	// Set pin as output in Data Direction Register...
    	DDRB = DDRB | (1<<LED_5);
    	// ...and turn LED off in Data Register
   	PORTB = PORTB & ~(1<<LED_5);	
	//set pin as input in Data Direction Register
	DDRD = DDRD & ~(1<<PUSH_BUTTON);
	//set pull up resistor on
	PORTD = PORTD | (1<<PUSH_BUTTON);
    // Infinite loop
    while (1)
    {
			if(bit_is_clear(PIND,PUSH_BUTTON))
			{
								// Pause several milliseconds
			    
				PORTB = PORTB ^ (1<<LED_1);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_1);
				PORTB = PORTB ^ (1<<LED_2);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_2);
				PORTB = PORTB ^ (1<<LED_3);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_3);
				PORTB = PORTB ^ (1<<LED_4);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_4);
				PORTB = PORTB ^ (1<<LED_5);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_4);
				PORTB = PORTB ^ (1<<LED_5);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_3);
				PORTB = PORTB ^ (1<<LED_4);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_2);
				PORTB = PORTB ^ (1<<LED_3);
				_delay_ms(BLINK_DELAY);
				PORTB = PORTB ^ (1<<LED_2);
				
			}
    }
    // Will never reach this
    return 0;
}


