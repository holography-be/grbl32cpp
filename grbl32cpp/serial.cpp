#include "serial.h"

//uint8_t serial_rx_buffer[RX_BUFFER_SIZE];
//uint16_t serial_rx_buffer_head = 0;
//volatile uint16_t serial_rx_buffer_tail = 0;
//
//uint8_t serial_tx_buffer[TX_BUFFER_SIZE];
//uint8_t serial_tx_buffer_head = 0;
//volatile uint8_t serial_tx_buffer_tail = 0;

// Serial interrupt
void irq_serial(int ch) {
	uint8_t data = ch;
	uint8_t next_head;

	// Pick off realtime command characters directly from the serial stream. These characters are
	// not passed into the buffer, but these set system state flag bits for realtime execution.
	switch (data) {
	case CMD_STATUS_REPORT: bit_true_atomic(sys_rt_exec_state, EXEC_STATUS_REPORT); break; // Set as true
	case CMD_CYCLE_START:   bit_true_atomic(sys_rt_exec_state, EXEC_CYCLE_START); break; // Set as true
	case CMD_FEED_HOLD:     bit_true_atomic(sys_rt_exec_state, EXEC_FEED_HOLD); break; // Set as true
	case CMD_SAFETY_DOOR:   bit_true_atomic(sys_rt_exec_state, EXEC_SAFETY_DOOR); break; // Set as true
	case CMD_RESET:         mc_reset(); break; // Call motion control reset routine.
	default: // Write character to buffer    
		next_head = serial_rx_buffer_head + 1;
		if (next_head == RX_BUFFER_SIZE) { next_head = 0; }

		// Write data to buffer unless it is full.
		if (next_head != serial_rx_buffer_tail) {
			serial_rx_buffer[serial_rx_buffer_head] = data;
			serial_rx_buffer_head = next_head;
		}
		//TODO: else alarm on overflow?
	}
}

// Returns the number of bytes used in the RX serial buffer.
uint8_t get_rx_buffer_count()
{
	uint16_t rtail = serial_rx_buffer_tail; // Copy to limit multiple calls to volatile
	if (serial_rx_buffer_head >= rtail) { return(serial_rx_buffer_head - rtail); }
	return (RX_BUFFER_SIZE - (rtail - serial_rx_buffer_head));
}


// Returns the number of bytes used in the TX serial buffer.
// NOTE: Not used except for debugging and ensuring no TX bottlenecks.
uint8_t get_tx_buffer_count()
{
	uint8_t ttail = serial_tx_buffer_tail; // Copy to limit multiple calls to volatile
	if (serial_tx_buffer_head >= ttail) { return(serial_tx_buffer_head - ttail); }
	return (TX_BUFFER_SIZE - (ttail - serial_tx_buffer_head));
}

void init()
{
	Serial.begin(BAUD_RATE);
	Serial.attachInterrupt(irq_serial);
}

// Writes one byte to the TX serial buffer. Called by main program.
// TODO: Check if we can speed this up for writing strings, rather than single bytes.

void write(uint8_t data) {
	Serial.write(data);

}

void write(uint8_t *data) {
  Serial.write(data);

}

void write(char * data) {
  Serial.write(data);
}

void write(uint8_t *buffer, size_t size) {
	Serial.write(buffer, size);
}

void serial_ln() {
	Serial.println("");
}

// Fetches the first byte in the serial read buffer. Called by main program.
uint8_t read()
{
	uint16_t tail = serial_rx_buffer_tail; // Temporary serial_rx_buffer_tail (to optimize for volatile)
	if (serial_rx_buffer_head == tail) {
		return SERIAL_NO_DATA;
	}
	else {
		uint8_t data = serial_rx_buffer[tail];
		tail++;
		if (tail == RX_BUFFER_SIZE) { tail = 0; }
		serial_rx_buffer_tail = tail;
		return data;
	}
}

void reset_read_buffer()
{
	serial_rx_buffer_tail = serial_rx_buffer_head;
}

