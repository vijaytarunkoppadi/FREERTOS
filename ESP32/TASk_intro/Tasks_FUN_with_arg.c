#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;

static const char *arg1="Task 1 is running\r\n";
static const char *arg2="Task 2 is running\r\n";


void Task1(void *arg1)
{
    char *txt=(char*)arg1;
    while(1){
        printf("%s",txt);
       // printf("Task1 printing..\n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);   //delay generation for 1 sec 
    }
}

void Task2(void *arg2)
{
  char *txt=(char*)arg2;
    while(1){
        printf("%s",txt);
        //printf("Task2 printing..\n");
        vTaskDelay(1000/ portTICK_PERIOD_MS);    //delay generation for 1 sec 
    }
}

void app_main(void)
{
  //TASKS are created here 
   xTaskCreate(Task1, "Task1", 4096, (void *)arg1, 10, &myTaskHandle);  // {task_function_name,context,stack_size,arguments_fun,priority,task_handler}
   xTaskCreatePinnedToCore(Task2, "Task2", 4096,(void*)arg2,10, &myTaskHandle2, 1);  // {task_function_name,context,stack_size,arguments_fun,priority,task_handler,coretopin}
  // After Creating the Task by default it will start scheduling  {vTaskStartSchedular();}
}
