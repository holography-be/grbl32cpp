#include "coolant_control.h"


void Ccoolant::init()
{
	LASER_ON_PORT->TRISxCLR.w = LASER_ON_MASK;
	LASER_ON_PORT->ODCxCLR.w = LASER_ON_MASK;
	LASER_FAN_PORT->TRISxCLR.w = LASER_FAN_MASK;
	LASER_FAN_PORT->ODCxCLR.w = LASER_FAN_MASK;
	stop();
	//printStringln("Laser init");
}

void Ccoolant::stop()
{
	//LASER_ON_PORT->LATxCLR.w = LASER_ON_MASK;
	//LASER_FAN_PORT->LATxCLR.w = LASER_FAN_MASK;
	Laser.off();
}

void Ccoolant::set_state(uint8_t mode)
{
	if (mode == LASER_ENABLE) {
		//LASER_ON_PORT->LATxSET.w = LASER_ON_MASK;
		//LASER_FAN_PORT->LATxSET.w = LASER_FAN_MASK;
		Laser.on();
	}
	else {
		stop();
	}
}

void Ccoolant::run(uint8_t mode)
{
	if (sys.state == STATE_CHECK_MODE) { return; }
	// laser fan must be on immediately
	protocol_buffer_synchronize(); // Ensure coolant turns on when specified in program.  
	set_state(mode);
}

