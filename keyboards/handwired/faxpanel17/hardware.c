#include "ch.h"
#include "hal.h"

#include "util.h"
#include "wait.h"

#include "hardware.h"

void setPortBMode(unsigned mode) {
  palSetPadMode(GPIOB, 8, mode);
  palSetPadMode(GPIOB, 9, mode);
  palSetPadMode(GPIOB, 10, mode);
  palSetPadMode(GPIOB, 11, mode);
  palSetPadMode(GPIOB, 12, mode);
  palSetPadMode(GPIOB, 13, mode);
  palSetPadMode(GPIOB, 14, mode);
  palSetPadMode(GPIOB, 15, mode);
}

void write374(uint8_t data) {
    // The outputs or the 374 are tied to various other chips
    // output 5 to the gate of the 74259, must stay high when not addressing the 74259
    // output 7 to the LCD E, must stay low when not addressing the LCD
    // output 8 to the LCD RS
    palWriteGroup(GPIOB, 0xff, 8, data);
    palSetPad(GPIOA, 0);
    palClearPad(GPIOA, 0);
}

void write259(uint8_t data, uint8_t output) {
    // first 3 outputs of the 259 are tied to the 3 leds, they turn on when the output is low
    // output 8 can enable or disable the speaker
    // output 1 of the 374 is tied to the data input of the 259
    // outputs 2-4 of the 374 are tied to the 3 addr inputs of the 259
    if (data) {
        data=1;
    }
    write374(data+output*2+16);
    write374(data+output*2);
    write374(data+output*2+16);
}

void mute(void) {
    write259(1, 7);
}

void unmute(void) {
    write259(0, 7);
}

void initFaxPanel(void) {
  palSetPadMode(GPIOA, 0, PAL_MODE_OUTPUT_PUSHPULL); // 74374 clock, latches on low to high event
  palClearPad(GPIOA, 0);
  palSetPadMode(GPIOA, 1, PAL_MODE_OUTPUT_PUSHPULL); // gate on 74244
  palSetPad(GPIOA, 1);
  palSetPadMode(GPIOA, 6, PAL_MODE_OUTPUT_PUSHPULL);
  palClearPad(GPIOA, 6); // speaker

  setPortBMode(PAL_MODE_OUTPUT_PUSHPULL);

  mute();
  write259(LEDOFF, 0); // send
  write259(LEDOFF, 1); // receive
  write259(LEDOFF, 2); // auto receive
}
