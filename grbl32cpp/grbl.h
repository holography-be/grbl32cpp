// Grbl versioning system
#define GRBL_VERSION "1.0"
#define GRBL_VERSION_BUILD "20170605"

#include <xc.h>
#include "p32mx795f512l.h"
#include "p32_defs.h"
#include <math.h>
#include <inttypes.h>    
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <binary.h>
#include <wiring.h>

// Define the Grbl system include files. NOTE: Do not alter organization.
#include "cpu_map.h"
#include "extern_definition.h"
#include "config.h"
#include "nuts_bolts.h"
#include "settings.h"
#include "system.h"
#include "defaults.h"
#include "led.h"
#include "laser.h"
#include "coolant_control.h"
#include "eeprom.h"
#include "gcode.h"
#include "limits.h"
#include "motion_control.h"
#include "planner.h"
#include "grbl_print.h"
#include "probe.h"
#include "protocol.h"
#include "report.h"
#include "serial.h"
#include "spindle_control.h"
#include "stepper.h"