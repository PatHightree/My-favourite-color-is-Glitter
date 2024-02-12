#include <Arduino.h>

const int enPin = 2;
const int dirPin = 3;
const int stepPin = 4;

void setup()
{
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);
  digitalWrite(dirPin, HIGH);
}

void loop()
{
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(50);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(50);
}