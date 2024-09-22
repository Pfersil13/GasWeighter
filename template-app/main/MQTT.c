/* MQTT (over TCP) Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "MQTT.h"
#include "WIFI/Secrets.h"

#define TOPIC_TEMP "homeassistant/sensor/gasTemp/config"
#define PAYLOAD_TEMP "{\n   \"name\":\"Gas Temperature\",\n   \"unique_id\":\"gasTempMeasure\",\n   \"state_topic\":\"state/temperature\",\n   \"unit_of_measurement\":\"°C\",\n  \n   \"device\": {\n     \"name\":\"GasWeighter\",\n     \"identifiers\":\"GasWeighter_MAC\",\n     \"manufacturer\":\"Zeeta\",\n     \"model\":\"ESP32s3\",\n     \"hw_version\":\"1.0\",\n     \"sw_version\":\"1\"\n  }\n}"

#define TOPIC_GAS "homeassistant/sensor/gasKg/config"
#define PAYLOAD_GAS "{\n   \"name\":\"Gas Weight\",\n   \"unique_id\":\"gasMassMeasure\",\n   \"state_topic\":\"state/mass\",\n   \"unit_of_measurement\":\"kg\",\n  \"device\": {\n     \"name\":\"GasWeighter\",\n     \"identifiers\":\"GasWeighter_MAC\"\n  }\n}"

#define TOPIC_GAS_DATA "state/mass"
/*
{
   "name":"Gas Temperature",
   "unique_id":"gasTempMeasure",
   "state_topic":"stat/mydevice/temperature",
   "unit_of_measurement":"°C",
  
   "device": {
     "name":"GasWeighter",
     "identifiers":"GasWeighter_MAC",
     "manufacturer":"Zeeta",
     "model":"ESP32s3",
     "hw_version":"1.0",
     "sw_version":"1"
  }
}

{
   "name":"Gas Weight",
   "unique_id":"gasMassMeasure",
   "state_topic":"state/mass",
   "unit_of_measurement":"kg",
  "device": {
     "name":"GasWeighter",
     "identifiers":"GasWeighter_MAC"
  }
}
*/

static const char *TAG = "mqtt_example";
//esp_mqtt_client_config_t mqtt_cfg;
 esp_mqtt_client_config_t  mqtt_cfg = {
        .broker.address.uri = URL,
        .task.priority = 10,
    };
esp_mqtt_client_handle_t client;

//esp_mqtt_client_handle_t client;

static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%" PRIi32 "", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    /*
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
        ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
        ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;

    }*/
}

void mqtt_app_start(void)
{
   
    client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    //esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(client);
    //esp_mqtt_client_publish(client,"A","B",0,1,0);
    esp_mqtt_client_publish(client,TOPIC_TEMP,PAYLOAD_TEMP,0,1,0);
    esp_mqtt_client_publish(client,TOPIC_GAS,PAYLOAD_GAS,0,1,0);
}


void mqtt_publish(double data){
 
    //esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    char arr[sizeof(data)];
    snprintf(arr,sizeof(data), "%.3lf", data); 
    esp_mqtt_client_publish(client,TOPIC_GAS_DATA ,arr,0,1,0);
}
