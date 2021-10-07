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

	public:
		//Constructor
		SR501(gpio_num_t);
		//Determine the signal on the sensor
		int get_signal();
		//Wait until someone is in the room
		int wait_until_populated();
		//Signals if the room is now empty
		int is_unpopulated();
};

#endif
