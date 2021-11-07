#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "SR501.h"
#include "Wifi_Interface.h"


//Modified delay functions
#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)
#define delay_sec(sec) for(int i = 0; i < sec; i++) {delay(1000);}
#define delay_min(min) for(int i = 0; i < min; i++) {delay_sec(60);}
#define delay_hr(hour) for(int i = 0; i < hour; i++) {delay_min(60);}

static const char *TAG = "SR501";


SR501::SR501(gpio_num_t pin, external_handler_t external_handler)
{
	//Initialize the simple members
	this->pin = pin;
	this->populated = 0;
	this->handle = NULL;
	this->external_handler = external_handler;

	esp_event_loop_args_t loop_args =
	{
		.queue_size = 4,
		.task_name = "Sensor to wifi event loop",
		.task_priority = 5,
		.task_stack_size = 4096,
		.task_core_id = 0
	};

    //Initialize the input GPIO
    gpio_config_t input_io;
    input_io.pin_bit_mask = 1 << this->pin; //Set these settings on the specified pin
    input_io.mode = GPIO_MODE_INPUT;
    input_io.pull_up_en = GPIO_PULLUP_DISABLE;
    input_io.pull_down_en = GPIO_PULLDOWN_ENABLE;
    input_io.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&input_io);
}


int SR501::get_signal()
{
	return gpio_get_level(pin);
}


void poll_for_people(void *arg)
{
	int status = 0;
	int minutes_off = 0;
	int sec_off = 0;

	//Get a version of the object which is running this task
	SR501 *task_this = (SR501*) arg;

	while(1)
	{
		//Get the current level on the PIR
		status = task_this->get_signal();

		//Debugging section
		ESP_LOGI(TAG, "Getting signal...");
//		if(!status) wifi.send("SR501: Getting signal: LOW \n");
//		else		wifi.send("SR501: Getting signal: HIGH\n");

		if(status)
		{//If it's high, set the status high & start the counter
			if(!task_this->populated)
			{//If transitioning from unpopulated to populated, send a signal
				ESP_LOGI(TAG, "ENTERED");
				task_this->external_handler(NULL, PIR_EVENT, PIR_EVENT_ENTERED_ROOM, NULL);
			}
			task_this->populated = true;
			minutes_off = 0;
			sec_off = 0;
			delay(100);
		}
		else
		{//If it's low, wait a minute & decrement the counter
			delay_sec(task_this->polling_delay);
			sec_off += task_this->polling_delay;
			if(sec_off >= 60)
			{
				sec_off = 0;
				minutes_off++;
			}
		}

		//If the counter reaches the maximum time, set the status to off
		if(minutes_off >= 30)
		{
			if(task_this->populated)
			{//If transitioning to from populated to unpopulated, send a signal
				ESP_LOGI(TAG, "EXITED");
				task_this->external_handler(NULL, PIR_EVENT, PIR_EVENT_EXITED_ROOM, NULL);
			}
			task_this->populated = 0;
			minutes_off = 0;
		}
	}
}


int SR501::enable()
{
	BaseType_t status;

    status = xTaskCreate(poll_for_people, "Poll sensor for people & keep variable updated", 4096, (void*) this, 5, &this->handle);
    if(status == pdPASS)
    	return 0;
    else
    	return -1;
}


void SR501::disable()
{
	//Only call the task if it won't delete the current task
	if(this->handle != NULL)
	{
		//Delete the the task
		vTaskDelete(this->handle);
	}
}
