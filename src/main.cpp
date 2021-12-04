#include <Arduino.h>

int a;

void setup() {
  // put your setup code here, to run once:
  a = 5;
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PIN0, a);
}