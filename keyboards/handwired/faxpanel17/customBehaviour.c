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
#include "sound.h"
#include "song_list.h"
#include "customSongs.h"

bool locked    = true;
int  songShift = 0;

static const float song1[][2]  = SONG(ROCK_A_BYE_BABY);
static const float song2[][2]  = SONG(ODE_TO_JOY);
static const float song3[][2]  = SONG(WORKMAN_SOUND);
static const float song4[][2]  = SONG(CAMPANELLA);
static const float song5[][2]  = SONG(FANTASIE_IMPROMPTU);
static const float song6[][2]  = SONG(NOCTURNE_OP_9_NO_1);
static const float song7[][2]  = SONG(USSR_ANTHEM);
static const float song8[][2]  = SONG(CUST_CLOSE_ENCOUNTERS_5_NOTE);
static const float song9[][2]  = SONG(CUST_DOE_A_DEER);
static const float song10[][2] = SONG(DVORAK_SOUND);
static const float song11[][2] = SONG(CUST_IMPERIAL_MARCH);
static const float song12[][2] = SONG(MUSIC_ON_SOUND);
static const float song13[][2] = SONG(CUST_COIN_SOUND);
static const float song14[][2] = SONG(CUST_ONE_UP_SOUND);
static const float song15[][2] = SONG(CUST_SONIC_RING);
static const float song16[][2] = SONG(CUST_ZELDA_PUZZLE);
static const float song17[][2] = SONG(CUST_ZELDA_TREASURE);
static const float song18[][2] = SONG(CUST_OVERWATCH_THEME);
static const float song19[][2] = SONG(CUST_MARIO_THEME);
static const float song20[][2] = SONG(CUST_MARIO_GAMEOVER);
static const float song21[][2] = SONG(CUST_MARIO_MUSHROOM);
static const float song22[][2] = SONG(CUST_E1M1_DOOM);
static const float song23[][2] = SONG(CUST_DISNEY_SONG);
static const float song24[][2] = SONG(CUST_NUMBER_ONE);
static const float song25[][2] = SONG(CUST_CABBAGE_SONG);
static const float song26[][2] = SONG(CUST_OLD_SPICE);
static const float song27[][2] = SONG(CUST_VICTORY_FANFARE_SHORT);
static const float song28[][2] = SONG(CUST_ALL_STAR);
static const float song29[][2] = SONG(CUST_RICK_ROLL);
static const float song30[][2] = SONG(CUST_FF_PRELUDE);
static const float song31[][2] = SONG(CUST_TO_BOLDLY_GO);
static const float song32[][2] = SONG(CUST_KATAWARE_DOKI);
static const float song33[][2] = SONG(CUST_MEGALOVANIA);
static const float song34[][2] = SONG(CUST_MICHISHIRUBE);
static const float song35[][2] = SONG(CUST_LIEBESLEID);
static const float song36[][2] = SONG(CUST_MELODIES_OF_LIFE);
static const float song37[][2] = SONG(CUST_EYES_ON_ME);
static const float song38[][2] = SONG(CUST_SONG_OF_THE_ANCIENTS);
static const float song39[][2] = SONG(CUST_NIER_AMUSEMENT_PARK);
static const float song40[][2] = SONG(CUST_COPIED_CITY);
static const float song41[][2] = SONG(CUST_VAGUE_HOPE_COLD_RAIN);
static const float song42[][2] = SONG(CUST_KAINE_SALVATION);
static const float song43[][2] = SONG(CUST_WEIGHT_OF_THE_WORLD);
static const float song44[][2] = SONG(CUST_ISABELLAS_LULLABY);
static const float song45[][2] = SONG(CUST_TERRAS_THEME);
static const float song46[][2] = SONG(CUST_RENAI_CIRCULATION);
static const float song47[][2] = SONG(CUST_PLATINUM_DISCO);
static const float song48[][2] = SONG(CUST_LP_NUMB);

void displayString(const char *string) {
    LiquidCrystalHome();
    LiquidCrystalPrintNChars(string, 8);
    LiquidCrystalSetCursor(40, 0);
    LiquidCrystalPrintNChars(string + 8, 8);
}

void displayMacroName(uint8_t mode) { displayString(macroNames[mode]); }

int8_t  macroIndex = 0;
uint8_t customString[16];
char    songStrings[][16] = {"   Songs 1-16   ", "   Songs 17-32  ", "   Songs 33-48  "};

uint8_t enteredPIN[MAXPINLENGTH];  // max length of PIN
uint8_t enteredPINIndex = 0;

uint8_t pinMatches(void) {
    if (enteredPINIndex != strlen(FAXPANELPIN)) {
        return false;
    }
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
    if (keycode == CUSTKC_FUNCTION) {
        if (record->event.pressed) {
            songShift++;
            if (songShift == 4) {
                songShift = 1;
            }
            displayString((char *)songStrings[songShift - 1]);
        }
        return false;
    }
    if (songShift) {
        if (!record->event.pressed) {
            return false;
        }
        clearCustomString();
        displayString((char *)customString);
        switch (keycode) {
            case CUSTKC_MODE:
                if (songShift == 1)
                    PLAY_SONG(song1);
                else if (songShift == 2)
                    PLAY_SONG(song17);
                else
                    PLAY_SONG(song33);
                break;
            case CUSTKC_RESET:
                if (songShift == 1)
                    PLAY_SONG(song2);
                else if (songShift == 2)
                    PLAY_SONG(song18);
                else
                    PLAY_SONG(song34);
                break;
            case KC_ASTERISK:
                if (songShift == 1)
                    PLAY_SONG(song3);
                else if (songShift == 2)
                    PLAY_SONG(song19);
                else
                    PLAY_SONG(song35);
                break;
            case KC_1:
                if (songShift == 1)
                    PLAY_SONG(song4);
                else if (songShift == 2)
                    PLAY_SONG(song20);
                else
                    PLAY_SONG(song36);
                break;
            case KC_2:
                if (songShift == 1)
                    PLAY_SONG(song5);
                else if (songShift == 2)
                    PLAY_SONG(song21);
                else
                    PLAY_SONG(song37);
                break;
            case KC_3:
                if (songShift == 1)
                    PLAY_SONG(song6);
                else if (songShift == 2)
                    PLAY_SONG(song22);
                else
                    PLAY_SONG(song38);
                break;
            case KC_4:
                if (songShift == 1)
                    PLAY_SONG(song7);
                else if (songShift == 2)
                    PLAY_SONG(song23);
                else
                    PLAY_SONG(song39);
                break;
            case KC_5:
                if (songShift == 1)
                    PLAY_SONG(song8);
                else if (songShift == 2)
                    PLAY_SONG(song24);
                else
                    PLAY_SONG(song40);
                break;
            case CUSTKC_CONTRAST:
                if (songShift == 1)
                    PLAY_SONG(song9);
                else if (songShift == 2)
                    PLAY_SONG(song25);
                else
                    PLAY_SONG(song41);
                break;
            case KC_HASH:
                if (songShift == 1)
                    PLAY_SONG(song10);
                else if (songShift == 2)
                    PLAY_SONG(song26);
                else
                    PLAY_SONG(song42);
                break;
            case KC_6:
                if (songShift == 1)
                    PLAY_SONG(song11);
                else if (songShift == 2)
                    PLAY_SONG(song27);
                else
                    PLAY_SONG(song43);
                break;
            case KC_7:
                if (songShift == 1)
                    PLAY_SONG(song12);
                else if (songShift == 2)
                    PLAY_SONG(song28);
                else
                    PLAY_SONG(song44);
                break;
            case KC_8:
                if (songShift == 1)
                    PLAY_SONG(song13);
                else if (songShift == 2)
                    PLAY_SONG(song29);
                else
                    PLAY_SONG(song45);
                break;
            case KC_9:
                if (songShift == 1)
                    PLAY_SONG(song14);
                else if (songShift == 2)
                    PLAY_SONG(song30);
                else
                    PLAY_SONG(song46);
                break;
            case KC_0:
                if (songShift == 1)
                    PLAY_SONG(song15);
                else if (songShift == 2)
                    PLAY_SONG(song31);
                else
                    PLAY_SONG(song47);
                break;
            case CUSTKC_START:
                if (songShift == 1)
                    PLAY_SONG(song16);
                else if (songShift == 2)
                    PLAY_SONG(song32);
                else
                    PLAY_SONG(song48);
                break;
        }
        songShift = 0;
        return false;
    }
    if (locked) {
        if (record->event.pressed) {
            switch (keycode) {
                case CUSTKC_RESET:
                    enteredPINIndex = 0;
                    PLAY_SONG(song14);
                case CUSTKC_START:
                case CUSTKC_CONTRAST:
                case CUSTKC_MODE:
                case CUSTKC_FUNCTION:
                case KC_ASTERISK:
                case KC_HASH:
                    break;
                default:
                    enteredPIN[enteredPINIndex] = keycodeToDigit(keycode) + '0';
                    if (enteredPINIndex == 0) {
                        clearCustomString();
                    }
                    customString[enteredPINIndex] = '*';
                    enteredPINIndex++;
                    displayString((char *)customString);
                    break;
            }
        } else {
            switch (keycode) {
                case CUSTKC_RESET:
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
                        PLAY_SONG(song17);
                        break;
                    }

                    if (enteredPINIndex >= MAXPINLENGTH) {
                        enteredPINIndex = 0;
                        displayString("Nope, try again ");
                        PLAY_SONG(song20);
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
                enteredPINIndex = 0;
                locked = true;
                PLAY_SONG(song14);
            }
            return false;
            break;
        case CUSTKC_CONTRAST:
            if (record->event.pressed) {
                macroIndex--;
            }
            break;
        case CUSTKC_FUNCTION:
            songShift = true;
            break;
        case CUSTKC_START:
            if (record->event.pressed) {
                SEND_STRING(macros[macroIndex]);
            }
            return false;
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
        if (macroIndex < 0) {
            macroIndex = MACROSNUMBER - 1;
        }
        if (macroIndex >= MACROSNUMBER) {
            macroIndex = 0;
        }
        displayMacroName(macroIndex);
    }
    return false;
};
