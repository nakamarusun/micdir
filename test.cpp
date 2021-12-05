#define TEST
#include <cmath>
#include <stdio.h>

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
// Time of microphone triggers (Ordered per pin)
uint32_t dets[] = {4380765, 4380000, 4381457};
uint8_t digitalRead(uint8_t pin) {

  return dets[pin] > _curtime && dets[pin] < (_curtime + SOUND_TIME);
}

// Load the microcontroller file
#include "src/logic.cpp"

// Create testing functions
template <typename T>
void assert_equal(const char test[], T a, T b) {
  printf("Assertion (%s) returns ", test);
  if (a == b) printf("[SUCCESS]\n"); else printf("FAILURE\n"); exit(1);
}
#define ASSERT_EQUAL assert_equal // Biar keren :v

#define STOP_AFTER 5000000 // Stop the program after x seconds has passed (us)


// When the sensors have determined a direction
void on_detection(double direction) {
  printf("Detected direction: %f", direction);
  exit(0);
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
  ASSERT_EQUAL("Sanity check 1=1", 1, 1);

  // Do loops
  while (test_looper());

  return 0;
}
