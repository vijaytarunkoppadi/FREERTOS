#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;

void Task1(void *arg)
{
    while(1){
        printf("Task1 printing..\n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);   //delay generation for 1 sec 
    }
}

void Task2(void *arg)
{
    while(1){
        printf("Task2 printing..\n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);    //delay generation for 1 sec 
    }
}

void app_main(void)
{
  //TASKS are created here 
   xTaskCreate(Task1, "Task1", 4096, NULL, 10, &myTaskHandle);  // {task_function_name,context,stack_size,arguments_fun,priority,task_handler}
   xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL,10, &myTaskHandle2, 1);  // {task_function_name,context,stack_size,arguments_fun,priority,task_handler,coretopin}
  // After Creating the Task by default it will start scheduling  {vTaskStartSchedular();}
}
