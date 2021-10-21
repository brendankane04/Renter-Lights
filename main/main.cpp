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

extern "C" { void app_main(); }


void scan_room(void *arg)
{
    Wifi_Interface wifi = Wifi_Interface::get_instance("Home Network", "ThanksBrendan!");
    wifi.set_target("192.168.1.155", 21);
    SR501 *pir = new SR501(GPIO_NUM_2);
	int status = 0, pos = 0;

	//Connect to the sensor
    SR501 pir(GPIO_NUM_2);


    while(1)
    {
    	status = pir.get_signal();
    	if(!status)	servo.set_on();
    	else		servo.set_off();

    	if(!status)	gpio_set_level(BLINK_GPIO, 1);
    	else		gpio_set_level(BLINK_GPIO, 0);

    	printf("Setting position to (%d)\n", pos);

    	delay(1000);
    }
}

void app_main(void)
{
    xTaskCreate(sense_room, "Scan the room for people with the sensor", 4096, NULL, 5, NULL);
}
