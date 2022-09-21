#ifndef SR501_H
#define SR501_H


#include "driver/gpio.h"
#include "esp_event.h"


using namespace std;


typedef void (*external_handler_t)(void*, esp_event_base_t, int32_t, void*);

//Defines for the event loop
static esp_event_base_t PIR_EVENT = "PIR_EVENT";
//ESP_EVENT_DEFINE_BASE(PIR_EVENT);

typedef enum
{
	PIR_EVENT_ENTERED_ROOM,
	PIR_EVENT_EXITED_ROOM
};


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
		TaskHandle_t handle;
		//Function run by the event loop
		external_handler_t external_handler;

		//The handler for the task which uses most of this class
		friend void poll_for_people(void*);


	public:
		//Constructor
		SR501(gpio_num_t, external_handler_t);
		//Determine the signal on the sensor
		int get_signal();
		//starts the task which reads in the signal
		int enable();
		//ends the task which reads in the signal
		void disable();
};


#endif
