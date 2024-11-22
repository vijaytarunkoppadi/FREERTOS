#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;

void Task1(void *arg)
{
    int count = 0;
    while(1){
        count++;
        printf("Task1 printing..\n");
        vTaskDelay(1000/  portTICK_PERIOD_MS);
        if (count == 5)                               // after 5 counts vTaskDelete function delete the Task2
        {
          vTaskDelete(myTaskHandle2);
          printf("Task2 is deleted!\n");
        }
    }
}

void Demo_Task2(void *arg)               // 5 times task2 printing happen at initial state 
{
    for(int i=0;i<5;i++){
        printf("Task2 printing..\n");
        vTaskDelay(1000/  portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
   xTaskCreate(Task, "Task1", 4096, NULL, 10, &myTaskHandle);
   xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL,10, &myTaskHandle2, 1);
 }
