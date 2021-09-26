#include <stdio.h>
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


extern "C" { void app_main(); }


void servo_control(void *arg)
{
    int status = 0, pos = 0;

    //Servo controlling the lights
    SG90 servo(GPIO_NUM_15);
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
    Wifi_Interface wifi1;

    wifi1.send("TEST\n");

    printf("Testing servo motor.......\n");
//    xTaskCreate(servo_control, "mcpwm_example_servo_control", 4096, NULL, 5, NULL);
}
