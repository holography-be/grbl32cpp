/*
cpu_map_atmega2560.h - CPU and pin mapping configuration file
Part of Grbl

Copyright (c) 2012-2015 Sungeun K. Jeon

Grbl is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Grbl is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Grbl.  If not, see <http://www.gnu.org/licenses/>.
*/

/* This cpu_map file serves as a central pin mapping settings file for AVR Mega 2560 */


#ifdef GRBL_PLATFORM
#error "cpu_map already defined: GRBL_PLATFORM=" GRBL_PLATFORM
#endif

#include "p32mx795f512l.h"



#define bit_true(x,mask) (x) |= (mask)
#define bit_false(x,mask) (x) &= ~(mask)
#define bit_istrue(x,mask) ((x & mask) != 0)
#define bit_isfalse(x,mask) ((x & mask) == 0)


#define LASER_STATE_ON			1
#define LASER_STATE_OFF			0

// stepper motors 
#define STEP_PORT	portE
#define X_STEP_PIN	30			// 30
#define X_STEP_BIT	7
#define Y_STEP_PIN	31
#define Y_STEP_BIT	6			// 31
#define	Z_STEP_PIN	32
#define Z_STEP_BIT	5
#define A_STEP_PIN	33
#define	A_STEP_BIT	4
#define STEP_MASK	(1 << X_STEP_BIT) | (1 << Y_STEP_BIT) | (1 << Z_STEP_BIT) | (1 << A_STEP_BIT)

//
//p32_A_dc
//// DIR_ection
#define DIR_PORT		portE
#define X_DIR_PIN		34
#define X_DIR_BIT		3
#define Y_DIR_PIN		35
#define Y_DIR_BIT		2
#define Z_DIR_PIN		36
#define Z_DIR_BIT		1
#define A_DIR_PIN		37
#define A_DIR_BIT		0
#define DIRECTION_MASK (1 << X_DIR_BIT) | (1 << Y_DIR_BIT) | (1 << Z_DIR_BIT) | (1 << A_DIR_BIT)
//
//
//
//#define BLINK_DDR   DDRB
//#define BLINK_PORT  PORTB
//#define BLINK_PIN	PINB
//#define BLINK_BIT   7 // MEGA2560 Digital Pin 13
//
//// Define stepper driver enable/disable output pin.
#define ENABLE_MOTOR_PORT		portF
#define ENABLE_MOTOR_PIN		45
#define ENABLE_MOTOR_BIT		0
#define ENABLE_MOTOR_MASK		(1<<ENABLE_MOTOR_BIT)

//


#define TIMER_PWM				timer3		
#define PWM						portOC3
#define	TIMER_PWM_PERIOD		2550
#define STEPPER_STEP_TIMER		timer4
#define STEPPER_STEP_VECTOR		_TIMER_4_VECTOR
#define STEPPER_STEP_IRQ		_TIMER_4_IRQ
#define STEPPER_RESET_TIMER		timer2
#define STEPPER_RESET_VECTOR	_TIMER_2_VECTOR
#define STEPPER_RESET_IRQ		_TIMER_2_IRQ

//
//
//
//// Controls
//// Define ho_MIN_g/hard limit switch input pins and limit interrupt vectors. 
//// NOTE: All limit bit pins must be on the same port
//
#define CONTROL_PORT	portB
#define X_MIN_PIN			54
#define X_MIN_BIT			0
#define X_MIN_CNBIT		2
#define Y_MIN_PIN			55
#define Y_MIN_BIT			1
#define Y_MIN_CNBIT		3
#define Z_MIN_PIN			56
#define Z_MIN_BIT			2
#define Z_MIN_CNBIT		4
#define X_MAX_PIN			57
#define X_MAX_BIT			3
#define X_MAX_CNBIT		5
#define Y_MAX_PIN			58
#define Y_MAX_BIT			4
#define Y_MAX_CNBIT		6
#define Z_MAX_PIN			59
#define Z_MAX_BIT			5
#define Z_MAX_CNBIT		7
#define CONTROL_MASK	(1 << X_MIN_BIT) | (1 << Y_MIN_BIT) | (1 << Z_MIN_BIT) | (1 << X_MAX_BIT) | (1 << Y_MAX_BIT) | (1 << Z_MAX_BIT)
#define CONTROL_CN_MASK	(1 << X_MIN_CNBIT) | (1 << Y_MIN_CNBIT) | (1 << Z_MIN_CNBIT) | (1 << X_MAX_CNBIT) | (1 << Y_MAX_CNBIT) | (1 << Z_MAX_CNBIT)
//
#define CONTROL2_PORT	portD
#define Z_PROBE_PIN		18
#define Z_PROBE_BIT		15
#define Z_PROBE_CNBIT		21
#define EMERGENCY_PIN	19
#define EMERGENCY_BIT	14
#define EMERGENCY_CNBIT	20
#define CONTROL2_MASK		(1 << Z_PROBE_BIT) | (1 << EMERGENCY_BIT)
#define CONTROL2_CN_MASK	(1 << Z_PROBE_CNBIT) | (1 << EMERGENCY_CNBIT);
//
//// I2C
#define SDA				SDA1	// 20
#define SCL				SCL1	// 21
//
//// Define enable relais (for laser)
#define ENABLE_RELAIS_PORT	portF
#define ENABLE_RELAIS_PIN	46
#define ENABLE_RELAIS_BIT	1
#define ENABLE_RELAIS_MASK	(1<<ENABLE_RELAIS_BIT)
//
#define RELAIS_PORT		portD
#define RELAIS_1_PIN	38
#define RELAIS_1_BIT	10
#define RELAIS_1_MASK	(1<<RELAIS_1_BIT)
#define RELAIS_2_PIN	47
#define RELAIS_2_BIT	6 
#define RELAIS_2_MASK	(1<<RELAIS_2_BIT)
#define RELAIS_3_PIN	48
#define RELAIS_3_BIT	8 
#define RELAIS_3_MASK	(1<<RELAIS_3_BIT)
#define RELAIS_4_PIN	49
#define RELAIS_4_BIT	11
#define RELAIS_4_MASK	(1<<RELAIS_4_BIT)
#define RELAIS_MASK		(1<<RELAIS_1_BIT) | (1<<RELAIS_2_BIT) | (1<<RELAIS_3_BIT) | (1<<RELAIS_4_BIT)

//// laser Power
#define LASER_POWER_PORT	portD
#define LASER_POWER_PIN		6
#define LASER_POWER_BIT		2
#define LASER_POWER_MASK	(1<<LASER_POWER_BIT);
#define LASER_ON_PORT		RELAIS_PORT
#define LASER_ON_BIT		RELAIS_1_BIT
#define LASER_ON_MASK		RELAIS_1_MASK
#define LASER_FAN_PORT		RELAIS_PORT
#define LASER_FAN_BIT		RELAIS_2_BIT
#define LASER_FAN_MASK		RELAIS_2_MASK


//
//#ifdef ENABLE_M7 // Mist coolant disabled by default. See config.h to enable/disable.
//#define COOLANT_MIST_DDR    DDRH
//#define COOLANT_MIST_PORT   PORTH
//#define COOLANT_MIST_BIT    6 // MEGA2560 Digital Pin 9
//#endif  
//
//
//
//// Start of PWM & Stepper Enabled Spindle
//#ifdef VARIABLE_SPINDLE
//// Advanced Configuration Below You should not need to touch these variables
//// Set Timer up to use TIMER4B which is attached to Digital Pin 7
//#define PWM__MAX__VALUE      255
//#define TCCRA_REGISTER		TCCR4A
//#define TCCRB_REGISTER		TCCR4B
//#define OCR_REGISTER		OCR4B	// digital port 7
//
//#define COMB_BIT			COM4B1
//#define WAVE0_REGISTER		WGM40
//#define WAVE1_REGISTER		WGM41
//#define WAVE2_REGISTER		WGM42
//#define WAVE3_REGISTER		WGM43
//
//#define SPINDLE_PWM_DDR		DDRH
//#define SPINDLE_PWM_PORT    PORTH
//#define SPINDLE_PWM_BIT		4 // MEGA2560 Digital Pin 7
//#endif // End of VARIABLE_SPINDLE
//
//#define Laser_Temp_PIN		ADC0D   // ANALOG 0
//
// Leds Control		
#define LED_PORT	portA
#define LED_1_PIN		70
#define LED_1_BIT		0
#define	LED_2_PIN		71
#define	LED_2_BIT		1
#define	LED_3_PIN		72
#define LED_3_BIT		4
#define	LED_4_PIN		73
#define	LED_4_BIT		5
#define LED_MASK	(1<<LED_1_BIT) | (1<<LED_2_BIT) | (1<<LED_3_BIT) | (1<<LED_4_BIT)

#define LED_READY		LED_1_BIT
#define LED_LASER		LED_2_BIT
#define LED_LASER_POWER	LED_3_BIT

#define INLED_PORT	portA
#define INLED_PIN	13
#define INLED_BIT	3
#define INLED_MASK	(1<<INLED_BIT)

#define DEBUG_PORT	portA
#define DEBUG_PIN	13
#define DEBUG_BIT	3
#define DEBUG_MASK	(1<<DEBUG_BIT)
#define DEBUG_MODE_ON	1
#define DEBUG_MODE_OFF	0
#define DEBUG_MODE		DEBUG_MODE_ON

#define FAIL(status) return(status);

// Homing axis search distance multiplier. Computed by this value times the cycle travel.
#ifndef HOMING_AXIS_SEARCH_SCALAR
#define HOMING_AXIS_SEARCH_SCALAR  1.5 // Must be > 1 to ensure limit switch will be engaged.
#endif
#ifndef HOMING_AXIS_LOCATE_SCALAR
#define HOMING_AXIS_LOCATE_SCALAR  5.0 // Must be > 1 to ensure limit switch is cleared.
#endif

#define GRBL_PLATFORM "ChipKIT Max32"

