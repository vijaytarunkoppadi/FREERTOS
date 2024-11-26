#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;

static const char *arg1="Task 1 is running\r\n";
static const char *arg2="Task 2 is running\r\n";

static TimerHandle_t one_short_timer=NULL;
static TimerHandle_t auto_reload_timer=NULL;

void myTimerCallback(TimerHandle_t xTimer){
    if((uint32_t)pvTimerGetTimerID(xTimer)==0){
      printf("one short timer expired\r\n");
    }
    if((uint32_t)pvTimerGetTimerID(xTimer)==1){
      printf("auto reload timer expired\r\n");
    }

}

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
   xTaskCreate(Task2, "Task2", 4096,(void*)arg2,2, &myTaskHandle2);  // {task_function_name,context,stack_size,arguments_fun,priority,task_handler,coretopin}
   one_short_timer=xTimerCreate("one_short_Timer",2000/portTICK_PERIOD_MS,pdFALSE,(void*)0,myTimerCallback);
   auto_reload_timer=xTimerCreate("auto_reload_Timer",2000/portTICK_PERIOD_MS,pdTRUE,(void*)1,myTimerCallback);

   xTimerStart(one_short_timer,portMAX_DELAY);
   xTimerStart(auto_reload_timer,portMAX_DELAY);

  // After Creating the Task by default it will start scheduling  {vTaskStartSchedular();}
}
