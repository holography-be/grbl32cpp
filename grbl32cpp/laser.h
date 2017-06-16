// laser.h

#ifndef _LASER_h
#define _LASER_h

#include "grbl.h"

class Laser {
private:

public:
	static void laser_init();
	static void laser_on();
	static void laser_off();
	static uint8_t laser_get_state();
	static void laser_set_power(float power);
	static uint8_t laser_getpower();
	static void laser_power_off();
	static void laser_set_divisor(float powerdivisor);
	static float laser_get_divisor();

};






#endif

