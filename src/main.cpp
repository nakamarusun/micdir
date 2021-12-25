#include <Arduino.h>
#include <Wire.h>
#include "logic.h"
#include <avr/io.h>

#define SLAVE_ADDRESS 0x5e

// Turns on LED if there is a new data, turn off if the new data is read
// #define LED_PIN 5

float last_direction = 0; // Last direction that the sensor detects

// Whether the data has not been sent previously. Begins with no
uint8_t new_data = 0;

void on_detection(float direction) {
  last_direction = direction;
  new_data = 1;

  // digitalWrite(LED_PIN, 1);
}

void send_data() {
  char buf_fl[10];
  char buf[12];
  dtostrf(last_direction, 5, 3, buf_fl); // Convert to string
  sprintf(buf, "%d,%s", new_data, buf_fl); // Construct string
  Wire.write(buf); // Send
  new_data = 0;
  // digitalWrite(LED_PIN, 0);
}

void setup() {
  // Initialize i2c slave connection
  Wire.begin(SLAVE_ADDRESS);

  // Initialize pins
  // pinMode(MIC1_PIN, INPUT);
  // pinMode(MIC2_PIN, INPUT);
  // pinMode(MIC3_PIN, INPUT);

  DDRB &= ~(1 << MIC1_PIN);
  DDRB &= ~(1 << MIC2_PIN);
  DDRB &= ~(1 << MIC3_PIN);

  PORTB |= (1 << MIC1_PIN);
  PORTB |= (1 << MIC2_PIN);
  PORTB |= (1 << MIC3_PIN);

  // pinMode(LED_PIN, OUTPUT);

  Wire.onRequest(&send_data);
}

void loop() {
  loop_on_detection(&on_detection);
}
