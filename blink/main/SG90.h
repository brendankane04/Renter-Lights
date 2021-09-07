#ifndef SG90_H
#define SG90_H

class SG90
{
	private:
		//You can get these value from the datasheet of servo you use, in general pulse width varies between 1000 to 2000 mocrosecond
		static const int SERVO_MIN_PULSEWIDTH = 1000; //Minimum pulse width in microseconds
		static const int SERVO_MAX_PULSEWIDTH = 3000; //Maximum pulse width in microseconds
		static const int SERVO_MAX_DEGREE = 180; //Maximum angle in degree upto which servo can rotate

	public:
		SG90();
		uint32_t degree_init(uint32_t);
		void set_pos(uint32_t pos);
		void set_on();
		void set_off();
};

#endif
