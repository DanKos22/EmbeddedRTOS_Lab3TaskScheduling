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

//Task handles are used to refer to the tasks for controlling or querying their status.
TaskHandle_t task1Handle = NULL, task2Handle = NULL;


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

	xTaskCreate(task1, "Task 1", 200, NULL, 3, &task1Handle);
	xTaskCreate(task2, "Task 2", 200, NULL, 1, &task2Handle);
	vTaskStartScheduler();

	while(1) {
	}
}

void task1(void * pvParameters) {
	printf("Starting task 1\r\n\n");
	while(1) {
		//Retrieve current priority of specified tasks.
		printf("Task 1 running priority %lu, Task2 running priority %lu\r\n", uxTaskPriorityGet(task1Handle), uxTaskPriorityGet(task2Handle));
		//Increment the priority of Task 2
		vTaskPrioritySet(task2Handle, uxTaskPriorityGet(task2Handle) + 1);
	}
}

void task2(void * pvParameters) {
	printf("Starting task 2\r\n\n");
	while(1) {
		printf("Task 2 Running at priority %lu\r\n", uxTaskPriorityGet(task2Handle));
		printf("Task 2 restoring to priority 1\r\n\n");
		vTaskPrioritySet(task2Handle, 1);
		//vTaskDelete(NULL);
	}

}
