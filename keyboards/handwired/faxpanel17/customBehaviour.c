/*
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
#include <string.h>
#include "LiquidCrystal.h"
#include "macrosStrings.h"
#include "customKeyCodes.h"
#include "pin.h"

#include <stdio.h>

bool locked = true;

void displayString(const char *string) {
    LiquidCrystalHome();
    LiquidCrystalPrintNChars(string, 8);
    LiquidCrystalSetCursor(40, 0);
    LiquidCrystalPrintNChars(string + 8, 8);
}

void displayMacroName(uint8_t mode) { displayString(macroNames[mode]); }

int8_t  macroIndex = 0;
uint8_t customString[16];

uint8_t enteredPIN[MAXPINLENGTH];  // max length of PIN
uint8_t enteredPINIndex = 0;

uint8_t pinMatches(void) {
    if (enteredPINIndex != strlen(FAXPANELPIN)) return false;
    return !(strncmp(FAXPANELPIN, (char *)enteredPIN, strlen(FAXPANELPIN)));
}

void clearCustomString(void) { memset(customString, ' ', sizeof(customString)); }

uint8_t keycodeToDigit(uint16_t keycode) {
    switch (keycode) {
        case KC_0:
            return 0;
        case KC_1:
            return 1;
        case KC_2:
            return 2;
        case KC_3:
            return 3;
        case KC_4:
            return 4;
        case KC_5:
            return 5;
        case KC_6:
            return 6;
        case KC_7:
            return 7;
        case KC_8:
            return 8;
        case KC_9:
            return 9;
    }
    return 0;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // here we define what to do when a key is pressed
    if (locked) {
        if (record->event.pressed) {
            switch (keycode) {
                case CUSTKC_START:
                case CUSTKC_CONTRAST:
                case CUSTKC_MODE:
                case CUSTKC_RESET:
                case CUSTKC_FUNCTION:
                case KC_ASTERISK:
                case KC_HASH:
                    break;
                default:
                    enteredPIN[enteredPINIndex] = keycodeToDigit(keycode) + '0';
                    if (enteredPINIndex == 0) clearCustomString();
                    customString[enteredPINIndex] = '*';
                    enteredPINIndex++;
                    displayString((char *)customString);
                    break;
            }
        } else {
            switch (keycode) {
                case CUSTKC_RESET:
                    enteredPINIndex = 0;
                case CUSTKC_START:
                case CUSTKC_CONTRAST:
                case CUSTKC_MODE:
                case CUSTKC_FUNCTION:
                case KC_ASTERISK:
                case KC_HASH:
                    if (enteredPINIndex == 0) {
                        displayString("Locked enter PIN");
                    }
                    break;
                default:
                    if (pinMatches()) {
                        locked     = false;
                        macroIndex = 0;
                        displayMacroName(macroIndex);
                        break;
                    }

                    if (enteredPINIndex >= MAXPINLENGTH) {
                        enteredPINIndex = 0;
                        displayString("Nope, try again ");
                    }
                    break;
            }
        }

        return false;
    }
    switch (keycode) {
        case CUSTKC_MODE:
            if (record->event.pressed) {
                macroIndex++;
            }
            break;
        case CUSTKC_RESET:
            if (record->event.pressed) {
                locked = true;
                return false;
            }
            break;
        case CUSTKC_CONTRAST:
            if (record->event.pressed) {
                macroIndex--;
            }
            break;
        case CUSTKC_FUNCTION:
            // do something, maybe play a tune?
            break;

        case CUSTKC_START:
            if (record->event.pressed) {
                SEND_STRING(macros[macroIndex]);
            }
            return false;
            //        break;
        case KC_1:
            macroIndex = 0;
            break;
        case KC_2:
            macroIndex = 1;
            break;
        case KC_3:
            macroIndex = 2;
            break;
        case KC_4:
            macroIndex = 3;
            break;
        case KC_5:
            macroIndex = 4;
            break;
        case KC_6:
            macroIndex = 5;
            break;
        case KC_7:
            macroIndex = 6;
            break;
        case KC_8:
            macroIndex = 7;
            break;
        case KC_9:
            macroIndex = 8;
            break;
        case KC_0:
            macroIndex = 9;
            break;
        case KC_ASTERISK:
            macroIndex = 10;
            break;
        case KC_HASH:
            macroIndex = 11;
            break;
    }

    if (record->event.pressed) {
        if (macroIndex < 0) macroIndex = MACROSNUMBER - 1;
        if (macroIndex >= MACROSNUMBER) macroIndex = 0;
        displayMacroName(macroIndex);
    }
    return false;
};
