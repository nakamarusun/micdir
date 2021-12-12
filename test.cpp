#define TEST
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Create functions and definitions to replace the ones from the arduino
// library
typedef unsigned int uint8_t;
typedef unsigned int uint32_t;

#define PI 3.14159
#define ACCURACY_ANGLE 15

// The amount that micro increments (default 8) according to
// https://www.arduino.cc/reference/en/language/functions/time/micros/
#define INCREMENT_MICRO 8

uint32_t _curtime = 0;
uint32_t micros() {
  return _curtime;
}

#define SOUND_TIME 100000 // Sound duration
#define TIME_OFFSET 7 // Offset to be added to the dets array
// Whether the mic is already triggered once, next ones is gonna be noise.
uint8_t trigd[] = {0, 0, 0};

// Time of microphone triggers (Ordered per pin)
uint32_t mul_i = 0;
uint32_t dets_multi[][3] = {
  {9 + TIME_OFFSET, 130 + TIME_OFFSET, 0 + TIME_OFFSET},
  {300000 + 116 + TIME_OFFSET, 300000 + 0 + TIME_OFFSET, 300000 + 19 + TIME_OFFSET},
};

// The correct direction
const float dirs[] = {
  295,
  173,
};

#include "include/logic.h"

uint8_t digitalRead(uint8_t pin) {
  switch (pin) {
    case MIC1_PIN: pin = 0; break;
    case MIC2_PIN: pin = 1; break;
    default: pin = 2;
  }

  if (_curtime > dets_multi[mul_i][pin] && _curtime < (dets_multi[mul_i][pin] + SOUND_TIME)) {
    if (trigd[pin]) {
      return (rand() % 100) < 10; // Simulate noise
    } else {
      trigd[pin] = 1;
      return true;
    }
  }
  return false;
}

// Load the microcontroller file
#include "src/logic.cpp"

// Create testing functions
template <typename T>
void assert_equal(const char test[], T a, T b) {
  printf("Assertion (%s) returns ", test);
  if (a == b) {
    printf("[SUCCESS]\n");
  } else {
    printf("FAILURE\n");
    exit(1);
  }
}
template <typename T>
void assert_range(const char test[], T a, T b) {
  printf("Assertion (%s) returns ", test);
  if (a <= b + ACCURACY_ANGLE / 2.0 && a >= b - ACCURACY_ANGLE / 2.0) {
    printf("[SUCCESS] (Expected %f)\n", b);
  } else {
    printf("[FAILURE] (Expected %f)\n", b);
    exit(1);
  }
}
#define ASSERT_EQUAL assert_equal // Biar keren :v
#define ASSERT_RANGE assert_range
#define STOP_AFTER 5000000 // Stop the program after x seconds has passed (us)


// Reset sensors
void reset_test_sensor() {
  mul_i++;
  for (int i = 0; i < sizeof(trigd)/sizeof(trigd[0]); i++) trigd[i] = 0;
}

// When the sensors have determined a direction
void on_detection(float direction) {
  printf("Detected direction: %f\n", direction * 180 / PI);

  // Assert whether direction is same
  ASSERT_RANGE("Calculated direction is within range", (float)(direction * 180 / PI), dirs[mul_i]);
  
  // If all detection is not done, reset sensor and continue
  if ((mul_i + 1) < (sizeof(dets_multi)/sizeof(dets_multi[0]))) {
    reset_test_sensor();
  } else {
    printf("Test done, simulated time: %dus", _curtime);
    exit(0);
  }
}

// Main loop
bool test_looper() {
  loop_on_detection(&on_detection);

  // Add to micros
  _curtime += INCREMENT_MICRO;

  if (_curtime > STOP_AFTER) { return 0; }

  return 1;
}

int main() {
  srand(time(NULL));
  ASSERT_EQUAL("Sanity check 1=1", 1, 1);

  // Do loops
  while (test_looper());

  return 0;
}
