// laser.h

#ifndef _LASER_h
#define _LASER_h

#include "grbl.h"

class Claser {
private:

public:
	void init();
	void on();
	void off();
	uint8_t get_state();
	void set_power(float power);
	uint8_t getpower();
	void power_off();
	void set_divisor(float powerdivisor);
	float get_divisor();

};

extern Claser Laser;

#endif

