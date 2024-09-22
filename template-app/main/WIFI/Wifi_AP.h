
#ifndef  Wifi_AP_H
    #define Wifi_AP_H

    #include <string.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "esp_mac.h"
    #include "esp_wifi.h"
    #include "esp_event.h"
    #include "esp_log.h"
    #include "nvs_flash.h"

    #include "lwip/err.h"
    #include "lwip/sys.h"

    /* The examples use WiFi configuration that you can set via project configuration menu.

    If you'd rather not, just change the below entries to strings with
    the config you want - ie #define EXAMPLE_WIFI_SSID "mywifissid"
    */
    #define EXAMPLE_ESP_WIFI_SSID      "GasWeighter_WiFi"
    #define EXAMPLE_ESP_WIFI_PASS      ""
    #define EXAMPLE_ESP_WIFI_CHANNEL   5
    #define EXAMPLE_MAX_STA_CONN       1

    void wifi_init_softap(void);

#endif


