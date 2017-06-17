// laser.h

#ifndef _LASER_h
#define _LASER_h

#include "grbl.h"

class laser {
private:

public:
	static void init();
	static void on();
	static void off();
	static uint8_t get_state();
	static void set_power(float power);
	static uint8_t getpower();
	static void power_off();
	static void set_divisor(float powerdivisor);
	static float get_divisor();

};

#endif

