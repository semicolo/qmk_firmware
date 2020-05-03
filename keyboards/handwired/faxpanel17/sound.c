#include "ch.h"
#include "hal.h"

#include "wait.h"
#include "hardware.h"
#include "sound.h"

static virtual_timer_t sound_vt;

static void notePlayCallBack(void *arg) {
    stopSound();
}

static PWMConfig pwmcfg = {
    SOUNDPWMFREQUENCY,                // pwm frequency
    SOUNDPWMFREQUENCY/220,                // number of periods
    NULL,  // callback
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* CH1 mode and callback.         */
        {PWM_OUTPUT_DISABLED, NULL},    /* CH2 mode and callback.         */
        {PWM_OUTPUT_DISABLED, NULL},    /* CH3 mode and callback.         */
        {PWM_OUTPUT_DISABLED, NULL}     /* CH4 mode and callback.         */
    },
    0, /* Control Register 2.            */
    0  /* DMA/Interrupt Enable Register. */
};

void playNote(uint16_t frequency, uint16_t ms) {
    unmute();
    pwmcfg.period = SOUNDPWMFREQUENCY/frequency;
    palSetPadMode(GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL);  //
    pwmStart(&PWMD3, &pwmcfg);
    pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));
    chVTSet(&sound_vt, TIME_MS2I(ms), notePlayCallBack, NULL);
}

void playSong(void) {
}

void playSound(void) {
    unmute();
    pwmcfg.period = SOUNDPWMFREQUENCY/NOTE_C4;
    palSetPadMode(GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL);  //
    pwmStart(&PWMD3, &pwmcfg);

    pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));
}

void stopSound(void) {
    mute();
    pwmDisableChannel(&PWMD3, 0);
    pwmStop(&PWMD3);
    // todo stop timer if running
}
