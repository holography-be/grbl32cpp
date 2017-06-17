#include "laser.h"


void init()
{
	laser_power = 0.0;
	laser_divisor = 1.00;
	laser_state = LASER_STATE_OFF;

	LASER_POWER_PORT->LATxCLR.w = LASER_POWER_MASK;	// be sure power is off	

	TIMER_PWM->tmxCon.reg = 0x0;
	TIMER_PWM->tmxCon.reg = TBCON_OFF | TBCON_PS_4 | TBCON_MODE16; // timer enable,1:4 prescaler,16 bits mode
	TIMER_PWM->tmxTmr.reg = 0x0;
	TIMER_PWM->tmxPr.reg = TIMER_PWM_PERIOD;

	PWM->ocxCon.clr = OCCON_ON;
	PWM->ocxCon.reg = OCCON_MODE16 | OCCON_SRC_TIMER3 | OCCON_PWM_FAULT_DISABLE;
	PWM->ocxR.reg = 0;
	PWM->ocxRs.reg = 0;

	//printStringln("Laser power init");
}

void on() {
	if (laser_state == LASER_STATE_OFF) {
		LASER_ON_PORT->LATxSET.w |= (1 << LASER_ON_MASK);
		LASER_FAN_PORT->LATxSET.w = LASER_FAN_MASK;
		laser_state = LASER_STATE_ON;
		led::on(LED_LASER);
	}
}

void off() {
	if (laser_state == LASER_STATE_ON) {
		LASER_ON_PORT->LATxCLR.w = (1 << LASER_ON_MASK);
		laser_state = LASER_STATE_OFF;
		power_off();
		led::off(LED_LASER);
	}
}

///<returns>return value : 0=off, 1=on</returns>
uint8_t get_state() {
	return laser_state;
}

void power_off() {
	set_power(0.00);
}

void set_power(float power) {
	if (laser_state == LASER_STATE_OFF) { return; }
	laser_power = power;
	if (laser_power <= 0.00) {
		// set TIMER_ off
		// force pin output to 0
		TIMER_PWM->tmxCon.clr = TBCON_ON;
		PWM->ocxCon.clr = OCCON_ON;
		LASER_POWER_PORT->LATxCLR.w = LASER_POWER_MASK;
		led::off(LED_LASER_POWER);
	}
	else
	{
		if ((laser_power >= 255.00) && (laser_divisor == 1.00)) {
			// set TIMER_ off
			// force pin output to 1
			TIMER_PWM->tmxCon.clr = TBCON_ON;
			PWM->ocxCon.clr = OCCON_ON;
			LASER_POWER_PORT->LATxSET.w = LASER_POWER_MASK;
		}
		else {
			TIMER_PWM->tmxCon.clr = TBCON_ON;
			PWM->ocxCon.clr = OCCON_ON;
			PWM->ocxR.reg = (unsigned int)((laser_power * 10.00) * laser_divisor);
			PWM->ocxRs.reg = (unsigned int)((laser_power * 10.00) * laser_divisor);
			PWM->ocxCon.set = OCCON_ON;
			TIMER_PWM->tmxTmr.reg = 0x0;
			TIMER_PWM->tmxCon.set = TBCON_ON;
		}
		led::on(LED_LASER_POWER);
	}
}


uint8_t get_power() {
	return (int)laser_power;
}

void set_divisor(float power_divisor) {
	if (power_divisor > 1.00) {
		power_divisor = 1.00;
	}
	else if (power_divisor < 0.00) {
		power_divisor = 0.00;
	}
	laser_divisor = power_divisor;
	set_power((int)laser_power);
}

float get_divisor() {
	return laser_divisor;
}

