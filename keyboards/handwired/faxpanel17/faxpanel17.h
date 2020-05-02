#pragma once

#include "quantum.h"
#include "led.h"

/*  LAYOUT
  Yes|Rst  | *  | 1  | 2  | 3  | 4  | 5  |
  No |Pause| #  | 6  | 7  | 8  | 9  | 0  |
                                          Start
*/

#define LAYOUT_17( \
  K00, K01, K02, K03,   K04,   K05,   K06, K07, \
  K10, K11, K12, K13,   K14,   K15,   K16, K17, \
                                                K20 \
) { \
  { K00, K01, K02, K03,   K04,   K05,   K06, K07 }, \
  { K10, K11, K12, K13,   K14,   K15,   K16, K17 }, \
  { K20, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO } \
}
