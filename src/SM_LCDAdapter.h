#ifndef SM_LCD_H_
#define SM_LCD_H_

#include "Arduino.h"
#include <inttypes.h>
#include "Print.h"

// commands
#define LCD_CLEARDISPLAY 0x01
#define LCD_RETURNHOME 0x02
#define LCD_ENTRYMODESET 0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT 0x10
#define LCD_FUNCTIONSET 0x20
#define LCD_SETCGRAMADDR 0x40
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT 0x00
#define LCD_ENTRYLEFT 0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON 0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON 0x02
#define LCD_CURSOROFF 0x00
#define LCD_BLINKON 0x01
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE 0x00
#define LCD_MOVERIGHT 0x04
#define LCD_MOVELEFT 0x00

// flags for function set
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

#define SM_LCD_I2C_HW_ADDRESS ((uint8_t)0x08)

#define SM_LCD_I2C_LCD_DATA 0x06
#define SM_LCD_I2C_LCD_CMD 0x07
#define SM_LCD_I2C_LCD_BL 0x08
#define SM_LCD_I2C_LCD_BL_G 0x09
#define SM_LCD_I2C_LCD_BL_B 0x0a
#define SM_LCD_I2C_ROTARY_VAL 0x0b
#define SM_LCD_I2C_ROTARY_RESET 0x0d
#define SM_LCD_I2C_SW_MOM_ADD 0x03
#define SM_LCD_I2C_SW_INT_ADD 0x04

#define RESET_ROTARY_KEY 0xaa

// access modes
#define SM_LCD_INST_WR 0x11
#define SM_LCD_BUSY_RD 0x22
#define SM_LCD_DATA_WR 0x33
#define SM_LCD_DATA_RD 0x44


class SM_LCDAdapter : public Print {
  public:
    SM_LCDAdapter(uint8_t intPin = 27);

    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS);
    void clear();
    void home();

    void noDisplay();
    void display();
    void noBlink();
    void blink();
    void noCursor();
    void cursor();
    void scrollDisplayLeft();
    void scrollDisplayRight();
    void leftToRight();
    void rightToLeft();
    void autoscroll();
    void noAutoscroll();
    void setRowOffsets(int row1, int row2, int row3, int row4);
    void createChar(uint8_t, uint8_t[]);
    void setCursor(uint8_t, uint8_t);
    
    void writeBl(uint8_t value);
    void writeRedBl(uint8_t value);
    void writeGreenBl(uint8_t value);
    void writeBlueBl(uint8_t value);
    uint8_t readButton(uint8_t button);
    uint8_t readButton();
    int readEncoder();
    void resetEncoder();
    uint8_t readButtonLatch();
    uint8_t readButtonLatch(uint8_t button);
    uint8_t readEncoderChangeInt();
    uint8_t readInterrupt();
    virtual size_t write(uint8_t);
    void command(uint8_t);

    using Print::write;
  private:
    bool _detected;
    int writeByteI2C(uint8_t add, uint8_t value);
    int writeBuffI2C(uint8_t add, uint8_t* buff, uint8_t size);
    int readByteI2C(uint8_t add, uint8_t* value);
    int readS16I2C(uint8_t add, int16_t* value);
    uint8_t _displayfunction;
    uint8_t _displaycontrol;
    uint8_t _displaymode;

    uint8_t _initialized;

    uint8_t _numlines;
    uint8_t _row_offsets[4];
    uint8_t _interruptPin;
};







#endif //SM_LCD_H_
