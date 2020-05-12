#pragma once

#define SOUNDPWMFREQUENCY 1000000

void playSound(void);
void stopSound(void);

void setTempo(int tempo);

void play_notes(const float (*np)[][2], uint16_t n_count);

#define SCALE (int8_t[]) { 0 + (12 * 0), 2 + (12 * 0), 4 + (12 * 0), 5 + (12 * 0), 7 + (12 * 0), 9 + (12 * 0), 11 + (12 * 0), 0 + (12 * 1), 2 + (12 * 1), 4 + (12 * 1), 5 + (12 * 1), 7 + (12 * 1), 9 + (12 * 1), 11 + (12 * 1), 0 + (12 * 2), 2 + (12 * 2), 4 + (12 * 2), 5 + (12 * 2), 7 + (12 * 2), 9 + (12 * 2), 11 + (12 * 2), 0 + (12 * 3), 2 + (12 * 3), 4 + (12 * 3), 5 + (12 * 3), 7 + (12 * 3), 9 + (12 * 3), 11 + (12 * 3), 0 + (12 * 4), 2 + (12 * 4), 4 + (12 * 4), 5 + (12 * 4), 7 + (12 * 4), 9 + (12 * 4), 11 + (12 * 4), }

// These macros are used to allow play_notes to play an array of indeterminate
// length. This works around the limitation of C's sizeof operation on pointers.
// The global float array for the song must be used here.
#define NOTE_ARRAY_SIZE(x) ((int16_t)(sizeof(x) / (sizeof(x[0]))))
#define PLAY_SONG(note_array) play_notes(&note_array, NOTE_ARRAY_SIZE((note_array)))

