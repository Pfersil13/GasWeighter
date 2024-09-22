
#ifndef  WEBSERVER_H
    #define WEBSERVER_H

    
    #include <stdio.h>
    #include <sys/param.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "freertos/timers.h"
    #include "freertos/event_groups.h"
    #include "esp_wifi.h"
    #include "esp_log.h"
    #include "nvs_flash.h"
    #include "esp_netif.h"
    #include "esp_http_server.h"

    httpd_handle_t start_webserver(void);
    void stop_webserver(httpd_handle_t server);


    #include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_err.h"
#include "nvs_flash.h"
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_err.h"
#include "esp_vfs_fat.h"
#include "esp_spiffs.h"
#include "sdkconfig.h"
#include "soc/soc_caps.h"
#include "driver/sdspi_host.h"
#include "driver/spi_common.h"
#if SOC_SDMMC_HOST_SUPPORTED
#include "driver/sdmmc_host.h"
#endif
#include "sdmmc_cmd.h"


esp_err_t example_mount_storage(const char* base_path);



#endif