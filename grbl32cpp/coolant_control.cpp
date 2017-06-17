#include "coolant_control.h"


void init()
{
	LASER_ON_PORT->TRISxCLR.w = LASER_ON_MASK;
	LASER_ON_PORT->ODCxCLR.w = LASER_ON_MASK;
	LASER_FAN_PORT->TRISxCLR.w = LASER_FAN_MASK;
	LASER_FAN_PORT->ODCxCLR.w = LASER_FAN_MASK;
	coolant::stop();
	//printStringln("Laser init");
}

void stop()
{
	//LASER_ON_PORT->LATxCLR.w = LASER_ON_MASK;
	//LASER_FAN_PORT->LATxCLR.w = LASER_FAN_MASK;
	laser::off();
}

void set_state(uint8_t mode)
{
	if (mode == LASER_ENABLE) {
		//LASER_ON_PORT->LATxSET.w = LASER_ON_MASK;
		//LASER_FAN_PORT->LATxSET.w = LASER_FAN_MASK;
		laser::on();
	}
	else {
		collant::stop();
	}
}

void run(uint8_t mode)
{
	if (sys.state == STATE_CHECK_MODE) { return; }
	// laser fan must be on immediately
	protocol_buffer_synchronize(); // Ensure coolant turns on when specified in program.  
	coolant::set_state(mode);
}

