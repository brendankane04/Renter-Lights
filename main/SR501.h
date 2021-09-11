#ifndef SR501_H
#define SR501_H

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
		//Signals if the room is now populated
		//Signals if the room is now empty
};

#endif
