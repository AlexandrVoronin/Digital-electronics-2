/*
 * main.c
 *
 * Created: 03.12.2020 13:33:19
 * Author : Alexandr Voronin, Richard Šebo
 */ 

/* Includes ----------------------------------------------------------*/
#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>			// AVR device-specific IO definitions
#include <avr/interrupt.h>		// Interrupts standard C library for AVR-GCC
#include <stdlib.h>			// C library. Needed for conversion function
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"			//Peter Fleury's UART library
#include "gpio.h"			//gpio library for AVR_GCC
#include "lcd.h"			//library for functions for lcd operations
#include "lcd_definitions.h"		//lcd pin definitions
#include "project_setup.h"		//pins definition and library for declaration of led functions
#include "project_functions.h"		//library for functions for displaying outputs
#include "timer.h"			//library for timer

volatile uint8_t sensor_id = 0;		//selects sensor for which the main loop executes
volatile float distances[] = {0,0};	//distance[0]=distance to front sensor distance[1]=distance to back sensor
char lcd_string[50];			//for displaying data on lcd

int main(void)
{
	lcd_config();	//initial configuration of LCD display
	
	uart_init(UART_BAUD_SELECT(9600,F_CPU));	//initialize UART		
	
	pins_config();		//initial configuration of pins
						
	LEDs_off();		//turn LEDs off				
	
	//enable overflow interrupt and set default value
	TIM2_overflow_16ms();
	TIM2_overflow_interrupt_enable();
	
	//Rising edge of INT1 generates an interrupt request
	EICRA |= (1 << ISC11) | (1 << ISC10);   
	EIMSK |= (1 << INT1);
	
	//Rising edge of INT0 generates an interrupt request
	EICRA |= (1 << ISC01) | (1 << ISC00);
	EIMSK |= (1 << INT0);
	
	sei(); //enable interrupts
	
    	while (1) 
    	{	
		if (sensor_id == 1)
		{
			_delay_ms(60);				//ensure one cycle lasts at least 60 ms
			GPIO_write_high(&PORTB,Back_trigger);	//
			_delay_us(10);				//send start pulse (10us) to back sensor
			GPIO_write_low(&PORTB,Back_trigger);	//		   
		}
		else
		{
			_delay_ms(60);				//ensure one cycle lasts at least 60 ms			
			GPIO_write_high(&PORTB,Front_trigger);	//
			_delay_us(10);				//send start pulse (10us) to front sensor
			GPIO_write_low(&PORTB,Front_trigger);	//
		}

		int smaller_distance = 1;			//for saving the smaller distance of the 2 sensors
		
		//choose smaller distance
		if(distances[0] > distances[1])
		{
			smaller_distance = distances[1];	
		}
		else
		{
			smaller_distance = distances[0];	
		}
		
		
		Load_bar(smaller_distance);				//update load bar based on smaller distance
		
		GPIO_write_low(&PORTB, alarm);				//set frequency signaling diode to low
						
		Update_warning(smaller_distance);			//update warning message on lcd and led stripe based on smaller distance	
	
		distances[sensor_id]=distances[sensor_id]*(0.15085);	//convert to cm
		
		itoa(distances[sensor_id], lcd_string, 10);		//convert decimal value to string
		
		Display_dist(sensor_id,distances,lcd_string);		//display distance on lcd
		
		Uart_info(distances);					//interesting info to uart
		
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
ISR(INT1_vect)
{
	do
	{
		distances[0]++;			//keep counting
	} while (GPIO_read(&PIND,Front_Echo));	//until echo is 0
}

//interrupt iterates as long as echo signal from back sensor is 1
ISR(INT0_vect)
{
	do
	{
		distances[1]++;			//keep counting
	} while (GPIO_read(&PIND,Back_Echo));	//until echo is 0
}

ISR(TIMER2_OVF_vect)
{
	int freq = 50;  //for saving closer distance
	
	//choose smaller distance
	if(distances[0] >= distances[1])
	{
		freq = distances[1];
	}
	else
	{
		freq = distances[0];
	}
	
	//flick the led if in range for flicking
	if (freq<=100)
	{
		GPIO_toggle(&PORTB, alarm);
	}	
	
	//select frequency of signal led based on smaller distance
	if (freq <= 100 && freq > 75)
	{
		TIM2_overflow_16ms();		//f = 30.5 Hz	
	}
	else if (freq <= 75 && freq > 50)
	{
		TIM2_overflow_4ms();		//f = 125 Hz
	}
	else if (freq <= 50 && freq > 25)
	{
		TIM2_overflow_2ms();		//f = 250 Hz
	}
	else if (freq <= 25 && freq > 10)
	{
		TIM2_overflow_1ms();		//f = 500 Hz
	}
	else if (freq <= 10)
	{
		TIM2_overflow_512us();		//f = 1 kHz
	}

}
