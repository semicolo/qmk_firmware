#include "ch.h"
#include "hal.h"

#include "wait.h"
#include "hardware.h"
#include "sound.h"
#include "musical_notes.h"
#include "song_list.h"

static virtual_timer_t sound_vt;
const float (*songplaying)[][2];
int songplayingIndex;
int songplayingNotesCount;
int noteTickDuration;
int songTempo=4*TEMPO_DEFAULT;

void setTempo(int tempo) {
    songTempo =tempo;
}

static PWMConfig pwmcfg = {
    SOUNDPWMFREQUENCY,        // pwm frequency
    SOUNDPWMFREQUENCY / 220,  // number of periods
    NULL,                     // callback
    {
        {PWM_OUTPUT_ACTIVE_HIGH, NULL}, /* CH1 mode and callback.         */
        {PWM_OUTPUT_DISABLED, NULL},    /* CH2 mode and callback.         */
        {PWM_OUTPUT_DISABLED, NULL},    /* CH3 mode and callback.         */
        {PWM_OUTPUT_DISABLED, NULL}     /* CH4 mode and callback.         */
    },
    0, /* Control Register 2.            */
    0  /* DMA/Interrupt Enable Register. */
};

void playNote(float frequency) {
    pwmcfg.period = SOUNDPWMFREQUENCY / frequency;
    palSetPadMode(GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL);  //
    pwmStart(&PWMD3, &pwmcfg);
    pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));
}

void stopNote(void) {
    pwmDisableChannel(&PWMD3, 0);
    pwmStop(&PWMD3);
}

static void notePlayCallBack(void *arg) {
    chSysLockFromISR();
    if (songplaying == NULL || songplayingIndex >= songplayingNotesCount) {
        stopSound();
    } else {
        playNote((*songplaying)[songplayingIndex][0]);
        chVTSetI(&sound_vt, TIME_MS2I(noteTickDuration*(*songplaying)[songplayingIndex][1]), notePlayCallBack, NULL);
        songplayingIndex++;
    }
    chSysUnlockFromISR();
}

void play_notes(const float (*np)[][2], uint16_t n_count) {
    songplaying           = np;
    songplayingNotesCount = n_count;
    songplayingIndex      = 1;
    // a quarter note is 16 units
    // one quarter is 60s/songTempo
    noteTickDuration = 60000 / (songTempo * 16);

    unmute();
    playNote((*np)[0][0]);
    chVTSet(&sound_vt, TIME_MS2I(noteTickDuration * (*np)[0][1]), notePlayCallBack, NULL);
}

void playSound(void) {
    unmute();
    pwmcfg.period = SOUNDPWMFREQUENCY / NOTE_C4;
    palSetPadMode(GPIOA, 6, PAL_MODE_STM32_ALTERNATE_PUSHPULL);  //
    pwmStart(&PWMD3, &pwmcfg);

    pwmEnableChannel(&PWMD3, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD3, 5000));
}

void stopSound(void) {
    mute();
    stopNote();
    songplaying = NULL;
}
