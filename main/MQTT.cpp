/*
 * MQTT.cpp
 *
 *  Created on: Jul 7, 2022
 *      Author: brend
 */


#include "MQTT.h"
#include "esp_log.h"
#include "SG90.h"


#define	SERVO_SIG_GPIO	GPIO_NUM_14


static const char *TAG = "mqtt";


static SG90 servo(SERVO_SIG_GPIO);


static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}


static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
    esp_mqtt_client_handle_t client = event->client;
    int msg_id;
    switch((esp_mqtt_event_id_t)event_id)
    {
		case MQTT_EVENT_CONNECTED:
			ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
//			msg_id = esp_mqtt_client_subscribe(client, "/topic/test_topic", 0);
//			ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);
		break;
		case MQTT_EVENT_DISCONNECTED:
			ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
		break;

		case MQTT_EVENT_SUBSCRIBED:
			ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
			msg_id = esp_mqtt_client_publish(client, "/topic/test_topic", "Device Subscribed", 0, 0, 0);
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
			ESP_LOGI(TAG, "TOPIC=%.*s\r\n", event->topic_len, event->topic);
			ESP_LOGI(TAG, "DATA=%.*s\r\n", event->data_len, event->data);

			if(strncmp(event->data, SERVO_ON, event->data_len) == 0)
			{
				servo.set_on();
			}
			else if(strncmp(event->data, SERVO_OFF, event->data_len) == 0)
			{
				servo.set_off();
			}
			else
			{
				ESP_LOGW(TAG, "Unexpected servo signal received %.*s", event->data_len, event->data);
			}

		break;
		case MQTT_EVENT_ERROR:
			ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
			if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT)
			{
				log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
				log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
				log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
				ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

			}
		break;
		default:
			ESP_LOGI(TAG, "Other event id:%d", event->event_id);
		break;
    }
}


//MQTT setup
esp_mqtt_client_handle_t mqtt_init()
{
	esp_mqtt_client_config_t mqtt_cfg =
	{
		.uri = CONFIG_BROKER_URL,
	};

	esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
	/* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
	esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
	esp_mqtt_client_start(client);

	return client;
}
