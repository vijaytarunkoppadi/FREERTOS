#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;

TaskHandle_t myTaskHandle1 = NULL;
TaskHandle_t myTaskHandle2 = NULL;

void Task1(void *arg)
{
    while(1){
        printf("Message Sent! [%d] \n", xTaskGetTickCount());
        xSemaphoreGive(xSemaphore);
        vTaskDelay(1000/ portTICK_RATE_MS);
    }
}

void Task2(void *arg)
{
    while(1){
     if(xSemaphoreTake(xSemaphore, portMAX_DELAY))
     {
      printf("Received Message [%d] \n", xTaskGetTickCount());
     }
    }
}

void app_main(void)
{
   xSemaphore = xSemaphoreCreateBinary();
   xTaskCreate(Task1, "Task1", 4096, NULL, 10, &myTaskHandle1);
   xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL,10, &myTaskHandle2, 1);
 }
