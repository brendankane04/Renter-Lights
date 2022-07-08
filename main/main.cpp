#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "sdkconfig.h"
#include "SG90.h"
#include "SR501.h"
#include "Wifi_Interface.h"
#include "mqtt_client.h"


#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)

static const char *TAG = "main";

//GPIOs currently in use
#define BLINK_GPIO 		GPIO_NUM_33
#define	PIR_SIG_GPIO 	GPIO_NUM_23
#define	SERVO_SIG_GPIO	GPIO_NUM_14

//Application-specific defines
#define TARGET_IP "10.0.0.104"
#define TARGET_PORT 21
#define OPERATING_MODE 2
#define TEST_STR "TEST STRING. If you're seeing this, the refactor worked!\n"
#define CONFIG_BROKER_URL "mqtt://testserver.local:1883"


extern "C" { void app_main(); }


//Generic blink
void blink()
{
	gpio_set_level(BLINK_GPIO, 0);
	delay(1000);
	gpio_set_level(BLINK_GPIO, 1);
	delay(1000);
}


//<MQTT>


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
void mqtt_init()
{
	esp_mqtt_client_config_t mqtt_cfg =
	{
		.uri = CONFIG_BROKER_URL,
	};

	esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
	/* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
	esp_mqtt_client_register_event(client, MQTT_EVENT_ANY, mqtt_event_handler, NULL);
	esp_mqtt_client_start(client);
}


//</MQTT>


//Send signals to the network based on the input
//Uncalled by the user, just used by the internals
void populated_signal_handler(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data)
{
//	Wifi_Interface wifi = Wifi_Interface::get_instance("Home Network", "ThanksBrendan!");
    wifi.set_target(TARGET_IP, TARGET_PORT);

	switch(id)
	{
		case PIR_EVENT_ENTERED_ROOM:
			ESP_LOGI(TAG, "PIR_ENTERED");
			wifi.send("PIR_ENTERED\n");
		break;
		case PIR_EVENT_EXITED_ROOM:
			ESP_LOGI(TAG, "PIR_EXITED");
			wifi.send("PIR_EXITED\n");
		break;
		default:
			ESP_LOGI(TAG, "PIR_UNDEFINED");
			wifi.send("PIR_UNDEFINED");
		break;
	}
}

//Call this function to run the device as a servo
void servo_handler(void *arg)
{
    wifi.set_target(TARGET_IP, TARGET_PORT);
    SG90 servo(SERVO_SIG_GPIO);
    char buffer[9];

	while(1)
	{
		//Read in a string & do a servo command based on it
		memset(buffer, 0x00, 9);
		wifi.recv(buffer, 8);
		ESP_LOGI(TAG, "Received signal (%s)", buffer);
		if(strncmp(buffer, SERVO_ON, 8) == 0)
		{
			servo.set_on();
		}
		else if(strncmp(buffer, SERVO_OFF, 8) == 0)
		{
			servo.set_off();
		}
		else
		{
			ESP_LOGW(TAG, "Unexpected servo signal received %s", buffer);
		}
		delay(100);
	}
}

//Call this function to run the device as a PIR sensor
int sensor_handler(void *arg)
{
    wifi.set_target(TARGET_IP, TARGET_PORT);

    //Start up a new sensor instance & run its internal handler
    SR501 *sensor = new SR501(PIR_SIG_GPIO, populated_signal_handler);
    return sensor->enable(); //Return the success status of the RTOS call
}

//Call this function to just run a blink
int blink_handler(void *arg)
{
	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	while (1)
	{
		gpio_set_level(BLINK_GPIO, 0);
		delay(100);
		gpio_set_level(BLINK_GPIO, 1);
		delay(100);
//		wifi.send(TEST_STR);
	}
}

void app_main(void)
{
	//Initialize the wifi
	wifi.init("Home Network", "ThanksBrendan!");
    wifi.set_target(TARGET_IP, TARGET_PORT);
//    SG90 servo(SERVO_SIG_GPIO);

	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	switch(OPERATING_MODE)
	{
		case 0:
			servo_handler(NULL);
		break;
		case 1:
			sensor_handler(NULL);
		break;
		case 2:
			blink_handler(NULL);
		default:
		break;
	}

	while(true) {;} //Run in main loop to keep watchdog from firing
}
