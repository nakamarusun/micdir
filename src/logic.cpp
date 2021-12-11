#include "logic.h"

uint8_t valid = 0;
uint32_t unvalid_time = 0;

// Mic variables
uint32_t sound_first; // Timestamp of first time sound is heard
uint8_t sound_heard = 0; // Whether a sound is heard in the duration
uint8_t all_sound = 0; // Whether all mic have heard a sound once

uint8_t mic_order_i = 0; // Index to keep track of the order of the mic
uint8_t mic_order[3]; // The order of the mic

mic_ mics[3];

// Function will be called if the duration in which a signal can be active
// has expired
void unvalid_event() {
  // Reset everything
  valid = 0;
  sound_heard = 0;
  all_sound = 0;
  mic_order_i = 0;
  
  // Reset mic activated state
  mics[0].act = 0;
  mics[1].act = 0;
  mics[2].act = 0;
}

// Calculates the sound source direction based on the delta and mic heard order
float calculate_direction() {
  float deg = acos((mics[mic_order[1]].time / 1000000.0 * SOUND_V) / DISTANCE);

  float res = 0;
  if (mic_order[0] == 0) {
    if (mic_order[1] == 1) res = deg - PI/6;
    else if (mic_order[1] == 2) res = PI*13/6 - deg;
  } else if (mic_order[0] == 1) {
    if (mic_order[1] == 0) res = PI*5/6 - deg;
    else if (mic_order[1] == 2) res = deg + PI/2;
  } else if (mic_order[0] == 2) {
    if (mic_order[1] == 1) res = PI*3/2 - deg;
    else if (mic_order[1] == 0) res = deg + PI*7/6;
  }
  
  return res;
}


void loop_on_detection(void (*func)(float)) {
  // Read mic pins
  uint8_t mic1_pin = digitalRead(MIC1_PIN);
  uint8_t mic2_pin = digitalRead(MIC2_PIN);
  uint8_t mic3_pin = digitalRead(MIC3_PIN);

  // If any of the pins are high, execute first event
  if (mic1_pin || mic2_pin || mic3_pin) {
    if (micros() > unvalid_time) {
      // Check if the time is valid, then reset the state of the algorithm
      unvalid_event();
    }

    if (!valid) {
      // Set timestamp
      valid = 1;
      unvalid_time = micros() + VALID_TIME;
    }

    // If the sound is valid
    if (valid) {
      if (!sound_heard) {
        // Check if sound has been heard, and record the time
        sound_heard = 1;
        sound_first = micros();
      }

      // Record first time for each mic and detect if all mic has heard a sound.
      // If all mic has heard a sound once, calculate direction
      if (!mics[0].act && mic1_pin) { mics[0].act = 1; mics[0].time = micros() - sound_first; mic_order[mic_order_i++] = 0; if (mics[1].act && mics[2].act) all_sound = 1; }
      if (!mics[1].act && mic2_pin) { mics[1].act = 1; mics[1].time = micros() - sound_first; mic_order[mic_order_i++] = 1; if (mics[0].act && mics[2].act) all_sound = 1; }
      if (!mics[2].act && mic3_pin) { mics[2].act = 1; mics[2].time = micros() - sound_first; mic_order[mic_order_i++] = 2; if (mics[0].act && mics[1].act) all_sound = 1; }

      // If all mic has heard a sound once, calculate direction and reset.
      if (all_sound) {
        unvalid_event();
        func(calculate_direction());
      }
    }
  }
}