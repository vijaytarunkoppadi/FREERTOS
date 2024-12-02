#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/gpio.h"
#include "freertos/semphr.h"

SemaphoreHandle_t xSemaphore = NULL;

#define ESP_INR_FLAG_DEFAULT 0
#define LED_PIN  27
#define PUSH_BUTTON_PIN  33

TaskHandle_t ISR = NULL;

TaskHandle_t myTaskHandle1 = NULL;
TaskHandle_t myTaskHandle2 = NULL;


void IRAM_ATTR button_isr_handler(void *arg){
  xSemaphoreGiveFromISR(xSemaphore, NULL);
  xTaskResumeFromISR(ISR);
}

void interrupt_task(void *arg){              // Task will be resume 1 time when button_interrupts & goes to suspend state 
  bool led_status = false;
  while(1){
    vTaskSuspend(NULL);
    led_status = !led_status;
    gpio_set_level(LED_PIN, led_status);
    printf("led on\n");
    
  }
    }

void Task1(void *arg)
{
    while(1){
        printf("Message Sent! [%ld] \n", xTaskGetTickCount());
        xSemaphoreGive(xSemaphore);
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void Task2(void *arg)
{
    while(1){
     if(xSemaphoreTake(xSemaphore, portMAX_DELAY))
     {
      printf("Received Message [%ld] \n", xTaskGetTickCount());
     }
    }
}

void app_main(void)
{
  esp_rom_gpio_pad_select_gpio(PUSH_BUTTON_PIN);
  esp_rom_gpio_pad_select_gpio(LED_PIN);

  gpio_set_direction(PUSH_BUTTON_PIN, GPIO_MODE_INPUT);
  gpio_set_direction(LED_PIN ,GPIO_MODE_OUTPUT);

  gpio_set_intr_type(PUSH_BUTTON_PIN, GPIO_INTR_POSEDGE);

  gpio_install_isr_service(ESP_INR_FLAG_DEFAULT);

  gpio_isr_handler_add(PUSH_BUTTON_PIN, button_isr_handler, NULL);

   xSemaphore = xSemaphoreCreateBinary();
   xTaskCreate(Task1, "Task1", 4096, NULL, 10, &myTaskHandle1);
   xTaskCreatePinnedToCore(Task2, "Task2", 4096, NULL,10, &myTaskHandle2, 1);

   xTaskCreate(interrupt_task, "interrupt_task", 4096, NULL, 10, &ISR);
}
