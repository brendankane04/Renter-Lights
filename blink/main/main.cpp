/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_attr.h"
#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "sdkconfig.h"
#include "SG90.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO GPIO_NUM_33
#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)

extern "C"
{
	void app_main();
}

////You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
//#define SERVO_MIN_PULSEWIDTH 1000 //Minimum pulse width in microseconds
//#define SERVO_MAX_PULSEWIDTH 3000 //Maximum pulse width in microseconds
//#define SERVO_MAX_DEGREE 180 //Maximum angle in degree upto which servo can rotate
//
//static void mcpwm_example_gpio_initialize(void)
//{
//    printf("initializing mcpwm servo control gpio......\n");
//    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 15);    //Set GPIO 15 as PWM0A, to which servo is connected
//}
//
///**
// * @brief Use this function to calcute pulse width for per degree rotation
// *
// * @param  degree_of_rotation the angle in degree to which servo has to rotate
// *
// * @return
// *     - calculated pulse width
// */
//static uint32_t servo_per_degree_init(uint32_t degree_of_rotation)
//{
//    uint32_t cal_pulsewidth = 0;
//    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
//    return cal_pulsewidth;
//}
//
//static void servo_set_pos(uint32_t pos)
//{
//	uint32_t angle = servo_per_degree_init(pos);
//	mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
//}
//
//static void servo_set_on()
//{
//	servo_set_pos(180);
//}
//
//static void servo_set_off()
//{
//	servo_set_pos(0);
//}

/**
 * @brief Configure MCPWM module
 */
void mcpwm_example_servo_control(void *arg)
{
    int status = 0, pos = 0;
    //1. mcpwm gpio initialization
    SG90 servo;

    //Initialize the input GPIO
    gpio_config_t input_io;
    input_io.pin_bit_mask = 1 << GPIO_NUM_2;
    input_io.mode = GPIO_MODE_INPUT;
    input_io.pull_up_en = GPIO_PULLUP_DISABLE;
    input_io.pull_down_en = GPIO_PULLDOWN_ENABLE;
    input_io.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&input_io);

    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    while (1) {
    	status = gpio_get_level(GPIO_NUM_2);
    	if(!status)	servo.set_on();
    	else		servo.set_off();

    	if(!status)	gpio_set_level(BLINK_GPIO, 1);
    	else		gpio_set_level(BLINK_GPIO, 0);

    	printf("Setting position to (%d)", pos);

    	delay(1000);
    }
}

void app_main(void)
{
    printf("Testing servo motor.......\n");
    xTaskCreate(mcpwm_example_servo_control, "mcpwm_example_servo_control", 4096, NULL, 5, NULL);
}

