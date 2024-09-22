// Input to HTML Server for ESP32

#include "WebServer/Webserver.h"
char resp[2000];
static const char *TAG = "ESP32 Server";

static void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        printf("WiFi connecting ... \n");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        printf("WiFi connected ... \n");
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        printf("WiFi lost connection ... \n");
        break;
    case IP_EVENT_STA_GOT_IP:
        printf("WiFi got IP ... \n\n");
        break;
    default:
        break;
    }
}

esp_err_t get_handler(httpd_req_t *req)
{
   FILE* f = fopen("/data/Config.html", "r");
        
        fread(resp, 1,sizeof(resp),f);
        fclose(f);
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t get_handler_str(httpd_req_t *req)
{
    // Read the URI line and get the host
    char *buf;
    size_t buf_len;
    buf_len = httpd_req_get_hdr_value_len(req, "Host") + 1;
    if (buf_len > 1)
    {
        buf = malloc(buf_len);
        if (httpd_req_get_hdr_value_str(req, "Host", buf, buf_len) == ESP_OK)
        {
            ESP_LOGI(TAG, "Host: %s", buf);
        }
        free(buf);
    }

    // Read the URI line and get the parameters
     buf_len = httpd_req_get_url_query_len(req) + 1;
    if (buf_len > 1) {
        buf = malloc(buf_len);
        if (httpd_req_get_url_query_str(req, buf, buf_len) == ESP_OK) {
            ESP_LOGI(TAG, "Found URL query: %s", buf);
            char param[32];
            char WiFI_SSID[32];
            char WiFI_PASS[32];
            if (httpd_query_key_value(buf, "WiFISSID", param, sizeof(param)) == ESP_OK) {

                FILE* f = fopen("/data/WifiSecrets.txt" , "r");
                fgets(WiFI_SSID,sizeof(WiFI_SSID),f);
                fgets(WiFI_PASS,sizeof(WiFI_PASS),f);
                fclose(f);

                f = fopen("/data/WifiSecrets.txt" , "w");
                fputs(param,f);
                fputc('\0',f);
                fputc('\n',f);
                fputs(WiFI_PASS,f);
                fclose(f);
            }
            if (httpd_query_key_value(buf, "WiFIPASS", param, sizeof(param)) == ESP_OK) {

                FILE* f = fopen("/data/WifiSecrets.txt" , "r");
                fgets(WiFI_SSID,sizeof(WiFI_SSID),f);
                fgets(WiFI_PASS,sizeof(WiFI_PASS),f);
                fclose(f);

                f = fopen("/data/WifiSecrets.txt" , "w");
                fputs(WiFI_SSID,f);
                fputc('\0',f);
                fputc('\n',f);
                fputs(param,f);
                fputc('\0',f);
                fputc('\n',f);
                fclose(f);
            }
    
        FILE* f = fopen("/data/WifiSecrets.txt" , "r");
                fgets(WiFI_SSID,sizeof(WiFI_SSID),f);
                fgets(WiFI_PASS,sizeof(WiFI_PASS),f);
                fclose(f);

                printf("%s\n", WiFI_SSID);
                printf("%s\n", WiFI_PASS);
        }
        free(buf);

        
    }

    // The response
    //const char resp[] = "The data was sent ...";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

/* URI handler structure for GET /uri */
httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = get_handler,
    .user_ctx = NULL};

httpd_uri_t uri_get_input = {
    .uri = "/get",
    .method = HTTP_GET,
    .handler = get_handler_str,
    .user_ctx = NULL};

httpd_handle_t start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_get_input);
    }
    return server;
}

void stop_webserver(httpd_handle_t server)
{
    if (server)
    {
        httpd_stop(server);
    }
}



/* Function to initialize SPIFFS */
esp_err_t example_mount_storage(const char* base_path)
{
    ESP_LOGI(TAG, "Initializing SPIFFS");

    esp_vfs_spiffs_conf_t conf = {
        .base_path = base_path,
        .partition_label = NULL,
        .max_files = 5,   // This sets the maximum number of files that can be open at the same time
        .format_if_mount_failed = true
    };

    esp_err_t ret = esp_vfs_spiffs_register(&conf);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return ret;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
        return ret;
    }

    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    return ESP_OK;
}