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

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO GPIO_NUM_33
#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)

//You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
#define SERVO_MIN_PULSEWIDTH 1000 //Minimum pulse width in microseconds
#define SERVO_MAX_PULSEWIDTH 3000 //Maximum pulse width in microseconds
#define SERVO_MAX_DEGREE 180 //Maximum angle in degree upto which servo can rotate

static void mcpwm_example_gpio_initialize(void)
{
    printf("initializing mcpwm servo control gpio......\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, 15);    //Set GPIO 15 as PWM0A, to which servo is connected
}

/**
 * @brief Use this function to calcute pulse width for per degree rotation
 *
 * @param  degree_of_rotation the angle in degree to which servo has to rotate
 *
 * @return
 *     - calculated pulse width
 */
static uint32_t servo_per_degree_init(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = (SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE)));
    return cal_pulsewidth;
}

static void servo_set_pos(uint32_t pos)
{
	uint32_t angle = servo_per_degree_init(pos);
	mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
}

static void servo_set_on()
{
	servo_set_pos(180);
}

static void servo_set_off()
{
	servo_set_pos(0);
}

/**
 * @brief Configure MCPWM module
 */
void mcpwm_example_servo_control(void *arg)
{
    uint32_t status = 0, pos = 0;
    //1. mcpwm gpio initialization
    mcpwm_example_gpio_initialize();

    //Initialize the input GPIO
    gpio_set_direction(GPIO_NUM_14, GPIO_MODE_INPUT);

    //2. initial mcpwm configuration
    printf("Configuring Initial Parameters of mcpwm......\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings
    while (1) {
//        for (count = 0; count < SERVO_MAX_DEGREE; count++) {
//            printf("Angle of rotation: %d\n", count);
//            servo_set_pos(count);
//            vTaskDelay(10);     //Add delay, since it takes time for servo to rotate, generally 100ms/60degree rotation at 5V
//        }

    	status = gpio_get_level(GPIO_NUM_14);
    	if(status)	servo_set_on();
    	else		servo_set_off();
    	printf("Setting position to (%d)", pos);

    	delay(1000);
    }
}

void app_main(void)
{
    printf("Testing servo motor.......\n");
    xTaskCreate(mcpwm_example_servo_control, "mcpwm_example_servo_control", 4096, NULL, 5, NULL);
}

