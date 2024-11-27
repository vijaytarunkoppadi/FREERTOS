#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"

typedef enum{
esender1,
esender2
}DataSource_t;

typedef struct{
uint8_t ucValue;
DataSource_t eDataSource;
}Data_t;

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle1 = NULL;
TaskHandle_t myTaskHandle2 = NULL;
QueueHandle_t queue;

static const Data_t xStructsToSend[2]={{100,esender1},{200,esender2}};

void Task(void *arg)
{
    while(1){
      xQueueSend(queue, arg, (TickType_t)100);
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}

void Task2(void *arg)
{
    //char rxBuffer[50];
    Data_t xReceivedStructure;
    BaseType_t xStatus;
    while(1){
     xStatus=xQueueReceive(queue, &xReceivedStructure, (TickType_t)5);
     if(xStatus == pdPASS){
      if(xReceivedStructure.eDataSource == esender1){
        printf("Receiverd: ");
        printf("from sender 1= %d\n",xReceivedStructure.ucValue);
      }
      else {
        printf("Received :  ");
        printf("from sender 2= %d\n",xReceivedStructure.ucValue);
      }
     }
    }
}

void app_main(void)
{
   queue = xQueueCreate(5, sizeof(Data_t)); 
    if (queue == 0)
    {
     printf("Failed to create queue= %p\n", queue);
    }
   xTaskCreate(Task, "Task", 4096, &(xStructsToSend[1]), 10, &myTaskHandle);
   xTaskCreate(Task, "Task1", 4096, &(xStructsToSend[0]), 10, &myTaskHandle1);
   xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL,10, &myTaskHandle2, 1);
 }
