/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

/* Can use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO GPIO_NUM_33
#define delay(cnt) vTaskDelay(cnt / portTICK_PERIOD_MS)

char sbt0811_in[4] =
{
		GPIO_NUM_2,
		GPIO_NUM_14,
		GPIO_NUM_15,
		GPIO_NUM_13
};   // IN1, IN2, IN3, IN4 to arduino pins

int full_steps_count	= 4095;    // steps for a full cycle
const int step_time		= 1;       // 1ms for one step

int direction    = 1;           // will be either 1 (clockwise) or -1 (anti-clockwise)
int current_step = 0;           // 0 to 7

bool step_positions[8][4] =
{
    { 1,0,0,1 },
    { 1,0,0,0 },
    { 1,1,0,0 },
    { 0,1,0,0 },
    { 0,1,1,0 },
    { 0,0,1,0 },
    { 0,0,1,1 },
    { 0,0,0,1 },
};

void move_stepper(int step_count)
{
    for (int i=0;i<step_count;i++)
    {
        // step one step in given direction
        current_step += direction;
        if (current_step > 7) { current_step = 0; }
        if (current_step < 0) { current_step = 7; }

        // set digital pins for the current_step
        for (int i = 0; i <= 3; i++)
        {
            gpio_set_level(sbt0811_in[i], step_positions[current_step][i]);
        }

        delay(step_time);
    }
}

void app_main(void)
{
	for (int i = 0; i <= 3; i++)
	{
	        gpio_reset_pin(sbt0811_in[i]);
	        gpio_set_direction(sbt0811_in[i], GPIO_MODE_OUTPUT);
	}

	int state = 1;
	gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

	int steps_left   = full_steps_count;

	printf("Setup Completed.\n\n\n");

    while(1)
    {

    	while (steps_left > 0)
    	{
    	         move_stepper(direction);
    	         steps_left -= 1;
    	}
    	steps_left = full_steps_count;
    	direction  = (direction > 0 ? -1 : 1);

    	delay(500);

    	gpio_set_level(BLINK_GPIO, state);
    	state = !state;

        printf("Stepping Process Completed\n");
    }
}

