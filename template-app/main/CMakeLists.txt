idf_component_register(SRCS "MQTT.c" "Scale.c" "Wifi_gas.c" "WIFI/Wifi_AP.c" "WebServer/WebServer.c"  "main.c" 
                    INCLUDE_DIRS "."
                    )

spiffs_create_partition_image(data ../Partition FLASH_IN_PROJECT)