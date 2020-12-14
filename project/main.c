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
#include "project_setup.h"
#include "project_functions.h"
#include "timer.h"


volatile uint8_t trigger_enable = 1;	//enables sending trigger(10us) pulse to sensor
volatile uint8_t sensor_id = 0;			//selects sensor for which the main loop executes
volatile float distances[] = {0,0};
char lcd_string[50];



int main(void)
{
	//initial configuration of LCD display
	lcd_config();

	//initialize UART
	uart_init(UART_BAUD_SELECT(9600,F_CPU));
		
	//initial configuration of pins
	pins_config();	
						
	//turn LEDs off
	LEDs_off();						
	
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
		//set frequency signaling diode to loew
		GPIO_write_low(&PORTB, alarm);
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
		
		//update load bar based on smaller distance
		Load_bar(smaller_distance);	
		
		//update warning message based on smaller distance				
		Update_warning(smaller_distance);					
	
		distances[sensor_id]=distances[sensor_id]*(0.15009);	//convert to cm
		
		itoa(distances[sensor_id], lcd_string, 10);				// Convert decimal value to string
		
		Display_dist(sensor_id,distances,lcd_string);			//display distance on lcd
		
		Uart_info(distances);									//interesting info to uart
		
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
		distances[0]++;						//keep counting
	} while (GPIO_read(&PIND,Front_Echo));	//until echo is 0
	trigger_enable=1;						//enable trigger
}

//interrupt iterates as long as echo signal from back sensor is 1
ISR(INT0_vect){
	do
	{
		distances[1]++;						//keep counting
	} while (GPIO_read(&PIND,Back_Echo));	//until echo is 0
	trigger_enable=1;						//enable trigger
}


ISR(TIMER2_OVF_vect)
{
	int freq = 50;  //for saving closer distance
	
	//choose the smaller distance
	if(distances[0] > distances[1])
	{
		freq = distances[1];
	}
	else
	{
		freq = distances[0];
	}
	
	//select frequency of signal led based on smaller distance
	if (freq <= 50 && freq > 40)
	{
		TIM2_overflow_16ms();				
		GPIO_toggle(&PORTB, alarm);								
	}
	else if (freq <= 40 && freq > 30)
	{
		TIM2_overflow_4ms();					
		GPIO_toggle(&PORTB, alarm);				
	}
	else if (freq <= 30 && freq > 20)
	{
		TIM2_overflow_2ms();					
		GPIO_toggle(&PORTB, alarm);				
	}
	else if (freq <= 20 && freq > 10)
	{
		TIM2_overflow_1ms();					
		GPIO_toggle(&PORTB, alarm);				
	}
	else if (freq <= 10)
	{
		TIM2_overflow_512us();					
		GPIO_toggle(&PORTB, alarm);				
	}
}
