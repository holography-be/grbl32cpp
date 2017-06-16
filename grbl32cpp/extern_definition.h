// extern_definition.h



#ifndef EXTERN_DEFINITION_H
#define EXTERN_DEFINITION_H

#include "grbl.h"

extern p32_ioport *portA;
extern p32_ioport *portB;
extern p32_ioport *portC;
extern p32_ioport *portD;
extern p32_ioport *portE;
extern p32_ioport *portF;
extern p32_ioport *portG;
extern p32_cn *portCN;
extern p32_adc *portADC;
extern p32_oc *portOC1;
extern p32_oc *portOC2;
extern p32_oc *portOC3;
extern p32_oc *portOC4;
extern p32_oc *portOC5;
extern p32_timer *timer1;
extern p32_timer *timer2;
extern p32_timer *timer3;
extern p32_timer *timer4;
extern p32_timer *timer5;

extern p32_cn *cnRegister;

extern p32_ioport *StepperPort;
extern uint32_t StepperMask;
extern uint32_t X_STEP_MASK;
extern uint32_t Y_STEP_MASK;
extern uint32_t Z_STEP_MASK;
extern uint32_t A_STEP_MASK;

extern p32_ioport *DirStepperPort;
extern uint32_t DirStepperMask;
extern uint32_t X_DIR_MASK;
extern uint32_t Y_DIR_MASK;
extern uint32_t Z_DIR_MASK;
extern uint32_t A_DIR_MASK;

extern p32_ioport *DisableStepperPort;
extern uint32_t STEPPER_DISABLE_MASK;
extern uint32_t DisableStepperMask;

extern p32_ioport *ControlPort;
extern uint32_t ControlMask;
extern uint32_t LimitMask;
extern uint32_t Xmin_LIMIT_MASK;
extern uint32_t Ymin_LIMIT_MASK;
extern uint32_t Zmin_LIMIT_MASK;
extern uint32_t Xmax_LIMIT_MASK;
extern uint32_t Ymax_LIMIT_MASK;
extern uint32_t Zmax_LIMIT_MASK;

extern bool bXmin;
extern bool bYmin;
extern bool bZmin;
extern bool bXmax;
extern bool bYmax;
extern bool bZmax;
extern bool bReset;

extern uint32_t RESET_MASK;
//extern uint32_t FEED_HOLD_MASK;
//extern uint32_t CYCLE_START_MASK;
//extern uint32_t SAFETY_DOOR_MASK;

extern p32_ioport *LaserPort;
extern uint32_t LaserMask;
extern p32_ioport *RelaisPort;
extern uint32_t RelaisMask;
extern p32_ioport *ProbePort;
extern uint32_t ProbeMask;
extern p32_ioport *LedPort;
extern uint32_t LED1_MASK;
extern uint32_t LED2_MASK;
extern uint32_t LED3_MASK;
extern uint32_t LED4_MASK;
extern uint32_t LED5_MASK;
extern uint32_t LED6_MASK;
extern uint32_t LED7_MASK;
extern uint32_t LED8_MASK;
extern uint32_t LedMask;

extern float Laser_power;
extern float Laser_divisor;
extern uint8_t Laser_state;

extern p32_ioport *blinkPort;
extern uint32_t blinkMask;

extern volatile uint8_t limit;

extern float laser_power;
extern float laser_divisor;
extern uint8_t laser_state;

// Define planner variables
typedef struct {
	uint32_t position[N_AXIS];          // The planner position of the tool in absolute steps. Kept separate
	// from g-code position for movements requiring multiple line motions,
	// i.e. arcs, canned cycles, and backlash compensation.
	float previous_unit_vec[N_AXIS];   // Unit vector of previous path line segment
	float previous_nominal_speed_sqr;  // Nominal speed of previous path line segment
} planner_t;

extern planner_t pl;

extern uint8_t serial_rx_buffer[RX_BUFFER_SIZE];
extern uint16_t serial_rx_buffer_head;
extern volatile uint16_t serial_rx_buffer_tail;

extern uint8_t serial_tx_buffer[TX_BUFFER_SIZE];
extern uint8_t serial_tx_buffer_head;
extern volatile uint8_t serial_tx_buffer_tail;

typedef struct {
	uint8_t direction_bits;
	uint32_t steps[N_AXIS];
	uint32_t step_event_count;
} st_block_t;

extern st_block_t st_block_buffer[SEGMENT_BUFFER_SIZE - 1];

typedef struct {
	uint16_t n_step;          // Number of step events to be executed for this segment
	uint8_t st_block_index;   // Stepper block data index. Uses this information to execute this segment.
	uint16_t cycles_per_tick; // Step distance traveled per ISR tick, aka step rate.
#ifdef ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING
	uint8_t amass_level;    // Indicates AMASS level for the ISR to execute this segment
#else
	uint8_t prescaler;      // Without AMASS, a prescaler is required to adjust for slow timing.
#endif
} segment_t;
extern segment_t segment_buffer[SEGMENT_BUFFER_SIZE];


typedef struct {
	// Used by the bresenham line algorithm
	uint32_t counter_x,        // Counter variables for the bresenham line tracer
		counter_y,
		counter_z;
#ifdef STEP_PULSE_DELAY
	uint8_t step_bits;  // Stores out_bits output to complete the step pulse delay
#endif

	uint8_t execute_step;     // Flags step execution for each interrupt.
	uint16_t step_pulse_time;  // Step pulse reset time after step rise
	uint8_t step_outbits;         // The next stepping-bits to be output
	uint8_t dir_outbits;
#ifdef ADAPTIVE_MULTI_AXIS_STEP_SMOOTHING
	uint32_t steps[N_AXIS];
#endif

	uint16_t step_count;       // Steps remaining in line segment motion  
	uint8_t exec_block_index; // Tracks the current st_block index. Change indicates new block.
	st_block_t *exec_block;   // Pointer to the block data for the segment being executed
	segment_t *exec_segment;  // Pointer to the segment being executed
} stepper_t;
extern stepper_t st;

// Step segment ring buffer indices
extern volatile uint8_t segment_buffer_tail;
extern uint8_t segment_buffer_head;
extern uint8_t segment_next_head;

// Step and direction port invert masks. 
extern uint8_t step_port_invert_mask;
extern uint8_t dir_port_invert_mask;

// Used to avoid ISR nesting of the "Stepper Driver Interrupt". Should never occur though.
extern volatile uint8_t busy;

// Pointers for the step segment being prepped from the planner buffer. Accessed only by the
// main program. Pointers may be planning segments or planner blocks ahead of what being executed.
extern plan_block_t *pl_block;     // Pointer to the planner block being prepped
extern st_block_t *st_prep_block;  // Pointer to the stepper block data being prepped 

// Segment preparation data struct. Contains all the necessary information to compute new segments
// based on the current executing planner block.
typedef struct {
	uint8_t st_block_index;  // Index of stepper common data block being prepped
	uint8_t flag_partial_block;  // Flag indicating the last block completed. Time to load a new one.

	float steps_remaining;
	float step_per_mm;           // Current planner block step/millimeter conversion scalar
	float req_mm_increment;
	float dt_remainder;

	uint8_t ramp_type;      // Current segment ramp state
	float mm_complete;      // End of velocity profile from end of current planner block in (mm).
	// NOTE: This value must coincide with a step(no mantissa) when converted.
	float current_speed;    // Current speed at the end of the segment buffer (mm/min)
	float maximum_speed;    // Maximum speed of executing block. Not always nominal speed. (mm/min)
	float exit_speed;       // Exit speed of executing block (mm/min)
	float accelerate_until; // Acceleration ramp end measured from end of block (mm)
	float decelerate_after; // Deceleration ramp start measured from end of block (mm)
} st_prep_t;
extern st_prep_t prep;

extern char line[LINE_BUFFER_SIZE]; // Line to be executed. Zero-terminated.

// NOTE: When this struct is zeroed, the above defines set the defaults for the system.
typedef struct {
	uint8_t motion;          // {G0,G1,G2,G3,G38.2,G80}
	uint8_t feed_rate;       // {G93,G94}
	uint8_t units;           // {G20,G21}
	uint8_t distance;        // {G90,G91}
	// uint8_t distance_arc; // {G91.1} NOTE: Don't track. Only default supported.
	uint8_t plane_select;    // {G17,G18,G19}
	// uint8_t cutter_comp;  // {G40} NOTE: Don't track. Only default supported.
	uint8_t tool_length;     // {G43.1,G49}
	uint8_t coord_select;    // {G54,G55,G56,G57,G58,G59}
	// uint8_t control;      // {G61} NOTE: Don't track. Only default supported.
	uint8_t program_flow;    // {M0,M1,M2,M30}
	uint8_t coolant;         // {M7,M8,M9}
	uint8_t spindle;         // {M3,M4,M5}
} gc_modal_t;

typedef struct {
	float f;         // Feed
	float ijk[3];    // I,J,K Axis arc offsets
	uint8_t l;       // G10 or canned cycles parameters
	int32_t n;       // Line number
	float p;         // G10 or dwell parameters
	// float q;      // G82 peck drilling
	float r;         // Arc radius
	float s;         // Spindle speed
	uint8_t t;       // Tool selection
	float xyz[3];    // X,Y,Z Translational axes
} gc_values_t;


typedef struct {
	gc_modal_t modal;

	float spindle_speed;          // RPM
	float feed_rate;              // Millimeters/min
	uint8_t tool;                 // Tracks tool number. NOT USED.
	int32_t line_number;          // Last line number sent

	float position[N_AXIS];       // Where the interpreter considers the tool to be at this point in the code

	float coord_system[N_AXIS];   // Current work coordinate system (G54+). Stores offset from absolute machine
	// position in mm. Loaded from EEPROM when called.  
	float coord_offset[N_AXIS];   // Retains the G92 coordinate offset (work coordinates) relative to
	// machine zero in mm. Non-persistent. Cleared upon reset and boot.    
	float tool_length_offset;     // Tracks tool length offset value when enabled.
} parser_state_t;
extern parser_state_t gc_state;

typedef struct {
	//   uint16_t command_words;  // NOTE: If this bitflag variable fills, G and M words can be separated.
	//   uint16_t value_words;

	uint8_t non_modal_command;
	gc_modal_t modal;
	gc_values_t values;

} parser_block_t;
extern parser_block_t gc_block;

#endif