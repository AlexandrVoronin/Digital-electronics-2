/*
 * project_functions.h
 *
 * Created: 14.12.2020 9:58:46
 *  Author: Vori
 */ 


#ifndef PROJECT_FUNCTIONS_H_
#define PROJECT_FUNCTIONS_H_

void Display_dist(uint8_t id,volatile float dist[], char string[]);
void Update_warning(int sm_dist);
void Load_bar(int distance);
void Uart_info(volatile float dist[]);


#endif /* PROJECT_FUNCTIONS_H_ */