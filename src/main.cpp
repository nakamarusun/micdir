#include <Arduino.h>
#include <Wire.h>
#include "logic.h"

#define SLAVE_ADDRESS 0x4

float last_direction = 0; // Last direction that the sensor detects

// Whether the data has not been sent previously. Begins with no
uint8_t new_data = 0;

void setup() {
  // Initialize i2c slave connection
  Wire.begin(SLAVE_ADDRESS);

  // Initialize pins
  pinMode(MIC1_PIN, INPUT);
  pinMode(MIC2_PIN, INPUT);
  pinMode(MIC3_PIN, INPUT);
}

void on_detection(float direction) {
  last_direction = direction;
  new_data = 1;
}

void send_data() {
  char buf_fl[10];
  char buf[12];
  dtostrf(last_direction, 5, 3, buf_fl); // Convert to string
  sprintf(buf, "%d.%s", new_data, buf_fl); // Construct string
  Wire.write(buf); // Send
  new_data = 0;
}

void loop() {
  loop_on_detection(&on_detection);
  Wire.onRequest(&send_data);
}
