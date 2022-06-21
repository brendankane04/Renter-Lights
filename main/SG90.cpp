#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "esp_log.h"
#include "SG90.h"


static const char *TAG = "servo";


SG90::SG90(gpio_num_t pin)
{
	//1. Initial gpio configuration
    ESP_LOGI(TAG, "initializing mcpwm servo control gpio......\n");
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, pin);    //Set GPIO 15 as PWM0A, to which servo is connected

    //2. initial mcpwm configuration
    ESP_LOGI(TAG, "Configuring Initial Parameters of mcpwm......\n");
    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    //frequency = 50Hz, i.e. for every servo motor time period should be 20ms
    pwm_config.cmpr_a = 0;    //duty cycle of PWMxA = 0
    pwm_config.cmpr_b = 0;    //duty cycle of PWMxb = 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);    //Configure PWM0A & PWM0B with above settings

}


uint32_t SG90::get_pulsewidth(uint32_t degree_of_rotation)
{
    uint32_t cal_pulsewidth = 0;
    cal_pulsewidth = SERVO_MIN_PULSEWIDTH + (((SERVO_MAX_PULSEWIDTH - SERVO_MIN_PULSEWIDTH) * (degree_of_rotation)) / (SERVO_MAX_DEGREE));
    return cal_pulsewidth;
}


void SG90::set_pos(uint32_t pos)
{
	uint32_t angle = get_pulsewidth(pos);
	ESP_LOGI(TAG,"Setting servo position (%d)", pos);
	mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, angle);
}


void SG90::set_on()
{
	set_pos(SERVO_ON_DEGREE);
}


void SG90::set_off()
{
	set_pos(SERVO_OFF_DEGREE);
}
