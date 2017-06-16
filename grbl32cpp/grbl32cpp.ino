#include "xc.h"
#include "p32mx795f512l.h"

#include "LiquidCrystal_I2C.h"


void setup()
{
	LiquidCrystal_I2C lcd(0x27, 2, 4);
	lcd.init();
	lcd.begin(20, 2, 8);
	
  /* add setup code here */
	
}

void loop()
{

  /* add main program code here */

}
