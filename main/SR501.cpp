#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "SR501.h"


//Modified delay functions
#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)
#define delay_sec(sec) for(int i = 0; i < sec; i++) {delay(1000);}
#define delay_min(min) for(int i = 0; i < min; i++) {delay_sec(60);}
#define delay_hr(hour) for(int i = 0; i < hour; i++) {delay_min(60);}


SR501::SR501(gpio_num_t pin)
{
	this->pin = pin;

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

int SR501::wait_until_populated()
{
	int populated = 0;


	return populated;
}
