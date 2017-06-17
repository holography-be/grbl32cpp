#include "probe.h"

#ifndef _probe

// Inverts the probe pin state depending on user settings and probing cycle mode.
uint32_t probe_invert_mask;


// Probe pin initialization routine.
void Cprobe::init()
{
	// Already done in limit_init() (limits.cpp)
	//PROBE_DDR &= ~(PROBE_MASK); // Configure as input pins
	//#ifdef DISABLE_PROBE_PIN_PULL_UP
	//  PROBE_PORT &= ~(PROBE_MASK); // Normal low operation. Requires external pull-down.
	//#else
	//  PROBE_PORT |= PROBE_MASK;    // Enable internal pull-up resistors. Normal high operation.
	//#endif
	//// probe_configure_invert_mask(false); // Initialize invert mask. Not required. Updated when in-use.

	// Pins et interrupt déjà définies dans system_init()

	printStringln("Probe init");
}


// Called by probe_init() and the mc_probe() routines. Sets up the probe pin invert mask to 
// appropriately set the pin logic according to setting for normal-high/normal-low operation 
// and the probing cycle modes for toward-workpiece/away-from-workpiece. 
void Cprobe::configure_invert_mask(uint8_t is_probe_away)
{
	probe_invert_mask = 0; // Initialize as zero.
	if (bit_isfalse(settings.flags, BITFLAG_INVERT_PROBE_PIN)) { probe_invert_mask ^= Z_PROBE_BIT; }
	if (is_probe_away) { probe_invert_mask ^= Z_PROBE_BIT; }
}


// Returns the probe pin state. Triggered = true. Called by gcode parser and probe state monitor.
//uint8_t probe_get_state() { return((PROBE_PIN & PROBE_MASK) ^ probe_invert_mask); }
uint8_t Cprobe::get_state() { return((CONTROL2_PORT->PORTxbits.w & Z_PROBE_BIT) ^ probe_invert_mask); }

// Monitors probe pin state and records the system position when detected. Called by the
// stepper ISR per ISR tick.
// NOTE: This function must be extremely efficient as to not bog down the stepper ISR.
void Cprobe::state_monitor()
{
	if (sys_probe_state == PROBE_ACTIVE) {
		if (probe_get_state()) {
			sys_probe_state = PROBE_OFF;
			memcpy(sys.probe_position, sys.position, sizeof(sys.position));
			bit_true(sys_rt_exec_state, EXEC_MOTION_CANCEL);
		}
	}
}
