/*!
*   Custom character usage example for Sequent Microsystems Six-in-one LCD Adapter Kit for Raspberry Pi with a 20X4 LCD 
*   -->https://sequentmicrosystems.com/products/six-in-one-lcd-adapter-kit-using-2004-or-1602-lcds-for-raspberry-pi
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
* This example demonstrate custom character ceation.
* Based on Arduino's example at
* https://docs.arduino.cc/learn/electronics/lcd-displays#custom-character	
*	
*
*/

// include the library code:
#include "SM_LCDAdapter.h"

SM_LCDAdapter lcd = SM_LCDAdapter();

// make some custom characters:
byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

byte smiley[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b10001,
  0b01110,
  0b00000
};

byte frownie[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b00000,
  0b00000,
  0b00000,
  0b01110,
  0b10001
};

byte armsDown[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b00100,
  0b01110,
  0b10101,
  0b00100,
  0b01010
};

byte armsUp[8] = {
  0b00100,
  0b01010,
  0b00100,
  0b10101,
  0b01110,
  0b00100,
  0b00100,
  0b01010
};

void setup() {
  Serial.begin (115200);
  // initialize LCD and set up the number of columns and rows:
  lcd.begin(20, 4);

  // Set the LCD backlight.
  lcd.writeBl(20);
  
  // create a new character
  lcd.createChar(0, heart);
  // create a new character
  lcd.createChar(1, smiley);
  // create a new character
  lcd.createChar(2, frownie);
  // create a new character
  lcd.createChar(3, armsDown);
  // create a new character
  lcd.createChar(4, armsUp);

  // set the cursor to the top left
  lcd.setCursor(0, 0);

  // Print a message to the lcd.
  lcd.print("I ");
  lcd.write(byte(0)); // when calling lcd.write() '0' must be cast as a byte
  lcd.print(" Arduino! ");
  lcd.write((byte)1);

}

void loop() {
  // read the potentiometer on A0:
  int sensorReading = analogRead(A0);
  // map the result to 200 - 1000:
  int delayTime = map(sensorReading, 0, 1023, 200, 1000);
  // set the cursor to the bottom row, 5th position:
  lcd.setCursor(4, 1);
  // draw the little man, arms down:
  lcd.write(3);
  delay(delayTime);
  lcd.setCursor(4, 1);
  // draw him arms up:
  lcd.write(4);
  delay(delayTime);
}
