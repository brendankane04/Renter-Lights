#ifndef SG90_H
#define SG90_H

//You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
#define SERVO_MIN_PULSEWIDTH 1000 //Minimum pulse width in microseconds
#define SERVO_MAX_PULSEWIDTH 3000 //Maximum pulse width in microseconds
#define SERVO_MAX_DEGREE 180 //Maximum angle in degree upto which servo can rotate

static void mcpwm_example_gpio_intialize();
static uint32_t servo_degree_init(uint32_t);
static void servo_set_pos(uint32_t pos);
static void servo_set_on();
static void servo_set_on();

#endif
