#ifndef LOGIC_MIC
#define LOGIC_MIC

// Pin definition
#define MIC1_PIN 1
#define MIC2_PIN 3
#define MIC3_PIN 4

#ifndef TEST
#include <Arduino.h>
#include <avr/io.h>

#define READMIC1 PINB & (1 << MIC1_PIN)
#define READMIC2 PINB & (1 << MIC2_PIN)
#define READMIC3 PINB & (1 << MIC3_PIN)
#endif

#define DISTANCE 0.065 // (m)
#define SOUND_V 343.0 // (m/s)

// Variables to check if a current signal is still valid
#define VALID_TIME 200000 // 200ms

// Mic struct
typedef struct {
  uint8_t act; // Whether this sensor already heard a sound
  uint32_t time; // Time since the first sound is detected
} mic_;

extern uint8_t valid;
extern uint32_t unvalid_time;

// Mic variables
extern uint32_t sound_first; // Timestamp of first time sound is heard
extern uint8_t sound_heard; // Whether a sound is heard in the duration

extern uint8_t mic_order_i; // Index to keep track of the order of the mic
extern uint8_t mic_order[3]; // The order of the mic

extern mic_ mics[3];

// Function will be called if the duration in which a signal can be active
// has expired
void unvalid_event();

// Calculates the sound source direction based on the delta and mic heard order
float calculate_direction();

// The main loop of the program that detects sound and calculates the direction
// The callback that is passed into this function will be called after a full
// triangulation has been done, and the direction of the sound will be passed
// as an argument. (Radians)
void loop_on_detection(void (*func)(float));

#endif