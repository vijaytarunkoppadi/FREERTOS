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
        vTaskDelay(1000/ portTICK_RATE_MS);
        if (count == 5)
        {
          vTaskSuspend(myTaskHandle2);              // when count reach 5 task2 suspends
          printf("Task2 is suspended!\n");
        }
        if (count == 8)
        {
          vTaskResume(myTaskHandle2);              // when count reach 8 task2 resume
          printf("Task2 is resumed!\n");
        }
        if (count == 10)
        {
          vTaskDelete(myTaskHandle2);               // when count reach 10 task2 delete
          printf("Task2 is deleted!\n");
        }
    }
}

void Task2(void *arg)
{
    for(int i=0;i<10;i++){
        printf("Task2 printing..\n");
        vTaskDelay(1000/ portTICK_RATE_MS);
    }
}

void app_main(void)
{
   xTaskCreate(Task1, "Task1", 4096, NULL, 10, &myTaskHandle);
   xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL,10, &myTaskHandle2, 1);
 }
