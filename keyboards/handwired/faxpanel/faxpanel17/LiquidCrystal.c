#include "LiquidCrystal.h"
#include "wait.h"
#include "hardware.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

// Adapted from the LiquidCrystalOverI2C fork of the official Arduino LiquidCrystal library
// Stupid c++ wasn't building, turned that class into pure c

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//

  uint8_t liquidcrystaldisplayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
  uint8_t liquidcrystaldisplaycontrol;
  uint8_t liquidcrystaldisplaymode;

  uint8_t liquidcrystalnumlines;
  uint8_t liquidcrystalrowoffsets[4];

/************ low level data pushing commands **********/

void LiquidCrystalWrite4bits(uint8_t value, uint8_t mode) {
  uint8_t bits = (value & 0x0F);
  write374(bits | mode | 16);
  write374(bits | mode | 16| EN_PIN);
  wait_us(1); // enable pulse must be >450ns
  write374(bits | mode | 16);
  wait_us(50); // commands need > 37us to settle
}

// write either command or data
void LiquidCrystalSend(uint8_t value, uint8_t mode) {
  LiquidCrystalWrite4bits(value >> 4, mode);
  LiquidCrystalWrite4bits(value, mode);
}

/*********** mid level commands, for sending data/cmds */

void LiquidCrystalCommand(uint8_t value) {
  LiquidCrystalSend(value, 0);
}

uint8_t LiquidCrystalWrite(uint8_t value) {
  LiquidCrystalSend(value, RS_PIN);
  return 1; // assume sucess
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void LiquidCrystalCreateChar(uint8_t location, uint8_t charmap[]) {
  location &= 0x7; // we only have 8 locations 0-7
  LiquidCrystalCommand(LCD_SETCGRAMADDR | (location << 3));
  for (int i=0; i<8; i++) {
    LiquidCrystalWrite(charmap[i]);
  }
}

/************                                 **********/

void LiquidCrystalBegin(uint8_t cols, uint8_t lines) {
    LiquidCrystalBeginWithCharSize(cols, lines, LCD_5x8DOTS);
}

void LiquidCrystalBeginWithCharSize(uint8_t cols, uint8_t lines, uint8_t dotsize) {
  if (lines > 1) {
    liquidcrystaldisplayfunction |= LCD_2LINE;
  }
  liquidcrystalnumlines = lines;

  LiquidCrystalSetRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

  // for some 1 line displays you can select a 10 pixel high font
  if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
    liquidcrystaldisplayfunction |= LCD_5x10DOTS;
  }

  // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
  // according to datasheet, we need at least 40ms after power rises above 2.7V
  // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
  wait_ms(50);

  //put the LCD into 4 bit  mode
  // this is according to the hitachi HD44780 datasheet
  // figure 24, pg 46

  // we start in 8bit mode, try to set 4 bit mode
  LiquidCrystalWrite4bits(0x03, 0);
  wait_us(4500);

  // second try
  LiquidCrystalWrite4bits(0x03, 0);
  wait_us(4500);

  // third go!
  LiquidCrystalWrite4bits(0x03, 0);
  wait_us(150);

  // finally, set to 4-bit interface
  LiquidCrystalWrite4bits(0x02, 0);

  // finally, set # lines, font size, etc.
  LiquidCrystalCommand(LCD_FUNCTIONSET | liquidcrystaldisplayfunction);

  // turn the display on with no cursor or blinking default
  liquidcrystaldisplaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
  LiquidCrystalDisplay();

  // clear it off
  LiquidCrystalClear();

  // Initialize to default text direction (for romance languages)
  liquidcrystaldisplaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  LiquidCrystalCommand(LCD_ENTRYMODESET | liquidcrystaldisplaymode);

}

void LiquidCrystalSetRowOffsets(int row0, int row1, int row2, int row3)
{
  liquidcrystalrowoffsets[0] = row0;
  liquidcrystalrowoffsets[1] = row1;
  liquidcrystalrowoffsets[2] = row2;
  liquidcrystalrowoffsets[3] = row3;
}

/********** high level commands, for the user! */
void LiquidCrystalClear(void)
{
  LiquidCrystalCommand(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
  wait_us(2000);
}

void LiquidCrystalHome(void)
{
  LiquidCrystalCommand(LCD_RETURNHOME);  // set cursor position to zero
  wait_us(2000);
}

void LiquidCrystalSetCursor(uint8_t col, uint8_t row)
{
  const uint8_t max_lines = sizeof(liquidcrystalrowoffsets) / sizeof(*liquidcrystalrowoffsets);
  if ( row >= max_lines ) {
    row = max_lines - 1;    // we count rows starting w/0
  }
  if ( row >= liquidcrystalnumlines ) {
    row = liquidcrystalnumlines - 1;    // we count rows starting w/0
  }

  LiquidCrystalCommand(LCD_SETDDRAMADDR | (col + liquidcrystalrowoffsets[row]));
}

void LiquidCrystalPrint(const char* fmt, ...) {
    for (int i=0; fmt[i]!=0; i++) {
        LiquidCrystalWrite(fmt[i]);
    }
}
// Turn the display on/off (quickly)
void LiquidCrystalNoDisplay(void) {
  liquidcrystaldisplaycontrol &= ~LCD_DISPLAYON;
  LiquidCrystalCommand(LCD_DISPLAYCONTROL | liquidcrystaldisplaycontrol);
}
void LiquidCrystalDisplay(void) {
  liquidcrystaldisplaycontrol |= LCD_DISPLAYON;
  LiquidCrystalCommand(LCD_DISPLAYCONTROL | liquidcrystaldisplaycontrol);
}

// Turns the underline cursor on/off
void LiquidCrystalNoCursor(void) {
  liquidcrystaldisplaycontrol &= ~LCD_CURSORON;
  LiquidCrystalCommand(LCD_DISPLAYCONTROL | liquidcrystaldisplaycontrol);
}
void LiquidCrystalCursor(void) {
  liquidcrystaldisplaycontrol |= LCD_CURSORON;
  LiquidCrystalCommand(LCD_DISPLAYCONTROL | liquidcrystaldisplaycontrol);
}

// Turn on and off the blinking cursor
void LiquidCrystalNoBlink(void) {
  liquidcrystaldisplaycontrol &= ~LCD_BLINKON;
  LiquidCrystalCommand(LCD_DISPLAYCONTROL | liquidcrystaldisplaycontrol);
}
void LiquidCrystalBlink(void) {
  liquidcrystaldisplaycontrol |= LCD_BLINKON;
  LiquidCrystalCommand(LCD_DISPLAYCONTROL | liquidcrystaldisplaycontrol);
}

// Turn the backlight on
void LiquidCrystalNoBacklight(void) {
}

// Turn the backlight off
void LiquidCrystalBacklight(void) {
}

// Set the brightness level (0-3) for supported VFD displays
void LiquidCrystalSetBrightness(uint8_t value) {
  liquidcrystaldisplayfunction &= ~0x03;
  liquidcrystaldisplayfunction |= (value & 0x03);
  LiquidCrystalCommand(LCD_FUNCTIONSET | liquidcrystaldisplayfunction);
}

// These commands scroll the display without changing the RAM
void LiquidCrystalScrollDisplayLeft(void) {
  LiquidCrystalCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void LiquidCrystalScrollDisplayRight(void) {
  LiquidCrystalCommand(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void LiquidCrystalLeftToRight(void) {
  liquidcrystaldisplaymode |= LCD_ENTRYLEFT;
  LiquidCrystalCommand(LCD_ENTRYMODESET | liquidcrystaldisplaymode);
}

// This is for text that flows Right to Left
void LiquidCrystalRightToLeft(void) {
  liquidcrystaldisplaymode &= ~LCD_ENTRYLEFT;
  LiquidCrystalCommand(LCD_ENTRYMODESET | liquidcrystaldisplaymode);
}

// This will 'right justify' text from the cursor
void LiquidCrystalAutoscroll(void) {
  liquidcrystaldisplaymode |= LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystalCommand(LCD_ENTRYMODESET | liquidcrystaldisplaymode);
}

// This will 'left justify' text from the cursor
void LiquidCrystalNoAutoscroll(void) {
  liquidcrystaldisplaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  LiquidCrystalCommand(LCD_ENTRYMODESET | liquidcrystaldisplaymode);
}

