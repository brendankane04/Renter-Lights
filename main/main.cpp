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

static const char *TAG = "MAIN";
extern esp_event_base_t PIR_EVENT = "PIR_EVENT";

enum
{
	PIR_EVENT_ENTERED_ROOM,
	PIR_EVENT_EXITED_ROOM
};

extern "C" { void app_main(); }


//Send signals to the network based on the input
void signal_handler(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data)
{
    // Event handler logic
}


void app_main(void)
{
//	SR501 *pir = new SR501(GPIO_NUM_2);
	esp_event_loop_handle_t *loop_handle = new esp_event_loop_handle_t;


	esp_event_loop_args_t loop_args =
	{
		.queue_size = 4,
		.task_name = "Sensor to wifi event loop",
		.task_priority = 5,
		.task_stack_size = 2048,
		.task_core_id = NULL
	};

	ESP_ERROR_CHECK(esp_event_loop_create(&loop_args, loop_handle));
	ESP_ERROR_CHECK(esp_event_handler_register_with(loop_handle, PIR_EVENT, ESP_EVENT_ANY_ID, signal_handler, NULL));

//	pir->enable();
}
