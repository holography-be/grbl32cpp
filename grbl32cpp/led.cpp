// 
// 
// 

#include "led.h"

void led::init()
{
	LED_PORT->TRISxCLR.w = LED_MASK;
	LED_PORT->ODCxCLR.w = LED_MASK;
	//printStringln("Led init");
}

void led::on(uint32_t led) {
	LED_PORT->LATxSET.w = led;
}

void led::off(uint32_t led) {
	LED_PORT->LATxCLR.w = led;
}

void led::toggle(uint32_t led) {
	LED_PORT->LATxINV.w = led;
}