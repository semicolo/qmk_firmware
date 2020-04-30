#pragma once

/* USB Device descriptor parameter */
#define VENDOR_ID       0xFEED
#define PRODUCT_ID      0x6464
#define DEVICE_VER      0x0001

#define MANUFACTURER "QMK"
#define PRODUCT "FaxPanel17"
#define DESCRIPTION "QMK keyboard firmware with ChibiOS"

/* key matrix size */
#define MATRIX_ROWS 3
#define MATRIX_COLS 8
#define DIODE_DIRECTION COL2ROW

// speaker
#define C4_AUDIO

