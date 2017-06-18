#include "report.h"


// Handles the primary confirmation protocol response for streaming interfaces and human-feedback.
// For every incoming line, this method responds with an 'ok' for a successful command or an 
// 'error:'  to indicate some error event with the line or some critical system error during 
// operation. Errors events can originate from the g-code parser, settings module, or asynchronously
// from a critical error, such as a triggered hard limit. Interface should always monitor for these
// responses.
// NOTE: In silent mode, all error codes are greater than zero.
// TODO: Install silent mode to return only numeric values, primarily for GUIs.
void Creport::status_message(uint8_t status_code)
{
	if (status_code == 0) { // STATUS_OK
		Print_grbl.Stringln("ok");
	}
	else {
		Print_grbl.String("error: ");
#ifdef REPORT_GUI_MODE
		Print_grbl.uint8_base10(status_code);
#else
		switch (status_code) {
		case STATUS_EXPECTED_COMMAND_LETTER:
			Print_grbl.Stringln("Expected command letter"); break;
		case STATUS_BAD_NUMBER_FORMAT:
			Print_grbl.Stringln("Bad number format"); break;
		case STATUS_INVALID_STATEMENT:
			Print_grbl.Stringln("Invalid statement"); break;
		case STATUS_NEGATIVE_VALUE:
			Print_grbl.Stringln("Value < 0"); break;
		case STATUS_SETTING_DISABLED:
			Print_grbl.Stringln("Setting disabled"); break;
		case STATUS_SETTING_STEP_PULSE_MIN:
			Print_grbl.Stringln("Value < 3 usec"); break;
		case STATUS_SETTING_READ_FAIL:
			Print_grbl.Stringln("EEPROM read fail. Using defaults"); break;
		case STATUS_IDLE_ERROR:
			Print_grbl.Stringln("Not idle"); break;
		case STATUS_ALARM_LOCK:
			Print_grbl.Stringln("Alarm lock"); break;
		case STATUS_SOFT_LIMIT_ERROR:
			Print_grbl.Stringln("Homing not enabled"); break;
		case STATUS_OVERFLOW:
			Print_grbl.Stringln("Line overflow"); break;
#ifdef MAX_STEP_RATE_HZ
		case STATUS_MAX_STEP_RATE_EXCEEDED:
			Print_grbl.Stringln("Step rate > 30kHz"); break;
#endif      
			// Common g-code parser errors.
		case STATUS_GCODE_MODAL_GROUP_VIOLATION:
			Print_grbl.Stringln("Modal group violation"); break;
		case STATUS_GCODE_UNSUPPORTED_COMMAND:
			Print_grbl.Stringln("Unsupported command"); break;
		case STATUS_GCODE_UNDEFINED_FEED_RATE:
			Print_grbl.Stringln("Undefined feed rate"); break;
		default:
			// Remaining g-code parser errors with error codes
			Print_grbl.String("Invalid gcode ID:");
			Print_grbl.uint8_base10(status_code); // Print error code for user reference
		}
#endif  

	}
}

// Prints alarm messages.
void Creport::alarm_message(int8_t alarm_code)
{
	Print_grbl.PgmString("ALARM: ");
#ifdef REPORT_GUI_MODE
	Print_grbl.uint8_base10(alarm_code);
#else
	switch (alarm_code) {
	case ALARM_HARD_LIMIT_ERROR:
		Print_grbl.Stringln("Hard limit"); break;
	case ALARM_SOFT_LIMIT_ERROR:
		Print_grbl.Stringln("Soft limit"); break;
	case ALARM_ABORT_CYCLE:
		Print_grbl.Stringln("Abort during cycle"); break;
	case ALARM_PROBE_FAIL:
		Print_grbl.Stringln("Probe fail"); break;
	case ALARM_HOMING_FAIL:
		Print_grbl.Stringln("Homing fail"); break;
	}
#endif
	delay_ms(500); // Force delay to ensure message clears serial write buffer.
}

// Prints feedback messages. This serves as a centralized method to provide additional
// user feedback for things that are not of the status/alarm message protocol. These are
// messages such as setup warnings, switch toggling, and how to exit alarms.
// NOTE: For interfaces, messages are always placed within brackets. And if silent mode
// is installed, the message number codes are less than zero.
// TODO: Install silence feedback messages option in settings
void Creport::feedback_message(uint8_t message_code)
{
	Print_grbl.PgmString("[");
	switch (message_code) {
	case MESSAGE_CRITICAL_EVENT:
		Print_grbl.PgmString("Reset to continue"); break;
	case MESSAGE_ALARM_LOCK:
		Print_grbl.PgmString("'$H'|'$X' to unlock"); break;
	case MESSAGE_ALARM_UNLOCK:
		Print_grbl.PgmString("Caution: Unlocked"); break;
	case MESSAGE_ENABLED:
		Print_grbl.PgmString("Enabled"); break;
	case MESSAGE_DISABLED:
		Print_grbl.PgmString("Disabled"); break;
	case MESSAGE_SAFETY_DOOR_AJAR:
		Print_grbl.PgmString("Check Door"); break;
	case MESSAGE_PROGRAM_END:
		Print_grbl.PgmString("Pgm End"); break;
	case MESSAGE_RESTORE_DEFAULTS:
		Print_grbl.PgmString("Restoring defaults"); break;
	}
	Print_grbl.Stringln("]");
}


// Welcome message
void Creport::init_message()
{
	Print_grbl.Stringln("\r\nGrbl Max " GRBL_VERSION " ['$' for help]");
}

// Grbl help message
void Creport::grbl_help() {
#ifndef REPORT_GUI_MODE
	Print_grbl.PgmString("$$ (view Grbl settings)\r\n"
		"$# (view # parameters)\r\n"
		"$G (view parser state)\r\n"
		"$I (view build info)\r\n"
		"$N (view startup blocks)\r\n"
		"$x=value (save Grbl setting)\r\n"
		"$Nx=line (save startup block)\r\n"
		"$C (check gcode mode)\r\n"
		"$X (kill alarm lock)\r\n"
		"$H (run homing cycle)\r\n"
		"~ (cycle start)\r\n"
		"! (feed hold)\r\n"
		"? (current status)\r\n"
		"ctrl-x (reset Grbl)\r\n");
#endif
}


// Grbl global settings print out.
// NOTE: The numbering scheme here must correlate to storing in settings.c
void Creport::grbl_settings() {
	// Print Grbl settings.
#ifdef REPORT_GUI_MODE
	Print_grbl.PgmString("$0="); Print_grbl.uint8_base10(Settings.settings.pulse_microseconds);
	Print_grbl.PgmString("\r\n$1="); Print_grbl.uint8_base10(Settings.settings.stepper_idle_lock_time);
	Print_grbl.PgmString("\r\n$2="); Print_grbl.uint8_base10(Settings.settings.step_invert_mask);
	Print_grbl.PgmString("\r\n$3="); Print_grbl.uint8_base10(Settings.settings.dir_invert_mask);
	Print_grbl.PgmString("\r\n$4="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_INVERT_ST_ENABLE));
	Print_grbl.PgmString("\r\n$5="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_INVERT_LIMIT_PINS));
	Print_grbl.PgmString("\r\n$6="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_INVERT_PROBE_PIN));
	Print_grbl.PgmString("\r\n$10="); Print_grbl.uint8_base10(Settings.settings.status_report_mask);
	Print_grbl.PgmString("\r\n$11="); Print_grbl.Float_SettingValue(Settings.settings.junction_deviation);
	Print_grbl.PgmString("\r\n$12="); Print_grbl.Float_SettingValue(Settings.settings.arc_tolerance);
	Print_grbl.PgmString("\r\n$13="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_REPORT_INCHES));
	Print_grbl.PgmString("\r\n$20="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_SOFT_LIMIT_ENABLE));
	Print_grbl.PgmString("\r\n$21="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_HARD_LIMIT_ENABLE));
	Print_grbl.PgmString("\r\n$22="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_HOMING_ENABLE));
	Print_grbl.PgmString("\r\n$23="); Print_grbl.uint8_base10(Settings.settings.homing_dir_mask);
	Print_grbl.PgmString("\r\n$24="); Print_grbl.Float_SettingValue(Settings.settings.homing_feed_rate);
	Print_grbl.PgmString("\r\n$25="); Print_grbl.Float_SettingValue(Settings.settings.homing_seek_rate);
	Print_grbl.PgmString("\r\n$26="); Print_grbl.uint8_base10(Settings.settings.homing_debounce_delay);
	Print_grbl.PgmString("\r\n$27="); Print_grbl.Float_SettingValue(Settings.settings.homing_pulloff);
	Print_grbl.PgmString("\r\n$28="); Print_grbl.uint8_base10(Settings.settings.laser_power_divisor);
	Print_grbl.PgmString("\r\n$29="); Print_grbl.uint8_base10(Settings.settings.debug_mode);
	Print_grbl.PgmString("\r\n");
#else      
	Print_grbl.PgmString("$0="); Print_grbl.uint8_base10(Settings.settings.pulse_microseconds);
	Print_grbl.PgmString(" (step pulse, usec)\r\n$1="); Print_grbl.uint8_base10(Settings.settings.stepper_idle_lock_time);
	Print_grbl.PgmString(" (step idle delay, msec)\r\n$2="); Print_grbl.uint8_base10(Settings.settings.step_invert_mask);
	Print_grbl.PgmString(" (step port invert mask:"); Print_grbl.uint8_base2(Settings.settings.step_invert_mask);
	Print_grbl.PgmString(")\r\n$3="); Print_grbl.uint8_base10(Settings.settings.dir_invert_mask);
	Print_grbl.PgmString(" (dir port invert mask:"); Print_grbl.uint8_base2(Settings.settings.dir_invert_mask);
	Print_grbl.PgmString(")\r\n$4="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_INVERT_ST_ENABLE));
	Print_grbl.PgmString(" (step enable invert, bool)\r\n$5="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_INVERT_LIMIT_PINS));
	Print_grbl.PgmString(" (limit pins invert, bool)\r\n$6="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_INVERT_PROBE_PIN));
	Print_grbl.PgmString(" (probe pin invert, bool)\r\n$10="); Print_grbl.uint8_base10(Settings.settings.status_report_mask);
	Print_grbl.PgmString(" (status report mask:"); Print_grbl.uint8_base2(Settings.settings.status_report_mask);
	Print_grbl.PgmString(")\r\n$11="); Print_grbl.Float_SettingValue(Settings.settings.junction_deviation);
	Print_grbl.PgmString(" (junction deviation, mm)\r\n$12="); Print_grbl.Float_SettingValue(Settings.settings.arc_tolerance);
	Print_grbl.PgmString(" (arc tolerance, mm)\r\n$13="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_REPORT_INCHES));
	Print_grbl.PgmString(" (report inches, bool)\r\n$20="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_SOFT_LIMIT_ENABLE));
	Print_grbl.PgmString(" (soft limits, bool)\r\n$21="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_HARD_LIMIT_ENABLE));
	Print_grbl.PgmString(" (hard limits, bool)\r\n$22="); Print_grbl.uint8_base10(bit_istrue(Settings.settings.flags, BITFLAG_HOMING_ENABLE));
	Print_grbl.PgmString(" (homing cycle, bool)\r\n$23="); Print_grbl.uint8_base10(Settings.settings.homing_dir_mask);
	Print_grbl.PgmString(" (homing dir invert mask:"); Print_grbl.uint8_base2(Settings.settings.homing_dir_mask);
	Print_grbl.PgmString(")\r\n$24="); Print_grbl.Float_SettingValue(Settings.settings.homing_feed_rate);
	Print_grbl.PgmString(" (homing feed, mm/min)\r\n$25="); Print_grbl.Float_SettingValue(Settings.settings.homing_seek_rate);
	Print_grbl.PgmString(" (homing seek, mm/min)\r\n$26="); Print_grbl.uint8_base10(Settings.settings.homing_debounce_delay);
	Print_grbl.PgmString(" (homing debounce, msec)\r\n$27="); Print_grbl.Float_SettingValue(Settings.settings.homing_pulloff);
	Print_grbl.PgmString(" (homing pull-off, mm)\r\n$28="); Print_grbl.uint8_base10(Settings.settings.laser_power_divisor);
	Print_grbl.PgmString(" (laser power divisor)\r\n$29="); Print_grbl.uint8_base10(Settings.settings.debug_mode);
	Print_grbl.PgmString(" (debug mode)\r\n");
#endif

	// Print axis settings
	uint8_t idx, set_idx;
	uint8_t val = AXIS_SETTINGS_START_VAL;
	for (set_idx = 0; set_idx<AXIS_N_SETTINGS; set_idx++) {
		for (idx = 0; idx<N_AXIS; idx++) {
			Print_grbl.PgmString("$");
			Print_grbl.uint8_base10(val + idx);
			Print_grbl.PgmString("=");
			switch (set_idx) {
			case 0: Print_grbl.Float_SettingValue(Settings.settings.steps_per_mm[idx]); break;
			case 1: Print_grbl.Float_SettingValue(Settings.settings.max_rate[idx]); break;
			case 2: Print_grbl.Float_SettingValue(Settings.settings.acceleration[idx] / (60 * 60)); break;
			case 3: Print_grbl.Float_SettingValue(-Settings.settings.max_travel[idx]); break;
			}
#ifdef REPORT_GUI_MODE
			Print_grbl.Stringln("");
#else
			Print_grbl.PgmString(" (");
			switch (idx) {
			case X_AXIS: Print_grbl.PgmString("x"); break;
			case Y_AXIS: Print_grbl.PgmString("y"); break;
			case Z_AXIS: Print_grbl.PgmString("z"); break;
			}
			switch (set_idx) {
			case 0: Print_grbl.PgmString(", step/mm"); break;
			case 1: Print_grbl.PgmString(" max rate, mm/min"); break;
			case 2: Print_grbl.PgmString(" accel, mm/sec^2"); break;
			case 3: Print_grbl.PgmString(" max travel, mm"); break;
			}
			Print_grbl.PgmString(")\r\n");
#endif
		}
		val += AXIS_SETTINGS_INCREMENT;
	}
}


// Prints current probe parameters. Upon a probe command, these parameters are updated upon a
// successful probe or upon a failed probe with the G38.3 without errors command (if supported). 
// These values are retained until Grbl is power-cycled, whereby they will be re-zeroed.
void Creport::probe_parameters()
{
	uint8_t i;
	float print_position[N_AXIS];

	// Report in terms of machine position.
	Print_grbl.PgmString("[PRB:");
	for (i = 0; i< N_AXIS; i++) {
		print_position[i] = System.convert_axis_steps_to_mpos(sys.probe_position, i);
		Print_grbl.Float_CoordValue(print_position[i]);
		if (i < (N_AXIS - 1)) { Print_grbl.PgmString(","); }
	}
	Print_grbl.PgmString(":");
	Print_grbl.uint8_base10(sys.probe_succeeded);
	Print_grbl.Stringln("]");
}


// Prints Grbl NGC parameters (coordinate offsets, probing)
void Creport::ngc_parameters()
{
	float coord_data[N_AXIS];
	uint8_t coord_select, i;
	for (coord_select = 0; coord_select <= SETTING_INDEX_NCOORD; coord_select++) {
		if (!(Settings.read_coord_data(coord_select, coord_data))) {
			status_message(STATUS_SETTING_READ_FAIL);
			return;
		}
		Print_grbl.PgmString("[G");
		switch (coord_select) {
		case 6: Print_grbl.PgmString("28"); break;
		case 7: Print_grbl.PgmString("30"); break;
		default: Print_grbl.uint8_base10(coord_select + 54); break; // G54-G59
		}
		Print_grbl.PgmString(":");
		for (i = 0; i<N_AXIS; i++) {
			Print_grbl.Float_CoordValue(coord_data[i]);
			if (i < (N_AXIS - 1)) { Print_grbl.PgmString(","); }
			else { Print_grbl.PgmString("]\r\n"); }
		}
	}
	Print_grbl.PgmString("[G92:"); // Print G92,G92.1 which are not persistent in memory
	for (i = 0; i<N_AXIS; i++) {
		Print_grbl.Float_CoordValue(gc_state.coord_offset[i]);
		if (i < (N_AXIS - 1)) { Print_grbl.PgmString(","); }
		else { Print_grbl.PgmString("]\r\n"); }
	}
	Print_grbl.PgmString("[TLO:"); // Print tool length offset value
	Print_grbl.Float_CoordValue(gc_state.tool_length_offset);
	Print_grbl.PgmString("]\r\n");
	probe_parameters(); // Print probe parameters. Not persistent in memory.
}


// Print current gcode parser mode state
void Creport::gcode_modes()
{
	Print_grbl.PgmString("[");

	switch (gc_state.modal.motion) {
	case MOTION_MODE_SEEK: Print_grbl.PgmString("G0"); break;
	case MOTION_MODE_LINEAR: Print_grbl.PgmString("G1"); break;
	case MOTION_MODE_CW_ARC: Print_grbl.PgmString("G2"); break;
	case MOTION_MODE_CCW_ARC: Print_grbl.PgmString("G3"); break;
	case MOTION_MODE_NONE: Print_grbl.PgmString("G80"); break;
	default:
		Print_grbl.PgmString("G38.");
		Print_grbl.uint8_base10(gc_state.modal.motion - (MOTION_MODE_PROBE_TOWARD - 2));
	}

	Print_grbl.PgmString(" G");
	Print_grbl.uint8_base10(gc_state.modal.coord_select + 54);

	switch (gc_state.modal.plane_select) {
	case PLANE_SELECT_XY: Print_grbl.PgmString(" G17"); break;
	case PLANE_SELECT_ZX: Print_grbl.PgmString(" G18"); break;
	case PLANE_SELECT_YZ: Print_grbl.PgmString(" G19"); break;
	}

	if (gc_state.modal.units == UNITS_MODE_MM) { Print_grbl.PgmString(" G21"); }
	else { Print_grbl.PgmString(" G20"); }

	if (gc_state.modal.distance == DISTANCE_MODE_ABSOLUTE) { Print_grbl.PgmString(" G90"); }
	else { Print_grbl.PgmString(" G91"); }

	if (gc_state.modal.feed_rate == FEED_RATE_MODE_INVERSE_TIME) { Print_grbl.PgmString(" G93"); }
	else { Print_grbl.PgmString(" G94"); }

	switch (gc_state.modal.program_flow) {
	case PROGRAM_FLOW_RUNNING: Print_grbl.PgmString(" M0"); break;
	case PROGRAM_FLOW_PAUSED: Print_grbl.PgmString(" M1"); break;
	case PROGRAM_FLOW_COMPLETED: Print_grbl.PgmString(" M2"); break;
	}

	switch (gc_state.modal.spindle) {
	case LASER_POWER_ENABLE: Print_grbl.PgmString(" M3"); break;
		//case LASER_POWER_ENABLE: Print_grbl.PgmString(" M4"); break;
	case LASER_POWER_DISABLE: Print_grbl.PgmString(" M5"); break;
	}

	switch (gc_state.modal.coolant) {
	case LASER_DISABLE: Print_grbl.PgmString(" M9"); break;
	case LASER_ENABLE: Print_grbl.PgmString(" M8"); break;
#ifdef ENABLE_M7
	case COOLANT_MIST_ENABLE: Print_grbl.PgmString(" M7"); break;
#endif
	}

	Print_grbl.PgmString(" T");
	Print_grbl.uint8_base10(gc_state.tool);

	Print_grbl.PgmString(" F");
	Print_grbl.Float_RateValue(gc_state.feed_rate);

#ifdef VARIABLE_SPINDLE
	Print_grbl.PgmString(" S");
	Print_grbl.Float_RateValue(gc_state.spindle_speed);
#endif

	Print_grbl.PgmString("]\r\n");
}

// Prints specified startup line
void Creport::startup_line(uint8_t n, char *line)
{
	Print_grbl.PgmString("$N"); Print_grbl.uint8_base10(n);
	Print_grbl.PgmString("="); Print_grbl.String(line);
	Print_grbl.PgmString("\r\n");
}


// Prints build info line
void Creport::build_info(char *line)
{
	Print_grbl.PgmString("[" GRBL_VERSION "." GRBL_VERSION_BUILD ":");
	Print_grbl.String(line);
	Print_grbl.PgmString("]\r\n");
}


// Prints the character string line Grbl has received from the user, which has been pre-parsed,
// and has been sent into protocol_execute_line() routine to be executed by Grbl.
void Creport::echo_line_received(char *line)
{
	Print_grbl.PgmString("[echo: "); Print_grbl.String(line);
	Print_grbl.PgmString("]\r\n");
}


// Prints real-time data. This function grabs a real-time snapshot of the stepper subprogram 
// and the actual location of the CNC machine. Users may change the following function to their
// specific needs, but the desired real-time data report must be as short as possible. This is
// requires as it minimizes the computational overhead and allows grbl to keep running smoothly, 
// especially during g-code programs with fast, short line segments and high frequency reports (5-20Hz).
void Creport::realtime_status()
{
	// **Under construction** Bare-bones status report. Provides real-time machine position relative to 
	// the system power on location (0,0,0) and work coordinate position (G54 and G92 applied). Eventually
	// to be added are distance to go on block, processed block id, and feed rate. Also a settings bitmask
	// for a user to select the desired real-time data.
	uint8_t idx;
	int32_t current_position[N_AXIS]; // Copy current state of the system position variable
	memcpy(current_position, sys.position, sizeof(sys.position));
	float print_position[N_AXIS];

	// Report current machine state
	switch (sys.state) {
	case STATE_IDLE: Print_grbl.PgmString("<Idle"); break;
	case STATE_MOTION_CANCEL: // Report run state.
	case STATE_CYCLE: Print_grbl.PgmString("<Run"); break;
	case STATE_HOLD: Print_grbl.PgmString("<Hold"); break;
	case STATE_HOMING: Print_grbl.PgmString("<Home"); break;
	case STATE_ALARM: Print_grbl.PgmString("<Alarm"); break;
	case STATE_CHECK_MODE: Print_grbl.PgmString("<Check"); break;
	case STATE_SAFETY_DOOR: Print_grbl.PgmString("<Door"); break;
	}

	// If reporting a position, convert the current step count (current_position) to millimeters.
	if (bit_istrue(Settings.settings.status_report_mask, (BITFLAG_RT_STATUS_MACHINE_POSITION | BITFLAG_RT_STATUS_WORK_POSITION))) {
		System.convert_array_steps_to_mpos(print_position, current_position);
	}

	// Report machine position
	if (bit_istrue(Settings.settings.status_report_mask, BITFLAG_RT_STATUS_MACHINE_POSITION)) {
		Print_grbl.PgmString(",MPos:");
		for (idx = 0; idx< N_AXIS; idx++) {
			Print_grbl.Float_CoordValue(print_position[idx]);
			if (idx < (N_AXIS - 1)) { Print_grbl.PgmString(","); }
		}
	}

	// Report work position
	if (bit_istrue(Settings.settings.status_report_mask, BITFLAG_RT_STATUS_WORK_POSITION)) {
		Print_grbl.PgmString(",WPos:");
		for (idx = 0; idx< N_AXIS; idx++) {
			// Apply work coordinate offsets and tool length offset to current position.
			print_position[idx] -= gc_state.coord_system[idx] + gc_state.coord_offset[idx];
			if (idx == TOOL_LENGTH_OFFSET_AXIS) { print_position[idx] -= gc_state.tool_length_offset; }
			Print_grbl.Float_CoordValue(print_position[idx]);
			if (idx < (N_AXIS - 1)) { Print_grbl.PgmString(","); }
		}
	}

	// Returns the number of active blocks are in the planner buffer.
	if (bit_istrue(Settings.settings.status_report_mask, BITFLAG_RT_STATUS_PLANNER_BUFFER)) {
		Print_grbl.PgmString(",Buf:");
		Print_grbl.uint8_base10(Planner.get_block_buffer_count());
	}

	// Report serial read buffer status
	if (bit_istrue(Settings.settings.status_report_mask, BITFLAG_RT_STATUS_SERIAL_RX)) {
		Print_grbl.PgmString(",RX:");
		Print_grbl.uint8_base10(Serialbuffer.get_rx_buffer_count());
	}

#ifdef USE_LINE_NUMBERS
	// Report current line number
	Print_grbl.PgmString(",Ln:");
	int32_t ln = 0;
	plan_block_t * pb = Planner.get_current_block();
	if (pb != NULL) {
		ln = pb->line_number;
	}
	Print_grbl.Integer(ln);
#endif

#ifdef REPORT_REALTIME_RATE
	// Report realtime rate 
	Print_grbl.PgmString(",F:");
	Print_grbl.Float_RateValue(st_get_realtime_rate());
#endif    

	if (bit_istrue(Settings.settings.status_report_mask, BITFLAG_RT_STATUS_LIMIT_PINS)) {
		Print_grbl.PgmString(",Lim:");
		Print_grbl.unsigned_int8(Limits.get_state(), 2, N_AXIS);
	}

#ifdef REPORT_CONTROL_PIN_STATE 
	Print_grbl.PgmString(",Ctl:");
	Print_grbl.uint8_base2(CONTROL_PIN & CONTROL_MASK);
#endif

	Print_grbl.Stringln(">");
}
