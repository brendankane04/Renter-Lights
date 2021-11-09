#ifndef SG90_H
#define SG90_H


#include "driver/gpio.h"


//Driver for the SG90 Servo motor
//Datasheet: http://www.ee.ic.ac.uk/pcheung/teaching/DE1_EE/stores/sg90_datasheet.pdf
class SG90
{
	private:
		//Servo pulse         width constants
		static const int SERVO_MIN_PULSEWIDTH = 1000; //Minimum pulse width in microseconds
		static const int SERVO_MAX_PULSEWIDTH = 3000; //Maximum pulse width in microseconds
		static const int SERVO_MAX_DEGREE = 170; //Maximum angle in degree upto which servo can rotate

		//Lever control constants
		static const int SERVO_ON_DEGREE = 180;
		static const int SERVO_OFF_DEGREE = 0;


	public:
		//Constructor
		SG90(gpio_num_t);
		//Get a pulse width from a specified angle
		uint32_t get_pulsewidth(uint32_t);
		//Set the servo to a certain angle
		void set_pos(uint32_t);
		//Set the servo connected to a lever to an "on" position
		void set_on();
		//Set the servo connected to a lever to an "off" position
		void set_off();
};

#endif
