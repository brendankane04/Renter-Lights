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


#define BLINK_GPIO GPIO_NUM_33
#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)

static const char *TAG = "main";


extern "C" { void app_main(); }


static TaskHandle_t *servo_handler = new TaskHandle_t;


//Send signals to the network based on the input
void populated_signal_handler(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data)
{
	Wifi_Interface wifi = Wifi_Interface::get_instance("Home Network", "ThanksBrendan!");
    wifi.set_target("192.168.1.155", 21);

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

void servo_listen(void *arg)
{
	ESP_LOGI(TAG, "beginning task...");
	Wifi_Interface wifi = Wifi_Interface::get_instance("Home Network", "ThanksBrendan!");
    wifi.set_target("192.168.1.155", 21);
    SG90 *servo = new SG90(GPIO_NUM_2);
    char buffer[8];

	while(1)
	{
		ESP_LOGI(TAG, "Getting buffer data");
		wifi.recv(buffer, 8);
		ESP_LOGI(TAG, "Buffer received: %s", buffer);

		if(strcmp(buffer, "SERVO_ON") == 0)
		{
			ESP_LOGI(TAG, "SERVO_ON");
			servo->set_on();
		}
		else if(strcmp(buffer, "SERVO_NO") == 0)
		{
			ESP_LOGI(TAG, "SERVO_NO");
			servo->set_off();
		}
		else
		{
			ESP_LOGW(TAG, "Unexpected servo signal received");
		}

	}
}

void app_main(void)
{
	xTaskCreate(servo_listen, "Servo receiver task", 4096, NULL, 4, servo_handler);
}
