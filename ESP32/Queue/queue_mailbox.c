#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "freertos/queue.h"

TaskHandle_t myTaskHandle = NULL;
TaskHandle_t myTaskHandle2 = NULL;
QueueHandle_t mailbox;

void Task(void *arg)
{
   int messageToSend = 0;

    while (1)
    {
        messageToSend++;
        if (xQueueOverwrite(mailbox, &messageToSend) == pdPASS)
        {
            printf("Sender: Message overwritten: %d\n", messageToSend);
        }
       
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void Task2(void *arg)
{
   int peekedMessage;
    
    while (1)
    {
        if (xQueuePeek(mailbox, &peekedMessage, 0) == pdPASS)
        {
            printf("Receiver: Peeking message: %d\n", peekedMessage);
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }

}

void app_main(void)
{
  mailbox = xQueueCreate(1, sizeof(int)); 
   xTaskCreate(Task, "Task", 4096, NULL, 10, &myTaskHandle);
   xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL,10, &myTaskHandle2, 1);
 }
