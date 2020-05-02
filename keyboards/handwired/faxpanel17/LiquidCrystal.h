#pragma once

#include <inttypes.h>
#include "print.h"

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
#define LCD_4BITMODE 0x00
#define LCD_2LINE 0x08
#define LCD_1LINE 0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS 0x00

// flags for backlight control (implemented by backpack, not part of the HD44780 protocol)
#define LCD_BACKLIGHTON 0x08
#define LCD_BACKLIGHTOFF 0x00

// flags for brightness control (for compatible VFD displays)
#define VFD_BRIGHT_25 0x03
#define VFD_BRIGHT_50 0x02
#define VFD_BRIGHT_75 0x01
#define VFD_BRIGHT_100 0x00

// lcd pins
#define EN_PIN 64
#define RS_PIN 128

  void LiquidCrystalBegin(uint8_t cols, uint8_t rows);
  void LiquidCrystalBeginWithCharSize(uint8_t cols, uint8_t rows, uint8_t charsize);

  void LiquidCrystalClear(void);
  void LiquidCrystalHome(void);

  void LiquidCrystalPrintNChars(const char* fmt, int num);
  void LiquidCrystalPrint(const char* fmt, ...);
  void LiquidCrystalNoDisplay(void);
  void LiquidCrystalDisplay(void);
  void LiquidCrystalNoBlink(void);
  void LiquidCrystalBlink(void);
  void LiquidCrystalNoBacklight(void);
  void LiquidCrystalBacklight(void);
  void LiquidCrystalNoCursor(void);
  void LiquidCrystalCursor(void);
  void LiquidCrystalScrollDisplayLeft(void);
  void LiquidCrystalScrollDisplayRight(void);
  void LiquidCrystalLeftToRight(void);
  void LiquidCrystalRightToLeft(void);
  void LiquidCrystalAutoscroll(void);
  void LiquidCrystalNoAutoscroll(void);
  void LiquidCrystalSetBrightness(uint8_t);

  void LiquidCrystalSetRowOffsets(int row1, int row2, int row3, int row4);
  void LiquidCrystalCreateChar(uint8_t, uint8_t[]);
  void LiquidCrystalSetCursor(uint8_t, uint8_t);


