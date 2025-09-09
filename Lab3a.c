/*
 * userApp.c
 *
 *  Created on: Dec 8, 2023
 *      Author: Niall.OKeeffe@atu.ie
 */

#include "userApp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

//--------------------------------------------------------------
//used for real time stats, do not delete code from this section
extern TIM_HandleTypeDef htim7;
extern volatile unsigned long ulHighFrequencyTimerTicks;
void configureTimerForRunTimeStats(void)
{
    ulHighFrequencyTimerTicks = 0;
    HAL_TIM_Base_Start_IT(&htim7);
}
unsigned long getRunTimeCounterValue(void)
{
	return ulHighFrequencyTimerTicks;
}
//end of real time stats code
//----------------------------------------------------------------

extern UART_HandleTypeDef huart1;


static void task1(void * pvParameters);
static void task2(void * pvParameters);

// _write function used for printf
int _write(int file, char *ptr, int len) {
	HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, HAL_MAX_DELAY);
	return len;
}


void userApp() {
	printf("Starting application\r\n\n");

	//HAL_UART_Receive_IT(&huart1, &ch, 1);
	//HAL_TIM_Base_Start_IT(&htim6);

	xTaskCreate(task1, "Task 1", 200, NULL, 2, NULL);
	//xTaskCreate(task2, "Task 2", 200, NULL, 2, NULL);
	vTaskStartScheduler();

	while(1) {
	}
}

void task1(void * pvParameters) {
	printf("Starting task 1\r\n\n");
	while(1) {
		printf("Task 1 Running...\r\n");
		printf("Creating Task 2...\r\n");
		xTaskCreate(task2, "Task 2", 200, NULL, 1, NULL);
		printf("Task 1 suspended...\r\n");
		vTaskSuspend(NULL);
		//vTaskDelay(pdMS_TO_TICKS(500));
	}
}

void task2(void * pvParameters) {
	printf("Starting task 2\r\n\n");
	while(1) {
		printf("Task 2 Running...\r\n");
		vTaskDelete(NULL);
		//vTaskDelay(pdMS_TO_TICKS(500));
	}
	printf("Deleting task 2...\r\n");
}
