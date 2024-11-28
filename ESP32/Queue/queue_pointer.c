#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#define QUEUE_SIZE 5  // Max number of items the queue can hold

// Define a structure that we will allocate dynamically
typedef struct {
    int id;
    char message[50];
} DataPacket;

// Create a queue to hold pointers to DataPacket
QueueHandle_t dataQueue;

// Producer Task - pushes data onto the queue
void producer_task(void *pvParameter) {
    DataPacket *data;
    for (int i = 0; i < 10; i++) {
        // Dynamically allocate memory for DataPacket
        data = malloc(sizeof(DataPacket));
        if (data == NULL) {
            printf("Memory allocation failed!\n");
            continue;
        }

        // Fill the data structure with some values
        data->id = i;
        snprintf(data->message, sizeof(data->message), "Message #%d", i);

        // Push the pointer to the queue
        if (xQueueSend(dataQueue, &data, portMAX_DELAY) != pdPASS) {
            printf("Failed to send data to queue\n");
            free(data);  // Free the memory if failed to send
        } else {
            printf("Produced: %s\n", data->message);
        }

        vTaskDelay(pdMS_TO_TICKS(100));  // Simulate work
    }

    vTaskDelete(NULL);  // Terminate the producer task
}

// Consumer Task - pops data from the queue
void consumer_task(void *pvParameter) {
    DataPacket *data;
    for (int i = 0; i < 10; i++) {
        // Wait for and receive data from the queue
        if (xQueueReceive(dataQueue, &data, portMAX_DELAY) == pdPASS) {
            // Process the data (e.g., print it)
            printf("Consumed: %s\n", data->message);

            // Free the dynamically allocated memory after processing
            free(data);
        } else {
            printf("Failed to receive data from queue\n");
        }
    }
     

    vTaskDelete(NULL);  // Terminate the consumer task
    vTaskDelay(pdMS_TO_TICKS(500)); 
}

void app_main(void) {
    // Create the queue to hold pointers to DataPacket
    dataQueue = xQueueCreate(QUEUE_SIZE, sizeof(DataPacket *));
    if (dataQueue == NULL) {
        printf("Queue creation failed!\n");
        return;
    }

    // Create producer and consumer tasks
    xTaskCreate(producer_task, "Producer", 2048, NULL, 1, NULL);
    xTaskCreate(consumer_task, "Consumer", 2048, NULL, 1, NULL);
}
