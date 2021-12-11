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
  {1000 + 116 + TIME_OFFSET, 1000 + 0 + TIME_OFFSET, 1000 + 19 + TIME_OFFSET},
};

uint8_t digitalRead(uint8_t pin) {
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
#define ASSERT_EQUAL assert_equal // Biar keren :v
#define STOP_AFTER 5000000 // Stop the program after x seconds has passed (us)


// Reset sensors
void reset_test_sensor() {
  mul_i++;
  for (int i = 0; i < sizeof(trigd)/sizeof(trigd[0]); i++) trigd[0] = 0;
}

// When the sensors have determined a direction
void on_detection(float direction) {
  printf("Detected direction: %f\n", direction * 180 / PI);
  
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
