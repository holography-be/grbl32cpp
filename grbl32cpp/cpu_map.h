/*
cpu_map.h - CPU and pin mapping configuration file
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

/* The cpu_map.h files serve as a central pin mapping selection file for different processor
types, i.e. AVR 328p or AVR Mega 2560. Each processor has its own pin mapping file.
(i.e. cpu_map_atmega328p.h)  Grbl officially supports the Arduino Uno, but the
other supplied pin mappings are supplied by users, so your results may vary. */

// NOTE: With new processors, only add the define name and filename to use.

#ifndef cpu_map_h
#define cpu_map_h

#include "cpu_map\cpu_map_ChipKitMax32.h"


#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 64
#endif

#define SERIAL_NO_DATA 0xff

#ifdef ENABLE_XONXOFF
  #define RX_BUFFER_FULL 96 // XOFF high watermark
  #define RX_BUFFER_LOW 64 // XON low watermark
  #define SEND_XOFF 1
  #define SEND_XON 2
  #define XOFF_SENT 3
  #define XON_SENT 4
  #define XOFF_CHAR 0x13
  #define XON_CHAR 0x11
#endif

#endif
