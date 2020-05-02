/*
Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "LiquidCrystal.h"
#include "macrosStrings.h"
#include "customKeyCodes.h"

void displayMacro(uint8_t mode) {
	LiquidCrystalHome();
	LiquidCrystalPrintNChars(macroNames[mode], 8);
	LiquidCrystalSetCursor(40,0);
	LiquidCrystalPrintNChars(macroNames[mode]+8, 8);
}

uint8_t macroMode=0;

bool custom_process_record_user(uint16_t keycode, keyrecord_t *record) {
    // here we define what to do when a key is pressed
    switch (keycode)
    {
        case CUSTKC_MODE:
        if (record->event.pressed) {
		macroMode=0;
            displayMacro(0);
        }
        break;
        case CUSTKC_RESET:
        if (record->event.pressed) {
		macroMode=1;
            displayMacro(1);
        }
        break;
        case CUSTKC_CONTRAST:
        if (record->event.pressed) {
		macroMode=2;
            displayMacro(2);
        }
        break;
        case CUSTKC_FUNCTION:
        if (record->event.pressed) {
		macroMode=3;
            displayMacro(3);
        }
        break;

        case CUSTKC_START:
        if (record->event.pressed) {
            SEND_STRING(macros[macroMode]);
        }
        break;

    default:
	return true;
        break;
    }
    return false;
};

