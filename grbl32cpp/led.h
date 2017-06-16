// led.h

#ifndef _LED_h
#define _LED_h

#include "grbl.h"

class led {
	private:

	public:
		static void init();
		static void on(uint32_t led);
		static void off(uint32_t led);
		static void toggle(uint32_t led);

};


#endif

