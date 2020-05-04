/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

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

#include "ch.h"
#include "hal.h"

/*
 * scan matrix
 */
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"
#include "wait.h"
#include "hardware.h"
#include "LiquidCrystal.h"
#include "sound.h"

#ifndef DEBOUNCE
#    define DEBOUNCE 5
#endif
static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void         init_cols(void);
static void         unselect_rows(void);
static void         select_row(uint8_t row);

inline uint8_t matrix_rows(void) {
    return MATRIX_ROWS;
}

inline uint8_t matrix_cols(void) {
    return MATRIX_COLS;
}

inline void LED_ON(void) {
    palClearPad(GPIOC, 13);
}

inline void LED_OFF(void) {
    palSetPad(GPIOC, 13);
}

__attribute__((weak))
void matrix_init_kb(void) {
    matrix_init_user();
}

__attribute__((weak))
void matrix_scan_kb(void) {
    matrix_scan_user();
}

__attribute__((weak))
void matrix_init_user(void) {
}

__attribute__((weak))
void matrix_scan_user(void) {
}

void matrix_init(void) {
    init_cols();

    initFaxPanel();
    playNote(NOTE_E6, 150);
    wait_ms(200);
    playNote(NOTE_A6, 150);
    wait_ms(200);
    playNote(NOTE_E7, 225);

    unselect_rows();

    // initialize matrix state: all keys off
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        matrix[i]            = 0;
        matrix_debouncing[i] = 0;
    }
    // debug
    //  debug_matrix = true;

    LiquidCrystalBegin(8, 2);
    LiquidCrystalClear();
    LiquidCrystalPrint(" Macro K");
    LiquidCrystalSetCursor(40, 0);
    LiquidCrystalPrint("eyboard ");

    LED_ON();
    wait_ms(5);
    LED_OFF();

    matrix_init_quantum();
}

uint8_t matrix_scan(void) {
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        wait_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: ");
                debug_hex(debouncing);
                debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            wait_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }
    matrix_scan_quantum();
    // until I find what to do with the 3 LEDs, turn them on when a key is depressed
    if (matrix_get_row(0)) {
        write259(LEDON, 0);
    } else {
        write259(LEDOFF, 0);
    }
    if (matrix_get_row(1)) {
        write259(LEDON, 1);
    } else {
        write259(LEDOFF, 1);
    }
    if (matrix_get_row(2)) {
        write259(LEDON, 2);
    } else {
        write259(LEDOFF, 2);
    }

    return 1;
}

inline bool matrix_is_on(uint8_t row, uint8_t col) {
    return (matrix[row] & ((matrix_row_t)1 << col));
}

inline matrix_row_t matrix_get_row(uint8_t row) {
    return matrix[row];
}

void matrix_print(void) {
    print("\nr/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        phex(row);
        print(": ");
        pbin_reverse16(matrix_get_row(row));
        print("\n");
    }
}

/* Column pin configuration
 */
static void init_cols(void) {
    //   palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
    //   palSetPadMode(GPIOB, 14, PAL_MODE_INPUT_PULLUP);
    //   palSetPadMode(GPIOB, 13, PAL_MODE_INPUT_PULLUP);
    //   palSetPadMode(GPIOB, 12, PAL_MODE_INPUT_PULLUP);
    //   palSetPadMode(GPIOB, 11, PAL_MODE_INPUT_PULLUP);
    //   palSetPadMode(GPIOB, 10, PAL_MODE_INPUT_PULLUP);
    //   palSetPadMode(GPIOB, 9, PAL_MODE_INPUT_PULLUP);
    //   palSetPadMode(GPIOB, 8, PAL_MODE_INPUT_PULLUP);
}

/* Returns status of switches(1:on, 0:off) */
static matrix_row_t read_cols(void) {
    setPortBMode(PAL_MODE_INPUT);
    palClearPad(GPIOA, 1);
    matrix_row_t data = ((palReadPad(GPIOB, 15) == PAL_HIGH) ? 0 : (1 << 10)) | ((palReadPad(GPIOB, 14) == PAL_HIGH) ? 0 : (1 << 9)) | ((palReadPad(GPIOB, 13) == PAL_HIGH) ? 0 : (1 << 8)) | ((palReadPad(GPIOB, 12) == PAL_HIGH) ? 0 : (1 << 7)) | ((palReadPad(GPIOB, 11) == PAL_HIGH) ? 0 : (1 << 6)) | ((palReadPad(GPIOB, 10) == PAL_HIGH) ? 0 : (1 << 5)) | ((palReadPad(GPIOB, 9) == PAL_HIGH) ? 0 : (1 << 4)) | ((palReadPad(GPIOB, 8) == PAL_HIGH) ? 0 : (1 << 3));

    data = (~palReadGroup(GPIOB, 0xff, 8)) & 0xff;
    palSetPad(GPIOA, 1);
    setPortBMode(PAL_MODE_OUTPUT_PUSHPULL);

    return data;
}

/* Row pin configuration
 */
static void unselect_rows(void) {
    write374(16);
}

static void select_row(uint8_t row) {
    switch (row) {
        case 0:
            write374(1 + 16);
            break;
        case 1:
            write374(2 + 16);
            break;
        case 2:
            write374(4 + 16);
            break;
    }
}
