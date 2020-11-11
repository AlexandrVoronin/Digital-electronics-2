/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

uint8_t customChar[] = {
	// addr 0: .....
	0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,	
	// addr 1: |....
	0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
	// addr 2: |....
	0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
	// addr 3: |....
	0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
	// addr 4: |....
	0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 
	// addr 5: |....
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
};
/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

    // Set pointer to beginning of CGRAM memory
		lcd_command(1 << LCD_CGRAM);
		for (uint8_t i = 0; i < 48; i++)
		{
			// Store all new chars to memory line by line
			lcd_data(customChar[i]);
		}
		// Set DDRAM address
		lcd_command(1 << LCD_DDRAM);
    
    // Display first custom character
    lcd_putc(0);
	
    // Put string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00:00.0");
	lcd_gotoxy(11, 0);
    lcd_putc('0');
	lcd_gotoxy(1, 1);
	lcd_putc('b');
	lcd_gotoxy(11, 1);
	lcd_putc('c');

    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set prescaler and enable overflow interrupt every 16 ms
	TIM2_overflow_16384us();
	TIM2_overflow_interrupt_enable();

	TIM0_overflow_16384us();
	TIM0_overflow_interrupt_enable();	
	

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
    static uint8_t tens = 0;        // Tenths of a second
    static uint8_t secs = 0;        // Seconds
	static uint8_t minutes = 0;     // Seconds
	static uint8_t secs_square = 0;        // Seconds
    char lcd_string[2] = "  ";      // String for converting numbers by itoa()

	
    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
		tens++;

		if (tens>9)
		{
			tens=0;
			itoa(tens, lcd_string, 10);     // Convert decimal value to string
			lcd_gotoxy(7, 0);
			lcd_puts(lcd_string);
			secs++;
			itoa(pow(secs,2), lcd_string, 10);     // Convert decimal value to string
			lcd_gotoxy(11, 0);
			lcd_puts(lcd_string);
			
			if (secs > 59) //every minute
			{

				secs=0;
				itoa(secs, lcd_string, 10);     // Convert decimal value to string
				lcd_gotoxy(5, 0);
				lcd_puts(lcd_string);
				lcd_gotoxy(4, 0);
				lcd_puts(lcd_string);
				lcd_gotoxy(11, 0);
				lcd_puts(lcd_string);
				lcd_gotoxy(12, 0);
				lcd_putc(0);
				lcd_gotoxy(13, 0);
				lcd_putc(0);
				lcd_gotoxy(14, 0);
				lcd_putc(0);
				
				minutes++;
				
				if(minutes>59)								//every hour
				{
					minutes=0;
					itoa(minutes, lcd_string, 10);			// Convert decimal value to string
					lcd_gotoxy(2, 0);
					lcd_puts(lcd_string);
					
				}
				else
				{
					if (minutes<10)							//gotoxy changed based on two or one digit num
					{
						itoa(minutes, lcd_string, 10);     // Convert decimal value to string
						lcd_gotoxy(2, 0);
						lcd_puts(lcd_string);						
					}
					else
					{
						itoa(minutes, lcd_string, 10);     // Convert decimal value to string
						lcd_gotoxy(1, 0);
						lcd_puts(lcd_string);												
					}
					
				}
				
			}
			else
			{
				if (secs<10)								//gotoxy changed based on two or one digit num
				{
					itoa(secs, lcd_string, 10);				// Convert decimal value to string
					lcd_gotoxy(5, 0);
					lcd_puts(lcd_string);
				} 
				else
				{
					itoa(secs, lcd_string, 10);			// Convert decimal value to string
					lcd_gotoxy(4, 0);
					lcd_puts(lcd_string);
				}
			}

		}
		else
		{
			itoa(tens, lcd_string, 10);     // Convert decimal value to string
			lcd_gotoxy(7, 0);
			lcd_puts(lcd_string);			
		}

    }

}

ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 0;
	static uint8_t position = 0;


	
	if (symbol>5)
	{
		position++;
		symbol=1;
		if (position>11)
		{
			position = 0;
			lcd_gotoxy(1,1);
			lcd_putc(0);
			lcd_gotoxy(2,1);
			lcd_putc(0);
			lcd_gotoxy(3,1);
			lcd_putc(0);
			lcd_gotoxy(4,1);
			lcd_putc(0);
			lcd_gotoxy(5,1);
			lcd_putc(0);
			lcd_gotoxy(6,1);
			lcd_putc(0);
			lcd_gotoxy(7,1);
			lcd_putc(0);
			lcd_gotoxy(8,1);
			lcd_putc(0);
			lcd_gotoxy(9,1);
			lcd_putc(0);
			lcd_gotoxy(10,1);
			lcd_putc(0);
			lcd_gotoxy(11,1);
			lcd_putc(0);
			lcd_gotoxy(12,1);
			lcd_putc(0);

		}
		else
		{
			lcd_gotoxy(1 + position, 1);
			lcd_putc(symbol);
		}
		

	}
	else
	{
		lcd_gotoxy(1 + position, 1);
		lcd_putc(symbol);	
	}

	symbol++;

}