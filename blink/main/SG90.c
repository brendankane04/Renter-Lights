#include "driver/gpio.h"
#include "driver/mcpwm.h"
#include "soc/mcpwm_periph.h"
#include "SG90.h"


static void mcpwm_example_gpio_initialize()
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
