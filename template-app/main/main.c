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

#include "WIFI/Wifi_AP.h"
#include "WebServer/Webserver.h"

/** GLOBALS **/
int k;
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
        //mqtt_publish();
        gpio_intr_enable(DOUT_PIN);

    }
    
        vTaskDelay(100);
    }
}




void app_main(void)
{
 
   /* Initialize file storage */
    const char* base_path = "/data";
    ESP_ERROR_CHECK(example_mount_storage(base_path));


    scale_init(HX711_ON_PIN, &gas);

    
    xTaskCreate(&Print, "Print", 2*2048, NULL, 1, NULL);
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

    //Create custom AP
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
   
	}

   
    
    /* Start the file server */
    //ESP_ERROR_CHECK(example_start_file_server(base_path));
    ESP_LOGI(TAG, "File server started");
    

	start_webserver();
   



    /* TEST MQTT*/
    /*
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

    */
    while (1)
    {
        vTaskDelay(100);
        //printf("MQTT Blocking\n");
        //printf("%d\n", k);
    }
    
}