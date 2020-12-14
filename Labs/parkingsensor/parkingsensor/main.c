/*
 * parkingsensor.c
 *
 * Created: 03.12.2020 13:33:19
 * Author : Vori
 */ 

/* Includes ----------------------------------------------------------*/
#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <stdlib.h>         // C library. Needed for conversion function
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"           // Peter Fleury's UART library
#include "gpio.h"  
#include "lcd.h"
#include "lcd_definitions.h"   

#include "LED_setup.h"



 

#define Front_trigger PB3
#define Back_trigger  PB2
#define Front_Echo	  PD3
#define Back_Echo	  PD2

#define LED1		PC1
#define LED2		PC2
#define LED3		PC3
#define LED4		PC4
#define LED5		PC5
#define LED_BLINK	PC0
	



volatile uint8_t trigger_enable = 1;	//enables sending trigger(10us) pulse to sensor
volatile uint8_t sensor_id = 0;		//selects sensor for which the main loop executes
volatile float distances[] = {0,0};
char lcd_string[50];

//array of custom characters representing different stages of progress on one segment of load bar
uint8_t customChar[] = {
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

//defines function for load bar 
void LoadBar(int distance);


int main(void)
{
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
	

	//initialize uart
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
	
	//configure echo pins as inputs without pull up resistor
	GPIO_config_input_nopull(&DDRD, Front_Echo);
	GPIO_config_input_nopull(&DDRD, Back_Echo);
	
	//configure trigger pins as outputs and set them to low
	GPIO_config_output(&DDRB, Front_trigger);
	GPIO_write_low(&PORTB, Front_trigger);
	GPIO_config_output(&DDRB, Back_trigger);
	GPIO_write_low(&PORTB, Back_trigger);
	GPIO_write_low(&PORTB, LED_BLINK);
	
	LEDs_out();
	LEDs_off();							// set all LED pins to low
	
	//Rising edge of INT1 generates an interrupt request
	EICRA |= (1 << ISC11) | (1 << ISC10);   
	EIMSK |= (1 << INT1);
	
	
	//Rising edge of INT0 generates an interrupt request
	EICRA |= (1 << ISC01) | (1 << ISC00);
	EIMSK |= (1 << INT0);
	
	sei(); //enable interrupts
	
    while (1) 
    {
		if (trigger_enable==1)
		{
		   if (sensor_id == 1)
		   {
			    _delay_us(50);
				//send start pulse (10us) to back sensor
				GPIO_write_high(&PORTB,Back_trigger);
				_delay_us(10);
				GPIO_write_low(&PORTB,Back_trigger);
				trigger_enable = 0;						//disable sending start pulse			   
		   }
		   else
		   {
			   _delay_us(50);
				GPIO_write_high(&PORTB,Front_trigger);
				_delay_us(10);
				GPIO_write_low(&PORTB,Front_trigger);
				trigger_enable = 0;						//disable sending start pulse
		   }
		}

		int smaller_distance = 1;					//for saving the smaller distance of the 2 sensors
	
		if(distances[0] > distances[1])
		{
			smaller_distance = distances[1];	
		}
		else
		{
			smaller_distance = distances[0];	
		}
		//update load bar
		LoadBar(smaller_distance);					
			if (smaller_distance>=400)
			{
				LEDs_off();
				LED_toggle(1);
				lcd_gotoxy(24, 2);
				lcd_puts("ALL CLEAR!");					
			}

			else if (smaller_distance <= 400 && smaller_distance > 200)
			{
				LEDs_off();
				LED_toggle(2);
				lcd_gotoxy(24, 2);
				lcd_puts("ALL CLEAR!");
			}

			else if (smaller_distance <= 200 && smaller_distance > 100)
			{
				LEDs_off();
				LED_toggle(3);
				lcd_gotoxy(24, 2);
				lcd_puts("PRIHORIVA!");			
			}

			else if (smaller_distance <= 100 && smaller_distance > 50)
			{
				LEDs_off();
				LED_toggle(4);
				lcd_gotoxy(24, 2);
				lcd_puts("!!!SLOW!!!");				
			}	
			else if (smaller_distance <= 50 )
			{
				LEDs_off();
				LED_toggle(5);
				lcd_gotoxy(24, 2);
				lcd_puts("!!!STOP!!!");
			}							
		
		distances[sensor_id]=distances[sensor_id]*(0.15009);	//convert to cm
		
		itoa(distances[sensor_id], lcd_string, 10);				// Convert decimal value to string
		
		if (distances[sensor_id]<10)
		{
			lcd_gotoxy(14,sensor_id);		//set position to hundrets
			lcd_puts("  ");					//clear hundrets and tenths
			lcd_gotoxy(16, sensor_id);		//go to ones
		}
		
		else if (distances[sensor_id]>=10 && distances[sensor_id]<100)
		{
			lcd_gotoxy(14,sensor_id);		//set position to hundrets
			lcd_puts(" ");					//clear hundrets
			lcd_gotoxy(15, sensor_id);		//set position to tenths
		}
		else
		{
			lcd_gotoxy(14, sensor_id);		//set position to hundrets
		}
		lcd_puts(lcd_string);		//display distance
		
		//interesting info to uart
		if (distances[0]<100 && distances[1]<100)
		{
			uart_puts("Obstacles in front and back!\n");
		}
		else if (distances[0]<100)
		{
			uart_puts("Obstacle in front!\n");
		}
		else if (distances[1]<100)
		{
			uart_puts("Obstacle in back!\n");
		}
		else
		{
			uart_puts("No obstacle nearby!\n");
		}	
		
		//change sensor id for next loop
		if (sensor_id==0)
		{
			sensor_id=1;
		}
		else
		{
			sensor_id=0;
		}		
	}
					
    	
}

//interrupt iterates as long as echo signal from front sensor is 1
ISR(INT1_vect){
	do
	{
		distances[0]++;					//keep counting
	} while (GPIO_read(&PIND,Front_Echo));	//until echo is 0
	trigger_enable=1;						//enable trigger
}

//interrupt iterates as long as echo signal from back sensor is 1
ISR(INT0_vect){
	do
	{
		distances[1]++;					//keep counting
	} while (GPIO_read(&PIND,Back_Echo));	//until echo is 0
	trigger_enable=1;						//enable trigger
}


//displays bar on lcd based on the smaller distance
void LoadBar(int distance){
	if (distance>=500)
	{
		lcd_gotoxy(20,0);
		lcd_puts("                    ");
	}
	else if (distance<=500 && distance>490)
	{
		lcd_gotoxy(20,0);
		lcd_putc(1);		
		lcd_gotoxy(21,0);
		lcd_puts("                    ");
	}	
	else if (distance<=490 && distance>480)
	{
		lcd_gotoxy(20,0);
		lcd_putc(2);
		lcd_gotoxy(21,0);
		lcd_puts("                    ");
	}	
	else if (distance<=480 && distance>470)
	{
		lcd_gotoxy(20,0);
		lcd_putc(3);
		lcd_gotoxy(21,0);
		lcd_puts("                    ");
	}
	else if (distance<=470 && distance>460)
	{
		lcd_gotoxy(20,0);
		lcd_putc(4);
		lcd_gotoxy(21,0);
		lcd_puts("                    ");
	}	
	else if (distance<=460 && distance>450)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_puts("                    ");
	}
	else if (distance<=460 && distance>450)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(1);
		lcd_gotoxy(22,0);
		lcd_puts("                   ");
	}
	else if (distance<=450 && distance>440)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(2);
		lcd_gotoxy(22,0);
		lcd_puts("                   ");
	}	
	else if (distance<=440 && distance>430)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(3);
		lcd_gotoxy(22,0);
		lcd_puts("                   ");
	}	
	else if (distance<=430 && distance>420)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(4);
		lcd_gotoxy(22,0);
		lcd_puts("                   ");
	}	
	else if (distance<=420 && distance>410)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_puts("                   ");
	}
	else if (distance<=410 && distance>400)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(1);
		lcd_gotoxy(23,0);
		lcd_puts("                  ");
	}	
	else if (distance<=400 && distance>390)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(2);
		lcd_gotoxy(23,0);
		lcd_puts("                  ");
	}
	else if (distance<=390 && distance>=380)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(3);
		lcd_gotoxy(23,0);
		lcd_puts("                  ");
	}
	else if (distance<=380 && distance>370)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(4);
		lcd_gotoxy(23,0);
		lcd_puts("                  ");
	}
	else if (distance<=370 && distance>360)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_puts("                  ");
	}
	else if (distance<=360 && distance>350)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(1);
		lcd_gotoxy(24,0);
		lcd_puts("                 ");
	}
	else if (distance<=350 && distance>340)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(2);
		lcd_gotoxy(24,0);
		lcd_puts("                 ");
	}
	else if (distance<=340 && distance>330)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(3);
		lcd_gotoxy(24,0);
		lcd_puts("                 ");
	}
	else if (distance<=330 && distance>320)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(4);
		lcd_gotoxy(24,0);
		lcd_puts("                 ");
	}
	else if (distance<=320 && distance>310)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_puts("                 ");
	}	
	else if (distance<=310 && distance>300)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(1);		
		lcd_gotoxy(25,0);
		lcd_puts("                ");
	}
	else if (distance<=300 && distance>290)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(2);
		lcd_gotoxy(25,0);
		lcd_puts("                ");
	}
	else if (distance<=290 && distance>280)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(3);
		lcd_gotoxy(25,0);
		lcd_puts("                ");
	}
	else if (distance<=280 && distance>270)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(4);
		lcd_gotoxy(25,0);
		lcd_puts("                ");
	}
	else if (distance<=270 && distance>260)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_puts("                ");
	}
	else if (distance<=260 && distance>250)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(1);		
		lcd_gotoxy(26,0);
		lcd_puts("               ");
	}
	else if (distance<=250 && distance>240)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(2);
		lcd_gotoxy(26,0);
		lcd_puts("               ");
	}
	else if (distance<=240 && distance>230)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(3);
		lcd_gotoxy(26,0);
		lcd_puts("               ");
	}
	else if (distance<=230 && distance>220)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(4);
		lcd_gotoxy(26,0);
		lcd_puts("               ");
	}
	else if (distance<=220 && distance>210)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_puts("               ");
	}
	else if (distance<=210 && distance>200)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(1);		
		lcd_gotoxy(27,0);
		lcd_puts("              ");
	}
	else if (distance<=200 && distance>190)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(2);
		lcd_gotoxy(27,0);
		lcd_puts("              ");
	}
	else if (distance<=190 && distance>180)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(3);
		lcd_gotoxy(27,0);
		lcd_puts("              ");
	}
	else if (distance<=180 && distance>170)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(4);
		lcd_gotoxy(27,0);
		lcd_puts("              ");
	}
	else if (distance<=170 && distance>160)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_puts("              ");
	}
	else if (distance<=160 && distance>150)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(1);	
		lcd_gotoxy(28,0);
		lcd_puts("             ");
	}
	else if (distance<=150 && distance>140)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(2);
		lcd_gotoxy(28,0);
		lcd_puts("             ");
	}
	else if (distance<=140 && distance>130)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(3);
		lcd_gotoxy(28,0);
		lcd_puts("             ");
	}
	else if (distance<=130 && distance>120)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(4);
		lcd_gotoxy(28,0);
		lcd_puts("             ");
	}
	else if (distance<=120 && distance>110)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_puts("             ");
	}
	else if (distance<=110 && distance>100)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(1);		
		lcd_gotoxy(29,0);
		lcd_puts("            ");
	}
	else if (distance<=100 && distance>90)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(2);
		lcd_gotoxy(29,0);
		lcd_puts("            ");
	}	
	else if (distance<=90 && distance>80)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(3);
		lcd_gotoxy(29,0);
		lcd_puts("            ");
	}
	else if (distance<=80 && distance>70)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(4);
		lcd_gotoxy(29,0);
		lcd_puts("            ");
	}
	else if (distance<=70 && distance>60)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(5);
		lcd_gotoxy(29,0);
		lcd_puts("            ");
	}
	else if (distance<=60 && distance>50)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(5);
		lcd_gotoxy(29,0);
		lcd_putc(1);		
		lcd_gotoxy(30,0);
		lcd_puts("           ");
	}
	else if (distance<=50 && distance>40)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(5);
		lcd_gotoxy(29,0);
		lcd_putc(2);
		lcd_gotoxy(30,0);
		lcd_puts("           ");
	}
	else if (distance<=40 && distance>30)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(5);
		lcd_gotoxy(29,0);
		lcd_putc(3);
		lcd_gotoxy(30,0);
		lcd_puts("           ");
	}
	else if (distance<=30 && distance>20)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(5);
		lcd_gotoxy(29,0);
		lcd_putc(4);
		lcd_gotoxy(30,0);
		lcd_puts("           ");
	}
	else if (distance<=20)
	{
		lcd_gotoxy(20,0);
		lcd_putc(5);
		lcd_gotoxy(21,0);
		lcd_putc(5);
		lcd_gotoxy(22,0);
		lcd_putc(5);
		lcd_gotoxy(23,0);
		lcd_putc(5);
		lcd_gotoxy(24,0);
		lcd_putc(5);
		lcd_gotoxy(25,0);
		lcd_putc(5);
		lcd_gotoxy(26,0);
		lcd_putc(5);
		lcd_gotoxy(27,0);
		lcd_putc(5);
		lcd_gotoxy(28,0);
		lcd_putc(5);
		lcd_gotoxy(29,0);
		lcd_putc(5);
		lcd_gotoxy(30,0);
		lcd_puts("           ");
	}																																								
}