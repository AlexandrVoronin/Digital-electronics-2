/*
 * project_functions.c
 *
 * Created: 14.12.2020 9:59:42
 *  Author: Alexandr Voronin, Richard Šebo
 */ 

#include "gpio.h"				//gpio library for AVR_GCC
#include "project_setup.h"		//pins definition and library for led functions
#include "lcd.h"				//library of functions for lcd operations
#include "project_functions.h"	//library of functions for displaying outputs
#include "uart.h"				//Peter Fleury's UART library

//display distances to sensor[id]
void Display_dist(uint8_t id,volatile float dist[], char string[])
{
	if (dist[id]<10)
	{
		lcd_gotoxy(14,id);		//set position to hundrets
		lcd_puts("  ");			//clear hundrets and tenths
		lcd_gotoxy(16, id);		//go to ones
	}
	
	else if (dist[id]>=10 && dist[id]<100)
	{
		lcd_gotoxy(14,id);		//set position to hundrets
		lcd_puts(" ");			//clear hundrets
		lcd_gotoxy(15, id);		//set position to tenths
	}
	else
	{
		lcd_gotoxy(14, id);		//set position to hundrets
	}
	lcd_puts(string);			//display distance on lcd
}

//update warning message based on smaller distance
void Update_warning(int sm_dist)
{	
	LEDs_off();

	if (sm_dist>=400)
	{
		LED_toggle(5);
		lcd_gotoxy(22, 2);
		lcd_puts("OUTSIDE OF RANGE!");
	}

	else if (sm_dist <= 400 && sm_dist > 200)
	{
		LED_toggle(4);
		lcd_gotoxy(22, 2);
		lcd_puts("    ALL CLEAR!    ");
	}
	else if (sm_dist <= 200 && sm_dist > 100)
	{
		LED_toggle(3);
		lcd_gotoxy(22, 2);
		lcd_puts("    PRIHORIVA!   ");
	}

	else if (sm_dist <= 100 && sm_dist > 50)
	{
		LED_toggle(2);
		lcd_gotoxy(22, 2);
		lcd_puts("    !!SLOW!!     ");
	}
	else if (sm_dist <= 50 && sm_dist>10 )
	{
		LED_toggle(1);
		lcd_gotoxy(22, 2);
		lcd_puts("   !!STOP!!      ");
	}
	else if (sm_dist <= 10 && sm_dist > 2 )
	{
		lcd_gotoxy(22, 2); 
		lcd_puts("   !!STOP!!      ");
	}
	else if (sm_dist <= 2)
	{
		lcd_gotoxy(22, 2);
		lcd_puts("     <2cm        ");	
	}
}

//display info about obstacles in front/back/both (range 1m)
void Uart_info(volatile float dist[])
{
	if (dist[0]<100 && dist[1]<100)
	{
		uart_puts("Obstacles in front and back!\n");
	}
	else if (dist[0]<100)
	{
		uart_puts("Obstacle in front!\n");
	}
	else if (dist[1]<100)
	{
		uart_puts("Obstacle in back!\n");
	}
	else
	{
		uart_puts("No obstacle nearby!\n");
	}
}

//displays bar on lcd based on the smaller distance
void Load_bar(int distance)
{
	if (distance>480)
	{
		lcd_gotoxy(20,0);
		lcd_puts("                     ");
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
