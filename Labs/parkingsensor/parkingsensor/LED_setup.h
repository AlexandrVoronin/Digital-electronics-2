/*
 * LED_setup.h
 *
 * Created: 12/11/2020 1:40:54 AM
 *  Author: Richard
 */ 


#ifndef LED_SETUP_H_
#define LED_SETUP_H_

void LEDs_off(void); //Turn off all LEDs
void LEDs_out(void); // Set up LED pins as output
void LED_toggle(int number_of_LEDs); // LEDs bar

#define LED_BLINK	PC0
#define LED1		PC1
#define LED2		PC2
#define LED3		PC3
#define LED4		PC4
#define LED5		PC5


#endif /* LED_SETUP_H_ */