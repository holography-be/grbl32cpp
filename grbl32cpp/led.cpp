// 
// 
// 

#include "led.h"

void Cled::init()
{
	LED_PORT->TRISxCLR.w = LED_MASK;
	LED_PORT->ODCxCLR.w = LED_MASK;
	//printStringln("Led init");
}

void Cled::on(uint32_t led) {
	LED_PORT->LATxSET.w = led;
}

void Cled::off(uint32_t led) {
	LED_PORT->LATxCLR.w = led;
}

void Cled::toggle(uint32_t led) {
	LED_PORT->LATxINV.w = led;
}