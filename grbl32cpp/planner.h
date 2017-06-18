/*
  planner.h - buffers movement commands and manages the acceleration profile plan
  Part of Grbl

  Copyright (c) 2011-2015 Sungeun K. Jeon 
  Copyright (c) 2009-2011 Simen Svale Skogsrud

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

#ifndef planner_h
#define planner_h

#include "grbl.h"

// The number of linear motions that can be in the plan at any give time
#ifndef BLOCK_BUFFER_SIZE
  #ifdef USE_LINE_NUMBERS
    #define BLOCK_BUFFER_SIZE 16
  #else
    #define BLOCK_BUFFER_SIZE 18
  #endif
#endif

// This struct stores a linear movement of a g-code block motion with its critical "nominal" values
// are as specified in the source g-code. 
typedef struct {
	// Fields used by the bresenham algorithm for tracing the line
	// NOTE: Used by stepper algorithm to execute the block correctly. Do not alter these values.
	uint8_t direction_bits;    // The direction bit set for this block (refers to *_DIRECTION_BIT in config.h)
	uint32_t steps[N_AXIS];    // Step count along each axis
	uint32_t step_event_count; // The maximum step axis count and number of steps required to complete this block. 

	// Fields used by the motion planner to manage acceleration
	float entry_speed_sqr;         // The current planned entry speed at block junction in (mm/min)^2
	float max_entry_speed_sqr;     // Maximum allowable entry speed based on the minimum of junction limit and 
	//   neighboring nominal speeds with overrides in (mm/min)^2
	float max_junction_speed_sqr;  // Junction entry speed limit based on direction vectors in (mm/min)^2
	float nominal_speed_sqr;       // Axis-limit adjusted nominal speed for this block in (mm/min)^2
	float acceleration;            // Axis-limit adjusted line acceleration in (mm/min^2)
	float millimeters;             // The remaining distance for this block to be executed in (mm)
	// uint8_t max_override;       // Maximum override value based on axis speed limits

#ifdef USE_LINE_NUMBERS
	int32_t line_number;
#endif
} plan_block_t;

#define SOME_LARGE_VALUE 1.0E+38 // Used by rapids and acceleration maximization calculations. Just needs
// to be larger than any feasible (mm/min)^2 or mm/sec^2 value.

class Cplanner {
private:

public:
// Initialize and reset the motion plan subsystem

	static plan_block_t block_buffer[BLOCK_BUFFER_SIZE];  // A ring buffer for motion instructions
	static uint8_t block_buffer_tail;     // Index of the block to process now
	static uint8_t block_buffer_head;     // Index of the next block to be pushed
	static uint8_t next_buffer_head;      // Index of the next buffer head
	static uint8_t block_buffer_planned;  // Index of the optimally planned block

	static void recalculate();
	static uint8_t prev_block_index(uint8_t block_index);
// Called periodically by step segment buffer. Mostly used internally by planner.
	static uint8_t next_block_index(uint8_t block_index);


  void reset();

// Add a new linear movement to the buffer. target[N_AXIS] is the signed, absolute target position 
// in millimeters. Feed rate specifies the speed of the motion. If feed rate is inverted, the feed
// rate is taken to mean "frequency" and would complete the operation in 1/feed_rate minutes.

  void buffer_line(float *target, float feed_rate, uint8_t invert_feed_rate, int32_t line_number);
  void buffer_line(float *target, float feed_rate, uint8_t invert_feed_rate);

// Called when the current block is no longer needed. Discards the block and makes the memory
// availible for new blocks.
void discard_current_block();

// Gets the current block. Returns NULL if buffer empty
plan_block_t *get_current_block();



// Called by step segment buffer when computing executing block velocity profile.
float get_exec_block_exit_speed();

// Reset the planner position vector (in steps)
void sync_position();

// Reinitialize plan with a partially completed block
void cycle_reinitialize();

// Returns the number of active blocks are in the planner buffer.
uint8_t get_block_buffer_count();

// Returns the status of the block ring buffer. True, if buffer is full.
uint8_t check_full_buffer();

};

extern Cplanner Planner;

#endif
