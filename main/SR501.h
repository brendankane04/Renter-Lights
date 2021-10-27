#ifndef SR501_H
#define SR501_H


#include "driver/gpio.h"


//Driver for the SR501 PIR sensor
//Datasheet: https://www.epitran.it/ebayDrive/datasheet/44.pdf
class SR501
{
	private:
		//The which the peripheral is connected to
		gpio_num_t pin;
		//Whether or not the sensor thinks the room is populated
		int populated;
		//Polling delay for scanning in the detection loop in seconds
		const int polling_delay = 2;
		//handler associated with each object
		esp_event_loop_handle_t *handle;
		//handler instance associated with the object
		esp_event_handler_instance_t *instance;

		//The handler for the task which uses most of this class
		friend void poll_for_people(void*);


	public:
		//Constructor
		SR501(gpio_num_t);
		//Determine the signal on the sensor
		int get_signal();
		//starts the task which reads in the signal
		int enable();
		//ends the task which reads in the signal
		void disable();
};

#endif
