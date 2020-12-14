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
	GPIO_config_output(&DDRC,LED_BLINK); // ??????????
	
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
	GPIO_write_low(&PORTB, LED_BLINK);
		
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