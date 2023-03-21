extern "C"
{
#include <inttypes.h>
}

#include "Arduino.h"
#include "Wire.h"
#include "SM_LCDAdapter.h"

SM_LCDAdapter::SM_LCDAdapter(uint8_t intPin)
{
  _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
  _interruptPin = intPin;
}


void SM_LCDAdapter::begin(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;

  setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }
  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40 ms after power rises above 2.7 V
  // before sending commands. Arduino can turn on way before 4.5 V so we'll wait 50
  delayMicroseconds(50000);

  // this is according to the Hitachi HD44780 datasheet
  // page 45 figure 23

  // Send function set command sequence
  command(LCD_FUNCTIONSET | _displayfunction);
  delayMicroseconds(4500);  // wait more than 4.1 ms

  // second try
  command(LCD_FUNCTIONSET | _displayfunction);
  delayMicroseconds(150);

  // third go
  command(LCD_FUNCTIONSET | _displayfunction);


  // finally, set # lines, font size, etc.
  command(LCD_FUNCTIONSET | _displayfunction);

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  display();

  // clear it off
  clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  command(LCD_ENTRYMODESET | _displaymode);

}
void SM_LCDAdapter::setRowOffsets(int row0, int row1, int row2, int row3)
{
  _row_offsets[0] = row0;
  _row_offsets[1] = row1;
  _row_offsets[2] = row2;
  _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void SM_LCDAdapter::clear()
{
  command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void SM_LCDAdapter::home()
{
  command(LCD_RETURNHOME);  // set cursor position to zero
  delayMicroseconds(2000);  // this command takes a long time!
}

void SM_LCDAdapter::setCursor(uint8_t col, uint8_t row)
{
  const size_t max_lines = sizeof(_row_offsets) / sizeof(*_row_offsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/ 0
  }
  if ( row >= _numlines ) {
    row = _numlines - 1;    // we count rows starting w/ 0
  }

  command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void SM_LCDAdapter::noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void SM_LCDAdapter::display() {
  _displaycontrol |= LCD_DISPLAYON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void SM_LCDAdapter::noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void SM_LCDAdapter::cursor() {
  _displaycontrol |= LCD_CURSORON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void SM_LCDAdapter::noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void SM_LCDAdapter::blink() {
  _displaycontrol |= LCD_BLINKON;
  command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void SM_LCDAdapter::scrollDisplayLeft(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void SM_LCDAdapter::scrollDisplayRight(void) {
  command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void SM_LCDAdapter::leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void SM_LCDAdapter::rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void SM_LCDAdapter::autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void SM_LCDAdapter::noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void SM_LCDAdapter::createChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  command(LCD_SETCGRAMADDR | (location << 3));
  for (int i = 0; i < 8; i++) {
    write(charmap[i]);
  }
}


void SM_LCDAdapter::writeBl(uint8_t value)
{
  if (value > 100)
    value = 100;
  writeByteI2C(SM_LCD_I2C_LCD_BL, value);
}

void SM_LCDAdapter::writeRedBl(uint8_t value)
{
  if (value > 100)
    value = 100;
  writeByteI2C(SM_LCD_I2C_LCD_BL, value);
}
void SM_LCDAdapter::writeGreenBl(uint8_t value)
{
  if (value > 100)
    value = 100;
  writeByteI2C(SM_LCD_I2C_LCD_BL_G, value);
}

void SM_LCDAdapter::writeBlueBl(uint8_t value)
{
  if (value > 100)
    value = 100;
  writeByteI2C(SM_LCD_I2C_LCD_BL_B, value);
}

uint8_t SM_LCDAdapter::readButton(uint8_t button)
{
  uint8_t aux = 0;
  uint8_t ret = 0;

  if ((button < 7) && (button > 0) && (0 == readByteI2C(SM_LCD_I2C_SW_MOM_ADD, &aux)))
  {
    if (aux & (uint8_t)(1 << (button - 1)))
    {
      ret = 1;
    }
  }
  return ret;
}
uint8_t SM_LCDAdapter::readButton()
{
  uint8_t aux = 0;
  readByteI2C(SM_LCD_I2C_SW_MOM_ADD, &aux);
  return aux;
}
int SM_LCDAdapter::readEncoder()
{
  int16_t encoder = 0;

  readS16I2C(SM_LCD_I2C_ROTARY_VAL, &encoder);
  return encoder;
}

void SM_LCDAdapter::resetEncoder()
{
  writeByteI2C(SM_LCD_I2C_ROTARY_RESET, RESET_ROTARY_KEY);
}
uint8_t SM_LCDAdapter::readButtonLatch()
{
  uint8_t aux = 0;
  uint8_t ret = 0;

  readByteI2C(SM_LCD_I2C_SW_INT_ADD, &aux);
  ret = 0x3f &  aux;

  aux &= 0x80;
  writeByteI2C(SM_LCD_I2C_SW_INT_ADD, aux);
  return ret;
}

uint8_t SM_LCDAdapter::readButtonLatch(uint8_t button)
{
  uint8_t aux = 0;
  uint8_t ret = 0;
  if (button > 0 && button < 7)
  {
    readByteI2C(SM_LCD_I2C_SW_INT_ADD, &aux);
    if ((aux & (uint8_t)(1 << (button - 1))) != 0)
    {
      ret = 1;
      aux &= ~(uint8_t)(1 << (button - 1));
      writeByteI2C(SM_LCD_I2C_SW_INT_ADD, aux);
    }
  }
  return ret;
}
uint8_t SM_LCDAdapter::readEncoderChangeInt()
{
  uint8_t aux = 0;
  uint8_t ret = 0;

  readByteI2C(SM_LCD_I2C_SW_INT_ADD, &aux);
  if (aux & 0x80)
  {
    ret = 1;
  }
  if (ret)
  {
    aux &= 0x7f;
    writeByteI2C(SM_LCD_I2C_SW_INT_ADD, aux);
  }
  return ret;
}
uint8_t SM_LCDAdapter::readInterrupt()
{
  uint8_t aux = 0;
  uint8_t ret = 0;

  readByteI2C(SM_LCD_I2C_SW_INT_ADD, &aux);
  ret =  aux;

  aux = 0;
  writeByteI2C(SM_LCD_I2C_SW_INT_ADD, aux);
  return ret;
}

/*********** mid level commands, for sending data/cmds */

inline void SM_LCDAdapter::command(uint8_t value) {
  uint8_t buff[2];

  buff[0] = value;
  buff[1] = SM_LCD_INST_WR;
  writeBuffI2C(SM_LCD_I2C_LCD_DATA, buff, 2);
}

inline size_t SM_LCDAdapter::write(uint8_t value) {
  uint8_t buff[2];

  buff[0] = value;
  buff[1] = SM_LCD_DATA_WR;
  if (0 == writeBuffI2C(SM_LCD_I2C_LCD_DATA, buff, 2))
  {
    return 1;
  }
  return 0;
}


/***************** SM_LCD_I2C access functions *********************************
 *  *********************************************************************
*/
int SM_LCDAdapter::writeByteI2C(uint8_t add, uint8_t value)
{
  Wire.begin();
  Wire.beginTransmission(SM_LCD_I2C_HW_ADDRESS);
  Wire.write(add);
  Wire.write(value);
  return Wire.endTransmission();
}

int SM_LCDAdapter::writeBuffI2C(uint8_t add, uint8_t* buff, uint8_t size)
{
  uint8_t i;
  Wire.begin();
  Wire.beginTransmission(SM_LCD_I2C_HW_ADDRESS);
  Wire.write(add);
  for (i = 0; i < size; i++)
  {
    Wire.write(buff[i]);
  }
  return Wire.endTransmission();

}

int SM_LCDAdapter::readByteI2C(uint8_t add, uint8_t* value)
{
  if (0 == value)
  {
    return -1;
  }
  Wire.begin();
  Wire.beginTransmission(SM_LCD_I2C_HW_ADDRESS);
  Wire.write(add);
  if (Wire.endTransmission() != 0)
  {
    return -1;
  }
  Wire.requestFrom(SM_LCD_I2C_HW_ADDRESS, (uint8_t)1);
  if (1 <= Wire.available())
  {
    *value = Wire.read();
  }
  else
  {
    return -1;
  }
  return 0;
}

int SM_LCDAdapter::readS16I2C(uint8_t add, int16_t* value)
{
  uint8_t buff[2];

  if (0 == value)
  {
    return -1;
  }
  Wire.begin();
  Wire.beginTransmission(SM_LCD_I2C_HW_ADDRESS);
  Wire.write(add);
  if (Wire.endTransmission() != 0)
  {
    return -1;
  }
  Wire.requestFrom(SM_LCD_I2C_HW_ADDRESS, (uint8_t)2);
  if (2 <= Wire.available())
  {
    buff[0] = Wire.read();
    buff[1] = Wire.read();
  }
  else
  {
    return -1;
  }
  memcpy(value, buff, 2);
  return 0;
}
