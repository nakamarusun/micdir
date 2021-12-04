#include <Arduino.h>

#define DISTANCE 0.05 // (m)
#define SOUND_V 343.0 // (m/s)

// Pin definition
#define MIC1_PIN 0
#define MIC2_PIN 1
#define MIC3_PIN 2

// Variables to check if a current signal is still valid
#define VALID_TIME 200000 // 200ms
uint8_t valid = 0;
uint32_t unvalid_time;

// Mic struct
typedef struct {
  uint8_t act; // Whether this sensor already heard a sound
  uint32_t time; // Time since the first sound is detected
} mic_;

// Mic variables
uint32_t sound_first; // Timestamp of first time sound is heard
uint8_t sound_heard = 0; // Whether a sound is heard in the duration
uint8_t all_sound = 0; // Whether all mic have heard a sound once

uint8_t mic_order_i = 0; // Index to keep track of the order of the mic
uint8_t mic_order[3]; // The order of the mic

mic_ mics[3];

void setup() {
  // TODO:
  // Initialize i2c slave connection

  // Initialize pins
  pinMode(MIC1_PIN, INPUT);
  pinMode(MIC2_PIN, INPUT);
  pinMode(MIC3_PIN, INPUT);
}

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

void calculate_direction() {
  float deg = acos((mics[mic_order[1]].time * SOUND_V) / DISTANCE);
  // TODO: Send signal to master device

  // Reset sensors
  unvalid_event();
}

void loop() {
  // Read mic pins
  uint8_t mic1_pin = digitalRead(MIC1_PIN);
  uint8_t mic2_pin = digitalRead(MIC2_PIN);
  uint8_t mic3_pin = digitalRead(MIC2_PIN);

  if (micros() > unvalid_time) {
    // Check if the time is valid, then reset the state of the algorithm
    unvalid_event();
  }

  // If any of the pins are high, execute first event
  if (mic1_pin || mic2_pin || mic3_pin) {
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
      if (all_sound) calculate_direction();
    } else {
      // Set timestamp
      valid = 1;
      unvalid_time = micros() + VALID_TIME;
    }
  }
}

// TODO: Overflow micros do calc