#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"


static TaskHandle_t myTaskHandle1 = NULL;
static TaskHandle_t myTaskHandle2 = NULL;
static TaskHandle_t myTaskHandle3 = NULL;

static QueueHandle_t queue1=NULL,queue2=NULL;
static QueueSetHandle_t queue_set=NULL;


void TX_Task1(void *arg)
{
const TickType_t xBlockTime = pdMS_TO_TICKS( 100 );
const char * const pcMessage = "Message from TX_Task1\r\n";
  
    while(1){
      
      xQueueSend(queue1, &pcMessage,0);
      printf("tx1\n");
      vTaskDelay( xBlockTime );
    }
}

void TX_Task2(void *arg)
{
    const TickType_t xBlockTime = pdMS_TO_TICKS( 100 );
    const char * const pcMessage = "Message from TX_Task2\r\n"; 
    while(1){
      xQueueSend(queue2, &pcMessage,0);
      printf("tx2\n");
      vTaskDelay( xBlockTime );
    }
}

void receive_task(void *arg)
{
    QueueHandle_t xQueueThatContainsData;
    char *pcReceivedString;

    BaseType_t xStatus;
    while(1){ 
      xQueueThatContainsData = ( QueueHandle_t ) xQueueSelectFromSet( queue_set,portMAX_DELAY );
       xStatus=xQueueReceive( xQueueThatContainsData, &pcReceivedString, 0 );
     if(xStatus == pdPASS){
        printf("%s",pcReceivedString);     
     }
    }
}

void app_main(void)
{
   queue1 = xQueueCreate(1, sizeof(char *)); 
   queue2 = xQueueCreate(1, sizeof(char *));

   queue_set = xQueueCreateSet(1*2);

   xQueueAddToSet(queue1,queue_set);
   xQueueAddToSet(queue2,queue_set);

    if (queue1 == 0 && queue2 == 0)
    {
     printf("Failed to create queues\n");
    }
   xTaskCreate(TX_Task1, "Task1", 4096, NULL, 10, &myTaskHandle1);
   xTaskCreate(TX_Task2, "Task2", 4096, NULL, 10, &myTaskHandle2);
   xTaskCreate(receive_task, "receive_task", 4096, NULL,11, &myTaskHandle3);
 }
