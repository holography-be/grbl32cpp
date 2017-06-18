/*
print.h - Functions for formatting output strings
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

#ifndef grbl_print_h
#define grbl_print_h

#include "grbl.h"
#include "HardwareSerial.h"

class Cprint {
private:
public:
	void String(const char *s);

	void Stringln(const char *s);

	void PgmString(const char *s);

	void Integer(long n);

	void uint32_base10(uint32_t n);

// Prints uint8 variable with base and number of desired digits.
	void unsigned_int8(uint8_t n, uint8_t base, uint8_t digits);

// Prints an uint8 variable in base 2.
	void uint8_base2(uint8_t n);

// Prints an uint8 variable in base 10.
	void uint8_base10(uint8_t n);

	void Float(float n, uint8_t decimal_places);

// Floating value printing handlers for special variables types used in Grbl. 
//  - CoordValue: Handles all position or coordinate values in inches or mm reporting.
//  - RateValue: Handles feed rate and current velocity in inches or mm reporting.
//  - SettingValue: Handles all floating point settings values (always in mm.)
	void Float_CoordValue(float n);

	void Float_RateValue(float n);

	void Float_SettingValue(float n);

// Debug tool to print free memory in bytes at the called point. Not used otherwise.
	void FreeMemory();


};

extern Cprint Print_grbl;


#endif