/*!
*   Scroll display usage example for Sequent Microsystems Six-in-one LCD Adapter Kit for Raspberry Pi with a 20X4 LCD 
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
*This example demonstrate display scrolling controlled by the rotary encoder. Press SW6 to see aouto scrolling.	
*	
*
*/

#include "SM_LCDAdapter.h"

SM_LCDAdapter lcd = SM_LCDAdapter();

void rightErase4Line(void)
{
  int i = 0;
  for(i = 0; i< 20; i++)
  {
    lcd.setCursor(19 - i, 0);
    lcd.print(" ");
    lcd.setCursor(19 - i, 1);
    lcd.print(" ");
    lcd.setCursor(19- i, 2);
    lcd.print(" ");
    lcd.setCursor(19- i, 3);
    lcd.print(" ");
    lcd.scrollDisplayRight();
    delay(300);
  }
  lcd.home();
}

void setup() {
  Serial.begin (115200);
  delay(500);
  lcd.writeBl(20);


  lcd.begin(20, 4);

  Serial.println("Sequent display init successful");
  lcd.setCursor(1, 0);
  lcd.print("FOUR LINES 20 CHAR");
  lcd.setCursor(0, 1);
  lcd.print("ENCODER & 6 SWITCHES");
  lcd.setCursor(1, 2);
  lcd.print("USING ESP32-PI FOR");
  lcd.setCursor(0, 3);
  lcd.print("COMPLEX EMBEDDED APP");
  delay(3000);
  rightErase4Line();
  
  lcd.setCursor(10, 0);
  lcd.print("#");
  lcd.setCursor(9, 1);
  lcd.print("###");
  lcd.setCursor(9, 2);
  lcd.print("###");
  lcd.setCursor(10, 3);
  lcd.print("#");

  lcd.resetEncoder();
}

int scroll = 0;
void loop() {
  int encoder = 0;

  encoder = lcd.readEncoder();
  if (encoder < scroll)
  {
    lcd.scrollDisplayRight();
  }
  if (encoder > scroll)
  {
    lcd.scrollDisplayLeft();
  }
  scroll = encoder;

  if(lcd.readButtonLatch(6))
  {
    setup();
  }
  delay(100);
}