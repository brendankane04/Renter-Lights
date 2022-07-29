#include <stdio.h>
#include <string.h>
#include <wifi_interface.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "sdkconfig.h"
#include "SG90.h"
#include "SR501.h"
#include "MQTT.h"
#include "pins.h"


#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)

static const char *TAG = "main";


//Application-specific defines
typedef enum {SERVO_MODE, SENSOR_MODE, BLINK_MODE} operating_mode_t;
#define TARGET_IP "10.0.0.104"
#define TARGET_PORT 21
#define TEST_STR "TEST STRING. If you're seeing this, the refactor worked!\n"


extern "C" { void app_main(); }

esp_mqtt_client_handle_t static client;
operating_mode_t operating_mode = BLINK_MODE;

//Generic blink
void blink()
{
	gpio_set_level(BLINK_GPIO, 0);
	delay(1000);
	gpio_set_level(BLINK_GPIO, 1);
	delay(1000);
}


//Send signals to the network based on the input
//Uncalled by the user, just used by the internals
void populated_signal_handler(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data)
{
    int msg_id;
    wifi.set_target(TARGET_IP, TARGET_PORT);

	switch(id)
	{
		case PIR_EVENT_ENTERED_ROOM:
			ESP_LOGI(TAG, "PIR_ENTERED");
			wifi.send("PIR_ENTERED\n");
			msg_id = esp_mqtt_client_publish(client, "/topic/test_topic", "SERVO_ON", 0, 1, 0);
			ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
		break;
		case PIR_EVENT_EXITED_ROOM:
			ESP_LOGI(TAG, "PIR_EXITED");
			wifi.send("PIR_EXITED\n");
			msg_id = esp_mqtt_client_publish(client, "/topic/test_topic", "SERVO_OFF", 0, 1, 0);
			ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
		break;
		default:
			ESP_LOGI(TAG, "PIR_UNDEFINED");
			wifi.send("PIR_UNDEFINED");
			msg_id = esp_mqtt_client_publish(client, "/topic/test_topic", "UNDEFINED", 0, 1, 0);
			ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
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
		delay(1000);
		gpio_set_level(BLINK_GPIO, 1);
		delay(1000);
	}
}

void app_main(void)
{
	//Initialize the wifi
	wifi.init("Home Network", "ThanksBrendan!");
    wifi.set_target(TARGET_IP, TARGET_PORT);
    SG90 servo(SERVO_SIG_GPIO);

	gpio_pad_select_gpio(BLINK_GPIO);
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	client = mqtt_init();

	esp_light_sleep_start();

//	switch(operating_mode)
//	{
//		case SERVO_MODE:
//			servo_handler(NULL);
//		break;
//		case SENSOR_MODE:
//			sensor_handler(NULL);
//		break;
//		case BLINK_MODE:
//			blink_handler(NULL);
//		default:
//		break;
//	}
}
