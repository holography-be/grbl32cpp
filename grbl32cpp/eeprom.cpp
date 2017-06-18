
#include "eeprom.h"

// global objetcs



/*! \brief  Read byte from EEPROM. */
unsigned char Ceeprom::get_char(unsigned int addr)
{
	return _eeprom.read(addr);
}

/*! \brief  Write byte to EEPROM. */
void Ceeprom::put_char(unsigned int addr, unsigned char new_value)
{
	_eeprom.write(addr, new_value);
}

void Ceeprom::memcpy_to_eeprom_with_checksum(unsigned int destination, char *source, unsigned int size) {
	_eeprom.writeChars(destination, source, size);
}

int Ceeprom::memcpy_from_eeprom_with_checksum(char *destination, unsigned int source, unsigned int size) {
	_eeprom.readChars(source, destination, size);
	return 1;
}

