/*!
*   Text direction example for Sequent Microsystems Six-in-one LCD Adapter Kit for Raspberry Pi with a 20X4 LCD 
*   -->https://sequentmicrosystems.com
*   Requirments: Any arduino card with I2C, Six-in-one LCD Adapter Kit from Sequent Microsystems
*
*   Method 1:
*     Connect I2C bus, 5V and GND from Arduino card to Home automation hat, pin map below
*    SIGNAL  CONNECTOR    SIGNAL\n
*     ---------------------------
*          -- 1|O O| 2--  +5V
*  I2C SDA -- 3|O O| 4--  +5V
*  I2C SCL -- 5|O O| 6--  GND
*          -- 7|O O| 8--
*      GND -- 9|O O|10--
*          --11|O O|12--
*          --13|O O|14--  GND
*          --15|O O|16--
*          --17|O O|18--
*          --19|O O|20--  GND
*          --21|O O|22--
*          --23|O O|24--
*      GND --25|O O|26--
*          --27|O O|28--
*          --29|O O|30--  GND
*          --31|O O|32--
*          --33|O O|34--  GND
*          --35|O O|36--
*          --37|O O|38--
*      GND --39|O O|40--
*
*   Method 2:
*   Use sequent Microsystems Uno, Nano, Teensy,Feather, ESP32 Raspberry Pi Replacement Kit with prefered arduino processor
*   -->https://sequentmicrosystems.com/products/raspberry-pi-replacement-card
*
*   Method 3:
*    Use Sequent Microsysstems ESP32 Pi replacement connected directly with the Home Automation card.
*    Set the board type to DOIT ESP32 DEVKIT V1: Tool >> Board >> ESP32 Arduino >> DOIT ESP32 DEVKIT V1
*  
*   Based on Arduino's Text Direction Example at
*   http://www.arduino.cc/en/Tutorial/LiquidCrystalTextDirection
*/

#include "SM_LCDAdapter.h"

SM_LCDAdapter lcd = SM_LCDAdapter();
int thisChar = 'a';

void setup() {

  // set up the LCD's number of columns and rows:

  lcd.begin(20, 4);

  // turn on the cursor:

  lcd.cursor();
}

void loop() {

  // reverse directions at 'm':

  if (thisChar == 'm') {

    // go right for the next letter

    lcd.rightToLeft();

  }

  // reverse again at 's':

  if (thisChar == 's') {

    // go left for the next letter

    lcd.leftToRight();

  }

  // reset at 'z':

  if (thisChar > 'z') {

    // go to (0,0):

    lcd.home();

    // start again at 0

    thisChar = 'a';

  }

  // print the character

  lcd.write(thisChar);

  // wait a second:

  delay(1000);

  // increment the letter:

  thisChar++;
}
