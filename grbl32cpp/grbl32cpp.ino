#include "xc.h"
#include "p32mx795f512l.h"
#include "grbl.h"

volatile uint32_t old_limit_port_value = 0;
volatile uint32_t new_limit_port_value = 0;

void __USER_ISR irq_change_notification() {
	old_limit_port_value = new_limit_port_value;
	new_limit_port_value = CONTROL_PORT->port.reg;
	clearIntFlag(_CHANGE_NOTICE_IRQ);
}

void setup()
{

	
  /* add setup code here */
	
}

void loop()
{

  /* add main program code here */

}
