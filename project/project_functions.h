/*
 * project_functions.h
 *
 * Created: 14.12.2020 9:58:46
 *  Author: Alexandr Voronin, Richard Šebo
 */ 


#ifndef PROJECT_FUNCTIONS_H_
#define PROJECT_FUNCTIONS_H_

void Display_dist(uint8_t id,volatile float dist[], char string[]);	//displays distance on lcd based on id
void Update_warning(int sm_dist);									//updates warning message on lcd
void Load_bar(int distance);										//updates load bar based on smaller distance
void Uart_info(volatile float dist[]);								//displays info about closeness of obstacles on uart


#endif /* PROJECT_FUNCTIONS_H_ */