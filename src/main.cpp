#include <Arduino.h>
#include "logic.h"

void setup() {
  // TODO:
  // Initialize i2c slave connection

  // Initialize pins
  pinMode(MIC1_PIN, INPUT);
  pinMode(MIC2_PIN, INPUT);
  pinMode(MIC3_PIN, INPUT);
}

void on_detection(double direction) {
  // TODO: Send signal to master device
}

void loop() {
  loop_on_detection(&on_detection);
}
