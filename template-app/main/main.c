#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_mac.h"
#include "Scale.h"

#define HX711_ON_PIN  21

#define ZERO_LOAD 16740795.0
#define Kilo 16679106.0

scale gas;

#define ESP_INTR_FLAG_DEFAULT 0


void  Print(void *pvParamters){
    while(1){

     if(gas.flag == 1){
       // printf("Decimal: %lu, Hex: %lx\n", byte, byte);
       int32_t NetWeight_ADC = (gas.byte-ZERO_LOAD) ;
       double Weight_Kg = NetWeight_ADC*(1/(Kilo-ZERO_LOAD));
       printf("%f\n" ,Weight_Kg);

        gas.byte =0;
        gas.flag = 0;
    }

        vTaskDelay(10);
    }
}


void app_main(void)
{
  

    scale_init(HX711_ON_PIN, &gas);

    
    xTaskCreate(&Print, "Print", 2048, NULL, 1, NULL);

    while (1)
    {
        vTaskDelay(1000);

    }
    
}