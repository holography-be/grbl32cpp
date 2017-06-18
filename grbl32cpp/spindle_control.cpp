#include "spindle_control.h"

#ifndef _spindle_control

void Cspindle::init()
{
	Laser.init();
}


void Cspindle::stop()
{
	Laser.power_off();
}


void Cspindle::set_state(uint8_t state, float rpm)
{
	Laser.set_power(rpm);
}


void Cspindle::run(uint8_t state, float rpm)
{
	if (sys.state == STATE_CHECK_MODE) { return; }
	Protocol.buffer_synchronize(); // Empty planner buffer to ensure spindle is set when programmed.  
	set_state(state, rpm);
}

#endif