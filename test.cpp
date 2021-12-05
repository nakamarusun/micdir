// Load the main microcontroller file
// *****************************************************************************
#define TEST
#include <cmath>

// Create functions
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

uint8_t digitalRead(uint8_t pin) {
  // TODO
  return 0;
}

#include "src/logic.h"

// Create testing functions
template <typename T>
bool assert_equal(T a, T b) {
  return a == b;
}
#define ASSERT_EQUAL assert_equal
// *****************************************************************************

#include <stdio.h>

bool test_looper() {
  // TODO: Do loop

  // Add to micros
  _curtime += INCREMENT_MICRO;

  if (_curtime > 1000) { return 0; }

  return 1;
}

// Do testing here
int main() {

  // Do loops
  while (test_looper());

  printf("%d", _curtime);

  return 0;
}
