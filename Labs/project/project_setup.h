/*
 * project_setup.h
 *
 * Created: 12/11/2020 1:40:54 AM
 *  Author: Alexandr Voronin, Richard Šebo
 */ 


#ifndef PROJECT_SETUP_H_
#define PROJECT_SETUP_H_

//define pins
#define LED1		PC1
#define LED2		PC2
#define LED3		PC3
#define LED4		PC4
#define LED5		PC5
#define alarm		PB4  

#define Front_trigger PB3
#define Back_trigger  PB2
#define Front_Echo	  PD3
#define Back_Echo	  PD2

//function declarations
void LEDs_off(void);					//Turn off all LEDs
void pins_config(void);					//Initial pins config
void LED_toggle(int number_of_LEDs);	//LEDs bar
void lcd_config(void);					//Initial LCD config
void Load_bar(int distance);			//loading bar function


#endif /* PROJECT_SETUP_H_ */