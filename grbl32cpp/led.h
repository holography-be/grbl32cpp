// led.h

#ifndef _LED_h
#define _LED_h

#include "grbl.h"

class Cled {
	private:

	public:
		void init();
		void on(uint32_t led);
		void off(uint32_t led);
		void toggle(uint32_t led);

};

extern Cled Led;

#endif

