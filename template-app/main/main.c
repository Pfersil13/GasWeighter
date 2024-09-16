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
#include "MQTT.h"
#include "Wifi_gas.h"

/** GLOBALS **/

// task tag
static const char *TAG = "WIFI";

#define HX711_ON_PIN  21

#define ZERO_LOAD 16740795.0
#define Kilo 16679106.0

scale gas;

#define ESP_INTR_FLAG_DEFAULT 0



void  Print(void *pvParamters){
    while(1){

     if(gas.flag == 1){
       printf("%f\n" ,scale_get_weight_Kg(&gas));
    }

        vTaskDelay(10);
    }
}




void app_main(void)
{
  

    scale_init(HX711_ON_PIN, &gas);

    
    xTaskCreate(&Print, "Print", 2048, NULL, 1, NULL);
    /*WIFI*/
   esp_err_t status = WIFI_FAILURE;

	//initialize storage
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // connect to wireless AP
	status = connect_wifi();
	if (WIFI_SUCCESS != status)
	{
		ESP_LOGI(TAG, "Failed to associate to AP, dying...");
		return;
	}
	
	
    /* TEST MQTT*/
   ESP_LOGI(TAG, "[APP] Startup..");
    ESP_LOGI(TAG, "[APP] Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("mqtt_client", ESP_LOG_VERBOSE);
    esp_log_level_set("mqtt_example", ESP_LOG_VERBOSE);
    esp_log_level_set("transport_base", ESP_LOG_VERBOSE);
    esp_log_level_set("esp-tls", ESP_LOG_VERBOSE);
    esp_log_level_set("transport", ESP_LOG_VERBOSE);
    esp_log_level_set("outbox", ESP_LOG_VERBOSE);

   mqtt_app_start();

    
    while (1)
    {
        vTaskDelay(1000);
        printf("MQTT Blocking\n");

    }
    
}